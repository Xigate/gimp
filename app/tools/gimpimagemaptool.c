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
#include <errno.h>

#include <gtk/gtk.h>

#include "libgimpbase/gimpbase.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "tools-types.h"

#include "core/gimp.h"
#include "core/gimpcontext.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"
#include "core/gimpimage-pick-color.h"
#include "core/gimpimagemap.h"
#include "core/gimptoolinfo.h"

#include "widgets/gimpitemfactory.h"
#include "widgets/gimppropwidgets.h"
#include "widgets/gimptooldialog.h"
#include "widgets/gimpviewabledialog.h"

#include "display/gimpdisplay.h"
#include "display/gimpdisplayshell.h"

#include "gimpcoloroptions.h"
#include "gimpimagemaptool.h"
#include "gimptoolcontrol.h"

#include "gimp-intl.h"


/*  local function prototypes  */

static void     gimp_image_map_tool_class_init (GimpImageMapToolClass *klass);
static void     gimp_image_map_tool_init       (GimpImageMapTool      *image_map_tool);

static void     gimp_image_map_tool_finalize   (GObject          *object);

static gboolean gimp_image_map_tool_initialize (GimpTool         *tool,
                                                GimpDisplay      *gdisp);
static void     gimp_image_map_tool_control    (GimpTool         *tool,
                                                GimpToolAction    action,
                                                GimpDisplay      *gdisp);

static gboolean gimp_image_map_tool_pick_color (GimpColorTool    *color_tool,
                                                gint              x,
                                                gint              y,
                                                GimpImageType    *sample_type,
                                                GimpRGB          *color,
                                                gint             *color_index);
static void     gimp_image_map_tool_map        (GimpImageMapTool *image_map_tool);
static void     gimp_image_map_tool_dialog     (GimpImageMapTool *image_map_tool);
static void     gimp_image_map_tool_reset      (GimpImageMapTool *image_map_tool);

static void     gimp_image_map_tool_flush      (GimpImageMap     *image_map,
                                                GimpImageMapTool *image_map_tool);

static void     gimp_image_map_tool_response   (GtkWidget        *widget,
                                                gint              response_id,
                                                GimpImageMapTool *image_map_tool);

static void     gimp_image_map_tool_load_clicked   (GtkWidget        *widget,
                                                    GimpImageMapTool *image_map_tool);
static void     gimp_image_map_tool_save_clicked   (GtkWidget        *widget,
                                                    GimpImageMapTool *image_map_tool);
static void    gimp_image_map_tool_settings_dialog (GimpImageMapTool *image_map_tool,
                                                    const gchar      *title,
                                                    gboolean          save);

static void     gimp_image_map_tool_notify_preview (GObject          *config,
                                                    GParamSpec       *pspec,
                                                    GimpImageMapTool *image_map_tool);


static GimpToolClass *parent_class = NULL;


GType
gimp_image_map_tool_get_type (void)
{
  static GType tool_type = 0;

  if (! tool_type)
    {
      static const GTypeInfo tool_info =
      {
        sizeof (GimpImageMapToolClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_image_map_tool_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpImageMapTool),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_image_map_tool_init,
      };

      tool_type = g_type_register_static (GIMP_TYPE_COLOR_TOOL,
					  "GimpImageMapTool",
                                          &tool_info, 0);
    }

  return tool_type;
}

static void
gimp_image_map_tool_class_init (GimpImageMapToolClass *klass)
{
  GObjectClass       *object_class;
  GimpToolClass      *tool_class;
  GimpColorToolClass *color_tool_class;

  object_class     = G_OBJECT_CLASS (klass);
  tool_class       = GIMP_TOOL_CLASS (klass);
  color_tool_class = GIMP_COLOR_TOOL_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->finalize = gimp_image_map_tool_finalize;

  tool_class->initialize = gimp_image_map_tool_initialize;
  tool_class->control    = gimp_image_map_tool_control;

  color_tool_class->pick = gimp_image_map_tool_pick_color;

  klass->shell_desc        = NULL;
  klass->settings_name     = NULL;
  klass->load_dialog_title = NULL;
  klass->save_dialog_title = NULL;

  klass->map           = NULL;
  klass->dialog        = NULL;
  klass->reset         = NULL;
  klass->settings_load = NULL;
  klass->settings_save = NULL;
}

