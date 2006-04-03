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

#include "libgimpbase/gimpbase.h"

#include "pdb-types.h"
#include "gimpargument.h"
#include "gimpprocedure.h"
#include "procedural_db.h"
#include "core/gimpparamspecs.h"

#include "core/gimpdrawable.h"
#include "core/gimpimage.h"
#include "core/gimplayer.h"
#include "text/gimptext-compat.h"

static GimpProcedure text_fontname_proc;
static GimpProcedure text_get_extents_fontname_proc;
static GimpProcedure text_proc;
static GimpProcedure text_get_extents_proc;

void
register_text_tool_procs (Gimp *gimp)
{
  GimpProcedure *procedure;

  /*
   * text_fontname
   */
  procedure = gimp_procedure_init (&text_fontname_proc, 10, 1);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         gimp,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_drawable_id ("drawable",
                                                            "drawable",
                                                            "The affected drawable: (-1 for a new text layer)",
                                                            gimp,
                                                            GIMP_PARAM_READWRITE | GIMP_PARAM_NO_VALIDATE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("x",
                                                    "x",
                                                    "The x coordinate for the left of the text bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("y",
                                                    "y",
                                                    "The y coordinate for the top of the text bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("text",
                                                       "text",
                                                       "The text to generate (in UTF-8 encoding)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("border",
                                                      "border",
                                                      "The size of the border (-1 <= border)",
                                                      -1, G_MAXINT32, -1,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_boolean ("antialias",
                                                     "antialias",
                                                     "Antialiasing (TRUE or FALSE)",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("size",
                                                    "size",
                                                    "The size of text in either pixels or points",
                                                    0, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("size-type",
                                                  "size type",
                                                  "The units of specified size: { GIMP_PIXELS (0), GIMP_POINTS (1) }",
                                                  GIMP_TYPE_SIZE_TYPE,
                                                  GIMP_PIXELS,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("fontname",
                                                       "fontname",
                                                       "The name of the font",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_layer_id ("text-layer",
                                                             "text layer",
                                                             "The new text layer or -1 if no layer was created.",
                                                             gimp,
                                                             GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * text_get_extents_fontname
   */
  procedure = gimp_procedure_init (&text_get_extents_fontname_proc, 4, 4);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("text",
                                                       "text",
                                                       "The text to generate (in UTF-8 encoding)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("size",
                                                    "size",
                                                    "The size of text in either pixels or points",
                                                    0, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("size-type",
                                                  "size type",
                                                  "The units of specified size: { GIMP_PIXELS (0), GIMP_POINTS (1) }",
                                                  GIMP_TYPE_SIZE_TYPE,
                                                  GIMP_PIXELS,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("fontname",
                                                       "fontname",
                                                       "The name of the font",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("width",
                                                          "width",
                                                          "The width of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("height",
                                                          "height",
                                                          "The height of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("ascent",
                                                          "ascent",
                                                          "The ascent of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("descent",
                                                          "descent",
                                                          "The descent of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * text
   */
  procedure = gimp_procedure_init (&text_proc, 17, 1);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         gimp,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_drawable_id ("drawable",
                                                            "drawable",
                                                            "The affected drawable: (-1 for a new text layer)",
                                                            gimp,
                                                            GIMP_PARAM_READWRITE | GIMP_PARAM_NO_VALIDATE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("x",
                                                    "x",
                                                    "The x coordinate for the left of the text bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("y",
                                                    "y",
                                                    "The y coordinate for the top of the text bounding box",
                                                    -G_MAXDOUBLE, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("text",
                                                       "text",
                                                       "The text to generate (in UTF-8 encoding)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("border",
                                                      "border",
                                                      "The size of the border (-1 <= border)",
                                                      -1, G_MAXINT32, -1,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_boolean ("antialias",
                                                     "antialias",
                                                     "Antialiasing (TRUE or FALSE)",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("size",
                                                    "size",
                                                    "The size of text in either pixels or points",
                                                    0, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("size-type",
                                                  "size type",
                                                  "The units of specified size: { GIMP_PIXELS (0), GIMP_POINTS (1) }",
                                                  GIMP_TYPE_SIZE_TYPE,
                                                  GIMP_PIXELS,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("foundry",
                                                       "foundry",
                                                       "The font foundry",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("family",
                                                       "family",
                                                       "The font family",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("weight",
                                                       "weight",
                                                       "The font weight",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("slant",
                                                       "slant",
                                                       "The font slant",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("set-width",
                                                       "set width",
                                                       "The font set-width",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("spacing",
                                                       "spacing",
                                                       "The font spacing",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("registry",
                                                       "registry",
                                                       "The font registry",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("encoding",
                                                       "encoding",
                                                       "The font encoding",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_layer_id ("text-layer",
                                                             "text layer",
                                                             "The new text layer or -1 if no layer was created.",
                                                             gimp,
                                                             GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

  /*
   * text_get_extents
   */
  procedure = gimp_procedure_init (&text_get_extents_proc, 11, 4);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("text",
                                                       "text",
                                                       "The text to generate (in UTF-8 encoding)",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("size",
                                                    "size",
                                                    "The size of text in either pixels or points",
                                                    0, G_MAXDOUBLE, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("size-type",
                                                  "size type",
                                                  "The units of specified size: { GIMP_PIXELS (0), GIMP_POINTS (1) }",
                                                  GIMP_TYPE_SIZE_TYPE,
                                                  GIMP_PIXELS,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("foundry",
                                                       "foundry",
                                                       "The font foundry",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("family",
                                                       "family",
                                                       "The font family",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("weight",
                                                       "weight",
                                                       "The font weight",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("slant",
                                                       "slant",
                                                       "The font slant",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("set-width",
                                                       "set width",
                                                       "The font set-width",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("spacing",
                                                       "spacing",
                                                       "The font spacing",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("registry",
                                                       "registry",
                                                       "The font registry",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("encoding",
                                                       "encoding",
                                                       "The font encoding",
                                                       TRUE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("width",
                                                          "width",
                                                          "The width of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("height",
                                                          "height",
                                                          "The height of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("ascent",
                                                          "ascent",
                                                          "The ascent of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("descent",
                                                          "descent",
                                                          "The descent of the specified font",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

}

static GimpArgument *
text_fontname_invoker (GimpProcedure      *procedure,
                       Gimp               *gimp,
                       GimpContext        *context,
                       GimpProgress       *progress,
                       const GimpArgument *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  GimpImage *image;
  GimpDrawable *drawable;
  gdouble x;
  gdouble y;
  const gchar *text;
  gint32 border;
  gboolean antialias;
  gdouble size;
  gint32 size_type;
  const gchar *fontname;
  GimpLayer *text_layer = NULL;

  image = gimp_value_get_image (&args[0].value, gimp);
  drawable = gimp_value_get_drawable (&args[1].value, gimp);
  x = g_value_get_double (&args[2].value);
  y = g_value_get_double (&args[3].value);
  text = g_value_get_string (&args[4].value);
  border = g_value_get_int (&args[5].value);
  antialias = g_value_get_boolean (&args[6].value);
  size = g_value_get_double (&args[7].value);
  size_type = g_value_get_enum (&args[8].value);
  fontname = g_value_get_string (&args[9].value);

  if (success)
    {
      if (drawable && ! gimp_item_is_attached (GIMP_ITEM (drawable)))
        success = FALSE;

      if (success)
        {
          gchar *real_fontname = g_strdup_printf ("%s %d", fontname, (gint) size);

          text_layer = text_render (image, drawable, context,
                                    x, y, real_fontname, text,
                                    border, antialias);

          g_free (real_fontname);
        }
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    gimp_value_set_layer (&return_vals[1].value, text_layer);

  return return_vals;
}

static GimpProcedure text_fontname_proc =
{
  TRUE, TRUE,
  "gimp-text-fontname",
  "gimp-text-fontname",
  "Add text at the specified location as a floating selection or a new layer.",
  "This tool requires a fontname matching an installed PangoFT2 font. You can specify the fontsize in units of pixels or points, and the appropriate metric is specified using the size_type argument. The x and y parameters together control the placement of the new text by specifying the upper left corner of the text bounding box. If the specified drawable parameter is valid, the text will be created as a floating selection attached to the drawable. If the drawable parameter is not valid (-1), the text will appear as a new layer. Finally, a border can be specified around the final rendered text. The border is measured in pixels.",
  "Martin Edlman & Sven Neumann",
  "Spencer Kimball & Peter Mattis",
  "1998- 2001",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { text_fontname_invoker } }
};

static GimpArgument *
text_get_extents_fontname_invoker (GimpProcedure      *procedure,
                                   Gimp               *gimp,
                                   GimpContext        *context,
                                   GimpProgress       *progress,
                                   const GimpArgument *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  const gchar *text;
  gdouble size;
  gint32 size_type;
  const gchar *fontname;
  gint32 width = 0;
  gint32 height = 0;
  gint32 ascent = 0;
  gint32 descent = 0;

  text = g_value_get_string (&args[0].value);
  size = g_value_get_double (&args[1].value);
  size_type = g_value_get_enum (&args[2].value);
  fontname = g_value_get_string (&args[3].value);

  if (success)
    {
      gchar *real_fontname = g_strdup_printf ("%s %d", fontname, (gint) size);

      success = text_get_extents (real_fontname, text,
                                  &width, &height,
                                  &ascent, &descent);

      g_free (real_fontname);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals[1].value, width);
      g_value_set_int (&return_vals[2].value, height);
      g_value_set_int (&return_vals[3].value, ascent);
      g_value_set_int (&return_vals[4].value, descent);
    }

  return return_vals;
}

static GimpProcedure text_get_extents_fontname_proc =
{
  TRUE, TRUE,
  "gimp-text-get-extents-fontname",
  "gimp-text-get-extents-fontname",
  "Get extents of the bounding box for the specified text.",
  "This tool returns the width and height of a bounding box for the specified text string with the specified font information. Ascent and descent for the specified font are returned as well.",
  "Martin Edlman & Sven Neumann",
  "Spencer Kimball & Peter Mattis",
  "1998- 2001",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { text_get_extents_fontname_invoker } }
};

static GimpArgument *
text_invoker (GimpProcedure      *procedure,
              Gimp               *gimp,
              GimpContext        *context,
              GimpProgress       *progress,
              const GimpArgument *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  GimpImage *image;
  GimpDrawable *drawable;
  gdouble x;
  gdouble y;
  const gchar *text;
  gint32 border;
  gboolean antialias;
  gdouble size;
  gint32 size_type;
  const gchar *foundry;
  const gchar *family;
  const gchar *weight;
  const gchar *slant;
  const gchar *set_width;
  const gchar *spacing;
  const gchar *registry;
  const gchar *encoding;
  GimpLayer *text_layer = NULL;

  image = gimp_value_get_image (&args[0].value, gimp);
  drawable = gimp_value_get_drawable (&args[1].value, gimp);
  x = g_value_get_double (&args[2].value);
  y = g_value_get_double (&args[3].value);
  text = g_value_get_string (&args[4].value);
  border = g_value_get_int (&args[5].value);
  antialias = g_value_get_boolean (&args[6].value);
  size = g_value_get_double (&args[7].value);
  size_type = g_value_get_enum (&args[8].value);
  foundry = g_value_get_string (&args[9].value);
  family = g_value_get_string (&args[10].value);
  weight = g_value_get_string (&args[11].value);
  slant = g_value_get_string (&args[12].value);
  set_width = g_value_get_string (&args[13].value);
  spacing = g_value_get_string (&args[14].value);
  registry = g_value_get_string (&args[15].value);
  encoding = g_value_get_string (&args[16].value);

  if (success)
    {
      if (drawable && ! gimp_item_is_attached (GIMP_ITEM (drawable)))
        success = FALSE;

      if (success)
        {
          gchar *real_fontname = g_strdup_printf ("%s %d", family, (gint) size);

          text_layer = text_render (image, drawable, context,
                                    x, y, real_fontname, text,
                                    border, antialias);

          g_free (real_fontname);
        }
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    gimp_value_set_layer (&return_vals[1].value, text_layer);

  return return_vals;
}

static GimpProcedure text_proc =
{
  TRUE, TRUE,
  "gimp-text",
  "gimp-text",
  "This procedure is deprecated! Use 'gimp-text-fontname' instead.",
  "This procedure is deprecated! Use 'gimp-text-fontname' instead.",
  "",
  "",
  "",
  "gimp-text-fontname",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { text_invoker } }
};

static GimpArgument *
text_get_extents_invoker (GimpProcedure      *procedure,
                          Gimp               *gimp,
                          GimpContext        *context,
                          GimpProgress       *progress,
                          const GimpArgument *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  const gchar *text;
  gdouble size;
  gint32 size_type;
  const gchar *foundry;
  const gchar *family;
  const gchar *weight;
  const gchar *slant;
  const gchar *set_width;
  const gchar *spacing;
  const gchar *registry;
  const gchar *encoding;
  gint32 width = 0;
  gint32 height = 0;
  gint32 ascent = 0;
  gint32 descent = 0;

  text = g_value_get_string (&args[0].value);
  size = g_value_get_double (&args[1].value);
  size_type = g_value_get_enum (&args[2].value);
  foundry = g_value_get_string (&args[3].value);
  family = g_value_get_string (&args[4].value);
  weight = g_value_get_string (&args[5].value);
  slant = g_value_get_string (&args[6].value);
  set_width = g_value_get_string (&args[7].value);
  spacing = g_value_get_string (&args[8].value);
  registry = g_value_get_string (&args[9].value);
  encoding = g_value_get_string (&args[10].value);

  if (success)
    {
      gchar *real_fontname = g_strdup_printf ("%s %d", family, (gint) size);

      success = text_get_extents (real_fontname, text,
                                  &width, &height,
                                  &ascent, &descent);

      g_free (real_fontname);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals[1].value, width);
      g_value_set_int (&return_vals[2].value, height);
      g_value_set_int (&return_vals[3].value, ascent);
      g_value_set_int (&return_vals[4].value, descent);
    }

  return return_vals;
}

static GimpProcedure text_get_extents_proc =
{
  TRUE, TRUE,
  "gimp-text-get-extents",
  "gimp-text-get-extents",
  "This procedure is deprecated! Use 'gimp-text-get-extents-fontname' instead.",
  "This procedure is deprecated! Use 'gimp-text-get-extents-fontname' instead.",
  "",
  "",
  "",
  "gimp-text-get-extents-fontname",
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { text_get_extents_invoker } }
};
