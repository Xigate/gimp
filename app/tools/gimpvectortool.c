/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * Vector tool
 * Copyright (C) 2003 Simon Budig  <simon@gimp.org>
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

#include <stdlib.h>

#include <gtk/gtk.h>

#include "libgimpmath/gimpmath.h"
#include "libgimpbase/gimpbase.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "tools-types.h"

#include "core/gimp.h"
#include "core/gimpcontext.h"
#include "core/gimpimage.h"
#include "core/gimpimage-undo-push.h"
#include "core/gimplist.h"
#include "core/gimptoolinfo.h"

#include "vectors/gimpanchor.h"
#include "vectors/gimpvectors.h"
#include "vectors/gimpbezierstroke.h"

#include "widgets/gimphelp-ids.h"

#include "display/gimpdisplay.h"
#include "display/gimpdisplay-foreach.h"

#include "gimptoolcontrol.h"
#include "gimpvectoroptions.h"
#include "gimpvectortool.h"

#include "gimp-intl.h"


#define TARGET 9

#define TOGGLE_MASK GDK_SHIFT_MASK
#define MOVE_MASK   GDK_MOD1_MASK
#define INSDEL_MASK GDK_CONTROL_MASK


/*  local function prototypes  */

static void   gimp_vector_tool_class_init      (GimpVectorToolClass *klass);
static void   gimp_vector_tool_init            (GimpVectorTool      *tool);

static void   gimp_vector_tool_control         (GimpTool        *tool,
                                                GimpToolAction   action,
                                                GimpDisplay     *gdisp);
static void   gimp_vector_tool_button_press    (GimpTool        *tool,
                                                GimpCoords      *coords,
                                                guint32          time,
                                                GdkModifierType  state,
                                                GimpDisplay     *gdisp);
static void   gimp_vector_tool_button_release  (GimpTool        *tool,
                                                GimpCoords      *coords,
                                                guint32          time,
                                                GdkModifierType  state,
                                                GimpDisplay     *gdisp);
static void   gimp_vector_tool_motion          (GimpTool        *tool,
                                                GimpCoords      *coords,
                                                guint32          time,
                                                GdkModifierType  state,
                                                GimpDisplay     *gdisp);
static void   gimp_vector_tool_modifier_key    (GimpTool        *tool,
                                                GdkModifierType  key,
                                                gboolean         press,
                                                GdkModifierType  state,
                                                GimpDisplay     *gdisp);
static void   gimp_vector_tool_oper_update     (GimpTool        *tool,
                                                GimpCoords      *coords,
                                                GdkModifierType  state,
                                                GimpDisplay     *gdisp);
static void   gimp_vector_tool_cursor_update   (GimpTool        *tool,
                                                GimpCoords      *coords,
                                                GdkModifierType  state,
                                                GimpDisplay     *gdisp);

static void   gimp_vector_tool_draw            (GimpDrawTool    *draw_tool);

static void   gimp_vector_tool_vectors_changed (GimpImage       *gimage,
                                                GimpVectorTool  *vector_tool);
static void   gimp_vector_tool_vectors_removed (GimpVectors     *vectors,
                                                GimpVectorTool  *vector_tool);
static void   gimp_vector_tool_vectors_visible (GimpVectors     *vectors,
                                                GimpVectorTool  *vector_tool);
static void   gimp_vector_tool_vectors_freeze  (GimpVectors     *vectors,
                                                GimpVectorTool  *vector_tool);
static void   gimp_vector_tool_vectors_thaw    (GimpVectors     *vectors,
                                                GimpVectorTool  *vector_tool);

static void   gimp_vector_tool_verify_state    (GimpVectorTool  *vector_tool);

static GimpDrawToolClass *parent_class = NULL;


void
gimp_vector_tool_register (GimpToolRegisterCallback callback,
                           gpointer                 data)
{
  (* callback) (GIMP_TYPE_VECTOR_TOOL,
                GIMP_TYPE_VECTOR_OPTIONS,
                gimp_vector_options_gui,
                0,
                "gimp-path-tool",
                _("Paths"),
                _("Create and edit paths"),
                N_("/Tools/_Paths"), NULL,
                NULL, GIMP_HELP_TOOL_PATH,
                GIMP_STOCK_TOOL_PATH,
                data);
}

GType
gimp_vector_tool_get_type (void)
{
  static GType tool_type = 0;

  if (! tool_type)
    {
      static const GTypeInfo tool_info =
      {
        sizeof (GimpVectorToolClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_vector_tool_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpVectorTool),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_vector_tool_init,
      };

      tool_type = g_type_register_static (GIMP_TYPE_DRAW_TOOL,
					  "GimpVectorTool",
                                          &tool_info, 0);
    }

  return tool_type;
}

static void
gimp_vector_tool_class_init (GimpVectorToolClass *klass)
{
  GObjectClass      *object_class;
  GimpToolClass     *tool_class;
  GimpDrawToolClass *draw_tool_class;

  object_class    = G_OBJECT_CLASS (klass);
  tool_class      = GIMP_TOOL_CLASS (klass);
  draw_tool_class = GIMP_DRAW_TOOL_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  tool_class->control        = gimp_vector_tool_control;
  tool_class->button_press   = gimp_vector_tool_button_press;
  tool_class->button_release = gimp_vector_tool_button_release;
  tool_class->motion         = gimp_vector_tool_motion;
  tool_class->modifier_key   = gimp_vector_tool_modifier_key;
  tool_class->oper_update    = gimp_vector_tool_oper_update;
  tool_class->cursor_update  = gimp_vector_tool_cursor_update;

  draw_tool_class->draw      = gimp_vector_tool_draw;
}

