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

#include <string.h>

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "actions-types.h"

#include "config/gimpguiconfig.h"

#include "core/gimp.h"
#include "core/gimpcontainer.h"
#include "core/gimpimage.h"
#include "core/gimpprogress.h"
#include "core/gimptemplate.h"

#include "file/file-open.h"
#include "file/file-save.h"
#include "file/file-utils.h"

#include "widgets/gimpdialogfactory.h"
#include "widgets/gimpfiledialog.h"
#include "widgets/gimphelp-ids.h"

#include "display/gimpdisplay.h"
#include "display/gimpdisplay-foreach.h"

#include "dialogs/dialogs.h"

#include "actions.h"
#include "file-commands.h"

#include "gimp-intl.h"


#define REVERT_DATA_KEY "revert-confirm-dialog"


/*  local function prototypes  */

static void   file_open_dialog_show        (GtkWidget   *parent,
                                            GimpImage   *gimage,
                                            const gchar *uri,
                                            gboolean     open_as_layer);
static void   file_save_dialog_show        (GimpImage   *gimage,
                                            GtkWidget   *parent,
                                            const gchar *title,
                                            gboolean     set_uri_and_proc,
                                            gboolean     set_image_clean);
static void   file_save_dialog_destroyed   (GtkWidget   *dialog,
                                            GimpImage   *gimage);
static void   file_new_template_callback   (GtkWidget   *widget,
                                            const gchar *name,
                                            gpointer     data);
static void   file_revert_confirm_callback (GtkWidget   *widget,
                                            gboolean     revert,
                                            gpointer     data);


/*  public functions  */

void
file_open_cmd_callback (GtkAction *action,
                        gpointer   data)
{
  GtkWidget *widget;
  return_if_no_widget (widget, data);

  file_open_dialog_show (widget, NULL, NULL, FALSE);
}

void
file_open_from_image_cmd_callback (GtkAction *action,
                                   gpointer   data)
{
  GtkWidget *widget;
  return_if_no_widget (widget, data);

  file_open_dialog_show (widget, action_data_get_image (data), NULL, FALSE);
}

void
file_open_as_layer_cmd_callback (GtkAction *action,
                                 gpointer   data)
{
  GimpDisplay *gdisp;
  GtkWidget   *widget;
  return_if_no_display (gdisp, data);
  return_if_no_widget (widget, data);

  file_open_dialog_show (widget, gdisp->gimage, NULL, TRUE);
}

void
file_open_location_cmd_callback (GtkAction *action,
                                 gpointer   data)
{
  GtkWidget *widget;
  return_if_no_widget (widget, data);

  gimp_dialog_factory_dialog_new (global_dialog_factory,
                                  gtk_widget_get_screen (widget),
                                  "gimp-file-open-location-dialog", -1, TRUE);
}

void
file_last_opened_cmd_callback (GtkAction *action,
                               gint       value,
                               gpointer   data)
{
  Gimp          *gimp;
  GimpImagefile *imagefile;
  gint           num_entries;
  return_if_no_gimp (gimp, data);

  num_entries = gimp_container_num_children (gimp->documents);

  if (value >= num_entries)
    return;

  imagefile = (GimpImagefile *)
    gimp_container_get_child_by_index (gimp->documents, value);

  if (imagefile)
    {
      GimpImage         *gimage;
      GimpPDBStatusType  status;
      GError            *error = NULL;

      gimage = file_open_with_display (gimp, action_data_get_context (data),
                                       NULL,
                                       GIMP_OBJECT (imagefile)->name,
                                       &status, &error);

      if (! gimage && status != GIMP_PDB_CANCEL)
        {
          gchar *filename;

          filename =
            file_utils_uri_to_utf8_filename (GIMP_OBJECT (imagefile)->name);

          g_message (_("Opening '%s' failed:\n\n%s"),
                     filename, error->message);
          g_clear_error (&error);

          g_free (filename);
        }
    }
}

