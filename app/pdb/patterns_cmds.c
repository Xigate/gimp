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
#include "core/gimpcontainer-filter.h"
#include "core/gimpcontext.h"
#include "core/gimpdatafactory.h"
#include "core/gimplist.h"
#include "core/gimppattern.h"

static ProcRecord patterns_refresh_proc;
static ProcRecord patterns_get_list_proc;
static ProcRecord patterns_get_pattern_proc;
static ProcRecord patterns_get_pattern_data_proc;

void
register_patterns_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &patterns_refresh_proc);
  procedural_db_register (gimp, &patterns_get_list_proc);
  procedural_db_register (gimp, &patterns_get_pattern_proc);
  procedural_db_register (gimp, &patterns_get_pattern_data_proc);
}

static Argument *
patterns_refresh_invoker (ProcRecord   *proc_record,
                          Gimp         *gimp,
                          GimpContext  *context,
                          GimpProgress *progress,
                          Argument     *args)
{
  gimp_data_factory_data_refresh (gimp->pattern_factory);
  return procedural_db_return_values (proc_record, TRUE);
}

static ProcRecord patterns_refresh_proc =
{
  "gimp-patterns-refresh",
  "gimp-patterns-refresh",
  "Refresh current patterns. This function always succeeds.",
  "This procedure retrieves all patterns currently in the user's pattern path and updates all pattern dialogs accordingly.",
  "Michael Natterer <mitch@gimp.org>",
  "Michael Natterer",
  "2002",
  NULL,
  GIMP_INTERNAL,
  0,
  NULL,
  0,
  NULL,
  { { patterns_refresh_invoker } }
};

static Argument *
patterns_get_list_invoker (ProcRecord   *proc_record,
                           Gimp         *gimp,
                           GimpContext  *context,
                           GimpProgress *progress,
                           Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gchar *filter;
  gint32 num_patterns = 0;
  gchar **pattern_list = NULL;

  filter = (gchar *) args[0].value.pdb_pointer;
  if (filter && !g_utf8_validate (filter, -1, NULL))
    success = FALSE;

  if (success)
    {
      pattern_list = gimp_container_get_filtered_name_array (gimp->pattern_factory->container,
                                                             filter, &num_patterns);
    }

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    {
      return_vals[1].value.pdb_int = num_patterns;
      return_vals[2].value.pdb_pointer = pattern_list;
    }

  return return_vals;
}

static ProcArg patterns_get_list_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "filter",
    "An optional regular expression used to filter the list"
  }
};

static ProcArg patterns_get_list_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "num-patterns",
    "The number of patterns in the pattern list"
  },
  {
    GIMP_PDB_STRINGARRAY,
    "pattern-list",
    "The list of pattern names"
  }
};

static ProcRecord patterns_get_list_proc =
{
  "gimp-patterns-get-list",
  "gimp-patterns-get-list",
  "Retrieve a complete listing of the available patterns.",
  "This procedure returns a complete listing of available GIMP patterns. Each name returned can be used as input to the 'gimp-context-set-pattern'.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  NULL,
  GIMP_INTERNAL,
  1,
  patterns_get_list_inargs,
  2,
  patterns_get_list_outargs,
  { { patterns_get_list_invoker } }
};

static Argument *
patterns_get_pattern_invoker (ProcRecord   *proc_record,
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

  GimpPattern *pattern = gimp_context_get_pattern (context);

  if (pattern)
    {
      name   = g_strdup (gimp_object_get_name (GIMP_OBJECT (pattern)));
      width  = pattern->mask->width;
      height = pattern->mask->height;
    }
  else
    success = FALSE;

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    {
      return_vals[1].value.pdb_pointer = name;
      return_vals[2].value.pdb_int = width;
      return_vals[3].value.pdb_int = height;
    }

  return return_vals;
}

static ProcArg patterns_get_pattern_outargs[] =
{
  {
    GIMP_PDB_STRING,
    "name",
    "The pattern name"
  },
  {
    GIMP_PDB_INT32,
    "width",
    "The pattern width"
  },
  {
    GIMP_PDB_INT32,
    "height",
    "The pattern height"
  }
};

