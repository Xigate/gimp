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

#include <string.h>

#include <glib-object.h>

#include "pdb-types.h"
#include "gimp-pdb.h"
#include "gimpprocedure.h"
#include "core/gimpparamspecs.h"

#include "base/temp-buf.h"
#include "core/gimp.h"
#include "core/gimpbrush.h"
#include "core/gimpcontainer-filter.h"
#include "core/gimpcontext.h"
#include "core/gimpdatafactory.h"
#include "core/gimplist.h"

static GimpProcedure brushes_refresh_proc;
static GimpProcedure brushes_get_list_proc;
static GimpProcedure brushes_get_brush_proc;
static GimpProcedure brushes_get_spacing_proc;
static GimpProcedure brushes_set_spacing_proc;
static GimpProcedure brushes_get_brush_data_proc;

void
register_brushes_procs (Gimp *gimp)
{
  GimpProcedure *procedure;

  /*
   * brushes_refresh
   */
  procedure = gimp_procedure_init (&brushes_refresh_proc, 0, 0);
  gimp_pdb_register (gimp, procedure);

  /*
   * brushes_get_list
   */
  procedure = gimp_procedure_init (&brushes_get_list_proc, 1, 2);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("filter",
                                                       "filter",
                                                       "An optional regular expression used to filter the list",
                                                       FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-brushes",
                                                          "num brushes",
                                                          "The number of brushes in the brush list",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string_array ("brush-list",
                                                                 "brush list",
                                                                 "The list of brush names",
                                                                 GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * brushes_get_brush
   */
  procedure = gimp_procedure_init (&brushes_get_brush_proc, 0, 4);
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("name",
                                                           "name",
                                                           "The brush name",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("width",
                                                          "width",
                                                          "The brush width",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("height",
                                                          "height",
                                                          "The brush height",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("spacing",
                                                          "spacing",
                                                          "The brush spacing (0 <= spacing <= 1000)",
                                                          0, 1000, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * brushes_get_spacing
   */
  procedure = gimp_procedure_init (&brushes_get_spacing_proc, 0, 1);
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("spacing",
                                                          "spacing",
                                                          "The brush spacing (0 <= spacing <= 1000)",
                                                          0, 1000, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * brushes_set_spacing
   */
  procedure = gimp_procedure_init (&brushes_set_spacing_proc, 1, 0);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("spacing",
                                                      "spacing",
                                                      "The brush spacing (0 <= spacing <= 1000)",
                                                      0, 1000, 0,
                                                      GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * brushes_get_brush_data
   */
  procedure = gimp_procedure_init (&brushes_get_brush_data_proc, 1, 8);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The brush name (\"\" means current active brush)",
                                                       FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string ("actual-name",
                                                           "actual name",
                                                           "The brush name",
                                                           FALSE, FALSE,
                                                           NULL,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   g_param_spec_double ("opacity",
                                                        "opacity",
                                                        "The brush opacity (0 <= opacity <= 100)",
                                                        0, 100, 0,
                                                        GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("spacing",
                                                          "spacing",
                                                          "The brush spacing (0 <= spacing <= 1000)",
                                                          0, 1000, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   g_param_spec_enum ("paint-mode",
                                                      "paint mode",
                                                      "The paint mode: { GIMP_NORMAL_MODE (0), GIMP_DISSOLVE_MODE (1), GIMP_BEHIND_MODE (2), GIMP_MULTIPLY_MODE (3), GIMP_SCREEN_MODE (4), GIMP_OVERLAY_MODE (5), GIMP_DIFFERENCE_MODE (6), GIMP_ADDITION_MODE (7), GIMP_SUBTRACT_MODE (8), GIMP_DARKEN_ONLY_MODE (9), GIMP_LIGHTEN_ONLY_MODE (10), GIMP_HUE_MODE (11), GIMP_SATURATION_MODE (12), GIMP_COLOR_MODE (13), GIMP_VALUE_MODE (14), GIMP_DIVIDE_MODE (15), GIMP_DODGE_MODE (16), GIMP_BURN_MODE (17), GIMP_HARDLIGHT_MODE (18), GIMP_SOFTLIGHT_MODE (19), GIMP_GRAIN_EXTRACT_MODE (20), GIMP_GRAIN_MERGE_MODE (21), GIMP_COLOR_ERASE_MODE (22) }",
                                                      GIMP_TYPE_LAYER_MODE_EFFECTS,
                                                      GIMP_NORMAL_MODE,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("width",
                                                          "width",
                                                          "The brush width",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("height",
                                                          "height",
                                                          "The brush height",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("length",
                                                          "length",
                                                          "Length of brush mask data",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int8_array ("mask-data",
                                                               "mask data",
                                                               "The brush mask data",
                                                               GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

}

static GValueArray *
brushes_refresh_invoker (GimpProcedure     *procedure,
                         Gimp              *gimp,
                         GimpContext       *context,
                         GimpProgress      *progress,
                         const GValueArray *args)
{
  gimp_data_factory_data_refresh (gimp->brush_factory);
  return gimp_procedure_get_return_values (procedure, TRUE);
}

static GimpProcedure brushes_refresh_proc =
{
  TRUE, TRUE,
  "gimp-brushes-refresh",
  "gimp-brushes-refresh",
  "Refresh current brushes. This function always succeeds.",
  "This procedure retrieves all brushes currently in the user's brush path and updates the brush dialogs accordingly.",
  "Seth Burgess",
  "Seth Burgess",
  "1997",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { brushes_refresh_invoker } }
};

static GValueArray *
brushes_get_list_invoker (GimpProcedure     *procedure,
                          Gimp              *gimp,
                          GimpContext       *context,
                          GimpProgress      *progress,
                          const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *filter;
  gint32 num_brushes = 0;
  gchar **brush_list = NULL;

  filter = g_value_get_string (&args->values[0]);

  if (success)
    {
      brush_list = gimp_container_get_filtered_name_array (gimp->brush_factory->container,
                                                           filter, &num_brushes);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals->values[1], num_brushes);
      gimp_value_take_stringarray (&return_vals->values[2], brush_list, num_brushes);
    }

  return return_vals;
}

static GimpProcedure brushes_get_list_proc =
{
  TRUE, TRUE,
  "gimp-brushes-get-list",
  "gimp-brushes-get-list",
  "Retrieve a complete listing of the available brushes.",
  "This procedure returns a complete listing of available GIMP brushes. Each name returned can be used as input to the 'gimp-context-set-brush' procedure.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { brushes_get_list_invoker } }
};

static GValueArray *
brushes_get_brush_invoker (GimpProcedure     *procedure,
                           Gimp              *gimp,
                           GimpContext       *context,
                           GimpProgress      *progress,
                           const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  gchar *name = NULL;
  gint32 width = 0;
  gint32 height = 0;
  gint32 spacing = 0;

  GimpBrush *brush = gimp_context_get_brush (context);

  if (brush)
    {
      name    = g_strdup (gimp_object_get_name (GIMP_OBJECT (brush)));
      width   = brush->mask->width;
      height  = brush->mask->height;
      spacing = gimp_brush_get_spacing (brush);
    }
  else
    success = FALSE;

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_take_string (&return_vals->values[1], name);
      g_value_set_int (&return_vals->values[2], width);
      g_value_set_int (&return_vals->values[3], height);
      g_value_set_int (&return_vals->values[4], spacing);
    }

  return return_vals;
}

static GimpProcedure brushes_get_brush_proc =
{
  TRUE, TRUE,
  "gimp-brushes-get-brush",
  "gimp-brushes-get-brush",
  "This procedure is deprecated! Use 'gimp-context-get-brush' instead.",
  "This procedure is deprecated! Use 'gimp-context-get-brush' instead.",
  "",
  "",
  "",
  "gimp-context-get-brush",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { brushes_get_brush_invoker } }
};

static GValueArray *
brushes_get_spacing_invoker (GimpProcedure     *procedure,
                             Gimp              *gimp,
                             GimpContext       *context,
                             GimpProgress      *progress,
                             const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  gint32 spacing = 0;

  GimpBrush *brush = gimp_context_get_brush (context);

  if (brush)
    spacing = gimp_brush_get_spacing (brush);
  else
    success = FALSE;

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_set_int (&return_vals->values[1], spacing);

  return return_vals;
}

static GimpProcedure brushes_get_spacing_proc =
{
  TRUE, TRUE,
  "gimp-brushes-get-spacing",
  "gimp-brushes-get-spacing",
  "This procedure is deprecated! Use 'gimp-brush-get-spacing' instead.",
  "This procedure is deprecated! Use 'gimp-brush-get-spacing' instead.",
  "",
  "",
  "",
  "gimp-brush-get-spacing",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { brushes_get_spacing_invoker } }
};

static GValueArray *
brushes_set_spacing_invoker (GimpProcedure     *procedure,
                             Gimp              *gimp,
                             GimpContext       *context,
                             GimpProgress      *progress,
                             const GValueArray *args)
{
  gboolean success = TRUE;
  gint32 spacing;

  spacing = g_value_get_int (&args->values[0]);

  if (success)
    {
      gimp_brush_set_spacing (gimp_context_get_brush (context), spacing);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure brushes_set_spacing_proc =
{
  TRUE, TRUE,
  "gimp-brushes-set-spacing",
  "gimp-brushes-set-spacing",
  "This procedure is deprecated! Use 'gimp-brush-set-spacing' instead.",
  "This procedure is deprecated! Use 'gimp-brush-set-spacing' instead.",
  "",
  "",
  "",
  "gimp-brush-set-spacing",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { brushes_set_spacing_invoker } }
};

static GValueArray *
brushes_get_brush_data_invoker (GimpProcedure     *procedure,
                                Gimp              *gimp,
                                GimpContext       *context,
                                GimpProgress      *progress,
                                const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  const gchar *name;
  gchar *actual_name = NULL;
  gdouble opacity = 0.0;
  gint32 spacing = 0;
  gint32 paint_mode = 0;
  gint32 width = 0;
  gint32 height = 0;
  gint32 length = 0;
  guint8 *mask_data = NULL;

  name = g_value_get_string (&args->values[0]);

  if (success)
    {
      GimpBrush *brush;

      if (name && strlen (name))
        {
          brush = (GimpBrush *)
            gimp_container_get_child_by_name (gimp->brush_factory->container, name);
        }
      else
        {
          brush = gimp_context_get_brush (context);
        }

      if (brush)
        {
          actual_name = g_strdup (gimp_object_get_name (GIMP_OBJECT (brush)));
          opacity     = 1.0;
          spacing     = gimp_brush_get_spacing (brush);
          paint_mode  = 0;
          width       = brush->mask->width;
          height      = brush->mask->height;
          length      = brush->mask->height * brush->mask->width;
          mask_data   = g_memdup (temp_buf_data (brush->mask), length);
        }
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_take_string (&return_vals->values[1], actual_name);
      g_value_set_double (&return_vals->values[2], opacity);
      g_value_set_int (&return_vals->values[3], spacing);
      g_value_set_enum (&return_vals->values[4], paint_mode);
      g_value_set_int (&return_vals->values[5], width);
      g_value_set_int (&return_vals->values[6], height);
      g_value_set_int (&return_vals->values[7], length);
      gimp_value_take_int8array (&return_vals->values[8], mask_data, length);
    }

  return return_vals;
}

static GimpProcedure brushes_get_brush_data_proc =
{
  TRUE, TRUE,
  "gimp-brushes-get-brush-data",
  "gimp-brushes-get-brush-data",
  "This procedure is deprecated! Use 'gimp-brush-get-pixels' instead.",
  "This procedure is deprecated! Use 'gimp-brush-get-pixels' instead.",
  "",
  "",
  "",
  "gimp-brush-get-pixels",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { brushes_get_brush_data_invoker } }
};
