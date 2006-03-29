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

#include "libgimpcolor/gimpcolor.h"

#include "pdb-types.h"
#include "procedural_db.h"
#include "core/gimpparamspecs.h"

#include "core/gimpchannel-select.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"
#include "gimp-intl.h"

static ProcRecord by_color_select_proc;
static ProcRecord ellipse_select_proc;
static ProcRecord free_select_proc;
static ProcRecord fuzzy_select_proc;
static ProcRecord rect_select_proc;

void
register_selection_tools_procs (Gimp *gimp)
{
  ProcRecord *procedure;

  /*
   * by_color_select
   */
  procedure = procedural_db_init_proc (&by_color_select_proc, 8, 0);
  procedural_db_add_argument (procedure,
                              GIMP_PDB_DRAWABLE,
                              gimp_param_spec_item_id ("drawable",
                                                       "drawable",
                                                       "The affected drawable",
                                                       gimp,
                                                       GIMP_TYPE_DRAWABLE,
                                                       GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_COLOR,
                              gimp_param_spec_rgb ("color",
                                                   "color",
                                                   "The color to select",
                                                   NULL,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_int ("threshold",
                                                "threshold",
                                                "Threshold in intensity levels (0 <= threshold <= 255)",
                                                0, 255, 0,
                                                GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_enum ("operation",
                                                 "operation",
                                                 "The selection operation: { GIMP_CHANNEL_OP_ADD (0), GIMP_CHANNEL_OP_SUBTRACT (1), GIMP_CHANNEL_OP_REPLACE (2), GIMP_CHANNEL_OP_INTERSECT (3) }",
                                                 GIMP_TYPE_CHANNEL_OPS,
                                                 GIMP_CHANNEL_OP_ADD,
                                                 GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("antialias",
                                                    "antialias",
                                                    "Antialiasing (TRUE or FALSE)",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("feather",
                                                    "feather",
                                                    "Feather option for selections",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("feather-radius",
                                                   "feather radius",
                                                   "Radius for feather operation",
                                                   0, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("sample-merged",
                                                    "sample merged",
                                                    "Use the composite image, not the drawable",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * ellipse_select
   */
  procedure = procedural_db_init_proc (&ellipse_select_proc, 9, 0);
  procedural_db_add_argument (procedure,
                              GIMP_PDB_IMAGE,
                              gimp_param_spec_image_id ("image",
                                                        "image",
                                                        "The image",
                                                        gimp,
                                                        GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("x",
                                                   "x",
                                                   "x coordinate of upper-left corner of ellipse bounding box",
                                                   -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("y",
                                                   "y",
                                                   "y coordinate of upper-left corner of ellipse bounding box",
                                                   -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("width",
                                                   "width",
                                                   "The width of the ellipse (0 < width)",
                                                   0, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("height",
                                                   "height",
                                                   "The height of the ellipse (0 < height)",
                                                   0, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_enum ("operation",
                                                 "operation",
                                                 "The selection operation: { GIMP_CHANNEL_OP_ADD (0), GIMP_CHANNEL_OP_SUBTRACT (1), GIMP_CHANNEL_OP_REPLACE (2), GIMP_CHANNEL_OP_INTERSECT (3) }",
                                                 GIMP_TYPE_CHANNEL_OPS,
                                                 GIMP_CHANNEL_OP_ADD,
                                                 GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("antialias",
                                                    "antialias",
                                                    "Antialiasing (TRUE or FALSE)",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("feather",
                                                    "feather",
                                                    "Feather option for selections",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("feather-radius",
                                                   "feather radius",
                                                   "Radius for feather operation",
                                                   0, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * free_select
   */
  procedure = procedural_db_init_proc (&free_select_proc, 7, 0);
  procedural_db_add_argument (procedure,
                              GIMP_PDB_IMAGE,
                              gimp_param_spec_image_id ("image",
                                                        "image",
                                                        "The image",
                                                        gimp,
                                                        GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_int ("num-segs",
                                                "num segs",
                                                "Number of points (count 1 coordinate as two points)",
                                                2, G_MAXINT32, 2,
                                                GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOATARRAY,
                              g_param_spec_pointer ("segs",
                                                    "segs",
                                                    "Array of points: { p1.x, p1.y, p2.x, p2.y, ..., pn.x, pn.y}",
                                                    GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_enum ("operation",
                                                 "operation",
                                                 "The selection operation: { GIMP_CHANNEL_OP_ADD (0), GIMP_CHANNEL_OP_SUBTRACT (1), GIMP_CHANNEL_OP_REPLACE (2), GIMP_CHANNEL_OP_INTERSECT (3) }",
                                                 GIMP_TYPE_CHANNEL_OPS,
                                                 GIMP_CHANNEL_OP_ADD,
                                                 GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("antialias",
                                                    "antialias",
                                                    "Antialiasing (TRUE or FALSE)",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("feather",
                                                    "feather",
                                                    "Feather option for selections",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("feather-radius",
                                                   "feather radius",
                                                   "Radius for feather operation",
                                                   0, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * fuzzy_select
   */
  procedure = procedural_db_init_proc (&fuzzy_select_proc, 9, 0);
  procedural_db_add_argument (procedure,
                              GIMP_PDB_DRAWABLE,
                              gimp_param_spec_item_id ("drawable",
                                                       "drawable",
                                                       "The affected drawable",
                                                       gimp,
                                                       GIMP_TYPE_DRAWABLE,
                                                       GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("x",
                                                   "x",
                                                   "x coordinate of initial seed fill point: (image coordinates)",
                                                   -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("y",
                                                   "y",
                                                   "y coordinate of initial seed fill point: (image coordinates)",
                                                   -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_int ("threshold",
                                                "threshold",
                                                "Threshold in intensity levels (0 <= threshold <= 255)",
                                                0, 255, 0,
                                                GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_enum ("operation",
                                                 "operation",
                                                 "The selection operation: { GIMP_CHANNEL_OP_ADD (0), GIMP_CHANNEL_OP_SUBTRACT (1), GIMP_CHANNEL_OP_REPLACE (2), GIMP_CHANNEL_OP_INTERSECT (3) }",
                                                 GIMP_TYPE_CHANNEL_OPS,
                                                 GIMP_CHANNEL_OP_ADD,
                                                 GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("antialias",
                                                    "antialias",
                                                    "Antialiasing (TRUE or FALSE)",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("feather",
                                                    "feather",
                                                    "Feather option for selections",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("feather-radius",
                                                   "feather radius",
                                                   "Radius for feather operation",
                                                   0, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("sample-merged",
                                                    "sample merged",
                                                    "Use the composite image, not the drawable",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * rect_select
   */
  procedure = procedural_db_init_proc (&rect_select_proc, 8, 0);
  procedural_db_add_argument (procedure,
                              GIMP_PDB_IMAGE,
                              gimp_param_spec_image_id ("image",
                                                        "image",
                                                        "The image",
                                                        gimp,
                                                        GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("x",
                                                   "x",
                                                   "x coordinate of upper-left corner of rectangle",
                                                   -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("y",
                                                   "y",
                                                   "y coordinate of upper-left corner of rectangle",
                                                   -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("width",
                                                   "width",
                                                   "The width of the rectangle (0 < width)",
                                                   0, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("height",
                                                   "height",
                                                   "The height of the rectangle (0 < height)",
                                                   0, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_enum ("operation",
                                                 "operation",
                                                 "The selection operation: { GIMP_CHANNEL_OP_ADD (0), GIMP_CHANNEL_OP_SUBTRACT (1), GIMP_CHANNEL_OP_REPLACE (2), GIMP_CHANNEL_OP_INTERSECT (3) }",
                                                 GIMP_TYPE_CHANNEL_OPS,
                                                 GIMP_CHANNEL_OP_ADD,
                                                 GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_INT32,
                              g_param_spec_boolean ("feather",
                                                    "feather",
                                                    "Feather option for selections",
                                                    FALSE,
                                                    GIMP_PARAM_READWRITE));
  procedural_db_add_argument (procedure,
                              GIMP_PDB_FLOAT,
                              g_param_spec_double ("feather-radius",
                                                   "feather radius",
                                                   "Radius for feather operation",
                                                   0, G_MAXDOUBLE, 0,
                                                   GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

}

static Argument *
by_color_select_invoker (ProcRecord   *proc_record,
                         Gimp         *gimp,
                         GimpContext  *context,
                         GimpProgress *progress,
                         Argument     *args)
{
  gboolean success = TRUE;
  GimpDrawable *drawable;
  GimpRGB color;
  gint32 threshold;
  gint32 operation;
  gboolean antialias;
  gboolean feather;
  gdouble feather_radius;
  gboolean sample_merged;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  gimp_value_get_rgb (&args[1].value, &color);
  threshold = g_value_get_int (&args[2].value);
  operation = g_value_get_enum (&args[3].value);
  antialias = g_value_get_boolean (&args[4].value);
  feather = g_value_get_boolean (&args[5].value);
  feather_radius = g_value_get_double (&args[6].value);
  sample_merged = g_value_get_boolean (&args[7].value);

  if (success)
    {
      GimpImage *image = gimp_item_get_image (GIMP_ITEM (drawable));

      gimp_channel_select_by_color (gimp_image_get_mask (image), drawable,
                                    sample_merged,
                                    &color,
                                    threshold,
                                    FALSE /* don't select transparent */,
                                    operation,
                                    antialias,
                                    feather,
                                    feather_radius,
                                    feather_radius);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcRecord by_color_select_proc =
{
  "gimp-by-color-select",
  "gimp-by-color-select",
  "Create a selection by selecting all pixels (in the specified drawable) with the same (or similar) color to that specified.",
  "This tool creates a selection over the specified image. A by-color selection is determined by the supplied color under the constraints of the specified threshold. Essentially, all pixels (in the drawable) that have color sufficiently close to the specified color (as determined by the threshold value) are included in the selection. The antialiasing parameter allows the final selection mask to contain intermediate values based on close misses to the threshold bar. Feathering can be enabled optionally and is controlled with the \"feather_radius\" parameter. If the sample_merged parameter is TRUE, the data of the composite image will be used instead of that for the specified drawable. This is equivalent to sampling for colors after merging all visible layers. In the case of a merged sampling, the supplied drawable is ignored.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { by_color_select_invoker } }
};

static Argument *
ellipse_select_invoker (ProcRecord   *proc_record,
                        Gimp         *gimp,
                        GimpContext  *context,
                        GimpProgress *progress,
                        Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;
  gdouble x;
  gdouble y;
  gdouble width;
  gdouble height;
  gint32 operation;
  gboolean antialias;
  gboolean feather;
  gdouble feather_radius;

  image = gimp_value_get_image (&args[0].value, gimp);
  x = g_value_get_double (&args[1].value);
  y = g_value_get_double (&args[2].value);
  width = g_value_get_double (&args[3].value);
  height = g_value_get_double (&args[4].value);
  operation = g_value_get_enum (&args[5].value);
  antialias = g_value_get_boolean (&args[6].value);
  feather = g_value_get_boolean (&args[7].value);
  feather_radius = g_value_get_double (&args[8].value);

  if (success)
    {
      gimp_channel_select_ellipse (gimp_image_get_mask (image),
                                   (gint) x, (gint) y,
                                   (gint) width, (gint) height,
                                   operation,
                                   antialias,
                                   feather,
                                   feather_radius,
                                   feather_radius);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcRecord ellipse_select_proc =
{
  "gimp-ellipse-select",
  "gimp-ellipse-select",
  "Create an elliptical selection over the specified image.",
  "This tool creates an elliptical selection over the specified image. The elliptical region can be either added to, subtracted from, or replace the contents of the previous selection mask. If antialiasing is turned on, the edges of the elliptical region will contain intermediate values which give the appearance of a sharper, less pixelized edge. This should be set as TRUE most of the time. If the feather option is enabled, the resulting selection is blurred before combining. The blur is a gaussian blur with the specified feather radius.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { ellipse_select_invoker } }
};

static Argument *
free_select_invoker (ProcRecord   *proc_record,
                     Gimp         *gimp,
                     GimpContext  *context,
                     GimpProgress *progress,
                     Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;
  gint32 num_segs;
  gdouble *segs;
  gint32 operation;
  gboolean antialias;
  gboolean feather;
  gdouble feather_radius;

  image = gimp_value_get_image (&args[0].value, gimp);
  num_segs = g_value_get_int (&args[1].value);
  segs = g_value_get_pointer (&args[2].value);
  operation = g_value_get_enum (&args[3].value);
  antialias = g_value_get_boolean (&args[4].value);
  feather = g_value_get_boolean (&args[5].value);
  feather_radius = g_value_get_double (&args[6].value);

  if (success)
    {
      gimp_channel_select_polygon (gimp_image_get_mask (image),
                                   _("Free Select"),
                                   num_segs / 2,
                                   (GimpVector2 *) segs,
                                   operation,
                                   antialias,
                                   feather,
                                   feather_radius,
                                   feather_radius);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcRecord free_select_proc =
{
  "gimp-free-select",
  "gimp-free-select",
  "Create a polygonal selection over the specified image.",
  "This tool creates a polygonal selection over the specified image. The polygonal region can be either added to, subtracted from, or replace the contents of the previous selection mask. The polygon is specified through an array of floating point numbers and its length. The length of array must be 2n, where n is the number of points. Each point is defined by 2 floating point values which correspond to the x and y coordinates. If the final point does not connect to the starting point, a connecting segment is automatically added. If the feather option is enabled, the resulting selection is blurred before combining. The blur is a gaussian blur with the specified feather radius.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { free_select_invoker } }
};

static Argument *
fuzzy_select_invoker (ProcRecord   *proc_record,
                      Gimp         *gimp,
                      GimpContext  *context,
                      GimpProgress *progress,
                      Argument     *args)
{
  gboolean success = TRUE;
  GimpDrawable *drawable;
  gdouble x;
  gdouble y;
  gint32 threshold;
  gint32 operation;
  gboolean antialias;
  gboolean feather;
  gdouble feather_radius;
  gboolean sample_merged;

  drawable = (GimpDrawable *) gimp_value_get_item (&args[0].value, gimp, GIMP_TYPE_DRAWABLE);
  x = g_value_get_double (&args[1].value);
  y = g_value_get_double (&args[2].value);
  threshold = g_value_get_int (&args[3].value);
  operation = g_value_get_enum (&args[4].value);
  antialias = g_value_get_boolean (&args[5].value);
  feather = g_value_get_boolean (&args[6].value);
  feather_radius = g_value_get_double (&args[7].value);
  sample_merged = g_value_get_boolean (&args[8].value);

  if (success)
    {
      GimpImage *image = gimp_item_get_image (GIMP_ITEM (drawable));

      gimp_channel_select_fuzzy (gimp_image_get_mask (image),
                                 drawable,
                                 sample_merged,
                                 x, y,
                                 threshold,
                                 FALSE /* don't select transparent */,
                                 operation,
                                 antialias,
                                 feather,
                                 feather_radius,
                                 feather_radius);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcRecord fuzzy_select_proc =
{
  "gimp-fuzzy-select",
  "gimp-fuzzy-select",
  "Create a fuzzy selection starting at the specified coordinates on the specified drawable.",
  "This tool creates a fuzzy selection over the specified image. A fuzzy selection is determined by a seed fill under the constraints of the specified threshold. Essentially, the color at the specified coordinates (in the drawable) is measured and the selection expands outwards from that point to any adjacent pixels which are not significantly different (as determined by the threshold value). This process continues until no more expansion is possible. The antialiasing parameter allows the final selection mask to contain intermediate values based on close misses to the threshold bar at pixels along the seed fill boundary. Feathering can be enabled optionally and is controlled with the \"feather_radius\" paramter. If the sample_merged parameter is TRUE, the data of the composite image will be used instead of that for the specified drawable. This is equivalent to sampling for colors after merging all visible layers. In the case of a merged sampling, the supplied drawable is ignored. If"
  "the sample is merged, the specified coordinates are relative to the image origin; otherwise, they are relative to the drawable's origin.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { fuzzy_select_invoker } }
};

static Argument *
rect_select_invoker (ProcRecord   *proc_record,
                     Gimp         *gimp,
                     GimpContext  *context,
                     GimpProgress *progress,
                     Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;
  gdouble x;
  gdouble y;
  gdouble width;
  gdouble height;
  gint32 operation;
  gboolean feather;
  gdouble feather_radius;

  image = gimp_value_get_image (&args[0].value, gimp);
  x = g_value_get_double (&args[1].value);
  y = g_value_get_double (&args[2].value);
  width = g_value_get_double (&args[3].value);
  height = g_value_get_double (&args[4].value);
  operation = g_value_get_enum (&args[5].value);
  feather = g_value_get_boolean (&args[6].value);
  feather_radius = g_value_get_double (&args[7].value);

  if (success)
    {
      gimp_channel_select_rectangle (gimp_image_get_mask (image),
                                     (gint) x, (gint) y,
                                     (gint) width, (gint) height,
                                     operation,
                                     feather,
                                     feather_radius,
                                     feather_radius);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcRecord rect_select_proc =
{
  "gimp-rect-select",
  "gimp-rect-select",
  "Create a rectangular selection over the specified image;",
  "This tool creates a rectangular selection over the specified image. The rectangular region can be either added to, subtracted from, or replace the contents of the previous selection mask. If the feather option is enabled, the resulting selection is blurred before combining. The blur is a gaussian blur with the specified feather radius.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { rect_select_invoker } }
};