void
file_save_cmd_callback (GtkAction *action,
                        gpointer   data)
{
  GimpDisplay *gdisp;
  return_if_no_display (gdisp, data);

  g_return_if_fail (gimp_image_active_drawable (gdisp->gimage));

  /*  Only save if the gimage has been modified  */
  if (gdisp->gimage->dirty ||
      ! GIMP_GUI_CONFIG (gdisp->gimage->gimp->config)->trust_dirty_flag)
    {
      const gchar *uri = gimp_object_get_name (GIMP_OBJECT (gdisp->gimage));

      if (! uri)
        {
          file_save_as_cmd_callback (action, data);
        }
      else
        {
          GimpPDBStatusType  status;
          GError            *error = NULL;

          status = file_save (gdisp->gimage, action_data_get_context (data),
                              GIMP_PROGRESS (gdisp),
                              GIMP_RUN_WITH_LAST_VALS, &error);

          if (status != GIMP_PDB_SUCCESS &&
              status != GIMP_PDB_CANCEL)
            {
              gchar *filename;

              filename = file_utils_uri_to_utf8_filename (uri);

              g_message (_("Saving '%s' failed:\n\n%s"),
                         filename, error->message);
              g_clear_error (&error);

              g_free (filename);
            }
        }
    }
}

void
file_save_as_cmd_callback (GtkAction *action,
                           gpointer   data)
{
  GimpDisplay *gdisp;
  GtkWidget   *widget;
  return_if_no_display (gdisp, data);
  return_if_no_widget (widget, data);

  if (! gimp_image_active_drawable (gdisp->gimage))
    return;

  file_save_dialog_show (gdisp->gimage, widget, _("Save Image"),
                         TRUE, TRUE);
}

void
file_save_a_copy_cmd_callback (GtkAction *action,
                               gpointer   data)
{
  GimpDisplay *gdisp;
  GtkWidget   *widget;
  return_if_no_display (gdisp, data);
  return_if_no_widget (widget, data);

  if (! gimp_image_active_drawable (gdisp->gimage))
    return;

  file_save_dialog_show (gdisp->gimage, widget, _("Save a Copy of the Image"),
                         FALSE, FALSE);
}

void
file_save_template_cmd_callback (GtkAction *action,
                                 gpointer   data)
{
  GimpDisplay *gdisp;
  GtkWidget   *qbox;
  return_if_no_display (gdisp, data);

  qbox = gimp_query_string_box (_("Create New Template"),
                                gdisp->shell,
                                gimp_standard_help_func,
                                GIMP_HELP_FILE_SAVE_AS_TEMPLATE,
                                _("Enter a name for this template"),
                                NULL,
                                G_OBJECT (gdisp->gimage), "disconnect",
                                file_new_template_callback, gdisp->gimage);
  gtk_widget_show (qbox);
}

void
file_revert_cmd_callback (GtkAction *action,
                          gpointer   data)
{
  GimpDisplay *gdisp;
  GtkWidget   *query_box;
  const gchar *uri;
  return_if_no_display (gdisp, data);

  uri = gimp_object_get_name (GIMP_OBJECT (gdisp->gimage));

  query_box = g_object_get_data (G_OBJECT (gdisp->gimage), REVERT_DATA_KEY);

  if (! uri)
    {
      g_message (_("Revert failed. No file name associated with this image."));
    }
  else if (query_box)
    {
      gtk_window_present (GTK_WINDOW (query_box->window));
    }
  else
    {
      gchar *basename;
      gchar *text;

      basename = g_path_get_basename (uri);

      text = g_strdup_printf (_("Revert '%s' to\n"
                                "'%s'?\n\n"
                                "You will lose all your changes, "
                                "including all undo information."),
                              basename, uri);

      g_free (basename);

      query_box = gimp_query_boolean_box (_("Revert Image"),
                                          gdisp->shell,
                                          gimp_standard_help_func,
                                          GIMP_HELP_FILE_REVERT,
                                          GIMP_STOCK_QUESTION,
                                          text,
                                          GTK_STOCK_YES, GTK_STOCK_NO,
                                          G_OBJECT (gdisp), "disconnect",
                                          file_revert_confirm_callback,
                                          gdisp);

      g_free (text);

      g_object_set_data (G_OBJECT (gdisp->gimage), REVERT_DATA_KEY,
                         query_box);

      gtk_window_set_transient_for (GTK_WINDOW (query_box),
                                    GTK_WINDOW (gdisp->shell));

      gtk_widget_show (query_box);
    }
}

void
file_quit_cmd_callback (GtkAction *action,
                        gpointer   data)
{
  Gimp *gimp;
  return_if_no_gimp (gimp, data);

  gimp_exit (gimp, FALSE);
}

void
file_file_open_dialog (Gimp        *gimp,
                       const gchar *uri,
                       GtkWidget   *parent)
{
  file_open_dialog_show (parent, NULL, uri, FALSE);
}


/*  private functions  */

