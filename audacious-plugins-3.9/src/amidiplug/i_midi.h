/*
*
* Author: Giacomo Lozito <james@develia.org>, (C) 2005-2006
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*
*/

#ifndef _I_MIDI_H
#define _I_MIDI_H 1

#include <stdint.h>
#include <libaudcore/index.h>

#include "i_midievent.h"

class VFSFile;


/* sequencer event type, got from ALSA header alsa/seq_event.h */
enum snd_seq_event_type
{
    /* system status; event data type = #snd_seq_result_t */
    SND_SEQ_EVENT_SYSTEM = 0,
    /* returned result status; event data type = #snd_seq_result_t */
    SND_SEQ_EVENT_RESULT,

    /* note on and off with duration; event data type = #snd_seq_ev_note_t */
    SND_SEQ_EVENT_NOTE = 5,
    /* note on; event data type = #snd_seq_ev_note_t */
    SND_SEQ_EVENT_NOTEON,
    /* note off; event data type = #snd_seq_ev_note_t */
    SND_SEQ_EVENT_NOTEOFF,
    /* key pressure change (aftertouch); event data type = #snd_seq_ev_note_t */
    SND_SEQ_EVENT_KEYPRESS,

    /* controller; event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_CONTROLLER = 10,
    /* program change; event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_PGMCHANGE,
    /* channel pressure; event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_CHANPRESS,
    /* pitchwheel; event data type = #snd_seq_ev_ctrl_t; data is from -8192 to 8191) */
    SND_SEQ_EVENT_PITCHBEND,
    /* 14 bit controller value; event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_CONTROL14,
    /* 14 bit NRPN;  event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_NONREGPARAM,
    /* 14 bit RPN; event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_REGPARAM,

    /* SPP with LSB and MSB values; event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_SONGPOS = 20,
    /* Song Select with song ID number; event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_SONGSEL,
    /* midi time code quarter frame; event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_QFRAME,
    /* SMF Time Signature event; event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_TIMESIGN,
    /* SMF Key Signature event; event data type = #snd_seq_ev_ctrl_t */
    SND_SEQ_EVENT_KEYSIGN,

    /* MIDI Real Time Start message; event data type = #snd_seq_ev_queue_control_t */
    SND_SEQ_EVENT_START = 30,
    /* MIDI Real Time Continue message; event data type = #snd_seq_ev_queue_control_t */
    SND_SEQ_EVENT_CONTINUE,
    /* MIDI Real Time Stop message; event data type = #snd_seq_ev_queue_control_t */
    SND_SEQ_EVENT_STOP,
    /* Set tick queue position; event data type = #snd_seq_ev_queue_control_t */
    SND_SEQ_EVENT_SETPOS_TICK,
    /* Set real-time queue position; event data type = #snd_seq_ev_queue_control_t */
    SND_SEQ_EVENT_SETPOS_TIME,
    /* (SMF) Tempo event; event data type = #snd_seq_ev_queue_control_t */
    SND_SEQ_EVENT_TEMPO,
    /* MIDI Real Time Clock message; event data type = #snd_seq_ev_queue_control_t */
    SND_SEQ_EVENT_CLOCK,
    /* MIDI Real Time Tick message; event data type = #snd_seq_ev_queue_control_t */
    SND_SEQ_EVENT_TICK,
    /* Queue timer skew; event data type = #snd_seq_ev_queue_control_t */
    SND_SEQ_EVENT_QUEUE_SKEW,
    /* Sync position changed; event data type = #snd_seq_ev_queue_control_t */
    SND_SEQ_EVENT_SYNC_POS,

    /* Tune request; event data type = none */
    SND_SEQ_EVENT_TUNE_REQUEST = 40,
    /* Reset to power-on state; event data type = none */
    SND_SEQ_EVENT_RESET,
    /* Active sensing event; event data type = none */
    SND_SEQ_EVENT_SENSING,

    /* Echo-back event; event data type = any type */
    SND_SEQ_EVENT_ECHO = 50,
    /* OSS emulation raw event; event data type = any type */
    SND_SEQ_EVENT_OSS,

    /* New client has connected; event data type = #snd_seq_addr_t */
    SND_SEQ_EVENT_CLIENT_START = 60,
    /* Client has left the system; event data type = #snd_seq_addr_t */
    SND_SEQ_EVENT_CLIENT_EXIT,
    /* Client status/info has changed; event data type = #snd_seq_addr_t */
    SND_SEQ_EVENT_CLIENT_CHANGE,
    /* New port was created; event data type = #snd_seq_addr_t */
    SND_SEQ_EVENT_PORT_START,
    /* Port was deleted from system; event data type = #snd_seq_addr_t */
    SND_SEQ_EVENT_PORT_EXIT,
    /* Port status/info has changed; event data type = #snd_seq_addr_t */
    SND_SEQ_EVENT_PORT_CHANGE,

    /* Ports connected; event data type = #snd_seq_connect_t */
    SND_SEQ_EVENT_PORT_SUBSCRIBED,
    /* Ports disconnected; event data type = #snd_seq_connect_t */
    SND_SEQ_EVENT_PORT_UNSUBSCRIBED,

    /* Sample select; event data type = #snd_seq_ev_sample_control_t */
    SND_SEQ_EVENT_SAMPLE = 70,
    /* Sample cluster select; event data type = #snd_seq_ev_sample_control_t */
    SND_SEQ_EVENT_SAMPLE_CLUSTER,
    /* voice start */
    SND_SEQ_EVENT_SAMPLE_START,
    /* voice stop */
    SND_SEQ_EVENT_SAMPLE_STOP,
    /* playback frequency */
    SND_SEQ_EVENT_SAMPLE_FREQ,
    /* volume and balance */
    SND_SEQ_EVENT_SAMPLE_VOLUME,
    /* sample loop */
    SND_SEQ_EVENT_SAMPLE_LOOP,
    /* sample position */
    SND_SEQ_EVENT_SAMPLE_POSITION,
    /* private (hardware dependent) event */
    SND_SEQ_EVENT_SAMPLE_PRIVATE1,

