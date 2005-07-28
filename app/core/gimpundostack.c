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

#include "config.h"

#include <glib-object.h>

#include "core-types.h"

#include "gimpimage.h"
#include "gimplist.h"
#include "gimpundo.h"
#include "gimpundostack.h"


static void    gimp_undo_stack_class_init  (GimpUndoStackClass  *klass);
static void    gimp_undo_stack_init        (GimpUndoStack       *stack);

static void    gimp_undo_stack_finalize    (GObject             *object);

static gint64  gimp_undo_stack_get_memsize (GimpObject          *object,
                                            gint64              *gui_size);

static void    gimp_undo_stack_pop         (GimpUndo            *undo,
                                            GimpUndoMode         undo_mode,
                                            GimpUndoAccumulator *accum);
static void    gimp_undo_stack_free        (GimpUndo            *undo,
                                            GimpUndoMode         undo_mode);


static GimpUndoClass *parent_class = NULL;


GType
gimp_undo_stack_get_type (void)
{
  static GType undo_stack_type = 0;

  if (! undo_stack_type)
    {
      static const GTypeInfo undo_stack_info =
      {
        sizeof (GimpUndoStackClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) gimp_undo_stack_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data     */
        sizeof (GimpUndoStack),
        0,              /* n_preallocs    */
        (GInstanceInitFunc) gimp_undo_stack_init,
      };

      undo_stack_type = g_type_register_static (GIMP_TYPE_UNDO,
                                                "GimpUndoStack",
                                                &undo_stack_info, 0);
    }

  return undo_stack_type;
}

static void
gimp_undo_stack_class_init (GimpUndoStackClass *klass)
{
  GObjectClass    *object_class      = G_OBJECT_CLASS (klass);
  GimpObjectClass *gimp_object_class = GIMP_OBJECT_CLASS (klass);
  GimpUndoClass   *undo_class        = GIMP_UNDO_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->finalize         = gimp_undo_stack_finalize;

  gimp_object_class->get_memsize = gimp_undo_stack_get_memsize;

  undo_class->pop                = gimp_undo_stack_pop;
  undo_class->free               = gimp_undo_stack_free;
}

static void
gimp_undo_stack_init (GimpUndoStack *stack)
{
  stack->undos = gimp_list_new (GIMP_TYPE_UNDO, FALSE);
}

static void
gimp_undo_stack_finalize (GObject *object)
{
  GimpUndoStack *stack = GIMP_UNDO_STACK (object);

  if (stack->undos)
    {
      g_object_unref (stack->undos);
      stack->undos = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static gint64
gimp_undo_stack_get_memsize (GimpObject *object,
                             gint64     *gui_size)
{
  GimpUndoStack *stack   = GIMP_UNDO_STACK (object);
  gint64         memsize = 0;

  if (stack->undos)
    memsize += gimp_object_get_memsize (GIMP_OBJECT (stack->undos), gui_size);

  return memsize + GIMP_OBJECT_CLASS (parent_class)->get_memsize (object,
                                                                  gui_size);
}

static void
gimp_undo_stack_pop (GimpUndo            *undo,
                     GimpUndoMode         undo_mode,
                     GimpUndoAccumulator *accum)
{
  GimpUndoStack *stack = GIMP_UNDO_STACK (undo);
  GList         *list;

  for (list = GIMP_LIST (stack->undos)->list;
       list;
       list = g_list_next (list))
    {
      GimpUndo *child = list->data;

      gimp_undo_pop (child, undo_mode, accum);
    }
}

static void
gimp_undo_stack_free (GimpUndo     *undo,
                      GimpUndoMode  undo_mode)
{
  GimpUndoStack *stack = GIMP_UNDO_STACK (undo);
  GList         *list;

  for (list = GIMP_LIST (stack->undos)->list;
       list;
       list = g_list_next (list))
    {
      GimpUndo *child = list->data;

      gimp_undo_free (child, undo_mode);
      g_object_unref (child);
    }

  while (GIMP_LIST (stack->undos)->list)
    gimp_container_remove (GIMP_CONTAINER (stack->undos),
                           GIMP_LIST (stack->undos)->list->data);
}

GimpUndoStack *
gimp_undo_stack_new (GimpImage *gimage)
{
  g_return_val_if_fail (GIMP_IS_IMAGE (gimage), NULL);

  return g_object_new (GIMP_TYPE_UNDO_STACK,
                       "image", gimage,
                       NULL);
}

void
gimp_undo_stack_push_undo (GimpUndoStack *stack,
                           GimpUndo      *undo)
{
  g_return_if_fail (GIMP_IS_UNDO_STACK (stack));
  g_return_if_fail (GIMP_IS_UNDO (undo));

  gimp_container_add (GIMP_CONTAINER (stack->undos), GIMP_OBJECT (undo));
}

GimpUndo *
gimp_undo_stack_pop_undo (GimpUndoStack       *stack,
                          GimpUndoMode         undo_mode,
                          GimpUndoAccumulator *accum)
{
  GimpUndo *undo;

  g_return_val_if_fail (GIMP_IS_UNDO_STACK (stack), NULL);
  g_return_val_if_fail (accum != NULL, NULL);

  undo = (GimpUndo *)
    gimp_container_get_child_by_index (GIMP_CONTAINER (stack->undos), 0);

  if (undo)
    {
      gimp_container_remove (GIMP_CONTAINER (stack->undos), GIMP_OBJECT (undo));
      gimp_undo_pop (undo, undo_mode, accum);

      return undo;
    }

  return NULL;
}

GimpUndo *
gimp_undo_stack_free_bottom (GimpUndoStack *stack,
                             GimpUndoMode   undo_mode)
{
  GimpUndo *undo;
  gint      n_children;

  g_return_val_if_fail (GIMP_IS_UNDO_STACK (stack), NULL);

  n_children = gimp_container_num_children (GIMP_CONTAINER (stack->undos));

  undo = (GimpUndo *)
    gimp_container_get_child_by_index (GIMP_CONTAINER (stack->undos),
                                       n_children - 1);

  if (undo)
    {
      gimp_container_remove (GIMP_CONTAINER (stack->undos), GIMP_OBJECT (undo));
      gimp_undo_free (undo, undo_mode);

      return undo;
    }

  return NULL;
}

GimpUndo *
gimp_undo_stack_peek (GimpUndoStack *stack)
{
  GimpObject *object;

  g_return_val_if_fail (GIMP_IS_UNDO_STACK (stack), NULL);

  object = gimp_container_get_child_by_index (GIMP_CONTAINER (stack->undos), 0);

  return (object ? GIMP_UNDO (object) : NULL);
}

gint
gimp_undo_stack_get_depth (GimpUndoStack *stack)
{
  g_return_val_if_fail (GIMP_IS_UNDO_STACK (stack), 0);

  return gimp_container_num_children (stack->undos);
}
