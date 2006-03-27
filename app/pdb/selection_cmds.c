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
#include "procedural_db.h"

#include "core/gimpchannel-select.h"
#include "core/gimpchannel.h"
#include "core/gimpdrawable.h"
#include "core/gimpimage.h"
#include "core/gimplayer.h"
#include "core/gimppickable.h"
#include "core/gimpselection.h"
#include "gimp-intl.h"

static ProcRecord selection_bounds_proc;
static ProcRecord selection_value_proc;
static ProcRecord selection_is_empty_proc;
static ProcRecord selection_translate_proc;
static ProcRecord selection_float_proc;
static ProcRecord selection_invert_proc;
static ProcRecord selection_sharpen_proc;
static ProcRecord selection_all_proc;
static ProcRecord selection_none_proc;
static ProcRecord selection_feather_proc;
static ProcRecord selection_border_proc;
static ProcRecord selection_grow_proc;
static ProcRecord selection_shrink_proc;
static ProcRecord selection_layer_alpha_proc;
static ProcRecord selection_load_proc;
static ProcRecord selection_save_proc;
static ProcRecord selection_combine_proc;

void
register_selection_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &selection_bounds_proc);
  procedural_db_register (gimp, &selection_value_proc);
  procedural_db_register (gimp, &selection_is_empty_proc);
  procedural_db_register (gimp, &selection_translate_proc);
  procedural_db_register (gimp, &selection_float_proc);
  procedural_db_register (gimp, &selection_invert_proc);
  procedural_db_register (gimp, &selection_sharpen_proc);
  procedural_db_register (gimp, &selection_all_proc);
  procedural_db_register (gimp, &selection_none_proc);
  procedural_db_register (gimp, &selection_feather_proc);
  procedural_db_register (gimp, &selection_border_proc);
  procedural_db_register (gimp, &selection_grow_proc);
  procedural_db_register (gimp, &selection_shrink_proc);
  procedural_db_register (gimp, &selection_layer_alpha_proc);
  procedural_db_register (gimp, &selection_load_proc);
  procedural_db_register (gimp, &selection_save_proc);
  procedural_db_register (gimp, &selection_combine_proc);
}

static Argument *
selection_bounds_invoker (ProcRecord   *proc_record,
                          Gimp         *gimp,
                          GimpContext  *context,
                          GimpProgress *progress,
                          Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpImage *image;
  gboolean non_empty = FALSE;
  gint32 x1 = 0;
  gint32 y1 = 0;
  gint32 x2 = 0;
  gint32 y2 = 0;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  if (success)
    {
      non_empty = gimp_channel_bounds (gimp_image_get_mask (image),
                                       &x1, &y1, &x2, &y2);
    }

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    {
      return_vals[1].value.pdb_int = non_empty;
      return_vals[2].value.pdb_int = x1;
      return_vals[3].value.pdb_int = y1;
      return_vals[4].value.pdb_int = x2;
      return_vals[5].value.pdb_int = y2;
    }

  return return_vals;
}

static ProcArg selection_bounds_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  }
};

static ProcArg selection_bounds_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "non-empty",
    "TRUE if there is a selection"
  },
  {
    GIMP_PDB_INT32,
    "x1",
    "x coordinate of upper left corner of selection bounds"
  },
  {
    GIMP_PDB_INT32,
    "y1",
    "y coordinate of upper left corner of selection bounds"
  },
  {
    GIMP_PDB_INT32,
    "x2",
    "x coordinate of lower right corner of selection bounds"
  },
  {
    GIMP_PDB_INT32,
    "y2",
    "y coordinate of lower right corner of selection bounds"
  }
};

static ProcRecord selection_bounds_proc =
{
  "gimp-selection-bounds",
  "gimp-selection-bounds",
  "Find the bounding box of the current selection.",
  "This procedure returns whether there is a selection for the specified image. If there is one, the upper left and lower right corners of the bounding box are returned. These coordinates are relative to the image. Please note that the pixel specified by the lower righthand coordinate of the bounding box is not part of the selection. The selection ends at the upper left corner of this pixel. This means the width of the selection can be calculated as (x2 - x1), its height as (y2 - y1).",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  selection_bounds_inargs,
  5,
  selection_bounds_outargs,
  { { selection_bounds_invoker } }
};

