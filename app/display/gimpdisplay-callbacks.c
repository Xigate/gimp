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
#include <stdlib.h>
#include "gdk/gdkkeysyms.h"
#include "appenv.h"
#include "colormaps.h"
#include "cursorutil.h"
#include "devices.h"
#include "disp_callbacks.h"
#include "gdisplay.h"
#include "general.h"
#include "gimpcontext.h"
#include "gimprc.h"
#include "interface.h"
#include "layer_select.h"
#include "move.h"
#include "scale.h"
#include "scroll.h"
#include "tools.h"
#include "gimage.h"
#include "dialog_handler.h"

#define HORIZONTAL  1
#define VERTICAL    2

/* Function declarations */

static void gdisplay_check_device_cursor (GDisplay *gdisp);

static void
redraw (GDisplay *gdisp,
	int       x,
	int       y,
	int       w,
	int       h)
{
  long x1, y1, x2, y2;    /*  coordinate of rectangle corners  */

  x1 = x;
  y1 = y;
  x2 = (x+w);
  y2 = (y+h);
  x1 = BOUNDS (x1, 0, gdisp->disp_width);
  y1 = BOUNDS (y1, 0, gdisp->disp_height);
  x2 = BOUNDS (x2, 0, gdisp->disp_width);
  y2 = BOUNDS (y2, 0, gdisp->disp_height);
  if ((x2 - x1) && (y2 - y1))
    {
      gdisplay_expose_area (gdisp, x1, y1, (x2 - x1), (y2 - y1));
      gdisplay_flush_displays_only (gdisp);
    }
}

static void
gdisplay_check_device_cursor (GDisplay *gdisp)
{
  GList *tmp_list;

  /* gdk_input_list_devices returns an internal list, so we shouldn't
     free it afterwards */
  tmp_list = gdk_input_list_devices();

  while (tmp_list)
    {
      GdkDeviceInfo *info = (GdkDeviceInfo *)tmp_list->data;

      if (info->deviceid == current_device)
	{
	  gdisp->draw_cursor = !info->has_cursor;
	  break;
	}
      
      tmp_list = tmp_list->next;
    }
}

static int
key_to_state (int key)
{
  switch (key)
  {
   case GDK_Alt_L: case GDK_Alt_R:
     return GDK_MOD1_MASK;
   case GDK_Shift_L: case GDK_Shift_R:
     return GDK_SHIFT_MASK;
   case GDK_Control_L: case GDK_Control_R:
     return GDK_CONTROL_MASK;
   default:
     return 0;
  }
}

gint
gdisplay_shell_events (GtkWidget *w,
		       GdkEvent  *event,
		       GDisplay  *gdisp)
{
  switch (event->type)
    {
    case GDK_BUTTON_PRESS:
    case GDK_KEY_PRESS:
      /*  Setting the context's display automatically sets the image, too  */
      gimp_context_set_display (gimp_context_get_user (), gdisp);
      break;
    default:
      break;
    }

  return FALSE;
}

