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
#include "procedural_db.h"

#include "core/gimp.h"

static ProcRecord version_proc;
static ProcRecord quit_proc;

void
register_misc_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &version_proc);
  procedural_db_register (gimp, &quit_proc);
}

static Argument *
version_invoker (Gimp         *gimp,
                 GimpContext  *context,
                 GimpProgress *progress,
                 Argument     *args)
{
  Argument *return_args;

  return_args = procedural_db_return_args (&version_proc, TRUE);
  return_args[1].value.pdb_pointer = g_strdup (GIMP_VERSION);

  return return_args;
}

static ProcArg version_outargs[] =
{
  {
    GIMP_PDB_STRING,
    "version",
    "The gimp version"
  }
};

static ProcRecord version_proc =
{
  "gimp_version",
  "Returns the host gimp version.",
  "This procedure returns the version number of the currently running gimp.",
  "Manish Singh",
  "Manish Singh",
  "1999",
  FALSE,
  GIMP_INTERNAL,
  0,
  NULL,
  1,
  version_outargs,
  { { version_invoker } }
};

static Argument *
quit_invoker (Gimp         *gimp,
              GimpContext  *context,
              GimpProgress *progress,
              Argument     *args)
{
  gboolean force;

  force = args[0].value.pdb_int ? TRUE : FALSE;

  gimp_exit (gimp, force);

  return procedural_db_return_args (&quit_proc, TRUE);
}

static ProcArg quit_inargs[] =
{
  {
    GIMP_PDB_INT32,
    "force",
    "Flag specifying whether to force the gimp to or exit normally"
  }
};

static ProcRecord quit_proc =
{
  "gimp_quit",
  "Causes the gimp to exit gracefully.",
  "The internal procedure which can either be used to make the gimp quit. If there are unsaved images in an interactive GIMP session, the user will be asked for confirmation. If force is TRUE, the application is quit without querying the user to save any dirty images.",
  "Spencer Kimball & Peter Mattis",
  "Spencer Kimball & Peter Mattis",
  "1995-1996",
  FALSE,
  GIMP_INTERNAL,
  1,
  quit_inargs,
  0,
  NULL,
  { { quit_invoker } }
};
