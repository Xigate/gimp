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

#ifndef __GIMP_COLORMAP_EDITOR_H__
#define __GIMP_COLORMAP_EDITOR_H__


#include "gimpeditor.h"

#include "gui/gui-types.h" /* temp hack */


#define GIMP_TYPE_COLORMAP_EDITOR            (gimp_colormap_editor_get_type ())
#define GIMP_COLORMAP_EDITOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_COLORMAP_EDITOR, GimpColormapEditor))
#define GIMP_COLORMAP_EDITOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_COLORMAP_EDITOR, GimpColormapEditorClass))
#define GIMP_IS_COLORMAP_EDITOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_COLORMAP_EDITOR))
#define GIMP_IS_COLORMAP_EDITOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_COLORMAP_EDITOR))
#define GIMP_COLORMAP_EDITOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_COLORMAP_EDITOR, GimpColormapEditorClass))


typedef struct _GimpColormapEditorClass GimpColormapEditorClass;

struct _GimpColormapEditor
{
  GimpEditor       parent_instance;

  GimpItemFactory *item_factory;

  GimpImage       *gimage;

  gint             col_index;
  gint             dnd_col_index;
  GtkWidget       *palette;
  gint             xn;
  gint             yn;
  gint             cellsize;
  GtkAdjustment   *index_adjustment;
  GtkWidget       *index_spinbutton;
  GtkWidget       *color_entry;
  ColorNotebook   *color_notebook;
};

struct _GimpColormapEditorClass
{
  GimpEditorClass  parent_class;

  void (* selected) (GimpColormapEditor *gcd);
};


GType       gimp_colormap_editor_get_type  (void) G_GNUC_CONST;

GtkWidget * gimp_colormap_editor_new       (GimpImage          *gimage,
                                            GimpMenuFactory    *menu_factory);

void        gimp_colormap_editor_selected  (GimpColormapEditor *editor);

void        gimp_colormap_editor_set_image (GimpColormapEditor *editor,
					    GimpImage          *gimage);
GimpImage * gimp_colormap_editor_get_image (GimpColormapEditor *editor);
gint        gimp_colormap_editor_col_index (GimpColormapEditor *editor);


#endif /* __GIMP_COLORMAP_EDITOR_H__ */