static Argument *
selection_value_invoker (ProcRecord   *proc_record,
                         Gimp         *gimp,
                         GimpContext  *context,
                         GimpProgress *progress,
                         Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpImage *image;
  gint32 x;
  gint32 y;
  gint32 value = 0;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  x = args[1].value.pdb_int;

  y = args[2].value.pdb_int;

  if (success)
    {
      value = gimp_pickable_get_opacity_at (GIMP_PICKABLE (gimp_image_get_mask (image)), x, y);
    }

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    return_vals[1].value.pdb_int = value;

  return return_vals;
}

static ProcArg selection_value_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_INT32,
    "x",
    "x coordinate of value"
  },
  {
    GIMP_PDB_INT32,
    "y",
    "y coordinate of value"
  }
};

static ProcArg selection_value_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "value",
    "Value of the selection (0 <= value <= 255)"
  }
};

static ProcRecord selection_value_proc =
{
  "gimp-selection-value",
  "gimp-selection-value",
  "Find the value of the selection at the specified coordinates.",
  "This procedure returns the value of the selection at the specified coordinates. If the coordinates lie out of bounds, 0 is returned.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  3,
  selection_value_inargs,
  1,
  selection_value_outargs,
  { { selection_value_invoker } }
};

static Argument *
selection_is_empty_invoker (ProcRecord   *proc_record,
                            Gimp         *gimp,
                            GimpContext  *context,
                            GimpProgress *progress,
                            Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpImage *image;
  gboolean is_empty = FALSE;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  if (success)
    {
      is_empty = gimp_channel_is_empty (gimp_image_get_mask (image));
    }

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    return_vals[1].value.pdb_int = is_empty;

  return return_vals;
}

static ProcArg selection_is_empty_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  }
};

static ProcArg selection_is_empty_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "is-empty",
    "Is the selection empty?"
  }
};

static ProcRecord selection_is_empty_proc =
{
  "gimp-selection-is-empty",
  "gimp-selection-is-empty",
  "Determine whether the selection is empty.",
  "This procedure returns TRUE if the selection for the specified image is not empty.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  selection_is_empty_inargs,
  1,
  selection_is_empty_outargs,
  { { selection_is_empty_invoker } }
};

static Argument *
selection_translate_invoker (ProcRecord   *proc_record,
                             Gimp         *gimp,
                             GimpContext  *context,
                             GimpProgress *progress,
                             Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;
  gint32 offx;
  gint32 offy;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  offx = args[1].value.pdb_int;

  offy = args[2].value.pdb_int;

  if (success)
    {
      gimp_item_translate (GIMP_ITEM (gimp_image_get_mask (image)),
                           offx, offy, TRUE);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_translate_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_INT32,
    "offx",
    "x offset for translation"
  },
  {
    GIMP_PDB_INT32,
    "offy",
    "y offset for translation"
  }
};

static ProcRecord selection_translate_proc =
{
  "gimp-selection-translate",
  "gimp-selection-translate",
  "Translate the selection by the specified offsets.",
  "This procedure actually translates the selection for the specified image by the specified offsets. Regions that are translated from beyond the bounds of the image are set to empty. Valid regions of the selection which are translated beyond the bounds of the image because of this call are lost.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  3,
  selection_translate_inargs,
  0,
  NULL,
  { { selection_translate_invoker } }
};

static Argument *
selection_float_invoker (ProcRecord   *proc_record,
                         Gimp         *gimp,
                         GimpContext  *context,
                         GimpProgress *progress,
                         Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpDrawable *drawable;
  gint32 offx;
  gint32 offy;
  GimpLayer *layer = NULL;

  drawable = (GimpDrawable *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! (GIMP_IS_DRAWABLE (drawable) && ! gimp_item_is_removed (GIMP_ITEM (drawable))))
    success = FALSE;

  offx = args[1].value.pdb_int;

  offy = args[2].value.pdb_int;

  if (success)
    {
      if (gimp_item_is_attached (GIMP_ITEM (drawable)))
        {
          GimpImage *image = gimp_item_get_image (GIMP_ITEM (drawable));

          layer = gimp_selection_float (gimp_image_get_mask (image),
                                        drawable, context, TRUE, offx, offy);
          if (! layer)
            success = FALSE;
        }
      else
        success = FALSE;
    }

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    return_vals[1].value.pdb_int = layer ? gimp_item_get_ID (GIMP_ITEM (layer)) : -1;

  return return_vals;
}