gint
gdisplay_canvas_events (GtkWidget *canvas,
			GdkEvent  *event)
{
  GDisplay *gdisp;
  GdkEventExpose *eevent;
  GdkEventMotion *mevent;
  GdkEventButton *bevent;
  GdkEventKey *kevent;
  gdouble tx, ty;
  guint state = 0;
  gint return_val = FALSE;
  static gboolean scrolled = FALSE;
  static guint key_signal_id = 0;
  int update_cursor = FALSE;

  tx = ty = 0;

  gdisp = (GDisplay *) gtk_object_get_user_data (GTK_OBJECT (canvas));

  if (!canvas->window) 
    return FALSE;

  /*  If this is the first event...  */
  if (!gdisp->select)
    {
      /*  create the selection object  */
      gdisp->select = selection_create (gdisp->canvas->window, gdisp,
					gdisp->gimage->height,
					gdisp->gimage->width, marching_speed);

      gdisp->disp_width = gdisp->canvas->allocation.width;
      gdisp->disp_height = gdisp->canvas->allocation.height;

      /*  create GC for scrolling */
 
      gdisp->scroll_gc = gdk_gc_new (gdisp->canvas->window);
      gdk_gc_set_exposures (gdisp->scroll_gc, TRUE);

      /*  set up the scrollbar observers  */
      gtk_signal_connect (GTK_OBJECT (gdisp->hsbdata), "value_changed",
			  (GtkSignalFunc) scrollbar_horz_update,
			  gdisp);
      gtk_signal_connect (GTK_OBJECT (gdisp->vsbdata), "value_changed",
			  (GtkSignalFunc) scrollbar_vert_update,
			  gdisp);

      /*  setup scale properly  */
      setup_scale (gdisp);
    }

  /* Find out what device the event occurred upon */
  
  if (devices_check_change (event))
    gdisplay_check_device_cursor (gdisp);
  
  switch (event->type)
    {
    case GDK_EXPOSE:
      eevent = (GdkEventExpose *) event;
      /*printf(" EXP:%d,%d(%dx%d) ",eevent->area.x, eevent->area.y,
	eevent->area.width, eevent->area.height);fflush(stdout);*/
      redraw (gdisp, eevent->area.x, eevent->area.y,
	      eevent->area.width, eevent->area.height);
      break;

    case GDK_CONFIGURE:
      /*printf(" CNF ");fflush(stdout);*/
      if ((gdisp->disp_width != gdisp->canvas->allocation.width) ||
	  (gdisp->disp_height != gdisp->canvas->allocation.height))
	{
	  gdisp->disp_width = gdisp->canvas->allocation.width;
	  gdisp->disp_height = gdisp->canvas->allocation.height;
	  resize_display (gdisp, 0, FALSE);
	}
      break;

    case GDK_LEAVE_NOTIFY:
      gdisplay_update_cursor (gdisp, 0, 0);
      gtk_label_set_text (GTK_LABEL (gdisp->cursor_label), "");
    case GDK_PROXIMITY_OUT:
      gdisp->proximity = FALSE;
      break;

    case GDK_ENTER_NOTIFY:
      /* Actually, should figure out tx,ty here */
      break;

    case GDK_BUTTON_PRESS:
      bevent = (GdkEventButton *) event;
      state = bevent->state;

      switch (bevent->button)
	{
	case 1:
	  state |= GDK_BUTTON1_MASK;
	  gtk_grab_add (canvas);

	  /* This is a hack to prevent other stuff being run in the middle of
	     a tool operation (like changing image types.... brrrr). We just
	     block all the keypress event. A better solution is to implement
	     some sort of locking for images.
	     Note that this is dependent on specific GTK behavior, and isn't
	     guaranteed to work in future versions of GTK.
	     -Yosh
	   */
	  if (key_signal_id == 0)
	    key_signal_id = gtk_signal_connect (GTK_OBJECT (canvas),
						"key_press_event",
						GTK_SIGNAL_FUNC (gtk_true),
						NULL);

	  if (active_tool && ((active_tool->type == MOVE) ||
			      !gimage_is_empty (gdisp->gimage)))
	      {
		if (active_tool->auto_snap_to)
		  {
		    gdisplay_snap_point (gdisp, bevent->x, bevent->y, &tx, &ty);
		    bevent->x = tx;
		    bevent->y = ty;
		    update_cursor = TRUE;
		  }
		
		/* reset the current tool if we're changing gdisplays */
		/*
		if (active_tool->gdisp_ptr) {
		  tool_gdisp = active_tool->gdisp_ptr;
		  if (tool_gdisp != gdisp) {
		    tools_initialize (active_tool->type, gdisp);
		    active_tool->drawable = gimage_active_drawable(gdisp->gimage);
		  }
		} else
		*/
		/* reset the current tool if we're changing drawables */
		  if (active_tool->drawable) {
		    if (((gimage_active_drawable(gdisp->gimage)) !=
			 active_tool->drawable) &&
			!active_tool->preserve)
		      tools_initialize (active_tool->type, gdisp);
		  } else
		    active_tool->drawable = gimage_active_drawable(gdisp->gimage);
		
		(* active_tool->button_press_func) (active_tool, bevent, gdisp);
	      }
	  break;

	case 2:
	  state |= GDK_BUTTON2_MASK;
	  scrolled = TRUE;
	  gtk_grab_add (canvas);
	  start_grab_and_scroll (gdisp, bevent);
	  break;

	case 3:
	  state |= GDK_BUTTON3_MASK;
	  gtk_menu_popup (GTK_MENU (gdisp->popup),
			  NULL, NULL, NULL, NULL, 3, bevent->time);
	  break;

	default:
	  break;
	}
      break;

    case GDK_BUTTON_RELEASE:
      bevent = (GdkEventButton *) event;
      state = bevent->state;

      switch (bevent->button)
	{
	case 1:
	  state &= ~GDK_BUTTON1_MASK;
	  
	  /* Lame hack. See above */
	  if (key_signal_id)
	    {
	      gtk_signal_disconnect (GTK_OBJECT (canvas), key_signal_id);
	      key_signal_id = 0;
	    }

	  gtk_grab_remove (canvas);
	  gdk_pointer_ungrab (bevent->time);  /* fixes pointer grab bug */
	  if (active_tool && ((active_tool->type == MOVE) ||
			      !gimage_is_empty (gdisp->gimage)))
	    {
	      if (active_tool->state == ACTIVE)
		{
		  if (active_tool->auto_snap_to)
		    {
		      gdisplay_snap_point (gdisp, bevent->x, bevent->y, &tx, &ty);
		      bevent->x = tx;
		      bevent->y = ty;
		      update_cursor = TRUE;
		    }
		  
		  (* active_tool->button_release_func) (active_tool, bevent,
							gdisp);
		}
	    }
	  break;

	case 2:
	  state &= ~GDK_BUTTON2_MASK;
	  scrolled = FALSE;
	  gtk_grab_remove (canvas);
	  end_grab_and_scroll (gdisp, bevent);
	  break;

	case 3:
	  state &= ~GDK_BUTTON3_MASK;
	  break;

	default:
	  break;
	}
      break;

    case GDK_MOTION_NOTIFY:
      mevent = (GdkEventMotion *) event;
      state = mevent->state;

     /* Ask for the pointer position, but ignore it except for cursor
      * handling, so motion events sync with the button press/release events */

      if (mevent->is_hint)
	gdk_input_window_get_pointer (canvas->window, current_device, &tx, &ty,
				      NULL, NULL, NULL, NULL);
      else
	{
	  tx = mevent->x;
	  ty = mevent->y;
	}
      update_cursor = TRUE;

      if (!gdisp->proximity)
	{
	  gdisp->proximity = TRUE;
	  gdisplay_check_device_cursor (gdisp);
	}
      
      if (active_tool && ((active_tool->type == MOVE) ||
			  !gimage_is_empty (gdisp->gimage)) &&
	  (mevent->state & GDK_BUTTON1_MASK))
	{
	  if (active_tool->state == ACTIVE)
	    {
	      /*  if the first mouse button is down, check for automatic
	       *  scrolling...
	       */
	      if ((mevent->state & GDK_BUTTON1_MASK) && !active_tool->scroll_lock)
		{
		  if (mevent->x < 0 || mevent->y < 0 ||
		      mevent->x > gdisp->disp_width ||
		      mevent->y > gdisp->disp_height)
		    scroll_to_pointer_position (gdisp, mevent);
		}

	      if (active_tool->auto_snap_to)
		{
		  gdisplay_snap_point (gdisp, mevent->x, mevent->y, &tx, &ty);
		  mevent->x = tx;
		  mevent->y = ty;
		  update_cursor = TRUE;
		}

	      (* active_tool->motion_func) (active_tool, mevent, gdisp);
	    }
	}
      else if ((mevent->state & GDK_BUTTON2_MASK) && scrolled)
	{
	  grab_and_scroll (gdisp, mevent);
	}
      break;

    case GDK_KEY_PRESS:
      kevent = (GdkEventKey *) event;
      state = kevent->state;

      switch (kevent->keyval)
	{
	case GDK_Left: case GDK_Right:
	case GDK_Up: case GDK_Down:
	  if (active_tool && !gimage_is_empty (gdisp->gimage))
	    (* active_tool->arrow_keys_func) (active_tool, kevent, gdisp);
	  return_val = TRUE;
	  break;

	case GDK_Tab:
	  if (kevent->state & GDK_MOD1_MASK && !gimage_is_empty (gdisp->gimage))
	    layer_select_init (gdisp->gimage, 1, kevent->time);
	  if (kevent->state & GDK_CONTROL_MASK && !gimage_is_empty (gdisp->gimage))
	    layer_select_init (gdisp->gimage, -1, kevent->time);

	  /* Hide or show all dialogs */
	  if (!kevent->state)
	    dialog_toggle();

	  return_val = TRUE;
	  break;

	  /*  Update the state based on modifiers being pressed  */
	case GDK_Alt_L: case GDK_Alt_R:
	case GDK_Shift_L: case GDK_Shift_R:
	case GDK_Control_L: case GDK_Control_R:
	  state |= key_to_state(kevent->keyval);
	  /* For all modifier keys: call the tools modifier_key_func */
	  if (active_tool && !gimage_is_empty (gdisp->gimage))
	    {
	      gdk_input_window_get_pointer (canvas->window, current_device, 
					    &tx, &ty, NULL, NULL, NULL, NULL);
	      (* active_tool->modifier_key_func) (active_tool, kevent, gdisp);
	      return_val = TRUE;
	    }
	  break;
	}
      break;

    case GDK_KEY_RELEASE:
      kevent = (GdkEventKey *) event;
      state = kevent->state;

      switch (kevent->keyval)
	{
	case GDK_Alt_L: case GDK_Alt_R:
	case GDK_Shift_L: case GDK_Shift_R:
	case GDK_Control_L: case GDK_Control_R:
	  state &= ~key_to_state(kevent->keyval);
	  /* For all modifier keys: call the tools modifier_key_func */
	  if (active_tool && !gimage_is_empty (gdisp->gimage))
	    {
	      gdk_input_window_get_pointer (canvas->window, current_device, 
					    &tx, &ty, NULL, NULL, NULL, NULL);
	      (* active_tool->modifier_key_func) (active_tool, kevent, gdisp);
	      return_val = TRUE;
	    }
	  break;
	}

      return_val = TRUE;
      break;

    default:
      break;
    }

  if (no_cursor_updating == 0)
    {
      if (active_tool && !gimage_is_empty (gdisp->gimage) &&
	  !(state & (GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK)))
	{
	  GdkEventMotion me;
	  me.x = tx;  me.y = ty;
	  me.state = state;
	  (* active_tool->cursor_update_func) (active_tool, &me, gdisp);
	}
      else if (gimage_is_empty (gdisp->gimage))
	gdisplay_install_tool_cursor (gdisp, GDK_TOP_LEFT_ARROW);
    }

  if (update_cursor)
    gdisplay_update_cursor (gdisp, tx, ty);

  return return_val;
}

