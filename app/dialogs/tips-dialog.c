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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <gtk/gtk.h>

#include "libgimpbase/gimpbase.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "core/core-types.h"

#include "tips-dialog.h"
#include "tips-parser.h"

#include "gimprc.h"

#include "libgimp/gimpintl.h"


static void  tips_dialog_destroy (GtkWidget   *widget,
                                  gpointer     data);
static void  tips_show_previous  (GtkWidget   *widget,
                                  gpointer     data);
static void  tips_show_next      (GtkWidget   *widget,
                                  gpointer     data);
static void  tips_toggle_update  (GtkWidget   *widget,
                                  gpointer     data);



static GtkWidget *tips_dialog   = NULL;
static GtkWidget *welcome_label = NULL;
static GtkWidget *thetip_label  = NULL;
static GList     *tips          = NULL;
static GList     *current_tip   = NULL;
static gint       tips_count    = 0;
static gint       old_show_tips = 0;


GtkWidget *
tips_dialog_create (void)
{
  GtkWidget *vbox;
  GtkWidget *vbox2;
  GtkWidget *hbox;
  GtkWidget *bbox;
  GtkWidget *button;
  GdkPixbuf *wilber;
  GimpTip   *tip;
  gchar     *filename;

  if (!tips)
    {
      GError *error = NULL;
      gchar  *filename;

      filename = g_build_filename (gimp_data_directory (), "tips", 
                                   "gimp-tips.xml", NULL);

      tips = gimp_tips_from_file (filename, &error);
      g_free (filename);

      if (error)
        {
          tips = g_list_prepend (tips, 
                                 gimp_tip_new (_("The GIMP tips file could not be parsed correctly!"), error->message));
          g_error_free (error);
        }
    }

  tips_count = g_list_length (tips);

  if (gimprc.last_tip >= tips_count || gimprc.last_tip < 0)
    gimprc.last_tip = 0;

  current_tip = g_list_nth (tips, gimprc.last_tip);

  if (tips_dialog)
    return tips_dialog;

  tips_dialog = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_type_hint (GTK_WINDOW (tips_dialog),
			    GDK_WINDOW_TYPE_HINT_DIALOG);
  gtk_window_set_wmclass (GTK_WINDOW (tips_dialog), "tip_of_the_day", "Gimp");
  gtk_window_set_title (GTK_WINDOW (tips_dialog), _("GIMP Tip of the Day"));
  gtk_window_set_position (GTK_WINDOW (tips_dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable (GTK_WINDOW (tips_dialog), TRUE);

  g_signal_connect (G_OBJECT (tips_dialog), "delete_event",
                    G_CALLBACK (gtk_widget_destroy),
                    NULL);

  g_signal_connect (G_OBJECT (tips_dialog), "destroy",
		    G_CALLBACK (tips_dialog_destroy),
		    NULL);

  /* destroy the tips window if the mainlevel gtk_main() function is left */
  gtk_quit_add_destroy (1, GTK_OBJECT (tips_dialog));

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (tips_dialog), vbox);      
  gtk_widget_show (vbox);

  hbox = gtk_hbox_new (FALSE, 4);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);
  gtk_widget_show (hbox);

  vbox2 = gtk_vbox_new (FALSE, 6);
  gtk_box_pack_start (GTK_BOX (hbox), vbox2, TRUE, TRUE, 0);
  gtk_widget_show (vbox2);

  tip = (GimpTip *) current_tip->data;

  welcome_label = gtk_label_new (tip->welcome);
  gtk_label_set_justify (GTK_LABEL (welcome_label), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (welcome_label), TRUE);
  gtk_misc_set_alignment (GTK_MISC (welcome_label), 0.5, 0.5);
  gtk_box_pack_start (GTK_BOX (vbox2), welcome_label, FALSE, FALSE, 0);
  if (tip->welcome)
    gtk_widget_show (welcome_label);

  thetip_label = gtk_label_new (tip->thetip);
  gtk_label_set_justify (GTK_LABEL (thetip_label), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (thetip_label), TRUE);
  gtk_misc_set_alignment (GTK_MISC (thetip_label), 0.5, 0.5);
  gtk_box_pack_start (GTK_BOX (vbox2), thetip_label, TRUE, TRUE, 0);
  gtk_widget_show (thetip_label);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_end (GTK_BOX (hbox), vbox2, FALSE, FALSE, 0);
  gtk_widget_show (vbox2);

  filename = g_build_filename (gimp_data_directory (), 
                               "images", "wilber-tips.png", NULL);
  wilber = gdk_pixbuf_new_from_file (filename, NULL);
  g_free (filename);

  if (wilber)
    {
      GtkWidget *image;

      image = gtk_image_new_from_pixbuf (wilber);
      g_object_unref (wilber);

      gtk_box_pack_start (GTK_BOX (vbox2), image, TRUE, FALSE, 0);
      gtk_widget_show (image);
    }

  hbox = gtk_hbox_new (FALSE, 15);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 10);
  gtk_box_pack_end (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  button = 
    gtk_check_button_new_with_mnemonic (_("Show tip next time GIMP starts"));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), gimprc.show_tips);
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  g_signal_connect (G_OBJECT (button), "toggled",
		    G_CALLBACK (tips_toggle_update),
		    &gimprc.show_tips);

  old_show_tips = gimprc.show_tips;

  bbox = gtk_hbutton_box_new ();
  gtk_box_pack_end (GTK_BOX (hbox), bbox, FALSE, FALSE, 0);
  gtk_widget_show (bbox);

  button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
  GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
  gtk_window_set_default (GTK_WINDOW (tips_dialog), button);
  gtk_container_add (GTK_CONTAINER (bbox), button);
  gtk_widget_show (button);

  g_signal_connect_swapped (G_OBJECT (button), "clicked",
			    G_CALLBACK (gtk_widget_destroy),
			    tips_dialog);

  bbox = gtk_hbutton_box_new ();
  gtk_button_box_set_layout (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_END);
  gtk_box_set_spacing (GTK_BOX (bbox), 5);
  gtk_box_pack_end (GTK_BOX (hbox), bbox, FALSE, FALSE, 0);
  gtk_widget_show (bbox);

  button = gtk_button_new_with_mnemonic (_("_Previous Tip"));
  GTK_WIDGET_UNSET_FLAGS (button, GTK_RECEIVES_DEFAULT);
  gtk_widget_set_sensitive (button, (tips_count > 1));
  gtk_container_add (GTK_CONTAINER (bbox), button);
  gtk_widget_show (button);

  g_signal_connect (G_OBJECT (button), "clicked",
		    G_CALLBACK (tips_show_previous),
		    NULL);

  button = gtk_button_new_with_mnemonic (_("_Next Tip"));
  GTK_WIDGET_UNSET_FLAGS (button, GTK_RECEIVES_DEFAULT);
  gtk_widget_set_sensitive (button, (tips_count > 1));
  gtk_container_add (GTK_CONTAINER (bbox), button);
  gtk_widget_show (button);

  g_signal_connect (G_OBJECT (button), "clicked",
		    G_CALLBACK (tips_show_next),
		    NULL);

  /*  Connect the "F1" help key  */
  gimp_help_connect (tips_dialog,
		     gimp_standard_help_func,
		     "dialogs/tip_of_the_day.html");

  return tips_dialog;
}

