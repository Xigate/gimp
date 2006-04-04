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
#include "gimp-pdb.h"
#include "gimpprocedure.h"
#include "core/gimpparamspecs.h"

#include "core/gimpchannel-combine.h"
#include "core/gimpchannel.h"
#include "core/gimpimage.h"

static GimpProcedure channel_new_proc;
static GimpProcedure channel_new_from_component_proc;
static GimpProcedure channel_copy_proc;
static GimpProcedure channel_combine_masks_proc;
static GimpProcedure channel_get_show_masked_proc;
static GimpProcedure channel_set_show_masked_proc;
static GimpProcedure channel_get_opacity_proc;
static GimpProcedure channel_set_opacity_proc;
static GimpProcedure channel_get_color_proc;
static GimpProcedure channel_set_color_proc;

void
register_channel_procs (Gimp *gimp)
{
  GimpProcedure *procedure;

  /*
   * channel_new
   */
  procedure = gimp_procedure_init (&channel_new_proc, 6, 1);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image to which to add the channel",
                                                         gimp,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("width",
                                                      "width",
                                                      "The channel width (1 <= width)",
                                                      1, G_MAXINT32, 1,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("height",
                                                      "height",
                                                      "The channel height (1 <= height)",
                                                      1, G_MAXINT32, 1,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The channel name",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("opacity",
                                                    "opacity",
                                                    "The channel opacity (0 <= opacity <= 100)",
                                                    0, 100, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_rgb ("color",
                                                    "color",
                                                    "The channel compositing color",
                                                    NULL,
                                                    GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_channel_id ("channel",
                                                               "channel",
                                                               "The newly created channel",
                                                               gimp,
                                                               GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * channel_new_from_component
   */
  procedure = gimp_procedure_init (&channel_new_from_component_proc, 3, 1);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image to which to add the channel",
                                                         gimp,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("component",
                                                  "component",
                                                  "The image component: { GIMP_RED_CHANNEL (0), GIMP_GREEN_CHANNEL (1), GIMP_BLUE_CHANNEL (2), GIMP_GRAY_CHANNEL (3), GIMP_INDEXED_CHANNEL (4), GIMP_ALPHA_CHANNEL (5) }",
                                                  GIMP_TYPE_CHANNEL_TYPE,
                                                  GIMP_RED_CHANNEL,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("name",
                                                       "name",
                                                       "The channel name",
                                                       FALSE, FALSE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_channel_id ("channel",
                                                               "channel",
                                                               "The newly created channel",
                                                               gimp,
                                                               GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * channel_copy
   */
  procedure = gimp_procedure_init (&channel_copy_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_channel_id ("channel",
                                                           "channel",
                                                           "The channel to copy",
                                                           gimp,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_channel_id ("channel-copy",
                                                               "channel copy",
                                                               "The newly copied channel",
                                                               gimp,
                                                               GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * channel_combine_masks
   */
  procedure = gimp_procedure_init (&channel_combine_masks_proc, 5, 0);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_channel_id ("channel1",
                                                           "channel1",
                                                           "The channel1",
                                                           gimp,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_channel_id ("channel2",
                                                           "channel2",
                                                           "The channel2",
                                                           gimp,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_enum ("operation",
                                                  "operation",
                                                  "The selection operation: { GIMP_CHANNEL_OP_ADD (0), GIMP_CHANNEL_OP_SUBTRACT (1), GIMP_CHANNEL_OP_REPLACE (2), GIMP_CHANNEL_OP_INTERSECT (3) }",
                                                  GIMP_TYPE_CHANNEL_OPS,
                                                  GIMP_CHANNEL_OP_ADD,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("offx",
                                                      "offx",
                                                      "x offset between upper left corner of channels: (second - first)",
                                                      G_MININT32, G_MAXINT32, 0,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("offy",
                                                      "offy",
                                                      "y offset between upper left corner of channels: (second - first)",
                                                      G_MININT32, G_MAXINT32, 0,
                                                      GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * channel_get_show_masked
   */
  procedure = gimp_procedure_init (&channel_get_show_masked_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_channel_id ("channel",
                                                           "channel",
                                                           "The channel",
                                                           gimp,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   g_param_spec_boolean ("show-masked",
                                                         "show masked",
                                                         "The channel composite method",
                                                         FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * channel_set_show_masked
   */
  procedure = gimp_procedure_init (&channel_set_show_masked_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_channel_id ("channel",
                                                           "channel",
                                                           "The channel",
                                                           gimp,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_boolean ("show-masked",
                                                     "show masked",
                                                     "The new channel composite method",
                                                     FALSE,
                                                     GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * channel_get_opacity
   */
  procedure = gimp_procedure_init (&channel_get_opacity_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_channel_id ("channel",
                                                           "channel",
                                                           "The channel",
                                                           gimp,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   g_param_spec_double ("opacity",
                                                        "opacity",
                                                        "The channel opacity",
                                                        0, 100, 0,
                                                        GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * channel_set_opacity
   */
  procedure = gimp_procedure_init (&channel_set_opacity_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_channel_id ("channel",
                                                           "channel",
                                                           "The channel",
                                                           gimp,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_double ("opacity",
                                                    "opacity",
                                                    "The new channel opacity (0 <= opacity <= 100)",
                                                    0, 100, 0,
                                                    GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * channel_get_color
   */
  procedure = gimp_procedure_init (&channel_get_color_proc, 1, 1);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_channel_id ("channel",
                                                           "channel",
                                                           "The channel",
                                                           gimp,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_rgb ("color",
                                                        "color",
                                                        "The channel compositing color",
                                                        NULL,
                                                        GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

  /*
   * channel_set_color
   */
  procedure = gimp_procedure_init (&channel_set_color_proc, 2, 0);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_channel_id ("channel",
                                                           "channel",
                                                           "The channel",
                                                           gimp,
                                                           GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_rgb ("color",
                                                    "color",
                                                    "The new channel compositing color",
                                                    NULL,
                                                    GIMP_PARAM_READWRITE));
  gimp_pdb_register (gimp, procedure);

}

static GValueArray *
channel_new_invoker (GimpProcedure     *procedure,
                     Gimp              *gimp,
                     GimpContext       *context,
                     GimpProgress      *progress,
                     const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpImage *image;
  gint32 width;
  gint32 height;
  const gchar *name;
  gdouble opacity;
  GimpRGB color;
  GimpChannel *channel = NULL;

  image = gimp_value_get_image (&args->values[0], gimp);
  width = g_value_get_int (&args->values[1]);
  height = g_value_get_int (&args->values[2]);
  name = g_value_get_string (&args->values[3]);
  opacity = g_value_get_double (&args->values[4]);
  gimp_value_get_rgb (&args->values[5], &color);

  if (success)
    {
      GimpRGB rgb_color = color;

      rgb_color.a = opacity / 100.0;
      channel = gimp_channel_new (image, width, height, name, &rgb_color);

      if (! channel)
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    gimp_value_set_channel (&return_vals->values[1], channel);

  return return_vals;
}

static GimpProcedure channel_new_proc =
{
  TRUE, TRUE,
  "gimp-channel-new",
  "gimp-channel-new",
  "Create a new channel.",
  "This procedure creates a new channel with the specified width and height. Name, opacity, and color are also supplied parameters. The new channel still needs to be added to the image, as this is not automatic. Add the new channel with the 'gimp_image_add_channel' command. Other attributes such as channel show masked, should be set with explicit procedure calls. The channel's contents are undefined initially.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { channel_new_invoker } }
};

static GValueArray *
channel_new_from_component_invoker (GimpProcedure     *procedure,
                                    Gimp              *gimp,
                                    GimpContext       *context,
                                    GimpProgress      *progress,
                                    const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpImage *image;
  gint32 component;
  const gchar *name;
  GimpChannel *channel = NULL;

  image = gimp_value_get_image (&args->values[0], gimp);
  component = g_value_get_enum (&args->values[1]);
  name = g_value_get_string (&args->values[2]);

  if (success)
    {
      if (gimp_image_get_component_index (image, component) != -1)
        channel = gimp_channel_new_from_component (image,
                                                   component, name, NULL);

      if (channel)
        gimp_item_set_visible (GIMP_ITEM (channel), FALSE, FALSE);
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    gimp_value_set_channel (&return_vals->values[1], channel);

  return return_vals;
}

static GimpProcedure channel_new_from_component_proc =
{
  TRUE, TRUE,
  "gimp-channel-new-from-component",
  "gimp-channel-new-from-component",
  "Create a new channel from a color component",
  "This procedure creates a new channel from a color component.",
  "Shlomi Fish <shlomif@iglu.org.il>",
  "Shlomi Fish",
  "2005",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { channel_new_from_component_invoker } }
};

static GValueArray *
channel_copy_invoker (GimpProcedure     *procedure,
                      Gimp              *gimp,
                      GimpContext       *context,
                      GimpProgress      *progress,
                      const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpChannel *channel;
  GimpChannel *channel_copy = NULL;

  channel = gimp_value_get_channel (&args->values[0], gimp);

  if (success)
    {
      channel_copy = GIMP_CHANNEL (gimp_item_duplicate (GIMP_ITEM (channel),
                                   G_TYPE_FROM_INSTANCE (channel), FALSE));

      if (! channel_copy)
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    gimp_value_set_channel (&return_vals->values[1], channel_copy);

  return return_vals;
}

static GimpProcedure channel_copy_proc =
{
  TRUE, TRUE,
  "gimp-channel-copy",
  "gimp-channel-copy",
  "Copy a channel.",
  "This procedure copies the specified channel and returns the copy.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { channel_copy_invoker } }
};

static GValueArray *
channel_combine_masks_invoker (GimpProcedure     *procedure,
                               Gimp              *gimp,
                               GimpContext       *context,
                               GimpProgress      *progress,
                               const GValueArray *args)
{
  gboolean success = TRUE;
  GimpChannel *channel1;
  GimpChannel *channel2;
  gint32 operation;
  gint32 offx;
  gint32 offy;

  channel1 = gimp_value_get_channel (&args->values[0], gimp);
  channel2 = gimp_value_get_channel (&args->values[1], gimp);
  operation = g_value_get_enum (&args->values[2]);
  offx = g_value_get_int (&args->values[3]);
  offy = g_value_get_int (&args->values[4]);

  if (success)
    {
      gimp_channel_combine_mask (channel1, channel2, operation, offx, offy);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure channel_combine_masks_proc =
{
  TRUE, TRUE,
  "gimp-channel-combine-masks",
  "gimp-channel-combine-masks",
  "Combine two channel masks.",
  "This procedure combines two channel masks. The result is stored in the first channel.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { channel_combine_masks_invoker } }
};

static GValueArray *
channel_get_show_masked_invoker (GimpProcedure     *procedure,
                                 Gimp              *gimp,
                                 GimpContext       *context,
                                 GimpProgress      *progress,
                                 const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpChannel *channel;
  gboolean show_masked = FALSE;

  channel = gimp_value_get_channel (&args->values[0], gimp);

  if (success)
    {
      show_masked = gimp_channel_get_show_masked (channel);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_set_boolean (&return_vals->values[1], show_masked);

  return return_vals;
}

static GimpProcedure channel_get_show_masked_proc =
{
  TRUE, TRUE,
  "gimp-channel-get-show-masked",
  "gimp-channel-get-show-masked",
  "Get the composite method of the specified channel.",
  "This procedure returns the specified channel's composite method. If it is TRUE, then the channel is composited with the image so that masked regions are shown. Otherwise, selected regions are shown.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { channel_get_show_masked_invoker } }
};

static GValueArray *
channel_set_show_masked_invoker (GimpProcedure     *procedure,
                                 Gimp              *gimp,
                                 GimpContext       *context,
                                 GimpProgress      *progress,
                                 const GValueArray *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;
  gboolean show_masked;

  channel = gimp_value_get_channel (&args->values[0], gimp);
  show_masked = g_value_get_boolean (&args->values[1]);

  if (success)
    {
      gimp_channel_set_show_masked (channel, show_masked);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure channel_set_show_masked_proc =
{
  TRUE, TRUE,
  "gimp-channel-set-show-masked",
  "gimp-channel-set-show-masked",
  "Set the composite method of the specified channel.",
  "This procedure sets the specified channel's composite method. If it is TRUE, then the channel is composited with the image so that masked regions are shown. Otherwise, selected regions are shown.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { channel_set_show_masked_invoker } }
};

static GValueArray *
channel_get_opacity_invoker (GimpProcedure     *procedure,
                             Gimp              *gimp,
                             GimpContext       *context,
                             GimpProgress      *progress,
                             const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpChannel *channel;
  gdouble opacity = 0.0;

  channel = gimp_value_get_channel (&args->values[0], gimp);

  if (success)
    {
      opacity = gimp_channel_get_opacity (channel) * 100;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    g_value_set_double (&return_vals->values[1], opacity);

  return return_vals;
}

static GimpProcedure channel_get_opacity_proc =
{
  TRUE, TRUE,
  "gimp-channel-get-opacity",
  "gimp-channel-get-opacity",
  "Get the opacity of the specified channel.",
  "This procedure returns the specified channel's opacity.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { channel_get_opacity_invoker } }
};

static GValueArray *
channel_set_opacity_invoker (GimpProcedure     *procedure,
                             Gimp              *gimp,
                             GimpContext       *context,
                             GimpProgress      *progress,
                             const GValueArray *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;
  gdouble opacity;

  channel = gimp_value_get_channel (&args->values[0], gimp);
  opacity = g_value_get_double (&args->values[1]);

  if (success)
    {
      gimp_channel_set_opacity (channel, opacity / 100.0, TRUE);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure channel_set_opacity_proc =
{
  TRUE, TRUE,
  "gimp-channel-set-opacity",
  "gimp-channel-set-opacity",
  "Set the opacity of the specified channel.",
  "This procedure sets the specified channel's opacity.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { channel_set_opacity_invoker } }
};

static GValueArray *
channel_get_color_invoker (GimpProcedure     *procedure,
                           Gimp              *gimp,
                           GimpContext       *context,
                           GimpProgress      *progress,
                           const GValueArray *args)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpChannel *channel;
  GimpRGB color = { 0.0, 0.0, 0.0, 1.0 };

  channel = gimp_value_get_channel (&args->values[0], gimp);

  if (success)
    {
      gimp_channel_get_color (channel, &color);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    gimp_value_set_rgb (&return_vals->values[1], &color);

  return return_vals;
}

static GimpProcedure channel_get_color_proc =
{
  TRUE, TRUE,
  "gimp-channel-get-color",
  "gimp-channel-get-color",
  "Get the compositing color of the specified channel.",
  "This procedure returns the specified channel's compositing color.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { channel_get_color_invoker } }
};

static GValueArray *
channel_set_color_invoker (GimpProcedure     *procedure,
                           Gimp              *gimp,
                           GimpContext       *context,
                           GimpProgress      *progress,
                           const GValueArray *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;
  GimpRGB color;

  channel = gimp_value_get_channel (&args->values[0], gimp);
  gimp_value_get_rgb (&args->values[1], &color);

  if (success)
    {
      GimpRGB rgb_color = color;

      rgb_color.a = channel->color.a;
      gimp_channel_set_color (channel, &rgb_color, TRUE);
    }

  return gimp_procedure_get_return_values (procedure, success);
}

static GimpProcedure channel_set_color_proc =
{
  TRUE, TRUE,
  "gimp-channel-set-color",
  "gimp-channel-set-color",
  "Set the compositing color of the specified channel.",
  "This procedure sets the specified channel's compositing color.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { channel_set_color_invoker } }
};