gint
gdisplay_hruler_button_press (GtkWidget      *widget,
			      GdkEventButton *event,
			      gpointer        data)
{
  GDisplay *gdisp;

  if (event->button == 1)
    {
      gdisp = data;

      gtk_widget_activate (tool_info[(int) MOVE].tool_widget);
      move_tool_start_hguide (active_tool, gdisp);
      gtk_grab_add (gdisp->canvas);
    }

  return FALSE;
}

gint
gdisplay_vruler_button_press (GtkWidget      *widget,
			      GdkEventButton *event,
			      gpointer        data)
{
  GDisplay *gdisp;

  if (event->button == 1)
    {
      gdisp = data;

      gtk_widget_activate (tool_info[(int) MOVE].tool_widget);
      move_tool_start_vguide (active_tool, gdisp);
      gtk_grab_add (gdisp->canvas);
    }

  return FALSE;
}

gint
gdisplay_origin_button_press (GtkWidget      *widget,
			      GdkEventButton *event,
			      gpointer        data)
{
  GDisplay *gdisp;

  if (event->button == 1)
    {
      gdisp = data;
      gtk_menu_popup (GTK_MENU (gdisp->popup),
		      NULL, NULL, NULL, NULL, 1, event->time);
    }

  /* Stop the signal emission so the button doesn't grab the
   * pointer away from us */
  gtk_signal_emit_stop_by_name (GTK_OBJECT (widget), "button_press_event");

  return FALSE;
}