static ProcRecord patterns_get_pattern_proc =
{
  "gimp-patterns-get-pattern",
  "gimp-patterns-get-pattern",
  "This procedure is deprecated! Use 'gimp-context-get-pattern' instead.",
  "This procedure is deprecated! Use 'gimp-context-get-pattern' instead.",
  "",
  "",
  "",
  "gimp-context-get-pattern",
  GIMP_INTERNAL,
  0,
  NULL,
  3,
  patterns_get_pattern_outargs,
  { { patterns_get_pattern_invoker } }
};

static Argument *
patterns_get_pattern_data_invoker (ProcRecord   *proc_record,
                                   Gimp         *gimp,
                                   GimpContext  *context,
                                   GimpProgress *progress,
                                   Argument     *args)
{
  gboolean success = TRUE;
  Argument *return_vals;
  gchar *name;
  gchar *actual_name = NULL;
  gint32 width = 0;
  gint32 height = 0;
  gint32 mask_bpp = 0;
  gint32 length = 0;
  guint8 *mask_data = NULL;

  name = (gchar *) args[0].value.pdb_pointer;
  if (name && !g_utf8_validate (name, -1, NULL))
    success = FALSE;

  if (success)
    {
      GimpPattern *pattern;

      if (name && strlen (name))
        {
          pattern = (GimpPattern *)
            gimp_container_get_child_by_name (gimp->pattern_factory->container,
                                              name);
        }
      else
        {
          pattern = gimp_context_get_pattern (context);
        }

      if (pattern)
        {
          actual_name = g_strdup (gimp_object_get_name (GIMP_OBJECT (pattern)));
          width       = pattern->mask->width;
          height      = pattern->mask->height;
          mask_bpp    = pattern->mask->bytes;
          length      = pattern->mask->height * pattern->mask->width *
                        pattern->mask->bytes;
          mask_data   = g_memdup (temp_buf_data (pattern->mask), length);
        }
      else
        success = FALSE;
    }

  return_vals = procedural_db_return_values (proc_record, success);

  if (success)
    {
      return_vals[1].value.pdb_pointer = actual_name;
      return_vals[2].value.pdb_int = width;
      return_vals[3].value.pdb_int = height;
      return_vals[4].value.pdb_int = mask_bpp;
      return_vals[5].value.pdb_int = length;
      return_vals[6].value.pdb_pointer = mask_data;
    }

  return return_vals;
}

static ProcArg patterns_get_pattern_data_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "name",
    "The pattern name (\"\" means currently active pattern)"
  }
};

static ProcArg patterns_get_pattern_data_outargs[] =
{
  {
    GIMP_PDB_STRING,
    "actual-name",
    "The pattern name"
  },
  {
    GIMP_PDB_INT32,
    "width",
    "The pattern width"
  },
  {
    GIMP_PDB_INT32,
    "height",
    "The pattern height"
  },
  {
    GIMP_PDB_INT32,
    "mask-bpp",
    "Pattern bytes per pixel"
  },
  {
    GIMP_PDB_INT32,
    "length",
    "Length of pattern mask data"
  },
  {
    GIMP_PDB_INT8ARRAY,
    "mask-data",
    "The pattern mask data"
  }
};

static ProcRecord patterns_get_pattern_data_proc =
{
  "gimp-patterns-get-pattern-data",
  "gimp-patterns-get-pattern-data",
  "This procedure is deprecated! Use 'gimp-pattern-get-pixels' instead.",
  "This procedure is deprecated! Use 'gimp-pattern-get-pixels' instead.",
  "",
  "",
  "",
  "gimp-pattern-get-pixels",
  GIMP_INTERNAL,
  1,
  patterns_get_pattern_data_inargs,
  6,
  patterns_get_pattern_data_outargs,
  { { patterns_get_pattern_data_invoker } }
};
