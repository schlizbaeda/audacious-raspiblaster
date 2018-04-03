/*
 * surface.h
 * Copyright 2011 John Lindgren
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

#ifndef SKINS_SURFACE_H
#define SKINS_SURFACE_H

#include <stdint.h>
#include <cairo.h>

cairo_surface_t * surface_new (int w, int h);
cairo_surface_t * surface_new_from_file (const char * name);
uint32_t surface_get_pixel (cairo_surface_t * s, int x, int y);
void surface_copy_rect (cairo_surface_t * a, int ax, int ay, int w, int h,
 cairo_surface_t * b, int bx, int by);

#endif
