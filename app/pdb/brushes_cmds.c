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
#include "procedural_db.h"

#include "base/temp-buf.h"
#include "core/gimp.h"
#include "core/gimpbrush.h"
#include "core/gimpcontainer-filter.h"
#include "core/gimpcontext.h"
#include "core/gimpdatafactory.h"
#include "core/gimplist.h"

static ProcRecord brushes_refresh_proc;
static ProcRecord brushes_get_list_proc;
static ProcRecord brushes_get_brush_proc;
static ProcRecord brushes_get_spacing_proc;
static ProcRecord brushes_set_spacing_proc;
static ProcRecord brushes_get_brush_data_proc;

void
register_brushes_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &brushes_refresh_proc);
  procedural_db_register (gimp, &brushes_get_list_proc);
  procedural_db_register (gimp, &brushes_get_brush_proc);
  procedural_db_register (gimp, &brushes_get_spacing_proc);
  procedural_db_register (gimp, &brushes_set_spacing_proc);
  procedural_db_register (gimp, &brushes_get_brush_data_proc);
}

static Argument *
brushes_refresh_invoker (ProcRecord   *proc_record,
                         Gimp         *gimp,
                         GimpContext  *context,
                         GimpProgress *progress,
                         Argument     *args)
{
  gimp_data_factory_data_refresh (gimp->brush_factory);
  return procedural_db_return_values (proc_record, TRUE);
}

static ProcRecord brushes_refresh_proc =
{
  "gimp-brushes-refresh",
  "gimp-brushes-refresh",
  "Refresh current brushes. This function always succeeds.",
  "This procedure retrieves all brushes currently in the user's brush path and updates the brush dialogs accordingly.",
  "Seth Burgess",
  "Seth Burgess",
  "1997",
  NULL,
  GIMP_INTERNAL,
  0,
  NULL,
  0,
  NULL,
  { { brushes_refresh_invoker } }
};

static Argument *
brushes_get_list_invoker (ProcRecord   *proc_record,
                          Gimp         *gimp,
                          GimpContext  *context,
                          GimpProgress *progress,
                          Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gchar *filter;
  gint32 num_brushes = 0;
  gchar **brush_list = NULL;

  filter = (gchar *) args[0].value.pdb_pointer;
  if (filter && !g_utf8_validate (filter, -1, NULL))
    success = FALSE;

  if (success)
    {
      brush_list = gimp_container_get_filtered_name_array (gimp->brush_factory->container,
                                                           filter, &num_brushes);
    }

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    {
      return_vals[1].value.pdb_int = num_brushes;
      return_vals[2].value.pdb_pointer = brush_list;
    }

  return return_vals;
}

static ProcArg brushes_get_list_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "filter",
    "An optional regular expression used to filter the list"
  }
};

static ProcArg brushes_get_list_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "num-brushes",
    "The number of brushes in the brush list"
  },
  {
    GIMP_PDB_STRINGARRAY,
    "brush-list",
    "The list of brush names"
  }
};

static ProcRecord brushes_get_list_proc =
{
  "gimp-brushes-get-list",
  "gimp-brushes-get-list",
  "Retrieve a complete listing of the available brushes.",
  "This procedure returns a complete listing of available GIMP brushes. Each name returned can be used as input to the 'gimp-context-set-brush' procedure.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  brushes_get_list_inargs,
  2,
  brushes_get_list_outargs,
  { { brushes_get_list_invoker } }
};

static Argument *
brushes_get_brush_invoker (ProcRecord   *proc_record,
                           Gimp         *gimp,
                           GimpContext  *context,
                           GimpProgress *progress,
                           Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
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

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    {
      return_vals[1].value.pdb_pointer = name;
      return_vals[2].value.pdb_int = width;
      return_vals[3].value.pdb_int = height;
      return_vals[4].value.pdb_int = spacing;
    }

  return return_vals;
}

static ProcArg brushes_get_brush_outargs[] =
{
  {
    GIMP_PDB_STRING,
    "name",
    "The brush name"
  },
  {
    GIMP_PDB_INT32,
    "width",
    "The brush width"
  },
  {
    GIMP_PDB_INT32,
    "height",
    "The brush height"
  },
  {
    GIMP_PDB_INT32,
    "spacing",
    "The brush spacing (0 <= spacing <= 1000)"
  }
};

static ProcRecord brushes_get_brush_proc =
{
  "gimp-brushes-get-brush",
  "gimp-brushes-get-brush",
  "This procedure is deprecated! Use 'gimp-context-get-brush' instead.",
  "This procedure is deprecated! Use 'gimp-context-get-brush' instead.",
  "",
  "",
  "",
  "gimp-context-get-brush",
  GIMP_INTERNAL,
  0,
  NULL,
  4,
  brushes_get_brush_outargs,
  { { brushes_get_brush_invoker } }
};

static Argument *
brushes_get_spacing_invoker (ProcRecord   *proc_record,
                             Gimp         *gimp,
                             GimpContext  *context,
                             GimpProgress *progress,
                             Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gint32 spacing = 0;

  GimpBrush *brush = gimp_context_get_brush (context);

  if (brush)
    spacing = gimp_brush_get_spacing (brush);
  else
    success = FALSE;

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    return_vals[1].value.pdb_int = spacing;

  return return_vals;
}

static ProcArg brushes_get_spacing_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "spacing",
    "The brush spacing (0 <= spacing <= 1000)"
  }
};