static void
gimp_vector_tool_init (GimpVectorTool *vector_tool)
{
  GimpTool *tool = GIMP_TOOL (vector_tool);

  gimp_tool_control_set_scroll_lock (tool->control, TRUE);
  gimp_tool_control_set_motion_mode (tool->control, GIMP_MOTION_MODE_COMPRESS);
  gimp_tool_control_set_tool_cursor (tool->control,
                                     GIMP_BEZIER_SELECT_TOOL_CURSOR);

  vector_tool->function       = VECTORS_CREATE_VECTOR;
  vector_tool->restriction    = GIMP_ANCHOR_FEATURE_NONE;
  vector_tool->modifier_lock  = FALSE;
  vector_tool->last_x         = 0.0;
  vector_tool->last_y         = 0.0;

  vector_tool->cur_anchor     = NULL;
  vector_tool->cur_stroke     = NULL;
  vector_tool->cur_position   = 0.0;
  vector_tool->vectors        = NULL;

  vector_tool->sel_count      = 0;
  vector_tool->sel_anchor     = NULL;
  vector_tool->sel_stroke     = NULL;
}


static void
gimp_vector_tool_control (GimpTool       *tool,
                          GimpToolAction  action,
                          GimpDisplay    *gdisp)
{
  GimpVectorTool *vector_tool = GIMP_VECTOR_TOOL (tool);

  switch (action)
    {
    case PAUSE:
      break;

    case RESUME:
      break;

    case HALT:
      /* gimp_tool_pop_status (tool); */
      gimp_vector_tool_set_vectors (vector_tool, NULL);
      break;

    default:
      break;
    }

  GIMP_TOOL_CLASS (parent_class)->control (tool, action, gdisp);
}

