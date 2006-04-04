/* The GIMP -- an image manipulation program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is autogenerated by pdbgen.pl. */

#include "config.h"


#include <glib-object.h>

#include "pdb-types.h"
#include "gimp-pdb.h"
#include "gimpprocedure.h"
#include "core/gimpparamspecs.h"

#include "core/gimp.h"
#include "core/gimpdatafactory.h"

static GimpProcedure brushes_popup_proc;
static GimpProcedure brushes_close_popup_proc;
static GimpProcedure brushes_set_popup_proc;

void
register_brush_select_procs (Gimp *gimp)
{
  GimpProcedure *procedure;

  /*
   * brushes_popup
   */
  procedure = gimp_procedure_init (&brushes_popup_proc, 6, 0);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("brush-callback",
                                                       "brush callback",
                                                       "The callback PDB proc to call when brush selection is made",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("popup-title",
                                                       "popup title",
                                                       "Title to give the brush popup window",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("initial-brush",
                                                       "initial brush",
                                                       "The name of the brush to set as the first selected",
                                                       FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("opacity",
                                                    "opacity",
                                                    "The initial opacity of the brush",
                                                    0, 100, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("spacing",
                                                      "spacing",
                                                      "The initial spacing of the brush (if < 0 then use brush default spacing)",
                                                      G_MININT32, 1000, 0,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("paint-mode",
                                                  "paint mode",
                                                  "The initial paint mode: { GIMP_NORMAL_MODE (0), GIMP_DISSOLVE_MODE (1), GIMP_BEHIND_MODE (2), GIMP_MULTIPLY_MODE (3), GIMP_SCREEN_MODE (4), GIMP_OVERLAY_MODE (5), GIMP_DIFFERENCE_MODE (6), GIMP_ADDITION_MODE (7), GIMP_SUBTRACT_MODE (8), GIMP_DARKEN_ONLY_MODE (9), GIMP_LIGHTEN_ONLY_MODE (10), GIMP_HUE_MODE (11), GIMP_SATURATION_MODE (12), GIMP_COLOR_MODE (13), GIMP_VALUE_MODE (14), GIMP_DIVIDE_MODE (15), GIMP_DODGE_MODE (16), GIMP_BURN_MODE (17), GIMP_HARDLIGHT_MODE (18), GIMP_SOFTLIGHT_MODE (19), GIMP_GRAIN_EXTRACT_MODE (20), GIMP_GRAIN_MERGE_MODE (21), GIMP_COLOR_ERASE_MODE (22) }",
                                                  GIMP_TYPE_LAYER_MODE_EFFECTS,
                                                  GIMP_NORMAL_MODE,
                                                  GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * brushes_close_popup
   */
  procedure = gimp_procedure_init (&brushes_close_popup_proc, 1, 0);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("brush-callback",
                                                       "brush callback",
                                                       "The name of the callback registered for this popup",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * brushes_set_popup
   */
  procedure = gimp_procedure_init (&brushes_set_popup_proc, 5, 0);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("brush-callback",
                                                       "brush callback",
                                                       "The name of the callback registered for this popup",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("brush-name",
                                                       "brush name",
                                                       "The name of the brush to set as selected",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("opacity",
                                                    "opacity",
                                                    "The initial opacity of the brush",
                                                    0, 100, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("spacing",
                                                      "spacing",
                                                      "The initial spacing of the brush (if < 0 then use brush default spacing)",
                                                      G_MININT32, 1000, 0,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("paint-mode",
                                                  "paint mode",
                                                  "The initial paint mode: { GIMP_NORMAL_MODE (0), GIMP_DISSOLVE_MODE (1), GIMP_BEHIND_MODE (2), GIMP_MULTIPLY_MODE (3), GIMP_SCREEN_MODE (4), GIMP_OVERLAY_MODE (5), GIMP_DIFFERENCE_MODE (6), GIMP_ADDITION_MODE (7), GIMP_SUBTRACT_MODE (8), GIMP_DARKEN_ONLY_MODE (9), GIMP_LIGHTEN_ONLY_MODE (10), GIMP_HUE_MODE (11), GIMP_SATURATION_MODE (12), GIMP_COLOR_MODE (13), GIMP_VALUE_MODE (14), GIMP_DIVIDE_MODE (15), GIMP_DODGE_MODE (16), GIMP_BURN_MODE (17), GIMP_HARDLIGHT_MODE (18), GIMP_SOFTLIGHT_MODE (19), GIMP_GRAIN_EXTRACT_MODE (20), GIMP_GRAIN_MERGE_MODE (21), GIMP_COLOR_ERASE_MODE (22) }",
                                                  GIMP_TYPE_LAYER_MODE_EFFECTS,
                                                  GIMP_NORMAL_MODE,
                                                  GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

}

static GValueArray *
brushes_popup_invoker (GimpProcedure     *procedure,
                       Gimp              *gimp,
                       GimpContext       *context,
                       GimpProgress      *progress,
                       const GValueArray *args)
{
  gboolean success = TRUE;
  const gchar *brush_callback;
  const gchar *popup_title;
  const gchar *initial_brush;
  gdouble opacity;
  gint32 spacing;
  gint32 paint_mode;

  brush_callback = g_value_get_string (&args->values[0]);
  popup_title = g_value_get_string (&args->values[1]);
  initial_brush = g_value_get_string (&args->values[2]);
  opacity = g_value_get_double (&args->values[3]);
  spacing = g_value_get_int (&args->values[4]);
  paint_mode = g_value_get_enum (&args->values[5]);

  if (success)
    {
      if (gimp->no_interface ||
          ! gimp_pdb_lookup (gimp, brush_callback) ||
          ! gimp_pdb_dialog_new (gimp, context, gimp->brush_factory->container,
                                 popup_title, brush_callback, initial_brush,
                                 "opacity",    opacity / 100.0,
                                 "paint-mode", paint_mode,
                                 "spacing",    spacing,
                                 NULL))
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure brushes_popup_proc =
{
  TRUE, TRUE,
  "gimp-brushes-popup",
  "gimp-brushes-popup",
  "Invokes the Gimp brush selection.",
  "This procedure popups the brush selection dialog.",
  "Andy Thomas",
  "Andy Thomas",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { brushes_popup_invoker } }
};

static GValueArray *
brushes_close_popup_invoker (GimpProcedure     *procedure,
                             Gimp              *gimp,
                             GimpContext       *context,
                             GimpProgress      *progress,
                             const GValueArray *args)
{
  gboolean success = TRUE;
  const gchar *brush_callback;

  brush_callback = g_value_get_string (&args->values[0]);

  if (success)
    {
      if (gimp->no_interface ||
          ! gimp_pdb_lookup (gimp, brush_callback) ||
          ! gimp_pdb_dialog_close (gimp, gimp->brush_factory->container,
                                   brush_callback))
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure brushes_close_popup_proc =
{
  TRUE, TRUE,
  "gimp-brushes-close-popup",
  "gimp-brushes-close-popup",
  "Popdown the Gimp brush selection.",
  "This procedure closes an opened brush selection dialog.",
  "Andy Thomas",
  "Andy Thomas",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { brushes_close_popup_invoker } }
};

static GValueArray *
brushes_set_popup_invoker (GimpProcedure     *procedure,
                           Gimp              *gimp,
                           GimpContext       *context,
                           GimpProgress      *progress,
                           const GValueArray *args)
{
  gboolean success = TRUE;
  const gchar *brush_callback;
  const gchar *brush_name;
  gdouble opacity;
  gint32 spacing;
  gint32 paint_mode;

  brush_callback = g_value_get_string (&args->values[0]);
  brush_name = g_value_get_string (&args->values[1]);
  opacity = g_value_get_double (&args->values[2]);
  spacing = g_value_get_int (&args->values[3]);
  paint_mode = g_value_get_enum (&args->values[4]);

  if (success)
    {
      if (gimp->no_interface ||
          ! gimp_pdb_lookup (gimp, brush_callback) ||
          ! gimp_pdb_dialog_set (gimp, gimp->brush_factory->container,
                                 brush_callback, brush_name,
                                 "opacity",    opacity / 100.0,
                                 "paint-mode", paint_mode,
                                 "spacing",    spacing,
                                 NULL))
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure brushes_set_popup_proc =
{
  TRUE, TRUE,
  "gimp-brushes-set-popup",
  "gimp-brushes-set-popup",
  "Sets the current brush selection in a popup.",
  "Sets the current brush selection in a popup.",
  "Andy Thomas",
  "Andy Thomas",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { brushes_set_popup_invoker } }
};