static void
file_open_dialog_show (GtkWidget   *parent,
                       GimpImage   *gimage,
                       const gchar *uri,
                       gboolean     open_as_layer)
{
  GtkWidget *dialog;

  dialog = gimp_dialog_factory_dialog_new (global_dialog_factory,
                                           gtk_widget_get_screen (parent),
                                           "gimp-file-open-dialog", -1, FALSE);

  if (dialog)
    {
      gimp_file_dialog_set_uri (GIMP_FILE_DIALOG (dialog), gimage, uri);

      if (open_as_layer)
        {
          gtk_window_set_title (GTK_WINDOW (dialog), _("Open Image as Layer"));
          GIMP_FILE_DIALOG (dialog)->gimage = gimage;
        }
      else
        {
          gtk_window_set_title (GTK_WINDOW (dialog), _("Open Image"));
          GIMP_FILE_DIALOG (dialog)->gimage = NULL;
        }

      gtk_window_present (GTK_WINDOW (dialog));
    }
}

static void
file_save_dialog_show (GimpImage   *gimage,
                       GtkWidget   *parent,
                       const gchar *title,
                       gboolean     set_uri_and_proc,
                       gboolean     set_image_clean)
{
  GtkWidget *dialog;

  dialog = g_object_get_data (G_OBJECT (gimage), "gimp-file-save-dialog");

  if (! dialog)
    {
      dialog = gimp_dialog_factory_dialog_new (global_dialog_factory,
                                               gtk_widget_get_screen (parent),
                                               "gimp-file-save-dialog",
                                               -1, FALSE);

      if (dialog)
        {
          g_object_set_data_full (G_OBJECT (gimage),
                                  "gimp-file-save-dialog", dialog,
                                  (GDestroyNotify) gtk_widget_destroy);
          g_signal_connect (dialog, "destroy",
                            G_CALLBACK (file_save_dialog_destroyed),
                            gimage);
        }
    }


  if (dialog)
    {
      gtk_window_set_title (GTK_WINDOW (dialog), title);

      gimp_file_dialog_set_image (GIMP_FILE_DIALOG (dialog), gimage,
                                  set_uri_and_proc, set_image_clean);

      gtk_window_present (GTK_WINDOW (dialog));
    }
}

static void
file_save_dialog_destroyed (GtkWidget *dialog,
                            GimpImage *gimage)
{
  if (GIMP_FILE_DIALOG (dialog)->gimage == gimage)
    g_object_set_data (G_OBJECT (gimage), "gimp-file-save-dialog", NULL);
}

static void
file_new_template_callback (GtkWidget   *widget,
                            const gchar *name,
                            gpointer     data)
{
  GimpTemplate *template;
  GimpImage    *gimage;

  gimage = (GimpImage *) data;

  if (! (name && strlen (name)))
    name = _("(Unnamed Template)");

  template = gimp_template_new (name);
  gimp_template_set_from_image (template, gimage);
  gimp_container_add (gimage->gimp->templates, GIMP_OBJECT (template));
  g_object_unref (template);
}

static void
file_revert_confirm_callback (GtkWidget *widget,
                              gboolean   revert,
                              gpointer   data)
{
  GimpDisplay *gdisp      = GIMP_DISPLAY (data);
  GimpImage   *old_gimage = gdisp->gimage;

  g_object_set_data (G_OBJECT (old_gimage), REVERT_DATA_KEY, NULL);

  if (revert)
    {
      Gimp              *gimp;
      GimpImage         *new_gimage;
      const gchar       *uri;
      GimpPDBStatusType  status;
      GError            *error = NULL;

      gimp = old_gimage->gimp;

      uri = gimp_object_get_name (GIMP_OBJECT (old_gimage));

      new_gimage = file_open_image (gimp, gimp_get_user_context (gimp),
                                    GIMP_PROGRESS (gdisp),
                                    uri, uri, NULL,
                                    GIMP_RUN_INTERACTIVE,
                                    &status, NULL, &error);

      if (new_gimage)
        {
          gimp_displays_reconnect (gimp, old_gimage, new_gimage);
          gimp_image_flush (new_gimage);

          /*  the displays own the image now  */
          g_object_unref (new_gimage);
        }
      else if (status != GIMP_PDB_CANCEL)
        {
          gchar *filename;

          filename = file_utils_uri_to_utf8_filename (uri);

          g_message (_("Reverting to '%s' failed:\n\n%s"),
                     filename, error->message);
          g_clear_error (&error);

          g_free (filename);
        }
    }
}