    /* user-defined event; event data type = any (fixed size) */
    SND_SEQ_EVENT_USR0 = 90,
    /* user-defined event; event data type = any (fixed size) */
    SND_SEQ_EVENT_USR1,
    /* user-defined event; event data type = any (fixed size) */
    SND_SEQ_EVENT_USR2,
    /* user-defined event; event data type = any (fixed size) */
    SND_SEQ_EVENT_USR3,
    /* user-defined event; event data type = any (fixed size) */
    SND_SEQ_EVENT_USR4,
    /* user-defined event; event data type = any (fixed size) */
    SND_SEQ_EVENT_USR5,
    /* user-defined event; event data type = any (fixed size) */
    SND_SEQ_EVENT_USR6,
    /* user-defined event; event data type = any (fixed size) */
    SND_SEQ_EVENT_USR7,
    /* user-defined event; event data type = any (fixed size) */
    SND_SEQ_EVENT_USR8,
    /* user-defined event; event data type = any (fixed size) */
    SND_SEQ_EVENT_USR9,

    /* begin of instrument management */
    SND_SEQ_EVENT_INSTR_BEGIN = 100,
    /* end of instrument management */
    SND_SEQ_EVENT_INSTR_END,
    /* query instrument interface info */
    SND_SEQ_EVENT_INSTR_INFO,
    /* result of instrument interface info */
    SND_SEQ_EVENT_INSTR_INFO_RESULT,
    /* query instrument format info */
    SND_SEQ_EVENT_INSTR_FINFO,
    /* result of instrument format info */
    SND_SEQ_EVENT_INSTR_FINFO_RESULT,
    /* reset instrument instrument memory */
    SND_SEQ_EVENT_INSTR_RESET,
    /* get instrument interface status */
    SND_SEQ_EVENT_INSTR_STATUS,
    /* result of instrument interface status */
    SND_SEQ_EVENT_INSTR_STATUS_RESULT,
    /* put an instrument to port */
    SND_SEQ_EVENT_INSTR_PUT,
    /* get an instrument from port */
    SND_SEQ_EVENT_INSTR_GET,
    /* result of instrument query */
    SND_SEQ_EVENT_INSTR_GET_RESULT,
    /* free instrument(s) */
    SND_SEQ_EVENT_INSTR_FREE,
    /* get instrument list */
    SND_SEQ_EVENT_INSTR_LIST,
    /* result of instrument list */
    SND_SEQ_EVENT_INSTR_LIST_RESULT,
    /* set cluster parameters */
    SND_SEQ_EVENT_INSTR_CLUSTER,
    /* get cluster parameters */
    SND_SEQ_EVENT_INSTR_CLUSTER_GET,
    /* result of cluster parameters */
    SND_SEQ_EVENT_INSTR_CLUSTER_RESULT,
    /* instrument change */
    SND_SEQ_EVENT_INSTR_CHANGE,

    /* system exclusive data (variable length);  event data type = #snd_seq_ev_ext_t */
    SND_SEQ_EVENT_SYSEX = 130,
    /* error event;  event data type = #snd_seq_ev_ext_t */
    SND_SEQ_EVENT_BOUNCE,
    /* reserved for user apps;  event data type = #snd_seq_ev_ext_t */
    SND_SEQ_EVENT_USR_VAR0 = 135,
    /* reserved for user apps; event data type = #snd_seq_ev_ext_t */
    SND_SEQ_EVENT_USR_VAR1,
    /* reserved for user apps; event data type = #snd_seq_ev_ext_t */
    SND_SEQ_EVENT_USR_VAR2,
    /* reserved for user apps; event data type = #snd_seq_ev_ext_t */
    SND_SEQ_EVENT_USR_VAR3,
    /* reserved for user apps; event data type = #snd_seq_ev_ext_t */
    SND_SEQ_EVENT_USR_VAR4,

    /* added to support meta-event - general purpose text */
    SND_SEQ_EVENT_META_TEXT = 150,
    /* added to support meta-event - lyrics */
    SND_SEQ_EVENT_META_LYRIC,

    /* NOP; ignored in any case */
    SND_SEQ_EVENT_NONE = 255
};


struct midifile_track_t
{
    List<midievent_t> events;           /* list of all events in this track */
    int start_tick;                     /* start of this track */
    int end_tick;			/* length of this track */
    midievent_t * current_event;	/* used while loading and playing */

    midievent_t * add_event ()
    {
        midievent_t * event = new midievent_t ();
        events.append (event);
        return event;
    }

    ~midifile_track_t ()
        { events.clear (); }
};


struct midifile_t
{
    Index<midifile_track_t> tracks;

    unsigned short format = 0;
    int start_tick = 0;
    int max_tick = 0;
    int smpte_timing = 0;

    int time_division = 0;
    int ppq = 0;
    int current_tempo = 0;

    int avg_microsec_per_tick = 0;
    int64_t length = 0;

    void get_bpm (int *, int *);
    bool parse_from_file (const char *, VFSFile & file);

private:
    String file_name;
    Index<char> file_data;
    int file_offset = 0;
    bool file_eof = false;

    void skip_bytes (int);
    int read_byte ();
    int read_32_le ();
    int read_id ();
    int read_int (int);
    int read_var ();
    bool read_track (midifile_track_t &, int, int);
    bool parse_smf (int);
    bool parse_riff ();
    bool setget_tempo ();
    void setget_length ();
};

#endif /* !_I_MIDI_H */
