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

#include "core/gimp.h"
#include "gimp-intl.h"
#include "plug-in/plug-in-progress.h"
#include "plug-in/plug-in.h"

static ProcRecord message_proc;
static ProcRecord message_get_handler_proc;
static ProcRecord message_set_handler_proc;

void
register_message_procs (Gimp *gimp)
{
  procedural_db_register (gimp, &message_proc);
  procedural_db_register (gimp, &message_get_handler_proc);
  procedural_db_register (gimp, &message_set_handler_proc);
}

static Argument *
message_invoker (ProcRecord   *proc_record,
                 Gimp         *gimp,
                 GimpContext  *context,
                 GimpProgress *progress,
                 Argument     *args)
{
  gboolean success = TRUE;
  gchar *message;

  message = (gchar *) args[0].value.pdb_pointer;
  if (message == NULL || !g_utf8_validate (message, -1, NULL))
    success = FALSE;

  if (success)
    {
      if (gimp->current_plug_in)
        plug_in_progress_message (gimp->current_plug_in, message);
      else
        gimp_message (gimp, NULL, message);
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg message_inargs[] =
{
  {
    GIMP_PDB_STRING,
    "message",
    "Message to display in the dialog"
  }
};

static ProcRecord message_proc =
{
  "gimp-message",
  "gimp-message",
  "Displays a dialog box with a message.",
  "Displays a dialog box with a message. Useful for status or error reporting. The message must be in UTF-8 encoding.",
  "Manish Singh",
  "Manish Singh",
  "1998",
  NULL,
  GIMP_INTERNAL,
  1,
  message_inargs,
  0,
  NULL,
  { { message_invoker } }
};

static Argument *
message_get_handler_invoker (ProcRecord   *proc_record,
                             Gimp         *gimp,
                             GimpContext  *context,
                             GimpProgress *progress,
                             Argument     *args)
{
  Argument *return_vals;
  gint32 handler = 0;

  handler = gimp->message_handler;

  return_vals = procedural_db_return_values (proc_record, TRUE);
  return_vals[1].value.pdb_int = handler;

  return return_vals;
}

static ProcArg message_get_handler_outargs[] =
{
  {
    GIMP_PDB_INT32,
    "handler",
    "The current handler type: { GIMP_MESSAGE_BOX (0), GIMP_CONSOLE (1), GIMP_ERROR_CONSOLE (2) }"
  }
};

static ProcRecord message_get_handler_proc =
{
  "gimp-message-get-handler",
  "gimp-message-get-handler",
  "Returns the current state of where warning messages are displayed.",
  "This procedure returns the way g_message warnings are displayed. They can be shown in a dialog box or printed on the console where gimp was started.",
  "Manish Singh",
  "Manish Singh",
  "1998",
  NULL,
  GIMP_INTERNAL,
  0,
  NULL,
  1,
  message_get_handler_outargs,
  { { message_get_handler_invoker } }
};

static Argument *
message_set_handler_invoker (ProcRecord   *proc_record,
                             Gimp         *gimp,
                             GimpContext  *context,
                             GimpProgress *progress,
                             Argument     *args)
{
  gboolean success = TRUE;
  gint32 handler;

  handler = args[0].value.pdb_int;
  if (handler < GIMP_MESSAGE_BOX || handler > GIMP_ERROR_CONSOLE)
    success = FALSE;

  if (success)
    {
      gimp->message_handler = handler;
    }

  return procedural_db_return_values (proc_record, success);
}

static ProcArg message_set_handler_inargs[] =
{
  {
    GIMP_PDB_INT32,
    "handler",
    "The new handler type: { GIMP_MESSAGE_BOX (0), GIMP_CONSOLE (1), GIMP_ERROR_CONSOLE (2) }"
  }
};

static ProcRecord message_set_handler_proc =
{
  "gimp-message-set-handler",
  "gimp-message-set-handler",
  "Controls where warning messages are displayed.",
  "This procedure controls how g_message warnings are displayed. They can be shown in a dialog box or printed on the console where gimp was started.",
  "Manish Singh",
  "Manish Singh",
  "1998",
  NULL,
  GIMP_INTERNAL,
  1,
  message_set_handler_inargs,
  0,
  NULL,
  { { message_set_handler_invoker } }
};