static ProcArg selection_float_inargs[] =
{
  {
    GIMP_PDB_DRAWABLE,
    "drawable",
    "The drawable from which to float selection"
  },
  {
    GIMP_PDB_INT32,
    "offx",
    "x offset for translation"
  },
  {
    GIMP_PDB_INT32,
    "offy",
    "y offset for translation"
  }
};

static ProcArg selection_float_outargs[] =
{
  {
    GIMP_PDB_LAYER,
    "layer",
    "The floated layer"
  }
};

static ProcRecord selection_float_proc =
{
  "gimp-selection-float",
  "gimp-selection-float",
  "Float the selection from the specified drawable with initial offsets as specified.",
  "This procedure determines the region of the specified drawable that lies beneath the current selection. The region is then cut from the drawable and the resulting data is made into a new layer which is instantiated as a floating selection. The offsets allow initial positioning of the new floating selection.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  3,
  selection_float_inargs,
  1,
  selection_float_outargs,
  { { selection_float_invoker } }
};

static Argument *
selection_invert_invoker (ProcRecord   *proc_record,
                          Gimp         *gimp,
                          GimpContext  *context,
                          GimpProgress *progress,
                          Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  if (success)
    {
      gimp_channel_invert (gimp_image_get_mask (image), TRUE);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_invert_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  }
};

static ProcRecord selection_invert_proc =
{
  "gimp-selection-invert",
  "gimp-selection-invert",
  "Invert the selection mask.",
  "This procedure inverts the selection mask. For every pixel in the selection channel, its new value is calculated as (255 - old_value).",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  selection_invert_inargs,
  0,
  NULL,
  { { selection_invert_invoker } }
};

static Argument *
selection_sharpen_invoker (ProcRecord   *proc_record,
                           Gimp         *gimp,
                           GimpContext  *context,
                           GimpProgress *progress,
                           Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  if (success)
    {
      gimp_channel_sharpen (gimp_image_get_mask (image), TRUE);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_sharpen_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  }
};

static ProcRecord selection_sharpen_proc =
{
  "gimp-selection-sharpen",
  "gimp-selection-sharpen",
  "Sharpen the selection mask.",
  "This procedure sharpens the selection mask. For every pixel in the selection channel, if the value is > 0, the new pixel is assigned a value of 255. This removes any \"anti-aliasing\" that might exist in the selection mask's boundary.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  selection_sharpen_inargs,
  0,
  NULL,
  { { selection_sharpen_invoker } }
};

static Argument *
selection_all_invoker (ProcRecord   *proc_record,
                       Gimp         *gimp,
                       GimpContext  *context,
                       GimpProgress *progress,
                       Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  if (success)
    {
      gimp_channel_all (gimp_image_get_mask (image), TRUE);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_all_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  }
};

static ProcRecord selection_all_proc =
{
  "gimp-selection-all",
  "gimp-selection-all",
  "Select all of the image.",
  "This procedure sets the selection mask to completely encompass the image. Every pixel in the selection channel is set to 255.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  selection_all_inargs,
  0,
  NULL,
  { { selection_all_invoker } }
};

static Argument *
selection_none_invoker (ProcRecord   *proc_record,
                        Gimp         *gimp,
                        GimpContext  *context,
                        GimpProgress *progress,
                        Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  if (success)
    {
      gimp_channel_clear (gimp_image_get_mask (image), NULL, TRUE);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_none_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  }
};

static ProcRecord selection_none_proc =
{
  "gimp-selection-none",
  "gimp-selection-none",
  "Deselect the entire image.",
  "This procedure deselects the entire image. Every pixel in the selection channel is set to 0.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  selection_none_inargs,
  0,
  NULL,
  { { selection_none_invoker } }
};

static Argument *
selection_feather_invoker (ProcRecord   *proc_record,
                           Gimp         *gimp,
                           GimpContext  *context,
                           GimpProgress *progress,
                           Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;
  gdouble radius;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  radius = args[1].value.pdb_float;
  if (radius < 0.0)
    success = FALSE;

  if (success)
    {
      gimp_channel_feather (gimp_image_get_mask (image),
                            radius, radius, TRUE);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_feather_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_FLOAT,
    "radius",
    "Radius of feather (in pixels)"
  }
};

static ProcRecord selection_feather_proc =
{
  "gimp-selection-feather",
  "gimp-selection-feather",
  "Feather the image's selection",
  "This procedure feathers the selection. Feathering is implemented using a gaussian blur.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  2,
  selection_feather_inargs,
  0,
  NULL,
  { { selection_feather_invoker } }
};

