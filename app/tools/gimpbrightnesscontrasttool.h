/* GIMP - The GNU Image Manipulation Program
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

#ifndef __GIMP_BRIGHTNESS_CONTRAST_TOOL_H__
#define __GIMP_BRIGHTNESS_CONTRAST_TOOL_H__


#include "gimpimagemaptool.h"


#define GIMP_TYPE_BRIGHTNESS_CONTRAST_TOOL            (gimp_brightness_contrast_tool_get_type ())
#define GIMP_BRIGHTNESS_CONTRAST_TOOL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_BRIGHTNESS_CONTRAST_TOOL, GimpBrightnessContrastTool))
#define GIMP_BRIGHTNESS_CONTRAST_TOOL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_BRIGHTNESS_CONTRAST_TOOL, GimpBrightnessContrastToolClass))
#define GIMP_IS_BRIGHTNESS_CONTRAST_TOOL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_BRIGHTNESS_CONTRAST_TOOL))
#define GIMP_IS_BRIGHTNESS_CONTRAST_TOOL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_BRIGHTNESS_CONTRAST_TOOL))
#define GIMP_BRIGHTNESS_CONTRAST_TOOL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_BRIGHTNESS_CONTRAST_TOOL, GimpBrightnessContrastToolClass))


typedef struct _GimpBrightnessContrastTool      GimpBrightnessContrastTool;
typedef struct _GimpBrightnessContrastToolClass GimpBrightnessContrastToolClass;

struct _GimpBrightnessContrastTool
{
  GimpImageMapTool  parent_instance;

  gdouble           x, y;
  gdouble           dx, dy;

  gdouble           brightness;
  gdouble           contrast;
  GimpLut          *lut;

  /*  dialog  */
  GtkAdjustment    *brightness_data;
  GtkAdjustment    *contrast_data;
};

struct _GimpBrightnessContrastToolClass
{
  GimpImageMapToolClass  parent_class;
};


void    gimp_brightness_contrast_tool_register (GimpToolRegisterCallback  callback,
                                                gpointer                  data);

GType   gimp_brightness_contrast_tool_get_type (void) G_GNUC_CONST;


#endif  /*  __GIMP_BRIGHTNESS_CONTRAST_TOOL_H__  */
