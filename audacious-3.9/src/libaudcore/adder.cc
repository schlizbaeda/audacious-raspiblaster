/*
 * adder.c
 * Copyright 2011-2016 John Lindgren
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    provided with the distribution.
 *
 * This software is provided "as is" and without any warranty, express or
 * implied. In no event shall the authors be liable for any damages arising from
 * the use of this software.
 */

#include "playlist-internal.h"
#include "internal.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "audstrings.h"
#include "hook.h"
#include "i18n.h"
#include "list.h"
#include "mainloop.h"
#include "plugins-internal.h"
#include "probe.h"
#include "runtime.h"
#include "tuple.h"
#include "interface.h"
#include "vfs.h"

// regrettably, strcmp_nocase can't be used directly as a
// callback for Index::sort due to taking a third argument;
// strcmp also triggers -Wnoexcept-type with GCC 7
static int filename_compare (const char * a, const char * b)
#ifdef _WIN32
    { return strcmp_nocase (a, b); }
#else
    { return strcmp (a, b); }
#endif

struct AddTask : public ListNode
{
    Playlist playlist;
    int at;
    bool play;
    Index<PlaylistAddItem> items;
    Playlist::FilterFunc filter;
    void * user;
};

struct AddResult : public ListNode
{
    Playlist playlist;
    int at;
    bool play;
    String title;
    Index<PlaylistAddItem> items;
    bool saw_folder, filtered;
};

static void * add_worker (void * unused);

static List<AddTask> add_tasks;
static List<AddResult> add_results;
static Playlist current_playlist;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static bool add_thread_started = false;
static bool add_thread_exited = false;
static pthread_t add_thread;
static QueuedFunc queued_add;
static QueuedFunc status_timer;

static char status_path[512];
static int status_count;
static bool status_shown = false;

static void status_cb (void * unused)
{
    pthread_mutex_lock (& mutex);

    char scratch[128];
    snprintf (scratch, sizeof scratch, dngettext (PACKAGE, "%d file found",
     "%d files found", status_count), status_count);

    if (aud_get_headless_mode ())
    {
        printf ("Searching, %s ...\r", scratch);
        fflush (stdout);
    }
    else
    {
        hook_call ("ui show progress", status_path);
        hook_call ("ui show progress 2", scratch);
    }

    status_shown = true;

    pthread_mutex_unlock (& mutex);
}

static void status_update (const char * filename, int found)
{
    pthread_mutex_lock (& mutex);

    snprintf (status_path, sizeof status_path, "%s", filename);
    status_count = found;

    if (! status_timer.running ())
        status_timer.start (250, status_cb, nullptr);

    pthread_mutex_unlock (& mutex);
}

static void status_done_locked ()
{
    status_timer.stop ();

    if (status_shown)
    {
        if (aud_get_headless_mode ())
            printf ("\n");
        else
            hook_call ("ui hide progress", nullptr);

        status_shown = false;
    }
}

static void add_file (PlaylistAddItem && item, Playlist::FilterFunc filter,
 void * user, AddResult * result, bool validate)
{
    AUDINFO ("Adding file: %s\n", (const char *) item.filename);
    status_update (item.filename, result->items.len ());

    /* If the item doesn't already have a valid tuple, and isn't a subtune
     * itself, then probe it to expand any subtunes.  The "validate" check (used
     * to skip non-audio files when adding folders) is also nested within this
     * block; note that "validate" is always false for subtunes. */
    if (! item.tuple.valid () && ! is_subtune (item.filename))
    {
        VFSFile file;

        if (! item.decoder)
        {
            bool fast = ! aud_get_bool (nullptr, "slow_probe");
            item.decoder = aud_file_find_decoder (item.filename, fast, file);
            if (validate && ! item.decoder)
                return;
        }

        if (item.decoder && input_plugin_has_subtunes (item.decoder))
            aud_file_read_tag (item.filename, item.decoder, file, item.tuple);
    }

    int n_subtunes = item.tuple.get_n_subtunes ();

    if (n_subtunes)
    {
        for (int sub = 0; sub < n_subtunes; sub ++)
        {
            StringBuf subname = str_printf ("%s?%d",
             (const char *) item.filename, item.tuple.get_nth_subtune (sub));

            if (! filter || filter (subname, user))
                add_file ({String (subname), Tuple (), item.decoder}, filter, user, result, false);
            else
                result->filtered = true;
        }
    }
    else
        result->items.append (std::move (item));
}

/* To prevent infinite recursion, we currently allow adding a folder from within
 * a playlist, but not a playlist from within a folder, nor a second playlist
 * from within a playlist (this last rule is enforced by setting
 * <allow_playlist> to false from within add_playlist()). */
static void add_generic (PlaylistAddItem && item, Playlist::FilterFunc filter,
 void * user, AddResult * result, bool save_title, bool allow_playlist);