static void
gimp_vector_tool_button_press (GimpTool        *tool,
                               GimpCoords      *coords,
                               guint32          time,
                               GdkModifierType  state,
                               GimpDisplay     *gdisp)
{
  GimpDrawTool      *draw_tool;
  GimpVectorTool    *vector_tool;
  GimpVectorOptions *options;
  GimpVectors       *vectors;

  draw_tool   = GIMP_DRAW_TOOL (tool);
  vector_tool = GIMP_VECTOR_TOOL (tool);
  options     = GIMP_VECTOR_OPTIONS (tool->tool_info->tool_options);

  g_return_if_fail (vector_tool->vectors  != NULL                  ||
                    vector_tool->function == VECTORS_SELECT_VECTOR ||
                    vector_tool->function == VECTORS_CREATE_VECTOR);

  /* Save the current modifier state */

  vector_tool->saved_state = state;

  /*  if we are changing displays, pop the statusbar of the old one  */
  if (gdisp != tool->gdisp)
    {
      /* gimp_tool_pop_status (tool); */
    }

  gimp_draw_tool_pause (draw_tool);

#if 0
  if (vector_tool->function == VECTORS_SELECT_VECTOR)
    {
      gimp_vector_tool_set_vectors (vector_tool, NULL);
    }
#endif

  if (gimp_draw_tool_is_active (draw_tool) && draw_tool->gdisp != gdisp)
    {
      g_print ("calling  draw_tool_stop\n");
      gimp_draw_tool_stop (draw_tool);
    }

  if (vector_tool->vectors)
    gimp_image_undo_push_vectors_mod (GIMP_ITEM (vector_tool->vectors)->gimage,
                                      "Vectors operation",
                                      vector_tool->vectors);

  gimp_tool_control_activate (tool->control);
  tool->gdisp = gdisp;

  /* select a vectors object */

  if (vector_tool->function == VECTORS_SELECT_VECTOR)
    {
      if (gimp_draw_tool_on_vectors (draw_tool, gdisp, coords, TARGET, TARGET,
                                     NULL, NULL, NULL, NULL, &vectors))
        {
          gimp_vector_tool_set_vectors (vector_tool, vectors); 
          gimp_image_set_active_vectors (gdisp->gimage, vectors);
        }
      vector_tool->function = VECTORS_FINISHED;
    }

  /* create a new vector from scratch */

  if (vector_tool->function == VECTORS_CREATE_VECTOR)
    {
      vectors = gimp_vectors_new (gdisp->gimage, _("Unnamed"));

      gimp_image_add_vectors (gdisp->gimage, vectors, -1);
      gimp_image_flush (gdisp->gimage);

      gimp_vector_tool_set_vectors (vector_tool, vectors);

      vector_tool->function = VECTORS_CREATE_STROKE;
    }

  gimp_vectors_freeze (vector_tool->vectors);

  /* create a new stroke */

  if (vector_tool->function == VECTORS_CREATE_STROKE)
    {
      g_return_if_fail (vector_tool->vectors != NULL);

      vector_tool->cur_stroke = gimp_bezier_stroke_new ();
      gimp_vectors_stroke_add (vector_tool->vectors, vector_tool->cur_stroke);

      vector_tool->sel_stroke = vector_tool->cur_stroke;
      vector_tool->cur_anchor = NULL;
      vector_tool->sel_anchor = NULL;
      vector_tool->function = VECTORS_ADD_ANCHOR;
    }


  /* add an anchor to an existing stroke */

  if (vector_tool->function == VECTORS_ADD_ANCHOR)
    {
      vector_tool->cur_anchor =
                     gimp_bezier_stroke_extend (vector_tool->sel_stroke, coords,
                                                vector_tool->sel_anchor,
                                                EXTEND_EDITABLE);

      vector_tool->restriction = GIMP_ANCHOR_FEATURE_SYMMETRIC;

      if (!options->polygonal)
        vector_tool->function = VECTORS_MOVE_HANDLE;
      else
        vector_tool->function = VECTORS_MOVE_ANCHOR;
      vector_tool->cur_stroke = vector_tool->sel_stroke;
    }


  /* Insertion of an anchor in a curve segment */

  if (vector_tool->function == VECTORS_INSERT_ANCHOR)
    {
      vector_tool->cur_anchor =
                         gimp_stroke_anchor_insert (vector_tool->cur_stroke,
                                                    vector_tool->cur_anchor,
                                                    vector_tool->cur_position);
      if (vector_tool->cur_anchor)
        {
          if (options->polygonal)
            {
              gimp_stroke_anchor_convert (vector_tool->cur_stroke,
                                          vector_tool->cur_anchor,
                                          GIMP_ANCHOR_FEATURE_EDGE);
            }
          vector_tool->function = VECTORS_MOVE_ANCHOR;
        }
      else
        {
          vector_tool->function = VECTORS_FINISHED;
        }
    }


  /* move a handle */

  if (vector_tool->function == VECTORS_MOVE_HANDLE)
    {
      if (vector_tool->cur_anchor->type == GIMP_ANCHOR_ANCHOR)
        {
          gimp_vectors_anchor_select (vector_tool->vectors,
                                      vector_tool->cur_stroke,
                                      vector_tool->cur_anchor, TRUE);
          gimp_draw_tool_on_vectors_handle (GIMP_DRAW_TOOL (tool), gdisp,
                                            vector_tool->vectors, coords,
                                            TARGET, TARGET,
                                            GIMP_ANCHOR_CONTROL,
                                            &vector_tool->cur_anchor,
                                            &vector_tool->cur_stroke);
        }
    }


  /* move an anchor */

  if (vector_tool->function == VECTORS_MOVE_ANCHOR)
    {
      gimp_vectors_anchor_select (vector_tool->vectors,
                                  vector_tool->cur_stroke,
                                  vector_tool->cur_anchor, TRUE);
    }


  /* connect two strokes */

  if (vector_tool->function == VECTORS_CONNECT_STROKES)
    {
      gimp_stroke_connect_stroke (vector_tool->sel_stroke,
                                  vector_tool->sel_anchor,
                                  vector_tool->cur_stroke,
                                  vector_tool->cur_anchor);
      if (vector_tool->cur_stroke != vector_tool->sel_stroke &&
          gimp_stroke_is_empty (vector_tool->cur_stroke))
        {
          gimp_vectors_stroke_remove (vector_tool->vectors,
                                      vector_tool->cur_stroke);
        }
      vector_tool->sel_anchor = vector_tool->cur_anchor;
      vector_tool->cur_stroke = vector_tool->sel_stroke;

      gimp_vectors_anchor_select (vector_tool->vectors,
                                  vector_tool->sel_stroke,
                                  vector_tool->sel_anchor, TRUE);

      vector_tool->function = VECTORS_FINISHED;
    }


  /* move a curve segment directly */

  if (vector_tool->function == VECTORS_MOVE_CURVE)
    {
      gimp_vectors_anchor_select (vector_tool->vectors,
                                  vector_tool->cur_stroke,
                                  vector_tool->cur_anchor, TRUE);
    }


  /* move a stroke or all strokes of a vectors object */

  if (vector_tool->function == VECTORS_MOVE_STROKE ||
      vector_tool->function == VECTORS_MOVE_VECTORS)
    {
      /* Work is being done in gimp_vector_tool_motion ()... */
      vector_tool->last_x = coords->x;
      vector_tool->last_y = coords->y;
    }


  /* convert an anchor to something that looks like an edge */

  if (vector_tool->function == VECTORS_CONVERT_EDGE)
    {
      gimp_stroke_anchor_convert (vector_tool->cur_stroke,
                                  vector_tool->cur_anchor,
                                  GIMP_ANCHOR_FEATURE_EDGE);

      if (vector_tool->cur_anchor->type == GIMP_ANCHOR_ANCHOR)
        {
          gimp_vectors_anchor_select (vector_tool->vectors,
                                      vector_tool->cur_stroke,
                                      vector_tool->cur_anchor, TRUE);

          vector_tool->function = VECTORS_MOVE_ANCHOR;
        }
      else
        {
          vector_tool->cur_stroke = NULL;
          vector_tool->cur_anchor = NULL;

          /* avoid doing anything stupid */
          vector_tool->function = VECTORS_FINISHED;
        }
    }


  /* removal of a node in a stroke */

  if (vector_tool->function == VECTORS_DELETE_ANCHOR)
    {
      gimp_stroke_anchor_delete (vector_tool->cur_stroke,
                                 vector_tool->cur_anchor);

      if (gimp_stroke_is_empty (vector_tool->cur_stroke))
        gimp_vectors_stroke_remove (vector_tool->vectors,
                                    vector_tool->cur_stroke);

      vector_tool->cur_stroke = NULL;
      vector_tool->cur_anchor = NULL;
      vector_tool->function = VECTORS_FINISHED;
    }


  /* deleting a segment (opening up a stroke) */

  if (vector_tool->function == VECTORS_DELETE_SEGMENT)
    {
      GimpStroke *new_stroke;
      new_stroke = gimp_stroke_open (vector_tool->cur_stroke,
                                     vector_tool->cur_anchor);
      if (new_stroke)
        gimp_vectors_stroke_add (vector_tool->vectors, new_stroke);

      vector_tool->cur_stroke = NULL;
      vector_tool->cur_anchor = NULL;
      vector_tool->function = VECTORS_FINISHED;
    }

  gimp_vectors_thaw (vector_tool->vectors);

  if (! gimp_draw_tool_is_active (draw_tool))
    gimp_draw_tool_start (draw_tool, gdisp);

  gimp_draw_tool_resume (draw_tool);
}

