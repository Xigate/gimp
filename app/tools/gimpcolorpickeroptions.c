/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2001 Spencer Kimball, Peter Mattis, and others
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

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "tools-types.h"

#include "config/gimpconfig-params.h"

#include "widgets/gimppropwidgets.h"
#include "widgets/gimpwidgets-utils.h"

#include "gimpcolorpickeroptions.h"

#include "gimp-intl.h"


enum
{
  PROP_0,
  PROP_UPDATE_TOOLBOX,
  PROP_PICK_MODE
};


static void   gimp_color_picker_options_init       (GimpColorPickerOptions      *options);
static void   gimp_color_picker_options_class_init (GimpColorPickerOptionsClass *options_class);

static void   gimp_color_picker_options_set_property (GObject      *object,
                                                      guint         property_id,
                                                      const GValue *value,
                                                      GParamSpec   *pspec);
static void   gimp_color_picker_options_get_property (GObject      *object,
                                                      guint         property_id,
                                                      GValue       *value,
                                                      GParamSpec   *pspec);


static GimpToolOptionsClass *parent_class = NULL;


GType
gimp_color_picker_options_get_type (void)
{
  static GType type = 0;

  if (! type)
    {
      static const GTypeInfo info =
      {
        sizeof (GimpColorPickerOptionsClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_color_picker_options_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpColorPickerOptions),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_color_picker_options_init,
      };

      type = g_type_register_static (GIMP_TYPE_COLOR_OPTIONS,
                                     "GimpColorPickerOptions",
                                     &info, 0);
    }

  return type;
}

static void
gimp_color_picker_options_class_init (GimpColorPickerOptionsClass *klass)
{
  GObjectClass *object_class;

  object_class = G_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->set_property = gimp_color_picker_options_set_property;
  object_class->get_property = gimp_color_picker_options_get_property;

  GIMP_CONFIG_INSTALL_PROP_BOOLEAN (object_class, PROP_UPDATE_TOOLBOX,
                                    "update-toolbox", NULL,
                                    TRUE,
                                    0);
  GIMP_CONFIG_INSTALL_PROP_ENUM (object_class, PROP_PICK_MODE,
                                 "pick-mode", NULL,
                                 GIMP_TYPE_COLOR_PICK_MODE,
                                 GIMP_COLOR_PICK_MODE_FOREGROUND,
                                 0);
}

static void
gimp_color_picker_options_init (GimpColorPickerOptions *options)
{
}

static void
gimp_color_picker_options_set_property (GObject      *object,
                                        guint         property_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
  GimpColorPickerOptions *options;

  options = GIMP_COLOR_PICKER_OPTIONS (object);

  switch (property_id)
    {
    case PROP_UPDATE_TOOLBOX:
      options->update_toolbox = g_value_get_boolean (value);
      break;
    case PROP_PICK_MODE:
      options->pick_mode = g_value_get_enum (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_color_picker_options_get_property (GObject    *object,
                                        guint       property_id,
                                        GValue     *value,
                                        GParamSpec *pspec)
{
  GimpColorPickerOptions *options;

  options = GIMP_COLOR_PICKER_OPTIONS (object);

  switch (property_id)
    {
    case PROP_UPDATE_TOOLBOX:
      g_value_set_boolean (value, options->update_toolbox);
      break;
    case PROP_PICK_MODE:
      g_value_set_enum (value, options->pick_mode);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

GtkWidget *
gimp_color_picker_options_gui (GimpToolOptions *tool_options)
{
  GimpColorPickerOptions *options;
  GObject                *config;
  GtkWidget              *vbox;
  GtkWidget              *button;
  GtkWidget              *frame;
  gchar                  *str;

  options = GIMP_COLOR_PICKER_OPTIONS (tool_options);
  config  = G_OBJECT (tool_options);

  vbox = gimp_color_options_gui (tool_options);

  /*  the sample merged toggle button  */
  button = gimp_prop_check_button_new (config, "sample-merged",
                                       _("Sample Merged"));
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  /*  the update toolbox color toggle button  */
  button = gimp_prop_check_button_new (config, "update-toolbox",
                                       _("Update Toolbox Color"));
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);

  /*  the pick FG/BG frame  */
  str = g_strdup_printf (_("Pick Mode  %s"), gimp_get_mod_name_control ());
  frame = gimp_prop_enum_radio_frame_new (config, "pick-mode", str, -1, -1);
  g_free (str);

  gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 0);
  gtk_widget_show (frame);

  gtk_widget_set_sensitive (frame, options->update_toolbox);
  g_object_set_data (G_OBJECT (button), "set_sensitive", frame);

  return vbox;
}
