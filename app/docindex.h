/* docindex.h - Header file for document index in gimp.
 *
 * Copyright (C) 1998 Chris Lahey.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __DOCINDEX_H__
#define __DOCINDEX_H__

#include <gtk/gtk.h>

typedef struct idea_manager
{
  /* The scrollbar */
  GtkWidget *vscrollbar,
    /* The GTK window */
    *window;
  GtkWidget *tree;
  /* The menufactory */
  GtkMenuFactory *menubar;
  /* The window menu widget */
  GtkWidget *window_menu;
  /* The status bar widget */
  GtkWidget *status;
  /* The main status context id */
  guint contextid;
  gboolean changed;
  gboolean named;
  gchar *title;
  gint auto_save;
  gint long_term_auto_save;
  gint count;
} idea_manager;

void make_idea_window( gint x, gint y );
void open_idea_window();
void close_idea_window();
void idea_add( gchar *label );
void idea_add_in_position( gchar *label, gint position );
void raise_idea_callback( GtkWidget *widget, gpointer data );

extern idea_manager *ideas;

#endif /* __DOCINDEX_H__ */