static void
gimp_vector_tool_button_release (GimpTool        *tool,
                                 GimpCoords      *coords,
                                 guint32          time,
                                 GdkModifierType  state,
                                 GimpDisplay     *gdisp)
{
  GimpVectorTool *vector_tool = GIMP_VECTOR_TOOL (tool);

  vector_tool->function = VECTORS_FINISHED;

  gimp_tool_control_halt (tool->control);
}

static void
gimp_vector_tool_motion (GimpTool        *tool,
                         GimpCoords      *coords,
                         guint32          time,
                         GdkModifierType  state,
                         GimpDisplay     *gdisp)
{
  GimpVectorTool    *vector_tool;
  GimpVectorOptions *options;
  GimpAnchor        *anchor;

  vector_tool = GIMP_VECTOR_TOOL (tool);
  options     = GIMP_VECTOR_OPTIONS (tool->tool_info->tool_options);

  if (vector_tool->function == VECTORS_FINISHED)
    return;

  gimp_vectors_freeze (vector_tool->vectors);

  if ((vector_tool->saved_state & TOGGLE_MASK) != (state & TOGGLE_MASK))
    vector_tool->modifier_lock = FALSE;

  if (!vector_tool->modifier_lock)
    {
      if (state & TOGGLE_MASK)
        {
          vector_tool->restriction = GIMP_ANCHOR_FEATURE_SYMMETRIC;
        }
      else
        {
          vector_tool->restriction = GIMP_ANCHOR_FEATURE_NONE;
        }
    }

  switch (vector_tool->function)
    {
    case VECTORS_MOVE_ANCHOR:
    case VECTORS_MOVE_HANDLE:
      anchor = vector_tool->cur_anchor;

      if (anchor)
        gimp_stroke_anchor_move_absolute (vector_tool->cur_stroke,
                                          vector_tool->cur_anchor,
                                          coords, vector_tool->restriction);
      break;

    case VECTORS_MOVE_CURVE:
      gimp_stroke_point_move_absolute (vector_tool->cur_stroke,
                                       vector_tool->cur_anchor,
                                       vector_tool->cur_position,
                                       coords, vector_tool->restriction);
      break;

    case VECTORS_MOVE_STROKE:
      if (vector_tool->cur_stroke)
        {
          gimp_stroke_translate (vector_tool->cur_stroke,
                                 coords->x - vector_tool->last_x,
                                 coords->y - vector_tool->last_y);
        }
      else if (vector_tool->sel_stroke)
        {
          gimp_stroke_translate (vector_tool->sel_stroke,
                                 coords->x - vector_tool->last_x,
                                 coords->y - vector_tool->last_y);
        }
      vector_tool->last_x = coords->x;
      vector_tool->last_y = coords->y;

      break;

    case VECTORS_MOVE_VECTORS:
      gimp_item_translate (GIMP_ITEM (vector_tool->vectors),
                           coords->x - vector_tool->last_x,
                           coords->y - vector_tool->last_y, FALSE);
      vector_tool->last_x = coords->x;
      vector_tool->last_y = coords->y;

      break;

    default:
      break;
    }

  gimp_vectors_thaw (vector_tool->vectors);
}

static void
gimp_vector_tool_modifier_key (GimpTool        *tool,
                               GdkModifierType  key,
                               gboolean         press,
                               GdkModifierType  state,
                               GimpDisplay     *gdisp)
{
  GimpVectorOptions *options;

  options = GIMP_VECTOR_OPTIONS (tool->tool_info->tool_options);

  if (key == TOGGLE_MASK)
    return;

  if (state & MOVE_MASK)
    {
      g_object_set (options,
                    "vectors-edit-mode", GIMP_VECTOR_MODE_MOVE,
                    NULL);
    }
  else if (state & INSDEL_MASK)
    {
      g_object_set (options,
                    "vectors-edit-mode", GIMP_VECTOR_MODE_ADJUST,
                    NULL);
    }
  else
    {
      g_object_set (options,
                    "vectors-edit-mode", GIMP_VECTOR_MODE_CREATE,
                    NULL);
    }
}

