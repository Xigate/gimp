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

#include <string.h>

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "actions-types.h"

#include "core/gimp.h"
#include "core/gimpbrushgenerated.h"
#include "core/gimpcontext.h"
#include "core/gimpdatafactory.h"
#include "core/gimplist.h"

#include "actions.h"
#include "context-commands.h"


/*  local function prototypes  */

static void   context_select_object (GimpActionSelectType  select_type,
                                     GimpContext          *context,
                                     GimpContainer        *container);


/*  public functions  */

void
context_colors_default_cmd_callback (GtkAction *action,
                                     gpointer   data)
{
  GimpContext *context;
  return_if_no_context (context, data);

  gimp_context_set_default_colors (context);
}

void
context_colors_swap_cmd_callback (GtkAction *action,
                                  gpointer   data)
{
  GimpContext *context;
  return_if_no_context (context, data);

  gimp_context_swap_colors (context);
}

void
context_foreground_red_cmd_callback (GtkAction *action,
                                     gint       value,
                                     gpointer   data)
{
  GimpContext *context;
  GimpRGB      color;
  return_if_no_context (context, data);

  gimp_context_get_foreground (context, &color);
  color.r = action_select_value ((GimpActionSelectType) value,
                                 color.r,
                                 0.0, 1.0,
                                 0.01, 0.1, FALSE);
  gimp_context_set_foreground (context, &color);
}

void
context_foreground_green_cmd_callback (GtkAction *action,
                                       gint       value,
                                       gpointer   data)
{
  GimpContext *context;
  GimpRGB      color;
  return_if_no_context (context, data);

  gimp_context_get_foreground (context, &color);
  color.g = action_select_value ((GimpActionSelectType) value,
                                 color.g,
                                 0.0, 1.0,
                                 0.01, 0.1, FALSE);
  gimp_context_set_foreground (context, &color);
}

void
context_foreground_blue_cmd_callback (GtkAction *action,
                                      gint       value,
                                      gpointer   data)
{
  GimpContext *context;
  GimpRGB      color;
  return_if_no_context (context, data);

  gimp_context_get_foreground (context, &color);
  color.b = action_select_value ((GimpActionSelectType) value,
                                 color.b,
                                 0.0, 1.0,
                                 0.01, 0.1, FALSE);
  gimp_context_set_foreground (context, &color);
}

void
context_background_red_cmd_callback (GtkAction *action,
                                     gint       value,
                                     gpointer   data)
{
  GimpContext *context;
  GimpRGB      color;
  return_if_no_context (context, data);

  gimp_context_get_background (context, &color);
  color.r = action_select_value ((GimpActionSelectType) value,
                                 color.r,
                                 0.0, 1.0,
                                 0.01, 0.1, FALSE);
  gimp_context_set_background (context, &color);
}

void
context_background_green_cmd_callback (GtkAction *action,
                                       gint       value,
                                       gpointer   data)
{
  GimpContext *context;
  GimpRGB      color;
  return_if_no_context (context, data);

  gimp_context_get_background (context, &color);
  color.g = action_select_value ((GimpActionSelectType) value,
                                 color.g,
                                 0.0, 1.0,
                                 0.01, 0.1, FALSE);
  gimp_context_set_background (context, &color);
}

void
context_background_blue_cmd_callback (GtkAction *action,
                                      gint       value,
                                      gpointer   data)
{
  GimpContext *context;
  GimpRGB      color;
  return_if_no_context (context, data);

  gimp_context_get_background (context, &color);
  color.b = action_select_value ((GimpActionSelectType) value,
                                 color.b,
                                 0.0, 1.0,
                                 0.01, 0.1, FALSE);
  gimp_context_set_background (context, &color);
}

void
context_opacity_cmd_callback (GtkAction *action,
                              gint       value,
                              gpointer   data)
{
  GimpContext *context;
  gdouble      opacity;
  return_if_no_context (context, data);

  opacity = action_select_value ((GimpActionSelectType) value,
                                 gimp_context_get_opacity (context),
                                 GIMP_OPACITY_TRANSPARENT,
                                 GIMP_OPACITY_OPAQUE,
                                 0.01, 0.1, FALSE);
  gimp_context_set_opacity (context, opacity);
}

void
context_tool_select_cmd_callback (GtkAction *action,
                                  gint       value,
                                  gpointer   data)
{
  GimpContext *context;
  return_if_no_context (context, data);

  context_select_object ((GimpActionSelectType) value,
                         context, context->gimp->tool_info_list);
}

void
context_brush_select_cmd_callback (GtkAction *action,
                                   gint       value,
                                   gpointer   data)
{
  GimpContext *context;
  return_if_no_context (context, data);

  context_select_object ((GimpActionSelectType) value,
                         context, context->gimp->brush_factory->container);
}