static void add_playlist (const char * filename, Playlist::FilterFunc filter,
 void * user, AddResult * result, bool save_title)
{
    AUDINFO ("Adding playlist: %s\n", filename);
    status_update (filename, result->items.len ());

    String title;
    Index<PlaylistAddItem> items;

    if (! playlist_load (filename, title, items))
        return;

    if (save_title)
        result->title = title;

    for (auto & item : items)
        add_generic (std::move (item), filter, user, result, false, false);
}

static void add_cuesheets (Index<String> & files, Playlist::FilterFunc filter,
 void * user, AddResult * result)
{
    Index<String> cuesheets;

    for (int i = 0; i < files.len ();)
    {
        if (str_has_suffix_nocase (files[i], ".cue"))
            cuesheets.move_from (files, i, -1, 1, true, true);
        else
            i ++;
    }

    if (! cuesheets.len ())
        return;

    // sort cuesheet list in natural order
    cuesheets.sort (str_compare_encoded);

    // sort file list in system-dependent order for duplicate removal
    files.sort (filename_compare);

    for (String & cuesheet : cuesheets)
    {
        AUDINFO ("Adding cuesheet: %s\n", (const char *) cuesheet);
        status_update (cuesheet, result->items.len ());

        String title; // ignored
        Index<PlaylistAddItem> items;

        if (! playlist_load (cuesheet, title, items))
            continue;

        String prev_filename;
        for (auto & item : items)
        {
            String filename = item.tuple.get_str (Tuple::AudioFile);
            if (! filename)
                continue; // shouldn't happen

            if (! filter || filter (item.filename, user))
                add_file (std::move (item), filter, user, result, false);
            else
                result->filtered = true;

            // remove duplicates from file list
            if (prev_filename && ! filename_compare (filename, prev_filename))
                continue;

            int idx = files.bsearch ((const char *) filename, filename_compare);
            if (idx >= 0)
                files.remove (idx, 1);

            prev_filename = std::move (filename);
        }
    }
}

static void add_folder (const char * filename, Playlist::FilterFunc filter,
 void * user, AddResult * result, bool save_title)
{
    AUDINFO ("Adding folder: %s\n", filename);
    status_update (filename, result->items.len ());

    String error;
    Index<String> files = VFSFile::read_folder (filename, error);

    if (error)
        aud_ui_show_error (str_printf (_("Error reading %s:\n%s"), filename, (const char *) error));

    if (! files.len ())
        return;

    if (save_title)
    {
        const char * slash = strrchr (filename, '/');
        if (slash)
            result->title = String (str_decode_percent (slash + 1));
    }

    add_cuesheets (files, filter, user, result);

    // sort file list in natural order (must come after add_cuesheets)
    files.sort (str_compare_encoded);

    for (const char * file : files)
    {
        if (filter && ! filter (file, user))
        {
            result->filtered = true;
            continue;
        }

        String error;
        VFSFileTest mode = VFSFile::test_file (file,
         VFSFileTest (VFS_IS_REGULAR | VFS_IS_SYMLINK | VFS_IS_DIR), error);

        if (error)
            AUDERR ("%s: %s\n", file, (const char *) error);

        if (mode & VFS_IS_SYMLINK)
            continue;

        if (mode & VFS_IS_REGULAR)
            add_file ({String (file)}, filter, user, result, true);
        else if (mode & VFS_IS_DIR)
            add_folder (file, filter, user, result, false);
    }
}

static void add_generic (PlaylistAddItem && item, Playlist::FilterFunc filter,
 void * user, AddResult * result, bool save_title, bool allow_playlist)
{
    if (filter && ! filter (item.filename, user))
    {
        result->filtered = true;
        return;
    }

    /* If the item has a valid tuple or known decoder, or it's a subtune, then
     * assume it's a playable file and skip some checks. */
    if (item.tuple.valid () || item.decoder || is_subtune (item.filename))
        add_file (std::move (item), filter, user, result, false);
    else
    {
        String error;
        VFSFileTest mode = VFSFile::test_file (item.filename,
         VFSFileTest (VFS_IS_DIR | VFS_NO_ACCESS), error);

        if (mode & VFS_NO_ACCESS)
            aud_ui_show_error (str_printf (_("Error reading %s:\n%s"),
             (const char *) item.filename, (const char *) error));
        else if (mode & VFS_IS_DIR)
        {
            add_folder (item.filename, filter, user, result, save_title);
            result->saw_folder = true;
        }
        else if (allow_playlist && Playlist::filename_is_playlist (item.filename))
            add_playlist (item.filename, filter, user, result, save_title);
        else
            add_file (std::move (item), filter, user, result, false);
    }
}

static void start_thread_locked ()
{
    if (add_thread_exited)
    {
        pthread_mutex_unlock (& mutex);
        pthread_join (add_thread, nullptr);
        pthread_mutex_lock (& mutex);
    }

    if (! add_thread_started || add_thread_exited)
    {
        pthread_create (& add_thread, nullptr, add_worker, nullptr);
        add_thread_started = true;
        add_thread_exited = false;
    }
}