static Argument *
selection_border_invoker (ProcRecord   *proc_record,
                          Gimp         *gimp,
                          GimpContext  *context,
                          GimpProgress *progress,
                          Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;
  gint32 radius;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  radius = args[1].value.pdb_int;
  if (radius < 0)
    success = FALSE;

  if (success)
    {
      gimp_channel_border (gimp_image_get_mask (image),
                           radius, radius, TRUE);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_border_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_INT32,
    "radius",
    "Radius of border (in pixels)"
  }
};

static ProcRecord selection_border_proc =
{
  "gimp-selection-border",
  "gimp-selection-border",
  "Border the image's selection",
  "This procedure borders the selection. Bordering creates a new selection which is defined along the boundary of the previous selection at every point within the specified radius.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  2,
  selection_border_inargs,
  0,
  NULL,
  { { selection_border_invoker } }
};

static Argument *
selection_grow_invoker (ProcRecord   *proc_record,
                        Gimp         *gimp,
                        GimpContext  *context,
                        GimpProgress *progress,
                        Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;
  gint32 steps;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  steps = args[1].value.pdb_int;
  if (steps < 0)
    success = FALSE;

  if (success)
    {
      gimp_channel_grow (gimp_image_get_mask (image),
                         steps, steps, TRUE);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_grow_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_INT32,
    "steps",
    "Steps of grow (in pixels)"
  }
};

static ProcRecord selection_grow_proc =
{
  "gimp-selection-grow",
  "gimp-selection-grow",
  "Grow the image's selection",
  "This procedure grows the selection. Growing involves expanding the boundary in all directions by the specified pixel amount.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  2,
  selection_grow_inargs,
  0,
  NULL,
  { { selection_grow_invoker } }
};

static Argument *
selection_shrink_invoker (ProcRecord   *proc_record,
                          Gimp         *gimp,
                          GimpContext  *context,
                          GimpProgress *progress,
                          Argument     *args)
{
  gboolean success = TRUE;
  GimpImage *image;
  gint32 steps;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  steps = args[1].value.pdb_int;
  if (steps < 0)
    success = FALSE;

  if (success)
    {
      gimp_channel_shrink (gimp_image_get_mask (image),
                           steps, steps, FALSE, TRUE);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_shrink_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  },
  {
    GIMP_PDB_INT32,
    "steps",
    "Steps of shrink (in pixels)"
  }
};

static ProcRecord selection_shrink_proc =
{
  "gimp-selection-shrink",
  "gimp-selection-shrink",
  "Shrink the image's selection",
  "This procedure shrinks the selection. Shrinking invovles trimming the existing selection boundary on all sides by the specified number of pixels.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  2,
  selection_shrink_inargs,
  0,
  NULL,
  { { selection_shrink_invoker } }
};

