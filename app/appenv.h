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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifndef __APPENV_H__
#define __APPENV_H__

#include "glib.h"

/* The GIMP shouldn't need to know much about X11 (or Windows), so
 * I'll remove this inclusion of gdkx.h. This will speed up compilations
 * a bit, too. If some source file needs gdkx.h, it can include it.
 */
#if 0
#include "gdk/gdkx.h"
#endif

#include "gtk/gtk.h"
#include "gimpsetF.h"
#include "colormap_dialog.t.h"

/* Without gdkx.h no GDK_DISPLAY() */
#if 0
#define DISPLAY              ((Display *) GDK_DISPLAY())
#endif

/*   important macros - we reuse the ones from glib */
#define BOUNDS(a,x,y)  CLAMP(a,x,y)
#define MINIMUM(x,y)   MIN(x,y)
#define MAXIMUM(x,y)   MAX(x,y)

/* limit a (0->511) int to 255 */
#define MAX255(a)  ((a) | (((a) & 256) - (((a) & 256) >> 8)))

/* clamp a >>int32<<-range int between 0 and 255 inclusive */
/* broken! -> #define CLAMP0255(a)  ((a & 0xFFFFFF00)? (~(a>>31)) : a) */
#define CLAMP0255(a)  CLAMP(a,0,255)


typedef enum {
  MESSAGE_BOX,
  CONSOLE,
  ERROR_CONSOLE
} MessageHandlerType;

extern int no_interface;
extern int no_splash;
extern int no_splash_image;
extern int no_data;
extern int be_verbose;
extern int use_debug_handler;
extern int console_messages;
extern int restore_session;
extern int we_are_exiting;       /* this is used in session_get_window_info() */
extern GimpSet* image_context;
extern MessageHandlerType message_handler;

#endif /*  APPENV_H  */
