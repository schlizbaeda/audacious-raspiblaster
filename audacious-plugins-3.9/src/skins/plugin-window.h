/*
 * plugin-window.h
 * Copyright 2014 John Lindgren
 *
 * This file is part of Audacious.
 *
 * Audacious is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 2 or version 3 of the License.
 *
 * Audacious is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Audacious. If not, see <http://www.gnu.org/licenses/>.
 *
 * The Audacious team does not consider modular code linking to Audacious or
 * using our public API to be a derived work.
 */

#ifndef SKINS_PLUGIN_WINDOW_H
#define SKINS_PLUGIN_WINDOW_H

class PluginHandle;

void create_plugin_windows ();
void show_plugin_windows ();
void focus_plugin_window (PluginHandle * plugin);
void hide_plugin_windows ();
void destroy_plugin_windows ();

#endif /* SKINS_PLUGIN_WINDOW_H */