static void
gimp_vector_tool_oper_update (GimpTool        *tool,
                              GimpCoords      *coords,
                              GdkModifierType  state,
                              GimpDisplay     *gdisp)
{
  GimpVectorTool    *vector_tool;
  GimpDrawTool      *draw_tool;
  GimpVectorOptions *options;
  GimpAnchor        *anchor         = NULL;
  GimpStroke        *stroke         = NULL;
  gdouble            position       = -1;
  gboolean           on_handle      = FALSE;
  gboolean           on_curve       = FALSE;

  vector_tool = GIMP_VECTOR_TOOL (tool);
  options     = GIMP_VECTOR_OPTIONS (tool->tool_info->tool_options);

  draw_tool = GIMP_DRAW_TOOL (tool);

  vector_tool->modifier_lock = FALSE;

  /* are we hovering the current vectors on the current display? */
  if (vector_tool->vectors && GIMP_DRAW_TOOL (tool)->gdisp == gdisp)
    {
      on_handle = gimp_draw_tool_on_vectors_handle (GIMP_DRAW_TOOL (tool),
                                                    gdisp,
                                                    vector_tool->vectors,
                                                    coords,
                                                    TARGET, TARGET,
                                                    GIMP_ANCHOR_ANCHOR,
                                                    &anchor, &stroke);

      if (! on_handle)
        on_curve = gimp_draw_tool_on_vectors_curve (GIMP_DRAW_TOOL (tool),
                                                    gdisp,
                                                    vector_tool->vectors,
                                                    coords,
                                                    TARGET, TARGET,
                                                    NULL,
                                                    &position, &anchor, &stroke);
    }

  vector_tool->cur_position   = position;
  vector_tool->cur_anchor     = anchor;
  vector_tool->cur_stroke     = stroke;

  switch (options->edit_mode)
    {
    case GIMP_VECTOR_MODE_CREATE:
      if (! vector_tool->vectors || GIMP_DRAW_TOOL (tool)->gdisp != gdisp)
        {
          if (GIMP_CONTAINER (gdisp->gimage->vectors)->num_children > 0)
            {
              vector_tool->function = VECTORS_SELECT_VECTOR;
            }
          else
            {
              vector_tool->function = VECTORS_CREATE_VECTOR;
              vector_tool->restriction = GIMP_ANCHOR_FEATURE_SYMMETRIC;
              vector_tool->modifier_lock = TRUE;
            }
        }
      else if (on_handle)
        {
          if (anchor->type == GIMP_ANCHOR_ANCHOR)
            {
              vector_tool->function = VECTORS_MOVE_ANCHOR;
            }
          else
            {
              vector_tool->function = VECTORS_MOVE_HANDLE;
              if (state & TOGGLE_MASK)
                vector_tool->restriction = GIMP_ANCHOR_FEATURE_SYMMETRIC;
              else
                vector_tool->restriction = GIMP_ANCHOR_FEATURE_NONE;
            }
        }
      else if (on_curve)
        {
          if (gimp_stroke_point_is_movable (stroke, anchor, position))
            {
              vector_tool->function = VECTORS_MOVE_CURVE;
              if (state & TOGGLE_MASK)
                vector_tool->restriction = GIMP_ANCHOR_FEATURE_SYMMETRIC;
              else
                vector_tool->restriction = GIMP_ANCHOR_FEATURE_NONE;
            }
          else
            {
              vector_tool->function = VECTORS_FINISHED;
            }
        }
      else
        {
          if (vector_tool->sel_stroke && vector_tool->sel_anchor &&
              gimp_stroke_is_extendable (vector_tool->sel_stroke,
                                         vector_tool->sel_anchor) &&
              !(state & TOGGLE_MASK))
            vector_tool->function = VECTORS_ADD_ANCHOR;
          else
            vector_tool->function = VECTORS_CREATE_STROKE;

          vector_tool->restriction = GIMP_ANCHOR_FEATURE_SYMMETRIC;
          vector_tool->modifier_lock = TRUE;
        }

      break;

    case GIMP_VECTOR_MODE_ADJUST:
      if (! vector_tool->vectors || GIMP_DRAW_TOOL (tool)->gdisp != gdisp)
        {
          if (GIMP_CONTAINER (gdisp->gimage->vectors)->num_children > 0)
            {
              vector_tool->function = VECTORS_SELECT_VECTOR;
            }
          else
            {
              vector_tool->function = VECTORS_FINISHED;
            }
        }
      else if (on_handle)
        {
          if (anchor->type == GIMP_ANCHOR_ANCHOR)
            {
              if (!(state & TOGGLE_MASK) && vector_tool->sel_anchor &&
                  vector_tool->sel_anchor != anchor &&
                  gimp_stroke_is_extendable (vector_tool->sel_stroke,
                                             vector_tool->sel_anchor) &&
                  gimp_stroke_is_extendable (stroke, anchor))
                {
                  vector_tool->function = VECTORS_CONNECT_STROKES;
                }
              else
                {
                  vector_tool->function = VECTORS_DELETE_ANCHOR;
                }
            }
          else
            {
              vector_tool->function = VECTORS_CONVERT_EDGE;
              if (state & TOGGLE_MASK)
                vector_tool->restriction = GIMP_ANCHOR_FEATURE_SYMMETRIC;
              else
                vector_tool->restriction = GIMP_ANCHOR_FEATURE_NONE;
            }
        }
      else if (on_curve)
        {
          if (state & TOGGLE_MASK)
            {
              vector_tool->function = VECTORS_DELETE_SEGMENT;
            }
          else if (gimp_stroke_anchor_is_insertable (stroke, anchor, position))
            {
              vector_tool->function = VECTORS_INSERT_ANCHOR;
            }
          else
            {
              vector_tool->function = VECTORS_FINISHED;
            }
        }
      else
        {
          vector_tool->function = VECTORS_FINISHED;
        }

      break;

    case GIMP_VECTOR_MODE_MOVE:
      if (! vector_tool->vectors || GIMP_DRAW_TOOL (tool)->gdisp != gdisp)
        {
          if (GIMP_CONTAINER (gdisp->gimage->vectors)->num_children > 0)
            {
              vector_tool->function = VECTORS_SELECT_VECTOR;
            }
          else
            {
              vector_tool->function = VECTORS_FINISHED;
            }
        }
      else if (on_handle || on_curve)
        {
          if (state & TOGGLE_MASK)
            {
              vector_tool->function = VECTORS_MOVE_VECTORS;
            }
          else
            {
              vector_tool->function = VECTORS_MOVE_STROKE;
            }
        }
      else
        {
          if (GIMP_CONTAINER (gdisp->gimage->vectors)->num_children > 0)
            {
              vector_tool->function = VECTORS_SELECT_VECTOR;
            }
          else
            {
              vector_tool->function = VECTORS_FINISHED;
            }
        }
      break;
    }
}