static void
gimp_image_map_tool_init (GimpImageMapTool *image_map_tool)
{
  GimpTool *tool = GIMP_TOOL (image_map_tool);

  gimp_tool_control_set_scroll_lock (tool->control, TRUE);
  gimp_tool_control_set_preserve    (tool->control, FALSE);

  image_map_tool->drawable  = NULL;
  image_map_tool->image_map = NULL;

  image_map_tool->shell       = NULL;
  image_map_tool->main_vbox   = NULL;
  image_map_tool->load_button = NULL;
  image_map_tool->save_button = NULL;
}

static void
gimp_image_map_tool_finalize (GObject *object)
{
  GimpImageMapTool *image_map_tool = GIMP_IMAGE_MAP_TOOL (object);

  if (image_map_tool->shell)
    {
      gtk_widget_destroy (image_map_tool->shell);
      image_map_tool->shell       = NULL;
      image_map_tool->main_vbox   = NULL;
      image_map_tool->load_button = NULL;
      image_map_tool->save_button = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

#define RESPONSE_RESET 1

static gboolean
gimp_image_map_tool_initialize (GimpTool    *tool,
				GimpDisplay *gdisp)
{
  GimpImageMapTool *image_map_tool = GIMP_IMAGE_MAP_TOOL (tool);
  GimpToolInfo     *tool_info;
  GimpDrawable     *drawable;

  tool_info = tool->tool_info;

  /*  set gdisp so the dialog can be hidden on display destruction  */
  tool->gdisp = gdisp;

  if (! image_map_tool->shell)
    {
      GimpImageMapToolClass *klass;
      GtkWidget             *shell;
      GtkWidget             *vbox;
      GtkWidget             *toggle;
      const gchar           *stock_id;

      klass = GIMP_IMAGE_MAP_TOOL_GET_CLASS (image_map_tool);

      stock_id = gimp_viewable_get_stock_id (GIMP_VIEWABLE (tool_info));

      image_map_tool->shell = shell =
        gimp_tool_dialog_new (tool_info, gdisp->shell,
                              klass->shell_desc,

                              GIMP_STOCK_RESET, RESPONSE_RESET,
                              GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                              GTK_STOCK_OK,     GTK_RESPONSE_OK,

                              NULL);

      g_signal_connect_object (shell, "response",
                               G_CALLBACK (gimp_image_map_tool_response),
                               G_OBJECT (image_map_tool), 0);

      image_map_tool->main_vbox = vbox = gtk_vbox_new (FALSE, 4);
      gtk_container_set_border_width (GTK_CONTAINER (vbox), 6);
      gtk_container_add (GTK_CONTAINER (GTK_DIALOG (shell)->vbox), vbox);

      /*  The preview toggle  */
      toggle = gimp_prop_check_button_new (G_OBJECT (tool_info->tool_options),
                                           "preview",
                                           _("_Preview"));

      gtk_box_pack_end (GTK_BOX (image_map_tool->main_vbox), toggle,
                        FALSE, FALSE, 0);
      gtk_widget_show (toggle);

      g_signal_connect_object (tool_info->tool_options, "notify::preview",
                               G_CALLBACK (gimp_image_map_tool_notify_preview),
                               image_map_tool, 0);

      if (klass->load_dialog_title)
        {
          image_map_tool->load_button =
            gtk_button_new_from_stock (GTK_STOCK_OPEN);

          g_signal_connect (image_map_tool->load_button, "clicked",
                            G_CALLBACK (gimp_image_map_tool_load_clicked),
                            image_map_tool);
        }

      if (klass->save_dialog_title)
        {
          image_map_tool->save_button =
            gtk_button_new_from_stock (GTK_STOCK_SAVE);

          g_signal_connect (image_map_tool->save_button, "clicked",
                            G_CALLBACK (gimp_image_map_tool_save_clicked),
                            image_map_tool);
        }

      /*  Fill in subclass widgets  */
      gimp_image_map_tool_dialog (image_map_tool);

      gtk_widget_show (vbox);
    }

  drawable = gimp_image_active_drawable (gdisp->gimage);

  gimp_viewable_dialog_set_viewable (GIMP_VIEWABLE_DIALOG (image_map_tool->shell),
                                     GIMP_VIEWABLE (drawable));

  gtk_widget_show (image_map_tool->shell);

  image_map_tool->drawable  = drawable;
  image_map_tool->image_map = gimp_image_map_new (TRUE, drawable,
                                                  tool_info->blurb);

  g_signal_connect (image_map_tool->image_map, "flush",
                    G_CALLBACK (gimp_image_map_tool_flush),
                    image_map_tool);

  {
    GimpDisplayShell *shell = GIMP_DISPLAY_SHELL (gdisp->shell);

    gimp_item_factory_update (shell->menubar_factory, shell);
    gimp_item_factory_update (shell->popup_factory,   shell);
  }

  return TRUE;
}

static void
gimp_image_map_tool_control (GimpTool       *tool,
			     GimpToolAction  action,
			     GimpDisplay    *gdisp)
{
  GimpImageMapTool *image_map_tool = GIMP_IMAGE_MAP_TOOL (tool);

  switch (action)
    {
    case HALT:
      if (image_map_tool->shell)
        gtk_dialog_response (GTK_DIALOG (image_map_tool->shell),
                             GTK_RESPONSE_CANCEL);
      break;

    default:
      break;
    }

  GIMP_TOOL_CLASS (parent_class)->control (tool, action, gdisp);
}

static gboolean
gimp_image_map_tool_pick_color (GimpColorTool *color_tool,
                                gint           x,
                                gint           y,
                                GimpImageType *sample_type,
                                GimpRGB       *color,
                                gint          *color_index)
{
  GimpImageMapTool *tool = GIMP_IMAGE_MAP_TOOL (color_tool);

  *sample_type = gimp_drawable_type (tool->drawable);

  return gimp_image_pick_color_by_func (GIMP_OBJECT (tool->image_map), x, y,
                                        (GimpImagePickColorFunc) gimp_image_map_get_color_at,
                                        color_tool->options->sample_average,
                                        color_tool->options->average_radius,
                                        color, color_index);
}

static void
gimp_image_map_tool_map (GimpImageMapTool *tool)
{
  GIMP_IMAGE_MAP_TOOL_GET_CLASS (tool)->map (tool);
}

static void
gimp_image_map_tool_dialog (GimpImageMapTool *tool)
{
  GIMP_IMAGE_MAP_TOOL_GET_CLASS (tool)->dialog (tool);
}

static void
gimp_image_map_tool_reset (GimpImageMapTool *tool)
{
  GIMP_IMAGE_MAP_TOOL_GET_CLASS (tool)->reset (tool);
}

static gboolean
gimp_image_tool_settings_load (GimpImageMapTool *tool,
                               gpointer          file)
{
  GimpImageMapToolClass *tool_class = GIMP_IMAGE_MAP_TOOL_GET_CLASS (tool);

  g_return_val_if_fail (tool_class->settings_load != NULL, FALSE);

  if (tool_class->settings_load (tool, file))
    {
      gimp_image_map_tool_preview (tool);
      return TRUE;
    }

  return FALSE;
}

static gboolean
gimp_image_tool_settings_save (GimpImageMapTool *tool,
                               gpointer          file)
{
  GimpImageMapToolClass *tool_class = GIMP_IMAGE_MAP_TOOL_GET_CLASS (tool);

  g_return_val_if_fail (tool_class->settings_save != NULL, FALSE);

  return tool_class->settings_save (tool, file);
}

static void
gimp_image_map_tool_flush (GimpImageMap     *image_map,
                           GimpImageMapTool *image_map_tool)
{
  GimpTool *tool = GIMP_TOOL (image_map_tool);

  gimp_display_flush_now (tool->gdisp);
}

static void
gimp_image_map_tool_response (GtkWidget        *widget,
                              gint              response_id,
                              GimpImageMapTool *image_map_tool)
{
  GimpDisplayShell    *shell;
  GimpImageMapOptions *options;
  GimpTool            *tool;

  tool = GIMP_TOOL (image_map_tool);

  switch (response_id)
    {
    case RESPONSE_RESET:
      gimp_image_map_tool_reset (image_map_tool);
      gimp_image_map_tool_preview (image_map_tool);
      break;

    case GTK_RESPONSE_OK:
      /* Fix for bug #126524 - only set shell in the case
       * where we need it */
      shell = GIMP_DISPLAY_SHELL (tool->gdisp->shell);
      gtk_widget_hide (image_map_tool->shell);

      gimp_tool_control_set_preserve (tool->control, TRUE);

      options = GIMP_IMAGE_MAP_OPTIONS (tool->tool_info->tool_options);

      if (! options->preview)
        {
          gimp_image_map_tool_map (image_map_tool);
        }

      if (image_map_tool->image_map)
        {
          gimp_image_map_commit (image_map_tool->image_map);
          image_map_tool->image_map = NULL;
        }

      gimp_tool_control_set_preserve (tool->control, FALSE);

      gimp_item_factory_update (shell->menubar_factory, shell);

      if (shell->gdisp == gimp_context_get_display
          (gimp_get_user_context (shell->gdisp->gimage->gimp)))
        gimp_item_factory_update (shell->popup_factory, shell);

      tool->gdisp    = NULL;
      tool->drawable = NULL;
      break;

    default:
      gtk_widget_hide (image_map_tool->shell);

      if (image_map_tool->image_map)
        {
          gimp_tool_control_set_preserve (tool->control, TRUE);

          gimp_image_map_abort (image_map_tool->image_map);
          image_map_tool->image_map = NULL;

          gimp_tool_control_set_preserve (tool->control, FALSE);

          gimp_image_flush (tool->gdisp->gimage);
        }

      tool->gdisp    = NULL;
      tool->drawable = NULL;
      break;
    }
}

static void
gimp_image_map_tool_notify_preview (GObject          *config,
                                    GParamSpec       *pspec,
                                    GimpImageMapTool *image_map_tool)
{
  GimpTool            *tool    = GIMP_TOOL (image_map_tool);
  GimpImageMapOptions *options = GIMP_IMAGE_MAP_OPTIONS (config);

  if (options->preview)
    {
      gimp_tool_control_set_preserve (tool->control, TRUE);

      gimp_image_map_tool_map (image_map_tool);

      gimp_tool_control_set_preserve (tool->control, FALSE);
    }
  else
    {
      if (image_map_tool->image_map)
	{
	  gimp_tool_control_set_preserve (tool->control, TRUE);

	  gimp_image_map_clear (image_map_tool->image_map);

	  gimp_tool_control_set_preserve (tool->control, FALSE);

	  gimp_image_flush (tool->gdisp->gimage);
	}
    }
}

void
gimp_image_map_tool_preview (GimpImageMapTool *image_map_tool)
{
  GimpTool            *tool;
  GimpImageMapOptions *options;

  g_return_if_fail (GIMP_IS_IMAGE_MAP_TOOL (image_map_tool));

  tool = GIMP_TOOL (image_map_tool);

  options = GIMP_IMAGE_MAP_OPTIONS (tool->tool_info->tool_options);

  if (options->preview)
    {
      gimp_tool_control_set_preserve (tool->control, TRUE);

      gimp_image_map_tool_map (image_map_tool);

      gimp_tool_control_set_preserve (tool->control, FALSE);
    }
}

static void
settings_dialog_response (GtkWidget        *dialog,
                          gint              response_id,
                          GimpImageMapTool *tool)
{
  gboolean save;

  save = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (dialog), "save"));

  if (response_id == GTK_RESPONSE_OK)
    {
      const gchar *filename;
      FILE        *file;

      filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (dialog));

      file = fopen (filename, save ? "wt" : "rt");

      if (! file)
        {
          g_message (save ?
                     _("Could not open '%s' for writing: %s") :
                     _("Could not open '%s' for reading: %s"),
                     gimp_filename_to_utf8 (filename),
		     g_strerror (errno));
          return;
        }

      g_object_set (GIMP_TOOL (tool)->tool_info->tool_options,
                    "settings", filename,
                    NULL);

      if (save)
        {
          gimp_image_tool_settings_save (tool, file);
        }
      else if (! gimp_image_tool_settings_load (tool, file))
        {
          g_message ("Error in reading file '%s'.",
		     gimp_filename_to_utf8 (filename));
        }

      fclose (file);
    }

  if (save)
    gtk_widget_set_sensitive (tool->load_button, TRUE);
  else
    gtk_widget_set_sensitive (tool->save_button, TRUE);

  gtk_widget_destroy (dialog);
}