static Argument *
selection_layer_alpha_invoker (ProcRecord   *proc_record,
                               Gimp         *gimp,
                               GimpContext  *context,
                               GimpProgress *progress,
                               Argument     *args)
{
  gboolean success = TRUE;
  GimpLayer *layer;

  layer = (GimpLayer *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! (GIMP_IS_LAYER (layer) && ! gimp_item_is_removed (GIMP_ITEM (layer))))
    success = FALSE;

  if (success)
    {
      GimpImage *image = gimp_item_get_image (GIMP_ITEM (layer));

      gimp_channel_select_alpha (gimp_image_get_mask (image),
                                 GIMP_DRAWABLE (layer),
                                 GIMP_CHANNEL_OP_REPLACE, FALSE, 0.0, 0.0);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_layer_alpha_inargs[] =
{
  {
    GIMP_PDB_LAYER,
    "layer",
    "Layer with alpha"
  }
};

static ProcRecord selection_layer_alpha_proc =
{
  "gimp-selection-layer-alpha",
  "gimp-selection-layer-alpha",
  "Transfer the specified layer's alpha channel to the selection mask.",
  "This procedure requires a layer with an alpha channel. The alpha channel information is used to create a selection mask such that for any pixel in the image defined in the specified layer, that layer pixel's alpha value is transferred to the selection mask. If the layer is undefined at a particular image pixel, the associated selection mask value is set to 0.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  selection_layer_alpha_inargs,
  0,
  NULL,
  { { selection_layer_alpha_invoker } }
};

static Argument *
selection_load_invoker (ProcRecord   *proc_record,
                        Gimp         *gimp,
                        GimpContext  *context,
                        GimpProgress *progress,
                        Argument     *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! (GIMP_IS_CHANNEL (channel) && ! gimp_item_is_removed (GIMP_ITEM (channel))))
    success = FALSE;

  if (success)
    {
      GimpImage *image;
      gint       off_x, off_y;

      image = gimp_item_get_image (GIMP_ITEM (channel));
      gimp_item_offsets (GIMP_ITEM (channel), &off_x, &off_y);

      gimp_channel_select_channel (gimp_image_get_mask (image),
                                   _("Channel to Selection"),
                                   channel,
                                   off_x, off_y,
                                   GIMP_CHANNEL_OP_REPLACE,
                                   FALSE, 0.0, 0.0);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_load_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  }
};

static ProcRecord selection_load_proc =
{
  "gimp-selection-load",
  "gimp-selection-load",
  "Transfer the specified channel to the selection mask.",
  "This procedure loads the specified channel into the selection mask.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  selection_load_inargs,
  0,
  NULL,
  { { selection_load_invoker } }
};

static Argument *
selection_save_invoker (ProcRecord   *proc_record,
                        Gimp         *gimp,
                        GimpContext  *context,
                        GimpProgress *progress,
                        Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  GimpImage *image;
  GimpChannel *channel = NULL;

  image = gimp_image_get_by_ID (gimp, args[0].value.pdb_int);
  if (! GIMP_IS_IMAGE (image))
    success = FALSE;

  if (success)
    {
      channel = gimp_selection_save (gimp_image_get_mask (image));

      if (! channel)
        success = FALSE;
    }

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    return_vals[1].value.pdb_int = channel ? gimp_item_get_ID (GIMP_ITEM (channel)) : -1;

  return return_vals;
}

static ProcArg selection_save_inargs[] =
{
  {
    GIMP_PDB_IMAGE,
    "image",
    "The image"
  }
};

static ProcArg selection_save_outargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The new channel"
  }
};

static ProcRecord selection_save_proc =
{
  "gimp-selection-save",
  "gimp-selection-save",
  "Copy the selection mask to a new channel.",
  "This procedure copies the selection mask and stores the content in a new channel. The new channel is automatically inserted into the image's list of channels.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  selection_save_inargs,
  1,
  selection_save_outargs,
  { { selection_save_invoker } }
};

static Argument *
selection_combine_invoker (ProcRecord   *proc_record,
                           Gimp         *gimp,
                           GimpContext  *context,
                           GimpProgress *progress,
                           Argument     *args)
{
  gboolean success = TRUE;
  GimpChannel *channel;
  gint32 operation;

  channel = (GimpChannel *) gimp_item_get_by_ID (gimp, args[0].value.pdb_int);
  if (! (GIMP_IS_CHANNEL (channel) && ! gimp_item_is_removed (GIMP_ITEM (channel))))
    success = FALSE;

  operation = args[1].value.pdb_int;
  if (operation < GIMP_CHANNEL_OP_ADD || operation > GIMP_CHANNEL_OP_INTERSECT)
    success = FALSE;

  if (success)
    {
      GimpImage *image;
      gint       off_x, off_y;

      image = gimp_item_get_image (GIMP_ITEM (channel));
      gimp_item_offsets (GIMP_ITEM (channel), &off_x, &off_y);

      gimp_channel_select_channel (gimp_image_get_mask (image),
                                   _("Channel to Selection"),
                                   channel,
                                   off_x, off_y,
                                   operation,
                                   FALSE, 0.0, 0.0);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg selection_combine_inargs[] =
{
  {
    GIMP_PDB_CHANNEL,
    "channel",
    "The channel"
  },
  {
    GIMP_PDB_INT32,
    "operation",
    "The selection operation: { GIMP_CHANNEL_OP_ADD (0), GIMP_CHANNEL_OP_SUBTRACT (1), GIMP_CHANNEL_OP_REPLACE (2), GIMP_CHANNEL_OP_INTERSECT (3) }"
  }
};

static ProcRecord selection_combine_proc =
{
  "gimp-selection-combine",
  "gimp-selection-combine",
  "Combines the specified channel with the selection mask.",
  "This procedure combines the specified channel into the selection mask.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  2,
  selection_combine_inargs,
  0,
  NULL,
  { { selection_combine_invoker } }
};