static void
gimp_vector_tool_cursor_update (GimpTool        *tool,
                                GimpCoords      *coords,
                                GdkModifierType  state,
                                GimpDisplay     *gdisp)
{
  GimpVectorTool     *vector_tool;
  GimpCursorType      cursor;
  GimpToolCursorType  tool_cursor;
  GimpCursorModifier  cmodifier;

  vector_tool = GIMP_VECTOR_TOOL (tool);

  cursor      = GIMP_MOUSE_CURSOR;
  tool_cursor = GIMP_BEZIER_SELECT_TOOL_CURSOR;
  cmodifier   = GIMP_CURSOR_MODIFIER_NONE;

  switch (vector_tool->function)
    {
    case VECTORS_SELECT_VECTOR:
      tool_cursor = GIMP_TOOL_CURSOR_NONE;
      cmodifier   = GIMP_CURSOR_MODIFIER_HAND;
      break;
    case VECTORS_CREATE_VECTOR:
    case VECTORS_CREATE_STROKE:
      cmodifier = GIMP_CURSOR_MODIFIER_CONTROL;
      break;
    case VECTORS_ADD_ANCHOR:
    case VECTORS_INSERT_ANCHOR:
      cmodifier = GIMP_CURSOR_MODIFIER_PLUS;
      break;
    case VECTORS_DELETE_ANCHOR:
    case VECTORS_DELETE_SEGMENT:
      cmodifier = GIMP_CURSOR_MODIFIER_MINUS;
      break;
    case VECTORS_MOVE_HANDLE:
    case VECTORS_CONVERT_EDGE:
      tool_cursor = GIMP_TOOL_CURSOR_NONE;
      cmodifier   = GIMP_CURSOR_MODIFIER_HAND;
      break;
    case VECTORS_MOVE_ANCHOR:
    case VECTORS_MOVE_CURVE:
    case VECTORS_MOVE_STROKE:
    case VECTORS_MOVE_VECTORS:
      cmodifier = GIMP_CURSOR_MODIFIER_MOVE;
      break;
    case VECTORS_CONNECT_STROKES:
      cmodifier = GIMP_CURSOR_MODIFIER_INTERSECT;
      break;
    default:
      cursor = GIMP_BAD_CURSOR;
      break;
    }

  gimp_tool_control_set_cursor (tool->control, cursor);
  gimp_tool_control_set_tool_cursor (tool->control, tool_cursor);
  gimp_tool_control_set_cursor_modifier (tool->control, cmodifier);

  GIMP_TOOL_CLASS (parent_class)->cursor_update (tool, coords, state, gdisp);
}