static ProcRecord brushes_get_spacing_proc =
{
  "gimp-brushes-get-spacing",
  "gimp-brushes-get-spacing",
  "This procedure is deprecated! Use 'gimp-brush-get-spacing' instead.",
  "This procedure is deprecated! Use 'gimp-brush-get-spacing' instead.",
  "",
  "",
  "",
  "gimp-brush-get-spacing",
  GIMP_INTERNAL,
  0,
  NULL,
  1,
  brushes_get_spacing_outargs,
  { { brushes_get_spacing_invoker } }
};

static Argument *
brushes_set_spacing_invoker (ProcRecord   *proc_record,
                             Gimp         *gimp,
                             GimpContext  *context,
                             GimpProgress *progress,
                             Argument     *args)
{
  gboolean success = TRUE;
  gint32 spacing;

  spacing = args[0].value.pdb_int;
  if (spacing < 0 || spacing > 1000)
    success = FALSE;

  if (success)
    {
      gimp_brush_set_spacing (gimp_context_get_brush (context), spacing);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg brushes_set_spacing_inargs[] =
{
  {
    GIMP_PDB_INT32,
    "spacing",
    "The brush spacing (0 <= spacing <= 1000)"
  }
};

static ProcRecord brushes_set_spacing_proc =
{
  "gimp-brushes-set-spacing",
  "gimp-brushes-set-spacing",
  "This procedure is deprecated! Use 'gimp-brush-set-spacing' instead.",
  "This procedure is deprecated! Use 'gimp-brush-set-spacing' instead.",
  "",
  "",
  "",
  "gimp-brush-set-spacing",
  GIMP_INTERNAL,
  1,
  brushes_set_spacing_inargs,
  0,
  NULL,
  { { brushes_set_spacing_invoker } }
};

static Argument *
brushes_get_brush_data_invoker (ProcRecord   *proc_record,
                                Gimp         *gimp,
                                GimpContext  *context,
                                GimpProgress *progress,
                                Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gchar *name;
  gchar *actual_name = NULL;
  gdouble opacity = 0.0;
  gint32 spacing = 0;
  gint32 paint_mode = 0;
  gint32 width = 0;
  gint32 height = 0;
  gint32 length = 0;
  guint8 *mask_data = NULL;

  name = (gchar *) args[0].value.pdb_pointer;
  if (name && !g_utf8_validate (name, -1, NULL))
    success = FALSE;

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

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    {
      return_vals[1].value.pdb_pointer = actual_name;
      return_vals[2].value.pdb_float = opacity;
      return_vals[3].value.pdb_int = spacing;
      return_vals[4].value.pdb_int = paint_mode;
      return_vals[5].value.pdb_int = width;
      return_vals[6].value.pdb_int = height;
      return_vals[7].value.pdb_int = length;
      return_vals[8].value.pdb_pointer = mask_data;
    }

  return return_vals;
}

static ProcArg brushes_get_brush_data_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "name",
    "The brush name (\"\" means current active brush)"
  }
};

static ProcArg brushes_get_brush_data_outargs[] =
{
  {
    GIMP_PDB_STRING,
    "actual-name",
    "The brush name"
  },
  {
    GIMP_PDB_FLOAT,
    "opacity",
    "The brush opacity (0 <= opacity <= 100)"
  },
  {
    GIMP_PDB_INT32,
    "spacing",
    "The brush spacing (0 <= spacing <= 1000)"
  },
  {
    GIMP_PDB_INT32,
    "paint-mode",
    "The paint mode: { GIMP_NORMAL_MODE (0), GIMP_DISSOLVE_MODE (1), GIMP_BEHIND_MODE (2), GIMP_MULTIPLY_MODE (3), GIMP_SCREEN_MODE (4), GIMP_OVERLAY_MODE (5), GIMP_DIFFERENCE_MODE (6), GIMP_ADDITION_MODE (7), GIMP_SUBTRACT_MODE (8), GIMP_DARKEN_ONLY_MODE (9), GIMP_LIGHTEN_ONLY_MODE (10), GIMP_HUE_MODE (11), GIMP_SATURATION_MODE (12), GIMP_COLOR_MODE (13), GIMP_VALUE_MODE (14), GIMP_DIVIDE_MODE (15), GIMP_DODGE_MODE (16), GIMP_BURN_MODE (17), GIMP_HARDLIGHT_MODE (18), GIMP_SOFTLIGHT_MODE (19), GIMP_GRAIN_EXTRACT_MODE (20), GIMP_GRAIN_MERGE_MODE (21), GIMP_COLOR_ERASE_MODE (22) }"
  },
  {
    GIMP_PDB_INT32,
    "width",
    "The brush width"
  },
  {
    GIMP_PDB_INT32,
    "height",
    "The brush height"
  },
  {
    GIMP_PDB_INT32,
    "length",
    "Length of brush mask data"
  },
  {
    GIMP_PDB_INT8ARRAY,
    "mask-data",
    "The brush mask data"
  }
};

static ProcRecord brushes_get_brush_data_proc =
{
  "gimp-brushes-get-brush-data",
  "gimp-brushes-get-brush-data",
  "This procedure is deprecated! Use 'gimp-brush-get-pixels' instead.",
  "This procedure is deprecated! Use 'gimp-brush-get-pixels' instead.",
  "",
  "",
  "",
  "gimp-brush-get-pixels",
  GIMP_INTERNAL,
  1,
  brushes_get_brush_data_inargs,
  8,
  brushes_get_brush_data_outargs,
  { { brushes_get_brush_data_invoker } }
};