void
context_pattern_select_cmd_callback (GtkAction *action,
                                     gint       value,
                                     gpointer   data)
{
  GimpContext *context;
  return_if_no_context (context, data);

  context_select_object ((GimpActionSelectType) value,
                         context, context->gimp->pattern_factory->container);
}

void
context_palette_select_cmd_callback (GtkAction *action,
                                     gint       value,
                                     gpointer   data)
{
  GimpContext *context;
  return_if_no_context (context, data);

  context_select_object ((GimpActionSelectType) value,
                         context, context->gimp->palette_factory->container);
}

void
context_gradient_select_cmd_callback (GtkAction *action,
                                      gint       value,
                                      gpointer   data)
{
  GimpContext *context;
  return_if_no_context (context, data);

  context_select_object ((GimpActionSelectType) value,
                         context, context->gimp->gradient_factory->container);
}

void
context_font_select_cmd_callback (GtkAction *action,
                                  gint       value,
                                  gpointer   data)
{
  GimpContext *context;
  return_if_no_context (context, data);

  context_select_object ((GimpActionSelectType) value,
                         context, context->gimp->fonts);
}

void
context_brush_radius_cmd_callback (GtkAction *action,
                                   gint       value,
                                   gpointer   data)
{
  GimpContext *context;
  GimpBrush   *brush;
  return_if_no_context (context, data);

  brush = gimp_context_get_brush (context);

  if (GIMP_IS_BRUSH_GENERATED (brush))
    {
      GimpBrushGenerated *generated = GIMP_BRUSH_GENERATED (brush);
      gdouble             radius;

      radius = gimp_brush_generated_get_radius (generated);

      radius = action_select_value ((GimpActionSelectType) value,
                                    radius,
                                    1.0, 256.0,
                                    1.0, 10.0, FALSE);
      gimp_brush_generated_set_radius (generated, radius);
    }
}

void
context_brush_hardness_cmd_callback (GtkAction *action,
                                     gint       value,
                                     gpointer   data)
{
  GimpContext *context;
  GimpBrush   *brush;
  return_if_no_context (context, data);

  brush = gimp_context_get_brush (context);

  if (GIMP_IS_BRUSH_GENERATED (brush))
    {
      GimpBrushGenerated *generated = GIMP_BRUSH_GENERATED (brush);
      gdouble             hardness;

      hardness = gimp_brush_generated_get_hardness (generated);

      hardness = action_select_value ((GimpActionSelectType) value,
                                      hardness,
                                      0.0, 1.0,
                                      0.01, 0.1, FALSE);
      gimp_brush_generated_set_hardness (generated, hardness);
    }
}

void
context_brush_aspect_cmd_callback (GtkAction *action,
                                   gint       value,
                                   gpointer   data)
{
  GimpContext *context;
  GimpBrush   *brush;
  return_if_no_context (context, data);

  brush = gimp_context_get_brush (context);

  if (GIMP_IS_BRUSH_GENERATED (brush))
    {
      GimpBrushGenerated *generated = GIMP_BRUSH_GENERATED (brush);
      gdouble             aspect;

      aspect = gimp_brush_generated_get_aspect_ratio (generated);

      aspect = action_select_value ((GimpActionSelectType) value,
                                    aspect,
                                    1.0, 20.0,
                                    1.0, 4.0, FALSE);
      gimp_brush_generated_set_aspect_ratio (generated, aspect);
    }
}

void
context_brush_angle_cmd_callback (GtkAction *action,
                                  gint       value,
                                  gpointer   data)
{
  GimpContext *context;
  GimpBrush   *brush;
  return_if_no_context (context, data);

  brush = gimp_context_get_brush (context);

  if (GIMP_IS_BRUSH_GENERATED (brush))
    {
      GimpBrushGenerated *generated = GIMP_BRUSH_GENERATED (brush);
      gdouble             angle;

      angle = gimp_brush_generated_get_angle (generated);

      if (value == GIMP_ACTION_SELECT_FIRST)
        angle = 0.0;
      else if (value == GIMP_ACTION_SELECT_LAST)
        angle = 90.0;
      else
        angle = action_select_value ((GimpActionSelectType) value,
                                     angle,
                                     0.0, 180.0,
                                     1.0, 15.0, TRUE);

      gimp_brush_generated_set_angle (generated, angle);
    }
}


/*  private functions  */

static void
context_select_object (GimpActionSelectType  select_type,
                       GimpContext          *context,
                       GimpContainer        *container)
{
  GimpObject *current;

  current = gimp_context_get_by_type (context, container->children_type);

  current = action_select_object (select_type, container, current);

  if (current)
    gimp_context_set_by_type (context, container->children_type, current);
}
