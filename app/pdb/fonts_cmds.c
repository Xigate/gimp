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
#include "gimpargument.h"
#include "gimpprocedure.h"
#include "procedural_db.h"
#include "core/gimpparamspecs.h"

#include "core/gimp.h"
#include "core/gimpcontainer-filter.h"
#include "core/gimpcontainer.h"
#include "text/gimp-fonts.h"

static GimpProcedure fonts_refresh_proc;
static GimpProcedure fonts_get_list_proc;

void
register_fonts_procs (Gimp *gimp)
{
  GimpProcedure *procedure;

  /*
   * fonts_refresh
   */
  procedure = gimp_procedure_init (&fonts_refresh_proc, 0, 0);
  procedural_db_register (gimp, procedure);

  /*
   * fonts_get_list
   */
  procedure = gimp_procedure_init (&fonts_get_list_proc, 1, 2);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_string ("filter",
                                                       "filter",
                                                       "An optional regular expression used to filter the list",
                                                       FALSE, TRUE,
                                                       NULL,
                                                       GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("num-fonts",
                                                          "num fonts",
                                                          "The number of available fonts",
                                                          0, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_string_array ("font-list",
                                                                 "font list",
                                                                 "The list of font names",
                                                                 GIMP_PARAM_READWRITE));
  procedural_db_register (gimp, procedure);

}

static GimpArgument *
fonts_refresh_invoker (GimpProcedure      *procedure,
                       Gimp               *gimp,
                       GimpContext        *context,
                       GimpProgress       *progress,
                       const GimpArgument *args)
{
  gimp_fonts_load (gimp);
  return gimp_procedure_get_return_values (procedure, TRUE);
}

static GimpProcedure fonts_refresh_proc =
{
  TRUE, TRUE,
  "gimp-fonts-refresh",
  "gimp-fonts-refresh",
  "Refresh current fonts. This function always succeeds.",
  "This procedure retrieves all fonts currently in the user's font path and updates the font dialogs accordingly.",
  "Sven Neumann <sven@gimp.org>",
  "Sven Neumann",
  "2003",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { fonts_refresh_invoker } }
};

static GimpArgument *
fonts_get_list_invoker (GimpProcedure      *procedure,
                        Gimp               *gimp,
                        GimpContext        *context,
                        GimpProgress       *progress,
                        const GimpArgument *args)
{
  gboolean success = TRUE;
  GimpArgument *return_vals;
  const gchar *filter;
  gint32 num_fonts = 0;
  gchar **font_list = NULL;

  filter = g_value_get_string (&args[0].value);

  if (success)
    {
      font_list = gimp_container_get_filtered_name_array (gimp->fonts,
                                                          filter, &num_fonts);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success);

  if (success)
    {
      g_value_set_int (&return_vals[1].value, num_fonts);
      gimp_value_take_stringarray (&return_vals[2].value, font_list, num_fonts);
    }

  return return_vals;
}

static GimpProcedure fonts_get_list_proc =
{
  TRUE, TRUE,
  "gimp-fonts-get-list",
  "gimp-fonts-get-list",
  "Retrieve the list of loaded fonts.",
  "This procedure returns a list of the fonts that are currently available.",
  "Sven Neumann <sven@gimp.org>",
  "Sven Neumann",
  "2003",
  NULL,
  GIMP_INTERNAL,
  0, NULL, 0, NULL,
  { { fonts_get_list_invoker } }
};
