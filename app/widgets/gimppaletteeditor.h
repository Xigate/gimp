/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
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

#ifndef __GIMP_PALETTE_EDITOR_H__
#define __GIMP_PALETTE_EDITOR_H__


#include "gimpdataeditor.h"

#include "gui/gui-types.h" /* FIXME */


typedef enum
{
  GIMP_UPDATE_COLOR_STATE_NEW,
  GIMP_UPDATE_COLOR_STATE_UPDATE_NEW,
  GIMP_UPDATE_COLOR_STATE_UPDATE
} GimpUpdateColorState;


#define GIMP_TYPE_PALETTE_EDITOR            (gimp_palette_editor_get_type ())
#define GIMP_PALETTE_EDITOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_PALETTE_EDITOR, GimpPaletteEditor))
#define GIMP_PALETTE_EDITOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_PALETTE_EDITOR, GimpPaletteEditorClass))
#define GIMP_IS_PALETTE_EDITOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_PALETTE_EDITOR))
#define GIMP_IS_PALETTE_EDITOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_PALETTE_EDITOR))
#define GIMP_PALETTE_EDITOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_PALETTE_EDITOR, GimpPaletteEditorClass))


typedef struct _GimpPaletteEditorClass GimpPaletteEditorClass;

struct _GimpPaletteEditor
{
  GimpDataEditor    parent_instance;

  GtkWidget        *color_area;
  GtkWidget        *scrolled_window;

  GtkWidget        *color_name;
  GtkAdjustment    *columns_data;

  ColorNotebook    *color_notebook;
  gboolean          color_notebook_active;

  GimpPaletteEntry *color;
  GimpPaletteEntry *dnd_color;

  guint             entry_sig_id;
  gfloat            zoom_factor;  /* range from 0.1 to 4.0 */
  gint              col_width;
  gint              last_width;
  gint              columns;
  gboolean          freeze_update;
  gboolean          columns_valid;
};

struct _GimpPaletteEditorClass
{
  GimpDataEditorClass  parent_class;
};


GType            gimp_palette_editor_get_type     (void) G_GNUC_CONST;

GimpDataEditor * gimp_palette_editor_new          (Gimp *gimp);

void             gimp_palette_editor_update_color (GimpContext          *context,
                                                   const GimpRGB        *color,
                                                   GimpUpdateColorState  state);


#endif /* __GIMP_PALETTE_EDITOR_H__ */