static void
gimp_vector_tool_draw (GimpDrawTool *draw_tool)
{
  GimpVectorTool  *vector_tool;
  GimpTool        *tool;
  GimpAnchor      *cur_anchor = NULL;
  GimpStroke      *cur_stroke = NULL;
  GimpVectors     *vectors;
  GArray          *coords;
  gboolean         closed;
  GList           *draw_anchors;
  GList           *list;

  vector_tool = GIMP_VECTOR_TOOL (draw_tool);
  tool        = GIMP_TOOL (draw_tool);

  vectors = vector_tool->vectors;

  if (!vectors)
    return;

  while ((cur_stroke = gimp_vectors_stroke_get_next (vectors, cur_stroke)))
    {
      /* anchor handles */
      draw_anchors = gimp_stroke_get_draw_anchors (cur_stroke);

      for (list = draw_anchors; list; list = g_list_next (list))
        {
          cur_anchor = GIMP_ANCHOR (list->data);

          if (cur_anchor->type == GIMP_ANCHOR_ANCHOR)
            {
              gimp_draw_tool_draw_handle (draw_tool,
                                          cur_anchor->selected ?
                                          GIMP_HANDLE_CIRCLE :
                                          GIMP_HANDLE_FILLED_CIRCLE,
                                          cur_anchor->position.x,
                                          cur_anchor->position.y,
                                          TARGET,
                                          TARGET,
                                          GTK_ANCHOR_CENTER,
                                          FALSE);
            }
        }

      g_list_free (draw_anchors);

      /* control handles */
      draw_anchors = gimp_stroke_get_draw_controls (cur_stroke);

      for (list = draw_anchors; list; list = g_list_next (list))
        {
          cur_anchor = GIMP_ANCHOR (list->data);

          gimp_draw_tool_draw_handle (draw_tool,
                                      GIMP_HANDLE_SQUARE,
                                      cur_anchor->position.x,
                                      cur_anchor->position.y,
                                      TARGET - 3,
                                      TARGET - 3,
                                      GTK_ANCHOR_CENTER,
                                      FALSE);
        }

      g_list_free (draw_anchors);

      /* the lines to the control handles */
      coords = gimp_stroke_get_draw_lines (cur_stroke);

      if (coords)
        {
          if (coords->len % 2 == 0)
            {
              gint i;

              for (i = 0; i < coords->len; i += 2)
                {
                  gimp_draw_tool_draw_dashed_line (draw_tool,
                                    g_array_index (coords, GimpCoords, i).x,
                                    g_array_index (coords, GimpCoords, i).y,
                                    g_array_index (coords, GimpCoords, i + 1).x,
                                    g_array_index (coords, GimpCoords, i + 1).y,
                                    FALSE);
                }
            }

          g_array_free (coords, TRUE);
        }

      /* the stroke itself */
      if (! gimp_item_get_visible (GIMP_ITEM (vectors)))
        {
          coords = gimp_stroke_interpolate (cur_stroke, 1.0, &closed);

          if (coords)
            {
              if (coords->len)
                gimp_draw_tool_draw_strokes (draw_tool,
                                             &g_array_index (coords,
                                                             GimpCoords, 0),
                                             coords->len, FALSE, FALSE);

              g_array_free (coords, TRUE);
            }
        }
    }
}

static void
gimp_vector_tool_vectors_changed (GimpImage      *gimage,
                                  GimpVectorTool *vector_tool)
{
  gimp_vector_tool_set_vectors (vector_tool,
                                gimp_image_get_active_vectors (gimage));
}

static void
gimp_vector_tool_vectors_removed (GimpVectors    *vectors,
                                  GimpVectorTool *vector_tool)
{
  gimp_vector_tool_set_vectors (vector_tool, NULL);
}

static void
gimp_vector_tool_vectors_visible (GimpVectors    *vectors,
                                  GimpVectorTool *vector_tool)
{
  GimpDrawTool *draw_tool = GIMP_DRAW_TOOL (vector_tool);

  if (gimp_draw_tool_is_active (draw_tool) && draw_tool->paused_count == 0)
    {
      GimpStroke *stroke = NULL;

      while ((stroke = gimp_vectors_stroke_get_next (vectors, stroke)))
        {
          GArray   *coords;
          gboolean  closed;

          coords = gimp_stroke_interpolate (stroke, 1.0, &closed);

          if (coords)
            {
              if (coords->len)
                gimp_draw_tool_draw_strokes (draw_tool,
                                             &g_array_index (coords,
                                                             GimpCoords, 0),
                                             coords->len, FALSE, FALSE);

              g_array_free (coords, TRUE);
            }
        }
    }
}

static void
gimp_vector_tool_vectors_freeze (GimpVectors    *vectors,
                                 GimpVectorTool *vector_tool)
{
  gimp_draw_tool_pause (GIMP_DRAW_TOOL (vector_tool));
}

static void
gimp_vector_tool_vectors_thaw (GimpVectors    *vectors,
                               GimpVectorTool *vector_tool)
{
  /* Ok, the vector might have changed externally (e.g. Undo)
   * we need to validate our internal state. */
  gimp_vector_tool_verify_state (vector_tool);

  gimp_draw_tool_resume (GIMP_DRAW_TOOL (vector_tool));
}