static void stop_thread_locked ()
{
    if (add_thread_started)
    {
        pthread_mutex_unlock (& mutex);
        pthread_join (add_thread, nullptr);
        pthread_mutex_lock (& mutex);
        add_thread_started = false;
        add_thread_exited = false;
    }
}

static void add_finish (void * unused)
{
    pthread_mutex_lock (& mutex);

    AddResult * result;
    while ((result = add_results.head ()))
    {
        add_results.remove (result);

        PlaylistEx playlist;
        int count;

        if (! result->items.len ())
        {
            if (result->saw_folder && ! result->filtered)
                aud_ui_show_error (_("No files found."));
            goto FREE;
        }

        playlist = result->playlist;
        if (! playlist.exists ()) /* playlist deleted */
            goto FREE;

        if (result->play)
        {
            if (aud_get_bool (nullptr, "clear_playlist"))
                playlist.remove_all_entries ();
            else
                playlist.queue_remove_all ();
        }

        count = playlist.n_entries ();
        if (result->at < 0 || result->at > count)
            result->at = count;

        if (result->title && ! count)
        {
            if (! strcmp (playlist.get_title (), _("New Playlist")))
                playlist.set_title (result->title);
        }

        /* temporarily disable scanning this playlist; the intent is to avoid
         * scanning until the currently playing entry is known, at which time it
         * can be scanned more efficiently (album art read in the same pass). */
        playlist_enable_scan (false);
        playlist.insert_flat_items (result->at, std::move (result->items));

        if (result->play)
        {
            if (! aud_get_bool (0, "shuffle"))
                playlist.set_position (result->at);

            playlist.start_playback ();
        }

        playlist_enable_scan (true);

    FREE:
        delete result;
    }

    if (add_thread_exited)
    {
        stop_thread_locked ();
        status_done_locked ();
    }

    pthread_mutex_unlock (& mutex);

    hook_call ("playlist add complete", nullptr);
}

static void * add_worker (void * unused)
{
    pthread_mutex_lock (& mutex);

    AddTask * task;
    while ((task = add_tasks.head ()))
    {
        add_tasks.remove (task);

        current_playlist = task->playlist;
        pthread_mutex_unlock (& mutex);

        playlist_cache_load (task->items);

        AddResult * result = new AddResult ();

        result->playlist = task->playlist;
        result->at = task->at;
        result->play = task->play;

        bool save_title = (task->items.len () == 1);

        for (auto & item : task->items)
            add_generic (std::move (item), task->filter, task->user, result, save_title, true);

        delete task;

        pthread_mutex_lock (& mutex);
        current_playlist = Playlist ();

        if (! add_results.head ())
            queued_add.queue (add_finish, nullptr);

        add_results.append (result);
    }

    add_thread_exited = true;
    pthread_mutex_unlock (& mutex);
    return nullptr;
}

void adder_cleanup ()
{
    pthread_mutex_lock (& mutex);

    add_tasks.clear ();

    stop_thread_locked ();
    status_done_locked ();

    add_results.clear ();

    queued_add.stop ();

    pthread_mutex_unlock (& mutex);
}

EXPORT void Playlist::insert_entry (int at,
 const char * filename, Tuple && tuple, bool play) const
{
    Index<PlaylistAddItem> items;
    items.append (String (filename), std::move (tuple));

    insert_items (at, std::move (items), play);
}

EXPORT void Playlist::insert_items (int at,
 Index<PlaylistAddItem> && items, bool play) const
{
    insert_filtered (at, std::move (items), nullptr, nullptr, play);
}

EXPORT void Playlist::insert_filtered (int at,
 Index<PlaylistAddItem> && items, Playlist::FilterFunc filter, void * user,
 bool play) const
{
    pthread_mutex_lock (& mutex);

    AddTask * task = new AddTask ();

    task->playlist = * this;
    task->at = at;
    task->play = play;
    task->items = std::move (items);
    task->filter = filter;
    task->user = user;

    add_tasks.append (task);
    start_thread_locked ();

    pthread_mutex_unlock (& mutex);
}

EXPORT bool Playlist::add_in_progress () const
{
    pthread_mutex_lock (& mutex);

    for (AddTask * task = add_tasks.head (); task; task = add_tasks.next (task))
    {
        if (task->playlist == * this)
            goto YES;
    }

    if (current_playlist == * this)
        goto YES;

    for (AddResult * result = add_results.head (); result; result = add_results.next (result))
    {
        if (result->playlist == * this)
            goto YES;
    }

    pthread_mutex_unlock (& mutex);
    return false;

YES:
    pthread_mutex_unlock (& mutex);
    return true;
}

EXPORT bool Playlist::add_in_progress_any ()
{
    pthread_mutex_lock (& mutex);

    bool in_progress = (add_tasks.head () ||
                        current_playlist != Playlist () ||
                        add_results.head ());

    pthread_mutex_unlock (& mutex);
    return in_progress;
}
