/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 * Copyright (C) 1998 Andy Thomas (alt@picnic.demon.co.uk)
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

#include "config.h"

#include <string.h>

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "gui-types.h"

#include "core/gimp.h"
#include "core/gimpcontainer.h"
#include "core/gimpcontext.h"
#include "core/gimpdatafactory.h"
#include "core/gimppalette.h"

#include "pdb/procedural_db.h"

#include "widgets/gimpdatafactoryview.h"

#include "dialogs-constructors.h"
#include "menus.h"
#include "palette-select.h"

#include "libgimp/gimpintl.h"


#define STD_PALETTE_COLUMNS  6
#define STD_PALETTE_ROWS     5 


/*  local function prototypes  */

static void   palette_select_change_callbacks (PaletteSelect *psp,
                                               gboolean       closing);
static void   palette_select_palette_changed  (GimpContext   *context,
                                               GimpPalette   *palette,
                                               PaletteSelect *psp);
static void   palette_select_close_callback   (GtkWidget     *widget,
                                               PaletteSelect *psp);


/*  list of active dialogs  */
static GSList *palette_active_dialogs = NULL;


/*  public functions  */

PaletteSelect *
palette_select_new (Gimp        *gimp,
                    const gchar *title,
		    const gchar *initial_palette,
                    const gchar *callback_name)
{
  PaletteSelect *psp;
  GimpPalette   *active = NULL;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);
  g_return_val_if_fail (title != NULL, NULL);

  if (gimp->no_data)
    {
      static gboolean first_call = TRUE;

      if (first_call)
        gimp_data_factory_data_init (gimp->palette_factory, FALSE);

      first_call = FALSE;
    }

  if (initial_palette && strlen (initial_palette))
    {
      active = (GimpPalette *)
	gimp_container_get_child_by_name (gimp->palette_factory->container,
					  initial_palette);
    }

  if (! active)
    active = gimp_context_get_palette (gimp_get_current_context (gimp));

  if (! active)
    return NULL;

  psp = g_new0 (PaletteSelect, 1);

  /*  Add to active palette dialogs list  */
  palette_active_dialogs = g_slist_append (palette_active_dialogs, psp);

  psp->context       = gimp_context_new (gimp, title, NULL);
  psp->callback_name = g_strdup (callback_name);

  gimp_context_set_palette (psp->context, active);

  g_signal_connect (psp->context, "palette_changed",
                    G_CALLBACK (palette_select_palette_changed),
                    psp);

  /*  the shell  */
  psp->shell = gimp_dialog_new (title, "palette_selection",
				gimp_standard_help_func,
				"dialogs/palette_selection.html",
				GTK_WIN_POS_MOUSE,
				FALSE, TRUE, FALSE,

				GTK_STOCK_CLOSE, palette_select_close_callback,
				psp, NULL, NULL, TRUE, TRUE,

				NULL);

  /*  The Palette List  */
  psp->view = gimp_data_factory_view_new (GIMP_VIEW_TYPE_LIST,
                                          gimp->palette_factory,
                                          dialogs_edit_palette_func,
                                          psp->context,
                                          32,
                                          5, 3,
                                          global_menu_factory, "<Palettes>");

  gtk_container_set_border_width (GTK_CONTAINER (psp->view), 4);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (psp->shell)->vbox), psp->view);
  gtk_widget_show (psp->view);

  gtk_widget_show (psp->shell);

  return psp;
}

void
palette_select_free (PaletteSelect *psp)
{
  g_return_if_fail (psp != NULL);

  gtk_widget_destroy (psp->shell); 

  /* remove from active list */
  palette_active_dialogs = g_slist_remove (palette_active_dialogs, psp);

  if (psp->callback_name)
    g_free (psp->callback_name);

  if (psp->context)
    g_object_unref (psp->context);

  g_free (psp);
}

PaletteSelect *
palette_select_get_by_callback (const gchar *callback_name)
{
  GSList        *list;
  PaletteSelect *psp;

  for (list = palette_active_dialogs; list; list = g_slist_next (list))
    {
      psp = (PaletteSelect *) list->data;

      if (psp->callback_name && ! strcmp (callback_name, psp->callback_name))
	return psp;
    }

  return NULL;
}

void
palette_select_dialogs_check (void)
{
  PaletteSelect *psp;
  GSList        *list;

  list = palette_active_dialogs;

  while (list)
    {
      psp = (PaletteSelect *) list->data;

      list = g_slist_next (list);

      if (psp->callback_name)
        {        
          if (!  procedural_db_lookup (psp->context->gimp, psp->callback_name))
            palette_select_close_callback (NULL, psp); 
        }
    }
}


/*  private functions  */

static void
palette_select_change_callbacks (PaletteSelect *psp,
				 gboolean       closing)
{
  ProcRecord  *proc = NULL;
  GimpPalette *palette;
  Argument    *return_vals; 
  gint         nreturn_vals;

  static gboolean busy = FALSE;

  if (! (psp && psp->callback_name) || busy)
    return;

  busy = TRUE;

  palette = gimp_context_get_palette (psp->context);

  /* If its still registered run it */
  proc = procedural_db_lookup (psp->context->gimp, psp->callback_name);

  if (proc && palette)
    {
      return_vals =
	procedural_db_run_proc (psp->context->gimp,
				psp->callback_name,
				&nreturn_vals,
				GIMP_PDB_STRING, GIMP_OBJECT (palette)->name,
				GIMP_PDB_INT32,  palette->n_colors,
				GIMP_PDB_INT32,  closing,
				GIMP_PDB_END);
 
      if (!return_vals || return_vals[0].value.pdb_int != GIMP_PDB_SUCCESS)
	g_message (_("Unable to run palette callback.\n"
                     "The corresponding plug-in may have crashed."));

      if (return_vals)
        procedural_db_destroy_args (return_vals, nreturn_vals);
    }

  busy = FALSE;
}

static void
palette_select_palette_changed (GimpContext   *context,
				GimpPalette   *palette,
				PaletteSelect *psp)
{
  if (palette)
    palette_select_change_callbacks (psp, FALSE);
}

static void
palette_select_close_callback (GtkWidget     *widget,
			       PaletteSelect *psp)
{
  palette_select_change_callbacks (psp, TRUE);
  palette_select_free (psp); 
}