void
gimp_vector_tool_set_vectors (GimpVectorTool *vector_tool,
                              GimpVectors    *vectors)
{
  GimpDrawTool *draw_tool;
  GimpTool     *tool;
  GimpItem     *item = NULL;

  g_return_if_fail (GIMP_IS_VECTOR_TOOL (vector_tool));
  g_return_if_fail (vectors == NULL || GIMP_IS_VECTORS (vectors));

  draw_tool = GIMP_DRAW_TOOL (vector_tool);
  tool      = GIMP_TOOL (vector_tool);

  if (vectors)
    item = GIMP_ITEM (vectors);

  if (vectors == vector_tool->vectors)
    return;

  gimp_draw_tool_pause (draw_tool);

  if (gimp_draw_tool_is_active (draw_tool) &&
      (! vectors || draw_tool->gdisp->gimage != item->gimage))
    gimp_draw_tool_stop (draw_tool);

  if (vector_tool->vectors)
    {
      GimpImage *old_gimage;

      old_gimage = gimp_item_get_image (GIMP_ITEM (vector_tool->vectors));

      g_signal_handlers_disconnect_by_func (old_gimage,
                                            gimp_vector_tool_vectors_changed,
                                            vector_tool);
      g_signal_handlers_disconnect_by_func (vector_tool->vectors,
                                            gimp_vector_tool_vectors_removed,
                                            vector_tool);
      g_signal_handlers_disconnect_by_func (vector_tool->vectors,
                                            gimp_vector_tool_vectors_visible,
                                            vector_tool);
      g_signal_handlers_disconnect_by_func (vector_tool->vectors,
                                            gimp_vector_tool_vectors_freeze,
                                            vector_tool);
      g_signal_handlers_disconnect_by_func (vector_tool->vectors,
                                            gimp_vector_tool_vectors_thaw,
                                            vector_tool);
      g_object_unref (vector_tool->vectors);
    }

  vector_tool->vectors    = vectors;
  vector_tool->function   = VECTORS_FINISHED;
  gimp_vector_tool_verify_state (vector_tool);

  if (! vector_tool->vectors)
    {
      tool->gdisp = NULL;

      /* leave draw_tool->paused_count in a consistent state */
      gimp_draw_tool_resume (draw_tool);

      vector_tool->function = VECTORS_CREATE_VECTOR;

      return;
    }

  g_object_ref (vectors);

  g_signal_connect_object (item->gimage, "active_vectors_changed",
                           G_CALLBACK (gimp_vector_tool_vectors_changed),
                           vector_tool, 0);
  g_signal_connect_object (vectors, "removed",
                           G_CALLBACK (gimp_vector_tool_vectors_removed),
                           vector_tool, 0);
  g_signal_connect_object (vectors, "visibility_changed",
                           G_CALLBACK (gimp_vector_tool_vectors_visible),
                           vector_tool, 0);
  g_signal_connect_object (vectors, "freeze",
                           G_CALLBACK (gimp_vector_tool_vectors_freeze),
                           vector_tool, 0);
  g_signal_connect_object (vectors, "thaw",
                           G_CALLBACK (gimp_vector_tool_vectors_thaw),
                           vector_tool, 0);

  if (! gimp_draw_tool_is_active (draw_tool))
    {
      if (tool->gdisp && tool->gdisp->gimage == item->gimage)
        {
          gimp_draw_tool_start (draw_tool, tool->gdisp);
        }
      else
        {
          GimpContext *context;
          GimpDisplay *gdisp;

          context = gimp_get_user_context (tool->tool_info->gimp);
          gdisp   = gimp_context_get_display (context);

          if (! gdisp || gdisp->gimage != item->gimage)
            {
              GList *list;

              gdisp = NULL;

              for (list = GIMP_LIST (item->gimage->gimp->displays)->list;
                   list;
                   list = g_list_next (list))
                {
                  if (((GimpDisplay *) list->data)->gimage == item->gimage)
                    {
                      gimp_context_set_display (context,
                                                (GimpDisplay *) list->data);

                      gdisp = gimp_context_get_display (context);
                      break;
                    }
                }
            }

          tool->gdisp = gdisp;

          if (tool->gdisp)
            gimp_draw_tool_start (draw_tool, tool->gdisp);
        }
    }

  gimp_draw_tool_resume (draw_tool);
}

static void
gimp_vector_tool_verify_state (GimpVectorTool *vector_tool)
{
  GimpStroke *cur_stroke = NULL;
  GimpAnchor *cur_anchor;
  GList      *draw_anchors;
  GList      *list;
  gboolean    cur_anchor_valid;
  gboolean    cur_stroke_valid;

  cur_anchor_valid = FALSE;
  cur_stroke_valid = FALSE;

  vector_tool->sel_count  = 0;
  vector_tool->sel_anchor = NULL;
  vector_tool->sel_stroke = NULL;

  if (!vector_tool->vectors)
    {
      vector_tool->cur_position = -1;
      vector_tool->cur_anchor   = NULL;
      vector_tool->cur_stroke   = NULL;
      return;
    }

  while ((cur_stroke = gimp_vectors_stroke_get_next (vector_tool->vectors,
                                                     cur_stroke)))
    {
      /* anchor handles */
      draw_anchors = gimp_stroke_get_draw_anchors (cur_stroke);

      if (cur_stroke == vector_tool->cur_stroke)
        cur_stroke_valid = TRUE;

      for (list = draw_anchors; list; list = g_list_next (list))
        {
          cur_anchor = GIMP_ANCHOR (list->data);

          if (cur_anchor == vector_tool->cur_anchor)
            cur_anchor_valid = TRUE;

          if (cur_anchor->type == GIMP_ANCHOR_ANCHOR &&
              cur_anchor->selected)
            {
              vector_tool->sel_count++;
              if (vector_tool->sel_count == 1)
                {
                  vector_tool->sel_anchor = cur_anchor;
                  vector_tool->sel_stroke = cur_stroke;
                }
              else
                {
                  vector_tool->sel_anchor = NULL;
                  vector_tool->sel_stroke = NULL;
                }
            }
        }

      draw_anchors = gimp_stroke_get_draw_controls (cur_stroke);

      for (list = draw_anchors; list; list = g_list_next (list))
        {
          cur_anchor = GIMP_ANCHOR (list->data);

          if (cur_anchor == vector_tool->cur_anchor)
            cur_anchor_valid = TRUE;
        }
    }

  if (!cur_stroke_valid)
    vector_tool->cur_stroke = NULL;

  if (!cur_anchor_valid)
    vector_tool->cur_anchor = NULL;

}