static void
gimp_image_map_tool_load_clicked (GtkWidget        *widget,
                                  GimpImageMapTool *image_map_tool)
{
  GimpImageMapToolClass *klass = GIMP_IMAGE_MAP_TOOL_GET_CLASS (image_map_tool);

  gimp_image_map_tool_settings_dialog (image_map_tool,
                                       klass->load_dialog_title, FALSE);
}

static void
gimp_image_map_tool_save_clicked (GtkWidget        *widget,
                                  GimpImageMapTool *image_map_tool)
{
  GimpImageMapToolClass *klass = GIMP_IMAGE_MAP_TOOL_GET_CLASS (image_map_tool);

  gimp_image_map_tool_settings_dialog (image_map_tool,
                                       klass->save_dialog_title, TRUE);
}

static void
gimp_image_map_tool_settings_dialog (GimpImageMapTool *tool,
                                     const gchar      *title,
                                     gboolean          save)
{
  GimpImageMapOptions *options;
  GtkFileSelection    *dialog;

  g_return_if_fail (GIMP_IS_IMAGE_MAP_TOOL (tool));

  if (tool->settings_dialog)
    {
      gtk_window_present (GTK_WINDOW (tool->settings_dialog));
      return;
    }

  if (save)
    gtk_widget_set_sensitive (tool->load_button, FALSE);
  else
    gtk_widget_set_sensitive (tool->save_button, FALSE);

  tool->settings_dialog = gtk_file_selection_new (title);

  dialog = GTK_FILE_SELECTION (tool->settings_dialog);

  g_object_set_data (G_OBJECT (dialog), "save", GINT_TO_POINTER (save));

  gtk_window_set_role (GTK_WINDOW (dialog), "gimp-load-save-settings");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);

  gtk_container_set_border_width (GTK_CONTAINER (dialog), 6);
  gtk_container_set_border_width (GTK_CONTAINER (dialog->button_area), 4);

  g_object_add_weak_pointer (G_OBJECT (dialog),
                             (gpointer) &tool->settings_dialog);

  gtk_window_set_transient_for (GTK_WINDOW (dialog),
                                GTK_WINDOW (tool->shell));
  gtk_window_set_destroy_with_parent (GTK_WINDOW (dialog), TRUE);

  g_signal_connect (dialog, "response",
                    G_CALLBACK (settings_dialog_response),
                    tool);

  options = GIMP_IMAGE_MAP_OPTIONS (GIMP_TOOL (tool)->tool_info->tool_options);

  if (options->settings)
    {
      gtk_file_selection_set_filename (dialog, options->settings);
    }
  else if (GIMP_IMAGE_MAP_TOOL_GET_CLASS (tool)->settings_name)
    {
      gchar *tmp;

      tmp = g_build_filename (gimp_directory (),
                              GIMP_IMAGE_MAP_TOOL_GET_CLASS (tool)->settings_name,
                              G_DIR_SEPARATOR_S,
                              NULL);

      gtk_file_selection_set_filename (dialog, tmp);
      g_free (tmp);
    }

  gimp_help_connect (tool->settings_dialog, gimp_standard_help_func,
                     GIMP_TOOL (tool)->tool_info->help_id, NULL);

  gtk_widget_show (tool->settings_dialog);
}