static void
tips_dialog_destroy (GtkWidget *widget,
		     gpointer   data)
{
  GList *update = NULL; /* options that should be updated in .gimprc */
  GList *remove = NULL; /* options that should be commented out */

  tips_dialog = NULL;

  current_tip = NULL;
  gimp_tips_free (tips);
  tips = NULL;

  /* the last-shown-tip is now saved in sessionrc */
  gimprc.last_tip = g_list_position (tips, current_tip);

  if (gimprc.show_tips != old_show_tips)
    {
      update = g_list_append (update, "show-tips");
      remove = g_list_append (remove, "dont-show-tips");
      old_show_tips = gimprc.show_tips;
      gimprc_save (&update, &remove);
    }
  g_list_free (update);
  g_list_free (remove);
}

static void
tips_set_labels (GimpTip *tip)
{
  g_return_if_fail (tip != NULL);

  if (tip->welcome)
    gtk_widget_show (welcome_label);
  else
    gtk_widget_hide (welcome_label);
    
  gtk_label_set_text (GTK_LABEL (welcome_label), tip->welcome);
  gtk_label_set_text (GTK_LABEL (thetip_label), tip->thetip);
}

static void
tips_show_previous (GtkWidget *widget,
		    gpointer  data)
{
  current_tip = current_tip->prev ? current_tip->prev : g_list_last (tips);

  tips_set_labels (current_tip->data);
}

static void
tips_show_next (GtkWidget *widget,
		gpointer   data)
{
  current_tip = current_tip->next ? current_tip->next : tips;

  tips_set_labels (current_tip->data);
}

static void
tips_toggle_update (GtkWidget *widget,
		    gpointer   data)
{
  gint *toggle_val;

  toggle_val = (gint *) data;

  if (GTK_TOGGLE_BUTTON (widget)->active)
    *toggle_val = TRUE;
  else
    *toggle_val = FALSE;
}
