/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef __COLORMAPS_H__
#define __COLORMAPS_H__

#include "gimage.h"               /* For the image types  */

/*  This is a macro for arranging the red, green, and blue components into
 *  a value acceptable to the target X server
 */
#define COLOR_COMPOSE(r,g,b) (lookup_red [r] | lookup_green [g] | lookup_blue [b])


extern GdkVisual *g_visual;
extern GdkColormap *g_cmap;

/*  Pixel values of black and white  */
extern gulong g_black_pixel;
extern gulong g_gray_pixel;
extern gulong g_white_pixel;
extern gulong g_color_pixel;
extern gulong g_normal_guide_pixel;
extern gulong g_active_guide_pixel;

/*  Foreground and Background colors  */
extern gulong foreground_pixel;
extern gulong background_pixel;

/*  Old and New colors  */
extern gulong old_color_pixel;
extern gulong new_color_pixel;

/*  Colormap entries reserved for color cycled marching ants--optional  */
extern gulong  marching_ants_pixels[8];

extern GtkDitherInfo *red_ordered_dither;
extern GtkDitherInfo *green_ordered_dither;
extern GtkDitherInfo *blue_ordered_dither;
extern GtkDitherInfo *gray_ordered_dither;

extern guchar ***ordered_dither_matrix;

extern gulong *g_lookup_red;
extern gulong *g_lookup_green;
extern gulong *g_lookup_blue;

extern gulong *color_pixel_vals;
extern gulong *gray_pixel_vals;

gulong get_color (int red, int green, int blue);
void   store_color (gulong *pixel, int red, int green, int blue);
void   get_standard_colormaps (void);


#endif  /*  __COLORMAPS_H__  */
