/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpcontext.c
 * Copyright (C) 1999-2001 Michael Natterer
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

#include <glib-object.h>

#include "libgimpcolor/gimpcolor.h"

#include "core-types.h"

#include "base/temp-buf.h"

#include "config/gimpconfig.h"
#include "config/gimpconfig-params.h"
#include "config/gimpconfig-serialize.h"
#include "config/gimpconfig-types.h"
#include "config/gimpconfigwriter.h"
#include "config/gimpcoreconfig.h"
#include "config/gimpscanner.h"

#include "gimp.h"
#include "gimpbrush.h"
#include "gimpbuffer.h"
#include "gimpcontainer.h"
#include "gimpcontext.h"
#include "gimpdatafactory.h"
#include "gimpimagefile.h"
#include "gimpgradient.h"
#include "gimpimage.h"
#include "gimpmarshal.h"
#include "gimppalette.h"
#include "gimppattern.h"
#include "gimptemplate.h"
#include "gimptoolinfo.h"

#include "text/gimpfont.h"

#include "gimp-intl.h"


typedef void (* GimpContextCopyPropFunc) (GimpContext *src,
					  GimpContext *dest);


#define context_find_defined(context,prop) \
  while (!(((context)->defined_props) & (1 << (prop))) && (context)->parent) \
    (context) = (context)->parent


/*  local function prototypes  */

static void    gimp_context_class_init        (GimpContextClass    *klass);
static void    gimp_context_init              (GimpContext         *context);
static void    gimp_context_config_iface_init (GimpConfigInterface *config_iface);

static GObject *  gimp_context_constructor    (GType                  type,
                                               guint                  n_params,
                                               GObjectConstructParam *params);
static void       gimp_context_dispose        (GObject               *object);
static void       gimp_context_finalize       (GObject               *object);
static void       gimp_context_set_property   (GObject               *object,
                                               guint                  property_id,
                                               const GValue          *value,
                                               GParamSpec            *pspec);
static void       gimp_context_get_property   (GObject               *object,
                                               guint                  property_id,
                                               GValue                *value,
                                               GParamSpec            *pspec);
static gint64    gimp_context_get_memsize    (GimpObject            *object,
                                              gint64                *gui_size);

static gboolean   gimp_context_serialize            (GimpConfig       *config,
                                                     GimpConfigWriter *writer,
                                                     gpointer          data);
static gboolean   gimp_context_serialize_property   (GimpConfig       *config,
                                                     guint             property_id,
                                                     const GValue     *value,
                                                     GParamSpec       *pspec,
                                                     GimpConfigWriter *writer);
static gboolean   gimp_context_deserialize_property (GimpConfig       *config,
                                                     guint             property_id,
                                                     GValue           *value,
                                                     GParamSpec       *pspec,
                                                     GScanner         *scanner,
                                                     GTokenType       *expected);

/*  image  */
static void gimp_context_image_removed       (GimpContainer    *container,
					      GimpImage        *image,
					      GimpContext      *context);
static void gimp_context_real_set_image      (GimpContext      *context,
					      GimpImage        *image);

/*  display  */
static void gimp_context_display_removed     (GimpContainer    *container,
					      gpointer          display,
					      GimpContext      *context);
static void gimp_context_real_set_display    (GimpContext      *context,
					      gpointer          display);

/*  tool  */
static void gimp_context_tool_dirty          (GimpToolInfo     *tool_info,
					      GimpContext      *context);
static void gimp_context_tool_removed        (GimpContainer    *container,
					      GimpToolInfo     *tool_info,
					      GimpContext      *context);
static void gimp_context_tool_list_thaw      (GimpContainer    *container,
					      GimpContext      *context);
static void gimp_context_real_set_tool       (GimpContext      *context,
					      GimpToolInfo     *tool_info);

/*  foreground  */
static void gimp_context_real_set_foreground (GimpContext      *context,
					      const GimpRGB    *color);

/*  background  */
static void gimp_context_real_set_background (GimpContext      *context,
					      const GimpRGB    *color);

/*  opacity  */
static void gimp_context_real_set_opacity    (GimpContext      *context,
					      gdouble           opacity);

/*  paint mode  */
static void gimp_context_real_set_paint_mode (GimpContext      *context,
					      GimpLayerModeEffects paint_mode);

/*  brush  */
static void gimp_context_brush_dirty         (GimpBrush        *brush,
					      GimpContext      *context);
static void gimp_context_brush_removed       (GimpContainer    *brush_list,
					      GimpBrush        *brush,
					      GimpContext      *context);
static void gimp_context_brush_list_thaw     (GimpContainer    *container,
					      GimpContext      *context);
static void gimp_context_real_set_brush      (GimpContext      *context,
					      GimpBrush        *brush);

/*  pattern  */
static void gimp_context_pattern_dirty       (GimpPattern      *pattern,
					      GimpContext      *context);
static void gimp_context_pattern_removed     (GimpContainer    *container,
					      GimpPattern      *pattern,
					      GimpContext      *context);
static void gimp_context_pattern_list_thaw   (GimpContainer    *container,
					      GimpContext      *context);
static void gimp_context_real_set_pattern    (GimpContext      *context,
					      GimpPattern      *pattern);

/*  gradient  */
static void gimp_context_gradient_dirty      (GimpGradient     *gradient,
					      GimpContext      *context);
static void gimp_context_gradient_removed    (GimpContainer    *container,
					      GimpGradient     *gradient,
					      GimpContext      *context);
static void gimp_context_gradient_list_thaw  (GimpContainer    *container,
					      GimpContext      *context);
static void gimp_context_real_set_gradient   (GimpContext      *context,
					      GimpGradient     *gradient);

/*  palette  */
static void gimp_context_palette_dirty       (GimpPalette      *palette,
					      GimpContext      *context);
static void gimp_context_palette_removed     (GimpContainer    *container,
					      GimpPalette      *palatte,
					      GimpContext      *context);
static void gimp_context_palette_list_thaw   (GimpContainer    *container,
					      GimpContext      *context);
static void gimp_context_real_set_palette    (GimpContext      *context,
					      GimpPalette      *palatte);

/*  font  */
static void gimp_context_font_dirty          (GimpFont         *font,
					      GimpContext      *context);
static void gimp_context_font_removed        (GimpContainer    *container,
					      GimpFont         *font,
					      GimpContext      *context);
static void gimp_context_font_list_thaw      (GimpContainer    *container,
					      GimpContext      *context);
static void gimp_context_real_set_font       (GimpContext      *context,
					      GimpFont         *font);

/*  buffer  */
static void gimp_context_buffer_dirty        (GimpBuffer       *buffer,
					      GimpContext      *context);
static void gimp_context_buffer_removed      (GimpContainer    *container,
					      GimpBuffer       *buffer,
					      GimpContext      *context);
static void gimp_context_buffer_list_thaw    (GimpContainer    *container,
					      GimpContext      *context);
static void gimp_context_real_set_buffer     (GimpContext      *context,
					      GimpBuffer       *buffer);

/*  imagefile  */
static void gimp_context_imagefile_dirty     (GimpImagefile    *imagefile,
					      GimpContext      *context);
static void gimp_context_imagefile_removed   (GimpContainer    *container,
					      GimpImagefile    *imagefile,
					      GimpContext      *context);
static void gimp_context_imagefile_list_thaw (GimpContainer    *container,
					      GimpContext      *context);
static void gimp_context_real_set_imagefile  (GimpContext      *context,
					      GimpImagefile    *imagefile);

/*  template  */
static void gimp_context_template_dirty      (GimpTemplate     *template,
					      GimpContext      *context);
static void gimp_context_template_removed    (GimpContainer    *container,
					      GimpTemplate     *template,
					      GimpContext      *context);
static void gimp_context_template_list_thaw  (GimpContainer    *container,
					      GimpContext      *context);
static void gimp_context_real_set_template   (GimpContext      *context,
					      GimpTemplate     *template);


/*  utilities  */
static gpointer gimp_context_find_object     (GimpContext      *context,
                                              GimpContainer    *container,
                                              const gchar      *object_name,
                                              gpointer          standard_object);


/*  properties & signals  */

enum
{
  GIMP_CONTEXT_PROP_0,
  GIMP_CONTEXT_PROP_GIMP

  /*  remaining values are in gimpcontext.h  */
};

enum
{
  DUMMY_0,
  DUMMY_1,
  IMAGE_CHANGED,
  DISPLAY_CHANGED,
  TOOL_CHANGED,
  FOREGROUND_CHANGED,
  BACKGROUND_CHANGED,
  OPACITY_CHANGED,
  PAINT_MODE_CHANGED,
  BRUSH_CHANGED,
  PATTERN_CHANGED,
  GRADIENT_CHANGED,
  PALETTE_CHANGED,
  FONT_CHANGED,
  BUFFER_CHANGED,
  IMAGEFILE_CHANGED,
  TEMPLATE_CHANGED,
  LAST_SIGNAL
};

static gchar *gimp_context_prop_names[] =
{
  NULL, /* PROP_0 */
  "gimp",
  "image",
  "display",
  "tool",
  "foreground",
  "background",
  "opacity",
  "paint-mode",
  "brush",
  "pattern",
  "gradient",
  "palette",
  "font",
  "buffer",
  "imagefile",
  "template"
};

static GType gimp_context_prop_types[] =
{
  G_TYPE_NONE, /* PROP_0    */
  G_TYPE_NONE, /* PROP_GIMP */
  0,
  G_TYPE_NONE,
  0,
  G_TYPE_NONE,
  G_TYPE_NONE,
  G_TYPE_NONE,
  G_TYPE_NONE,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};


static guint gimp_context_signals[LAST_SIGNAL] = { 0 };

static GimpObjectClass * parent_class = NULL;

static GimpToolInfo *standard_tool_info = NULL;
static GimpBrush    *standard_brush     = NULL;
static GimpPattern  *standard_pattern   = NULL;
static GimpGradient *standard_gradient  = NULL;
static GimpPalette  *standard_palette   = NULL;
static GimpFont     *standard_font      = NULL;


GType
gimp_context_get_type (void)
{
  static GType context_type = 0;

  if (! context_type)
    {
      static const GTypeInfo context_info =
      {
        sizeof (GimpContextClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_context_class_init,
	NULL,		/* class_finalize */
	NULL,		/* class_data     */
	sizeof (GimpContext),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_context_init,
      };
      static const GInterfaceInfo config_iface_info =
      {
        (GInterfaceInitFunc) gimp_context_config_iface_init,
        NULL,           /* iface_finalize */
        NULL            /* iface_data     */
      };

      context_type = g_type_register_static (GIMP_TYPE_OBJECT,
					     "GimpContext",
					     &context_info, 0);

      g_type_add_interface_static (context_type, GIMP_TYPE_CONFIG,
                                   &config_iface_info);
    }

  return context_type;
}

static void
gimp_context_class_init (GimpContextClass *klass)
{
  GObjectClass    *object_class;
  GimpObjectClass *gimp_object_class;
  GimpRGB          black;
  GimpRGB          white;

  gimp_rgba_set (&black, 0.0, 0.0, 0.0, GIMP_OPACITY_OPAQUE);
  gimp_rgba_set (&white, 1.0, 1.0, 1.0, GIMP_OPACITY_OPAQUE);

  object_class      = G_OBJECT_CLASS (klass);
  gimp_object_class = GIMP_OBJECT_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  gimp_context_signals[IMAGE_CHANGED] =
    g_signal_new ("image_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, image_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_IMAGE);

  gimp_context_signals[DISPLAY_CHANGED] =
    g_signal_new ("display_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, display_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_OBJECT);

  gimp_context_signals[TOOL_CHANGED] =
    g_signal_new ("tool_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, tool_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_TOOL_INFO);

  gimp_context_signals[FOREGROUND_CHANGED] =
    g_signal_new ("foreground_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, foreground_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__BOXED,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_COLOR | G_SIGNAL_TYPE_STATIC_SCOPE);

  gimp_context_signals[BACKGROUND_CHANGED] =
    g_signal_new ("background_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, background_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__BOXED,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_COLOR | G_SIGNAL_TYPE_STATIC_SCOPE);

  gimp_context_signals[OPACITY_CHANGED] =
    g_signal_new ("opacity_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, opacity_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__DOUBLE,
		  G_TYPE_NONE, 1,
		  G_TYPE_DOUBLE);

  gimp_context_signals[PAINT_MODE_CHANGED] =
    g_signal_new ("paint_mode_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, paint_mode_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__ENUM,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_LAYER_MODE_EFFECTS);

  gimp_context_signals[BRUSH_CHANGED] =
    g_signal_new ("brush_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, brush_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_BRUSH);

  gimp_context_signals[PATTERN_CHANGED] =
    g_signal_new ("pattern_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, pattern_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_PATTERN);

  gimp_context_signals[GRADIENT_CHANGED] =
    g_signal_new ("gradient_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, gradient_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_GRADIENT);

  gimp_context_signals[PALETTE_CHANGED] =
    g_signal_new ("palette_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, palette_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_PALETTE);

  gimp_context_signals[FONT_CHANGED] =
    g_signal_new ("font_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, font_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_FONT);

  gimp_context_signals[BUFFER_CHANGED] =
    g_signal_new ("buffer_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, buffer_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_BUFFER);

  gimp_context_signals[IMAGEFILE_CHANGED] =
    g_signal_new ("imagefile_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, imagefile_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_IMAGEFILE);

  gimp_context_signals[TEMPLATE_CHANGED] =
    g_signal_new ("template_changed",
		  G_TYPE_FROM_CLASS (klass),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GimpContextClass, template_changed),
		  NULL, NULL,
		  gimp_marshal_VOID__OBJECT,
		  G_TYPE_NONE, 1,
		  GIMP_TYPE_TEMPLATE);

  object_class->constructor      = gimp_context_constructor;
  object_class->set_property     = gimp_context_set_property;
  object_class->get_property     = gimp_context_get_property;
  object_class->dispose          = gimp_context_dispose;
  object_class->finalize         = gimp_context_finalize;

  gimp_object_class->get_memsize = gimp_context_get_memsize;

  klass->image_changed           = NULL;
  klass->display_changed         = NULL;
  klass->tool_changed            = NULL;
  klass->foreground_changed      = NULL;
  klass->background_changed      = NULL;
  klass->opacity_changed         = NULL;
  klass->paint_mode_changed      = NULL;
  klass->brush_changed           = NULL;
  klass->pattern_changed         = NULL;
  klass->gradient_changed        = NULL;
  klass->palette_changed         = NULL;
  klass->font_changed            = NULL;
  klass->buffer_changed          = NULL;
  klass->imagefile_changed       = NULL;
  klass->template_changed        = NULL;

  gimp_context_prop_types[GIMP_CONTEXT_PROP_IMAGE]     = GIMP_TYPE_IMAGE;
  gimp_context_prop_types[GIMP_CONTEXT_PROP_TOOL]      = GIMP_TYPE_TOOL_INFO;
  gimp_context_prop_types[GIMP_CONTEXT_PROP_BRUSH]     = GIMP_TYPE_BRUSH;
  gimp_context_prop_types[GIMP_CONTEXT_PROP_PATTERN]   = GIMP_TYPE_PATTERN;
  gimp_context_prop_types[GIMP_CONTEXT_PROP_GRADIENT]  = GIMP_TYPE_GRADIENT;
  gimp_context_prop_types[GIMP_CONTEXT_PROP_PALETTE]   = GIMP_TYPE_PALETTE;
  gimp_context_prop_types[GIMP_CONTEXT_PROP_FONT]      = GIMP_TYPE_FONT;
  gimp_context_prop_types[GIMP_CONTEXT_PROP_BUFFER]    = GIMP_TYPE_BUFFER;
  gimp_context_prop_types[GIMP_CONTEXT_PROP_IMAGEFILE] = GIMP_TYPE_IMAGEFILE;
  gimp_context_prop_types[GIMP_CONTEXT_PROP_TEMPLATE]  = GIMP_TYPE_TEMPLATE;

  g_object_class_install_property (object_class, GIMP_CONTEXT_PROP_GIMP,
                                   g_param_spec_object ("gimp",
                                                        NULL, NULL,
                                                        GIMP_TYPE_GIMP,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY));

  g_object_class_install_property (object_class, GIMP_CONTEXT_PROP_IMAGE,
                                   g_param_spec_object (gimp_context_prop_names[GIMP_CONTEXT_PROP_IMAGE],
                                                        NULL, NULL,
                                                        GIMP_TYPE_IMAGE,
                                                        G_PARAM_READWRITE));

  g_object_class_install_property (object_class, GIMP_CONTEXT_PROP_DISPLAY,
                                   g_param_spec_object (gimp_context_prop_names[GIMP_CONTEXT_PROP_DISPLAY],
                                                        NULL, NULL,
                                                        GIMP_TYPE_OBJECT,
                                                        G_PARAM_READWRITE));

  GIMP_CONFIG_INSTALL_PROP_OBJECT (object_class, GIMP_CONTEXT_PROP_TOOL,
                                   gimp_context_prop_names[GIMP_CONTEXT_PROP_TOOL], NULL,
                                   GIMP_TYPE_TOOL_INFO,
                                   0);

  GIMP_CONFIG_INSTALL_PROP_COLOR (object_class, GIMP_CONTEXT_PROP_FOREGROUND,
                                  gimp_context_prop_names[GIMP_CONTEXT_PROP_FOREGROUND],
                                  NULL,
                                  &black,
                                  0);

  GIMP_CONFIG_INSTALL_PROP_COLOR (object_class, GIMP_CONTEXT_PROP_BACKGROUND,
                                  gimp_context_prop_names[GIMP_CONTEXT_PROP_BACKGROUND],
                                  NULL,
                                  &white,
                                  0);

  GIMP_CONFIG_INSTALL_PROP_DOUBLE (object_class, GIMP_CONTEXT_PROP_OPACITY,
				   gimp_context_prop_names[GIMP_CONTEXT_PROP_OPACITY],
                                   NULL,
                                   GIMP_OPACITY_TRANSPARENT,
                                   GIMP_OPACITY_OPAQUE,
                                   GIMP_OPACITY_OPAQUE,
                                   0);

  GIMP_CONFIG_INSTALL_PROP_ENUM (object_class, GIMP_CONTEXT_PROP_PAINT_MODE,
				 gimp_context_prop_names[GIMP_CONTEXT_PROP_PAINT_MODE],
                                 NULL,
                                 GIMP_TYPE_LAYER_MODE_EFFECTS,
                                 GIMP_NORMAL_MODE,
                                 0);

  GIMP_CONFIG_INSTALL_PROP_OBJECT (object_class, GIMP_CONTEXT_PROP_BRUSH,
                                   gimp_context_prop_names[GIMP_CONTEXT_PROP_BRUSH],
                                   NULL,
                                   GIMP_TYPE_BRUSH,
                                   0);

  GIMP_CONFIG_INSTALL_PROP_OBJECT (object_class, GIMP_CONTEXT_PROP_PATTERN,
                                   gimp_context_prop_names[GIMP_CONTEXT_PROP_PATTERN],
                                   NULL,
                                   GIMP_TYPE_PATTERN,
                                   0);

  GIMP_CONFIG_INSTALL_PROP_OBJECT (object_class, GIMP_CONTEXT_PROP_GRADIENT,
                                   gimp_context_prop_names[GIMP_CONTEXT_PROP_GRADIENT],
                                   NULL,
                                   GIMP_TYPE_GRADIENT,
                                   0);

  GIMP_CONFIG_INSTALL_PROP_OBJECT (object_class, GIMP_CONTEXT_PROP_PALETTE,
                                   gimp_context_prop_names[GIMP_CONTEXT_PROP_PALETTE],
                                   NULL,
                                   GIMP_TYPE_PALETTE,
                                   0);

  GIMP_CONFIG_INSTALL_PROP_OBJECT (object_class, GIMP_CONTEXT_PROP_FONT,
                                   gimp_context_prop_names[GIMP_CONTEXT_PROP_FONT],
                                   NULL,
                                   GIMP_TYPE_FONT,
                                   0);

  g_object_class_install_property (object_class, GIMP_CONTEXT_PROP_BUFFER,
				   g_param_spec_object (gimp_context_prop_names[GIMP_CONTEXT_PROP_BUFFER],
							NULL, NULL,
							GIMP_TYPE_BUFFER,
							G_PARAM_READWRITE));

  g_object_class_install_property (object_class, GIMP_CONTEXT_PROP_IMAGEFILE,
				   g_param_spec_object (gimp_context_prop_names[GIMP_CONTEXT_PROP_IMAGEFILE],
							NULL, NULL,
							GIMP_TYPE_IMAGEFILE,
							G_PARAM_READWRITE));

  g_object_class_install_property (object_class, GIMP_CONTEXT_PROP_TEMPLATE,
				   g_param_spec_object (gimp_context_prop_names[GIMP_CONTEXT_PROP_TEMPLATE],
							NULL, NULL,
							GIMP_TYPE_TEMPLATE,
							G_PARAM_READWRITE));
}

static void
gimp_context_init (GimpContext *context)
{
  context->gimp           = NULL;

  context->parent         = NULL;

  context->defined_props  = GIMP_CONTEXT_ALL_PROPS_MASK;

  context->image          = NULL;
  context->display        = NULL;

  context->tool_info      = NULL;
  context->tool_name      = NULL;

  context->brush          = NULL;
  context->brush_name     = NULL;

  context->pattern        = NULL;
  context->pattern_name   = NULL;

  context->gradient       = NULL;
  context->gradient_name  = NULL;

  context->palette        = NULL;
  context->palette_name   = NULL;

  context->font           = NULL;
  context->font_name      = NULL;

  context->buffer         = NULL;
  context->buffer_name    = NULL;

  context->imagefile      = NULL;
  context->imagefile_name = NULL;

  context->template       = NULL;
  context->template_name  = NULL;
}

static void
gimp_context_config_iface_init (GimpConfigInterface *config_iface)
{
  config_iface->serialize            = gimp_context_serialize;

  config_iface->serialize_property   = gimp_context_serialize_property;
  config_iface->deserialize_property = gimp_context_deserialize_property;
}

static GObject *
gimp_context_constructor (GType                  type,
                          guint                  n_params,
                          GObjectConstructParam *params)
{
  GObject *object;
  Gimp    *gimp;

  object = G_OBJECT_CLASS (parent_class)->constructor (type, n_params, params);

  gimp = GIMP_CONTEXT (object)->gimp;

  g_assert (GIMP_IS_GIMP (gimp));

  gimp->context_list = g_list_prepend (gimp->context_list, object);

  g_signal_connect_object (gimp->images, "remove",
                           G_CALLBACK (gimp_context_image_removed),
                           object, 0);
  g_signal_connect_object (gimp->displays, "remove",
                           G_CALLBACK (gimp_context_display_removed),
                           object, 0);

  g_signal_connect_object (gimp->tool_info_list, "remove",
                           G_CALLBACK (gimp_context_tool_removed),
                           object, 0);
  g_signal_connect_object (gimp->tool_info_list, "thaw",
                           G_CALLBACK (gimp_context_tool_list_thaw),
                           object, 0);

  g_signal_connect_object (gimp->brush_factory->container, "remove",
                           G_CALLBACK (gimp_context_brush_removed),
                           object, 0);
  g_signal_connect_object (gimp->brush_factory->container, "thaw",
                           G_CALLBACK (gimp_context_brush_list_thaw),
                           object, 0);

  g_signal_connect_object (gimp->pattern_factory->container, "remove",
                           G_CALLBACK (gimp_context_pattern_removed),
                           object, 0);
  g_signal_connect_object (gimp->pattern_factory->container, "thaw",
                           G_CALLBACK (gimp_context_pattern_list_thaw),
                           object, 0);

  g_signal_connect_object (gimp->gradient_factory->container, "remove",
                           G_CALLBACK (gimp_context_gradient_removed),
                           object, 0);
  g_signal_connect_object (gimp->gradient_factory->container, "thaw",
                           G_CALLBACK (gimp_context_gradient_list_thaw),
                           object, 0);

  g_signal_connect_object (gimp->palette_factory->container, "remove",
                           G_CALLBACK (gimp_context_palette_removed),
                           object, 0);
  g_signal_connect_object (gimp->palette_factory->container, "thaw",
                           G_CALLBACK (gimp_context_palette_list_thaw),
                           object, 0);

  g_signal_connect_object (gimp->fonts, "remove",
                           G_CALLBACK (gimp_context_font_removed),
                           object, 0);
  g_signal_connect_object (gimp->fonts, "thaw",
                           G_CALLBACK (gimp_context_font_list_thaw),
                           object, 0);

  g_signal_connect_object (gimp->named_buffers, "remove",
                           G_CALLBACK (gimp_context_buffer_removed),
                           object, 0);
  g_signal_connect_object (gimp->named_buffers, "thaw",
			   G_CALLBACK (gimp_context_buffer_list_thaw),
			   object, 0);

  g_signal_connect_object (gimp->documents, "remove",
                           G_CALLBACK (gimp_context_imagefile_removed),
                           object, 0);
  g_signal_connect_object (gimp->documents, "thaw",
                           G_CALLBACK (gimp_context_imagefile_list_thaw),
                           object, 0);

  g_signal_connect_object (gimp->templates, "remove",
                           G_CALLBACK (gimp_context_template_removed),
                           object, 0);
  g_signal_connect_object (gimp->templates, "thaw",
                           G_CALLBACK (gimp_context_template_list_thaw),
                           object, 0);

  return object;
}

static void
gimp_context_dispose (GObject *object)
{
  GimpContext *context = GIMP_CONTEXT (object);

  if (context->gimp)
    {
      context->gimp->context_list = g_list_remove (context->gimp->context_list,
                                                   context);
      context->gimp = NULL;
    }

  G_OBJECT_CLASS (parent_class)->dispose (object);
}

static void
gimp_context_finalize (GObject *object)
{
  GimpContext *context = GIMP_CONTEXT (object);

  context->parent  = NULL;
  context->image   = NULL;
  context->display = NULL;

  if (context->tool_info)
    {
      g_object_unref (context->tool_info);
      context->tool_info = NULL;
    }
  if (context->tool_name)
    {
      g_free (context->tool_name);
      context->tool_name = NULL;
    }

  if (context->brush)
    {
      g_object_unref (context->brush);
      context->brush = NULL;
    }
  if (context->brush_name)
    {
      g_free (context->brush_name);
      context->brush_name = NULL;
    }

  if (context->pattern)
    {
      g_object_unref (context->pattern);
      context->pattern = NULL;
    }
  if (context->pattern_name)
    {
      g_free (context->pattern_name);
      context->pattern_name = NULL;
    }

  if (context->gradient)
    {
      g_object_unref (context->gradient);
      context->gradient = NULL;
    }
  if (context->gradient_name)
    {
      g_free (context->gradient_name);
      context->gradient_name = NULL;
    }

  if (context->palette)
    {
      g_object_unref (context->palette);
      context->palette = NULL;
    }
  if (context->palette_name)
    {
      g_free (context->palette_name);
      context->palette_name = NULL;
    }

  if (context->font)
    {
      g_object_unref (context->font);
      context->font = NULL;
    }
  if (context->font_name)
    {
      g_free (context->font_name);
      context->font_name = NULL;
    }

  if (context->buffer)
    {
      g_object_unref (context->buffer);
      context->buffer = NULL;
    }
  if (context->buffer_name)
    {
      g_free (context->buffer_name);
      context->buffer_name = NULL;
    }

  if (context->imagefile)
    {
      g_object_unref (context->imagefile);
      context->imagefile = NULL;
    }
  if (context->imagefile_name)
    {
      g_free (context->imagefile_name);
      context->imagefile_name = NULL;
    }

  if (context->template)
    {
      g_object_unref (context->template);
      context->template = NULL;
    }
  if (context->template_name)
    {
      g_free (context->template_name);
      context->template_name = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_context_set_property (GObject      *object,
			   guint         property_id,
			   const GValue *value,
			   GParamSpec   *pspec)
{
  GimpContext *context = GIMP_CONTEXT (object);

  switch (property_id)
    {
    case GIMP_CONTEXT_PROP_GIMP:
      context->gimp = g_value_get_object (value);
      break;
    case GIMP_CONTEXT_PROP_IMAGE:
      gimp_context_set_image (context, g_value_get_object (value));
      break;
    case GIMP_CONTEXT_PROP_DISPLAY:
      gimp_context_set_display (context, g_value_get_object (value));
      break;
    case GIMP_CONTEXT_PROP_TOOL:
      gimp_context_set_tool (context, g_value_get_object (value));
      break;
    case GIMP_CONTEXT_PROP_FOREGROUND:
      gimp_context_set_foreground (context, g_value_get_boxed (value));
      break;
    case GIMP_CONTEXT_PROP_BACKGROUND:
      gimp_context_set_background (context, g_value_get_boxed (value));
      break;
    case GIMP_CONTEXT_PROP_OPACITY:
      gimp_context_set_opacity (context, g_value_get_double (value));
      break;
    case GIMP_CONTEXT_PROP_PAINT_MODE:
      gimp_context_set_paint_mode (context, g_value_get_enum (value));
      break;
    case GIMP_CONTEXT_PROP_BRUSH:
      gimp_context_set_brush (context, g_value_get_object (value));
      break;
    case GIMP_CONTEXT_PROP_PATTERN:
      gimp_context_set_pattern (context, g_value_get_object (value));
      break;
    case GIMP_CONTEXT_PROP_GRADIENT:
      gimp_context_set_gradient (context, g_value_get_object (value));
      break;
    case GIMP_CONTEXT_PROP_PALETTE:
      gimp_context_set_palette (context, g_value_get_object (value));
      break;
    case GIMP_CONTEXT_PROP_FONT:
      gimp_context_set_font (context, g_value_get_object (value));
      break;
    case GIMP_CONTEXT_PROP_BUFFER:
      gimp_context_set_buffer (context, g_value_get_object (value));
      break;
    case GIMP_CONTEXT_PROP_IMAGEFILE:
      gimp_context_set_imagefile (context, g_value_get_object (value));
      break;
    case GIMP_CONTEXT_PROP_TEMPLATE:
      gimp_context_set_template (context, g_value_get_object (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_context_get_property (GObject    *object,
			   guint       property_id,
			   GValue     *value,
			   GParamSpec *pspec)
{
  GimpContext *context = GIMP_CONTEXT (object);

  switch (property_id)
    {
    case GIMP_CONTEXT_PROP_GIMP:
      g_value_set_object (value, context->gimp);
      break;
    case GIMP_CONTEXT_PROP_IMAGE:
      g_value_set_object (value, gimp_context_get_image (context));
      break;
    case GIMP_CONTEXT_PROP_DISPLAY:
      g_value_set_object (value, gimp_context_get_display (context));
      break;
    case GIMP_CONTEXT_PROP_TOOL:
      g_value_set_object (value, gimp_context_get_tool (context));
      break;
    case GIMP_CONTEXT_PROP_FOREGROUND:
      {
        GimpRGB color;

        gimp_context_get_foreground (context, &color);
        g_value_set_boxed (value, &color);
      }
      break;
    case GIMP_CONTEXT_PROP_BACKGROUND:
      {
        GimpRGB color;

        gimp_context_get_background (context, &color);
        g_value_set_boxed (value, &color);
      }
      break;
    case GIMP_CONTEXT_PROP_OPACITY:
      g_value_set_double (value, gimp_context_get_opacity (context));
      break;
    case GIMP_CONTEXT_PROP_PAINT_MODE:
      g_value_set_enum (value, gimp_context_get_paint_mode (context));
      break;
    case GIMP_CONTEXT_PROP_BRUSH:
      g_value_set_object (value, gimp_context_get_brush (context));
      break;
    case GIMP_CONTEXT_PROP_PATTERN:
      g_value_set_object (value, gimp_context_get_pattern (context));
      break;
    case GIMP_CONTEXT_PROP_GRADIENT:
      g_value_set_object (value, gimp_context_get_gradient (context));
      break;
    case GIMP_CONTEXT_PROP_PALETTE:
      g_value_set_object (value, gimp_context_get_palette (context));
      break;
    case GIMP_CONTEXT_PROP_FONT:
      g_value_set_object (value, gimp_context_get_font (context));
      break;
    case GIMP_CONTEXT_PROP_BUFFER:
      g_value_set_object (value, gimp_context_get_buffer (context));
      break;
    case GIMP_CONTEXT_PROP_IMAGEFILE:
      g_value_set_object (value, gimp_context_get_imagefile (context));
      break;
    case GIMP_CONTEXT_PROP_TEMPLATE:
      g_value_set_object (value, gimp_context_get_template (context));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static gint64
gimp_context_get_memsize (GimpObject *object,
                          gint64     *gui_size)
{
  GimpContext *context = GIMP_CONTEXT (object);
  gint64       memsize = 0;

  if (context->tool_name)
    memsize += strlen (context->tool_name) + 1;

  if (context->brush_name)
    memsize += strlen (context->brush_name) + 1;

  if (context->pattern_name)
    memsize += strlen (context->pattern_name) + 1;

  if (context->palette_name)
    memsize += strlen (context->palette_name) + 1;

  if (context->font_name)
    memsize += strlen (context->font_name) + 1;

  if (context->buffer_name)
    memsize += strlen (context->buffer_name) + 1;

  if (context->imagefile_name)
    memsize += strlen (context->imagefile_name) + 1;

  if (context->template_name)
    memsize += strlen (context->template_name) + 1;

  return memsize + GIMP_OBJECT_CLASS (parent_class)->get_memsize (object,
                                                                  gui_size);
}

static gboolean
gimp_context_serialize (GimpConfig       *config,
                        GimpConfigWriter *writer,
                        gpointer          data)
{
  return gimp_config_serialize_changed_properties (config, writer);
}

static gboolean
gimp_context_serialize_property (GimpConfig       *config,
                                 guint             property_id,
                                 const GValue     *value,
                                 GParamSpec       *pspec,
                                 GimpConfigWriter *writer)
{
  GimpContext *context;
  GimpObject  *serialize_obj;

  context = GIMP_CONTEXT (config);

#if 0
  /*  serialize nothing if the property is not defined  */
  if (! ((1 << property_id) & context->defined_props))
    return TRUE;
#endif

  switch (property_id)
    {
    case GIMP_CONTEXT_PROP_TOOL:
    case GIMP_CONTEXT_PROP_BRUSH:
    case GIMP_CONTEXT_PROP_PATTERN:
    case GIMP_CONTEXT_PROP_GRADIENT:
    case GIMP_CONTEXT_PROP_PALETTE:
    case GIMP_CONTEXT_PROP_FONT:
      serialize_obj = g_value_get_object (value);
      break;

    default:
      return FALSE;
    }

  gimp_config_writer_open (writer, pspec->name);

  if (serialize_obj)
    gimp_config_writer_string (writer, gimp_object_get_name (serialize_obj));
  else
    gimp_config_writer_print (writer, "NULL", 4);

  gimp_config_writer_close (writer);

  return TRUE;
}

static gboolean
gimp_context_deserialize_property (GimpConfig *object,
                                   guint       property_id,
                                   GValue     *value,
                                   GParamSpec *pspec,
                                   GScanner   *scanner,
                                   GTokenType *expected)
{
  GimpContext   *context;
  GimpContainer *container;
  GimpObject    *current;
  gchar        **name_loc;
  gboolean       no_data = FALSE;
  gchar         *object_name;

  context = GIMP_CONTEXT (object);

  switch (property_id)
    {
    case GIMP_CONTEXT_PROP_TOOL:
      container = context->gimp->tool_info_list;
      current   = (GimpObject *) context->tool_info;
      name_loc  = &context->tool_name;
      no_data   = TRUE;
      break;

    case GIMP_CONTEXT_PROP_BRUSH:
      container = context->gimp->brush_factory->container;
      current   = (GimpObject *) context->brush;
      name_loc  = &context->brush_name;
      break;

    case GIMP_CONTEXT_PROP_PATTERN:
      container = context->gimp->pattern_factory->container;
      current   = (GimpObject *) context->pattern;
      name_loc  = &context->pattern_name;
      break;

    case GIMP_CONTEXT_PROP_GRADIENT:
      container = context->gimp->gradient_factory->container;
      current   = (GimpObject *) context->gradient;
      name_loc  = &context->gradient_name;
      break;

    case GIMP_CONTEXT_PROP_PALETTE:
      container = context->gimp->palette_factory->container;
      current   = (GimpObject *) context->palette;
      name_loc  = &context->palette_name;
      break;

    case GIMP_CONTEXT_PROP_FONT:
      container = context->gimp->fonts;
      current   = (GimpObject *) context->font;
      name_loc  = &context->font_name;
      break;

    default:
      return FALSE;
    }

  if (! no_data)
    no_data = context->gimp->no_data;

  if (gimp_scanner_parse_identifier (scanner, "NULL"))
    {
      g_value_set_object (value, NULL);
    }
  else if (gimp_scanner_parse_string (scanner, &object_name))
    {
      GimpObject *deserialize_obj;

      if (! object_name)
        object_name = g_strdup ("");

      deserialize_obj = gimp_container_get_child_by_name (container,
                                                          object_name);

      if (! deserialize_obj)
        {
          if (no_data)
            {
              g_free (*name_loc);
              *name_loc = g_strdup (object_name);
            }
          else
            {
              deserialize_obj = current;
            }
        }

      g_value_set_object (value, deserialize_obj);

      g_free (object_name);
    }
  else
    {
      *expected = G_TOKEN_STRING;
    }

  return TRUE;
}


/*****************************************************************************/
/*  public functions  ********************************************************/

GimpContext *
gimp_context_new (Gimp        *gimp,
		  const gchar *name,
		  GimpContext *template)
{
  GimpContext *context;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);
  g_return_val_if_fail (name != NULL, NULL);
  g_return_val_if_fail (! template || GIMP_IS_CONTEXT (template), NULL);

  context = g_object_new (GIMP_TYPE_CONTEXT,
                          "name", name,
                          "gimp", gimp,
                          NULL);

  if (template)
    {
      context->defined_props = template->defined_props;

      gimp_context_copy_properties (template, context,
				    GIMP_CONTEXT_ALL_PROPS_MASK);
    }

  return context;
}

const gchar *
gimp_context_get_name (const GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return gimp_object_get_name (GIMP_OBJECT (context));
}

void
gimp_context_set_name (GimpContext *context,
		       const gchar *name)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  if (! name)
    name = _("Unnamed");

  gimp_object_set_name (GIMP_OBJECT (context), name);
}

GimpContext *
gimp_context_get_parent (const GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->parent;
}

static void
gimp_context_parent_notify (GimpContext *parent,
                            GParamSpec  *pspec,
                            GimpContext *context)
{
  /*  copy from parent if the changed property is undefined  */
  if (! ((1 << pspec->param_id) & context->defined_props))
    gimp_context_copy_property (parent, context, pspec->param_id);
}

void
gimp_context_set_parent (GimpContext *context,
			 GimpContext *parent)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  g_return_if_fail (parent == NULL || GIMP_IS_CONTEXT (parent));
  g_return_if_fail (parent == NULL || parent->parent != context);
  g_return_if_fail (context != parent);

  if (context->parent == parent)
    return;

  if (context->parent)
    {
      g_signal_handlers_disconnect_by_func (context->parent,
                                            gimp_context_parent_notify,
                                            context);
    }

  context->parent = parent;

  if (parent)
    {
      /*  copy all undefined properties from the new parent  */
      gimp_context_copy_properties (parent, context,
                                    ~context->defined_props &
                                    GIMP_CONTEXT_ALL_PROPS_MASK);

      g_signal_connect_object (parent, "notify",
                               G_CALLBACK (gimp_context_parent_notify),
                               context,
                               0);
    }
}


/*  define / undefinine context properties  */

void
gimp_context_define_property (GimpContext         *context,
			      GimpContextPropType  prop,
			      gboolean             defined)
{
  GimpContextPropMask mask;

  g_return_if_fail (GIMP_IS_CONTEXT (context));
  g_return_if_fail ((prop >= GIMP_CONTEXT_FIRST_PROP) &&
                    (prop <= GIMP_CONTEXT_LAST_PROP));

  mask = (1 << prop);

  if (defined)
    {
      if (! (context->defined_props & mask))
	{
	  context->defined_props |= mask;
	}
    }
  else
    {
      if (context->defined_props & mask)
	{
	  context->defined_props &= ~mask;

	  if (context->parent)
            gimp_context_copy_property (context->parent, context, prop);
	}
    }
}

gboolean
gimp_context_property_defined (GimpContext         *context,
			       GimpContextPropType  prop)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), FALSE);

  return (context->defined_props & (1 << prop)) ? TRUE : FALSE;
}

void
gimp_context_define_properties (GimpContext         *context,
				GimpContextPropMask  prop_mask,
				gboolean             defined)
{
  GimpContextPropType prop;

  g_return_if_fail (GIMP_IS_CONTEXT (context));

  for (prop = GIMP_CONTEXT_FIRST_PROP; prop <= GIMP_CONTEXT_LAST_PROP; prop++)
    if ((1 << prop) & prop_mask)
      gimp_context_define_property (context, prop, defined);
}


/*  copying context properties  */

void
gimp_context_copy_property (GimpContext         *src,
			    GimpContext         *dest,
			    GimpContextPropType  prop)
{
  gpointer   object          = NULL;
  gpointer   standard_object = NULL;
  gchar     *src_name        = NULL;
  gchar    **dest_name_loc   = NULL;

  g_return_if_fail (GIMP_IS_CONTEXT (src));
  g_return_if_fail (GIMP_IS_CONTEXT (dest));
  g_return_if_fail ((prop >= GIMP_CONTEXT_FIRST_PROP) &&
                    (prop <= GIMP_CONTEXT_LAST_PROP));

  switch (prop)
    {
    case GIMP_CONTEXT_PROP_IMAGE:
      gimp_context_real_set_image (dest, src->image);
      break;

    case GIMP_CONTEXT_PROP_DISPLAY:
      gimp_context_real_set_display (dest, src->display);
      break;

    case GIMP_CONTEXT_PROP_TOOL:
      gimp_context_real_set_tool (dest, src->tool_info);
      object          = src->tool_info;
      standard_object = standard_tool_info;
      src_name        = src->tool_name;
      dest_name_loc   = &dest->tool_name;
      break;

    case GIMP_CONTEXT_PROP_FOREGROUND:
      gimp_context_real_set_foreground (dest, &src->foreground);
      break;

    case GIMP_CONTEXT_PROP_BACKGROUND:
      gimp_context_real_set_background (dest, &src->background);
      break;

    case GIMP_CONTEXT_PROP_OPACITY:
      gimp_context_real_set_opacity (dest, src->opacity);
      break;

    case GIMP_CONTEXT_PROP_PAINT_MODE:
      gimp_context_real_set_paint_mode (dest, src->paint_mode);
      break;

    case GIMP_CONTEXT_PROP_BRUSH:
      gimp_context_real_set_brush (dest, src->brush);
      object          = src->brush;
      standard_object = standard_brush;
      src_name        = src->brush_name;
      dest_name_loc   = &dest->brush_name;
      break;

    case GIMP_CONTEXT_PROP_PATTERN:
      gimp_context_real_set_pattern (dest, src->pattern);
      object          = src->pattern;
      standard_object = standard_pattern;
      src_name        = src->pattern_name;
      dest_name_loc   = &dest->pattern_name;
      break;

    case GIMP_CONTEXT_PROP_GRADIENT:
      gimp_context_real_set_gradient (dest, src->gradient);
      object          = src->gradient;
      standard_object = standard_gradient;
      src_name        = src->gradient_name;
      dest_name_loc   = &dest->gradient_name;
      break;

    case GIMP_CONTEXT_PROP_PALETTE:
      gimp_context_real_set_palette (dest, src->palette);
      object          = src->palette;
      standard_object = standard_palette;
      src_name        = src->palette_name;
      dest_name_loc   = &dest->palette_name;
      break;

    case GIMP_CONTEXT_PROP_FONT:
      gimp_context_real_set_font (dest, src->font);
      object          = src->font;
      standard_object = standard_font;
      src_name        = src->font_name;
      dest_name_loc   = &dest->font_name;
      break;

    case GIMP_CONTEXT_PROP_BUFFER:
      gimp_context_real_set_buffer (dest, src->buffer);
      break;

    case GIMP_CONTEXT_PROP_IMAGEFILE:
      gimp_context_real_set_imagefile (dest, src->imagefile);
      break;

    case GIMP_CONTEXT_PROP_TEMPLATE:
      gimp_context_real_set_template (dest, src->template);
      break;

    default:
      break;
    }

  if (src_name && dest_name_loc)
    {
      if (! object || (standard_object && object == standard_object))
        {
          g_free (*dest_name_loc);
          *dest_name_loc = g_strdup (src_name);
        }
    }
}

void
gimp_context_copy_properties (GimpContext         *src,
			      GimpContext         *dest,
			      GimpContextPropMask  prop_mask)
{
  GimpContextPropType prop;

  g_return_if_fail (GIMP_IS_CONTEXT (src));
  g_return_if_fail (GIMP_IS_CONTEXT (dest));

  for (prop = GIMP_CONTEXT_FIRST_PROP; prop <= GIMP_CONTEXT_LAST_PROP; prop++)
    if ((1 << prop) & prop_mask)
      gimp_context_copy_property (src, dest, prop);
}

/*  attribute access functions  */

/*****************************************************************************/
/*  manipulate by GType  *****************************************************/

GimpContextPropType
gimp_context_type_to_property (GType type)
{
  GimpContextPropType prop;

  for (prop = GIMP_CONTEXT_FIRST_PROP; prop <= GIMP_CONTEXT_LAST_PROP; prop++)
    {
      if (g_type_is_a (type, gimp_context_prop_types[prop]))
	return prop;
    }

  return -1;
}

const gchar *
gimp_context_type_to_prop_name (GType type)
{
  GimpContextPropType prop;

  for (prop = GIMP_CONTEXT_FIRST_PROP; prop <= GIMP_CONTEXT_LAST_PROP; prop++)
    {
      if (g_type_is_a (type, gimp_context_prop_types[prop]))
	return gimp_context_prop_names[prop];
    }

  return NULL;
}

const gchar *
gimp_context_type_to_signal_name (GType type)
{
  GimpContextPropType prop;

  for (prop = GIMP_CONTEXT_FIRST_PROP; prop <= GIMP_CONTEXT_LAST_PROP; prop++)
    {
      if (g_type_is_a (type, gimp_context_prop_types[prop]))
	return g_signal_name (gimp_context_signals[prop]);
    }

  return NULL;
}

GimpObject *
gimp_context_get_by_type (GimpContext *context,
			  GType        type)
{
  GimpContextPropType  prop;
  GimpObject          *object = NULL;

  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);
  g_return_val_if_fail ((prop = gimp_context_type_to_property (type)) != -1,
			NULL);

  g_object_get (context,
		gimp_context_prop_names[prop], &object,
		NULL);

  /*  g_object_get() refs the object, this function however is a getter,
   *  which usually doesn't ref it's return value
   */
  if (object)
    g_object_unref (object);

  return object;
}

void
gimp_context_set_by_type (GimpContext *context,
			  GType        type,
			  GimpObject  *object)
{
  GimpContextPropType prop;

  g_return_if_fail (GIMP_IS_CONTEXT (context));
  g_return_if_fail ((prop = gimp_context_type_to_property (type)) != -1);

  g_object_set (context,
		gimp_context_prop_names[prop], object,
		NULL);
}

void
gimp_context_changed_by_type (GimpContext *context,
			      GType        type)
{
  GimpContextPropType  prop;
  GimpObject          *object;

  g_return_if_fail (GIMP_IS_CONTEXT (context));
  g_return_if_fail ((prop = gimp_context_type_to_property (type)) != -1);

  object = gimp_context_get_by_type (context, type);

  g_signal_emit (context,
		 gimp_context_signals[prop], 0,
		 object);
}

/*****************************************************************************/
/*  image  *******************************************************************/

GimpImage *
gimp_context_get_image (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->image;
}

void
gimp_context_set_image (GimpContext *context,
			GimpImage   *image)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_IMAGE);

  gimp_context_real_set_image (context, image);
}

void
gimp_context_image_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[IMAGE_CHANGED], 0,
		 context->image);
}

/*  handle disappearing images  */
static void
gimp_context_image_removed (GimpContainer *container,
			    GimpImage     *image,
			    GimpContext   *context)
{
  if (context->image == image)
    gimp_context_real_set_image (context, NULL);
}

static void
gimp_context_real_set_image (GimpContext *context,
			     GimpImage   *image)
{
  if (context->image == image)
    return;

  context->image = image;

  g_object_notify (G_OBJECT (context), "image");
  gimp_context_image_changed (context);
}


/*****************************************************************************/
/*  display  *****************************************************************/

gpointer
gimp_context_get_display (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->display;
}

void
gimp_context_set_display (GimpContext *context,
			  gpointer     display)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_DISPLAY);

  gimp_context_real_set_display (context, display);
}

void
gimp_context_display_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[DISPLAY_CHANGED], 0,
		 context->display);
}

/*  handle disappearing displays  */
static void
gimp_context_display_removed (GimpContainer *container,
                              gpointer       display,
                              GimpContext   *context)
{
  if (context->display == display)
    gimp_context_real_set_display (context, NULL);
}

static void
gimp_context_real_set_display (GimpContext *context,
			       gpointer     display)
{
  if (context->display == display)
    return;

  context->display = display;

  if (context->display)
    {
      GimpImage *gimage;

      g_object_get (display, "image", &gimage, NULL);

      gimp_context_real_set_image (context, gimage);

      g_object_unref (gimage);
    }

  g_object_notify (G_OBJECT (context), "display");
  gimp_context_display_changed (context);
}


/*****************************************************************************/
/*  tool  ********************************************************************/

GimpToolInfo *
gimp_context_get_tool (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->tool_info;
}

void
gimp_context_set_tool (GimpContext  *context,
		       GimpToolInfo *tool_info)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  g_return_if_fail (! tool_info || GIMP_IS_TOOL_INFO (tool_info));
  context_find_defined (context, GIMP_CONTEXT_PROP_TOOL);

  gimp_context_real_set_tool (context, tool_info);
}

void
gimp_context_tool_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[TOOL_CHANGED], 0,
		 context->tool_info);
}

/*  the active tool was modified  */
static void
gimp_context_tool_dirty (GimpToolInfo *tool_info,
			 GimpContext  *context)
{
  g_free (context->tool_name);
  context->tool_name = g_strdup (GIMP_OBJECT (tool_info)->name);
}

/*  the global tool list is there again after refresh  */
static void
gimp_context_tool_list_thaw (GimpContainer *container,
			     GimpContext   *context)
{
  GimpToolInfo *tool_info;

  if (! context->tool_name)
    context->tool_name = g_strdup ("gimp-rect-select-tool");

  tool_info = gimp_context_find_object (context, container,
                                        context->tool_name,
                                        gimp_tool_info_get_standard (context->gimp));

  gimp_context_real_set_tool (context, tool_info);
}

/*  the active tool disappeared  */
static void
gimp_context_tool_removed (GimpContainer *container,
			   GimpToolInfo  *tool_info,
			   GimpContext   *context)
{
  if (tool_info == context->tool_info)
    {
      context->tool_info = NULL;

      g_signal_handlers_disconnect_by_func (tool_info,
					    gimp_context_tool_dirty,
					    context);
      g_object_unref (tool_info);

      if (! gimp_container_frozen (container))
	gimp_context_tool_list_thaw (container, context);
    }
}

static void
gimp_context_real_set_tool (GimpContext  *context,
			    GimpToolInfo *tool_info)
{
  if (! standard_tool_info)
    standard_tool_info = gimp_tool_info_get_standard (context->gimp);

  if (context->tool_info == tool_info)
    return;

  if (context->tool_name && tool_info != standard_tool_info)
    {
      g_free (context->tool_name);
      context->tool_name = NULL;
    }

  /*  disconnect from the old tool's signals  */
  if (context->tool_info)
    {
      g_signal_handlers_disconnect_by_func (context->tool_info,
					    gimp_context_tool_dirty,
					    context);
      g_object_unref (context->tool_info);
    }

  context->tool_info = tool_info;

  if (tool_info)
    {
      g_object_ref (tool_info);

      g_signal_connect_object (tool_info, "name_changed",
			       G_CALLBACK (gimp_context_tool_dirty),
			       context,
			       0);

      if (tool_info != standard_tool_info)
	context->tool_name = g_strdup (GIMP_OBJECT (tool_info)->name);
    }

  g_object_notify (G_OBJECT (context), "tool");
  gimp_context_tool_changed (context);
}


/*****************************************************************************/
/*  foreground color  ********************************************************/

void
gimp_context_get_foreground (GimpContext *context,
			     GimpRGB     *color)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  g_return_if_fail (color != NULL);

  *color = context->foreground;
}

void
gimp_context_set_foreground (GimpContext   *context,
			     const GimpRGB *color)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  g_return_if_fail (color != NULL);
  context_find_defined (context, GIMP_CONTEXT_PROP_FOREGROUND);

  gimp_context_real_set_foreground (context, color);
}

void
gimp_context_foreground_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[FOREGROUND_CHANGED], 0,
		 &context->foreground);
}

static void
gimp_context_real_set_foreground (GimpContext   *context,
				  const GimpRGB *color)
{
  if (gimp_rgba_distance (&context->foreground, color) < 0.0001)
    return;

  context->foreground = *color;
  gimp_rgb_set_alpha (&context->foreground, GIMP_OPACITY_OPAQUE);

  g_object_notify (G_OBJECT (context), "foreground");
  gimp_context_foreground_changed (context);
}


/*****************************************************************************/
/*  background color  ********************************************************/

void
gimp_context_get_background (GimpContext *context,
			     GimpRGB     *color)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_return_if_fail (color != NULL);

  *color = context->background;
}

void
gimp_context_set_background (GimpContext   *context,
			     const GimpRGB *color)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  g_return_if_fail (color != NULL);
  context_find_defined (context, GIMP_CONTEXT_PROP_BACKGROUND);

  gimp_context_real_set_background (context, color);
}

void
gimp_context_background_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[BACKGROUND_CHANGED], 0,
		 &context->background);
}

static void
gimp_context_real_set_background (GimpContext   *context,
				  const GimpRGB *color)
{
  if (gimp_rgba_distance (&context->background, color) < 0.0001)
    return;

  context->background = *color;
  gimp_rgb_set_alpha (&context->background, GIMP_OPACITY_OPAQUE);

  g_object_notify (G_OBJECT (context), "background");
  gimp_context_background_changed (context);
}


/*****************************************************************************/
/*  color utility functions  *************************************************/

void
gimp_context_set_default_colors (GimpContext *context)
{
  GimpContext *bg_context;
  GimpRGB      fg;
  GimpRGB      bg;

  g_return_if_fail (GIMP_IS_CONTEXT (context));

  bg_context = context;

  context_find_defined (context, GIMP_CONTEXT_PROP_FOREGROUND);
  context_find_defined (bg_context, GIMP_CONTEXT_PROP_BACKGROUND);

  gimp_rgba_set (&fg, 0.0, 0.0, 0.0, GIMP_OPACITY_OPAQUE);
  gimp_rgba_set (&bg, 1.0, 1.0, 1.0, GIMP_OPACITY_OPAQUE);

  gimp_context_real_set_foreground (context, &fg);
  gimp_context_real_set_background (bg_context, &bg);
}

void
gimp_context_swap_colors (GimpContext *context)
{
  GimpContext *bg_context;
  GimpRGB      fg;
  GimpRGB      bg;

  g_return_if_fail (GIMP_IS_CONTEXT (context));

  bg_context = context;

  context_find_defined (context, GIMP_CONTEXT_PROP_FOREGROUND);
  context_find_defined (bg_context, GIMP_CONTEXT_PROP_BACKGROUND);

  gimp_context_get_foreground (context, &fg);
  gimp_context_get_background (bg_context, &bg);

  gimp_context_real_set_foreground (context, &bg);
  gimp_context_real_set_background (bg_context, &fg);
}

/*****************************************************************************/
/*  opacity  *****************************************************************/

gdouble
gimp_context_get_opacity (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), GIMP_OPACITY_OPAQUE);

  return context->opacity;
}

void
gimp_context_set_opacity (GimpContext *context,
			  gdouble      opacity)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_OPACITY);

  gimp_context_real_set_opacity (context, opacity);
}

void
gimp_context_opacity_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[OPACITY_CHANGED], 0,
		 context->opacity);
}

static void
gimp_context_real_set_opacity (GimpContext *context,
			       gdouble      opacity)
{
  if (context->opacity == opacity)
    return;

  context->opacity = opacity;

  g_object_notify (G_OBJECT (context), "opacity");
  gimp_context_opacity_changed (context);
}


/*****************************************************************************/
/*  paint mode  **************************************************************/

GimpLayerModeEffects
gimp_context_get_paint_mode (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), GIMP_NORMAL_MODE);

  return context->paint_mode;
}

void
gimp_context_set_paint_mode (GimpContext          *context,
			     GimpLayerModeEffects  paint_mode)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_PAINT_MODE);

  gimp_context_real_set_paint_mode (context, paint_mode);
}

void
gimp_context_paint_mode_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[PAINT_MODE_CHANGED], 0,
		 context->paint_mode);
}

static void
gimp_context_real_set_paint_mode (GimpContext          *context,
				  GimpLayerModeEffects  paint_mode)
{
  if (context->paint_mode == paint_mode)
    return;

  context->paint_mode = paint_mode;

  g_object_notify (G_OBJECT (context), "paint-mode");
  gimp_context_paint_mode_changed (context);
}


/*****************************************************************************/
/*  brush  *******************************************************************/

GimpBrush *
gimp_context_get_brush (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->brush;
}

void
gimp_context_set_brush (GimpContext *context,
			GimpBrush   *brush)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  g_return_if_fail (! brush || GIMP_IS_BRUSH (brush));
  context_find_defined (context, GIMP_CONTEXT_PROP_BRUSH);

  gimp_context_real_set_brush (context, brush);
}

void
gimp_context_brush_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[BRUSH_CHANGED], 0,
		 context->brush);
}

/*  the active brush was modified  */
static void
gimp_context_brush_dirty (GimpBrush   *brush,
			  GimpContext *context)
{
  g_free (context->brush_name);
  context->brush_name = g_strdup (GIMP_OBJECT (brush)->name);
}

/*  the global brush list is there again after refresh  */
static void
gimp_context_brush_list_thaw (GimpContainer *container,
			      GimpContext   *context)
{
  GimpBrush *brush;

  if (! context->brush_name)
    context->brush_name = g_strdup (context->gimp->config->default_brush);

  brush = gimp_context_find_object (context, container,
                                    context->brush_name,
                                    gimp_brush_get_standard ());

  gimp_context_real_set_brush (context, brush);
}

/*  the active brush disappeared  */
static void
gimp_context_brush_removed (GimpContainer *container,
			    GimpBrush     *brush,
			    GimpContext   *context)
{
  if (brush == context->brush)
    {
      context->brush = NULL;

      g_signal_handlers_disconnect_by_func (brush,
					    gimp_context_brush_dirty,
					    context);
      g_object_unref (brush);

      if (! gimp_container_frozen (container))
	gimp_context_brush_list_thaw (container, context);
    }
}

static void
gimp_context_real_set_brush (GimpContext *context,
			     GimpBrush   *brush)
{
  if (! standard_brush)
    standard_brush = GIMP_BRUSH (gimp_brush_get_standard ());

  if (context->brush == brush)
    return;

  if (context->brush_name && brush != standard_brush)
    {
      g_free (context->brush_name);
      context->brush_name = NULL;
    }

  /*  disconnect from the old brush's signals  */
  if (context->brush)
    {
      GimpBaseConfig *base_config;

      base_config = GIMP_BASE_CONFIG (context->gimp->config);

      /*  make sure the active brush is swapped before we get a new one...  */
      if (base_config->stingy_memory_use &&
	  context->brush->mask           &&
	  G_OBJECT (context->brush)->ref_count == 2)
	{
	  temp_buf_swap (context->brush->mask);
	}

      g_signal_handlers_disconnect_by_func (context->brush,
					    gimp_context_brush_dirty,
					    context);

      g_object_unref (context->brush);
    }

  context->brush = brush;

  if (brush)
    {
      GimpBaseConfig *base_config;

      base_config = GIMP_BASE_CONFIG (context->gimp->config);

      g_object_ref (brush);

      g_signal_connect_object (brush, "name_changed",
			       G_CALLBACK (gimp_context_brush_dirty),
			       context,
			       0);

      /*  Make sure the active brush is unswapped... */
      if (base_config->stingy_memory_use &&
	  brush->mask                    &&
	  G_OBJECT (brush)->ref_count < 2)
	{
	  temp_buf_unswap (brush->mask);
	}

      if (brush != standard_brush)
	context->brush_name = g_strdup (GIMP_OBJECT (brush)->name);
    }

  g_object_notify (G_OBJECT (context), "brush");
  gimp_context_brush_changed (context);
}


/*****************************************************************************/
/*  pattern  *****************************************************************/

GimpPattern *
gimp_context_get_pattern (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->pattern;
}

void
gimp_context_set_pattern (GimpContext *context,
			  GimpPattern *pattern)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_PATTERN);

  gimp_context_real_set_pattern (context, pattern);
}

void
gimp_context_pattern_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[PATTERN_CHANGED], 0,
		 context->pattern);
}

/*  the active pattern was modified  */
static void
gimp_context_pattern_dirty (GimpPattern *pattern,
			    GimpContext *context)
{
  g_free (context->pattern_name);
  context->pattern_name = g_strdup (GIMP_OBJECT (pattern)->name);
}

/*  the global pattern list is there again after refresh  */
static void
gimp_context_pattern_list_thaw (GimpContainer *container,
				GimpContext   *context)
{
  GimpPattern *pattern;

  if (! context->pattern_name)
    context->pattern_name = g_strdup (context->gimp->config->default_pattern);

  pattern = gimp_context_find_object (context, container,
                                      context->pattern_name,
                                      gimp_pattern_get_standard ());

  gimp_context_real_set_pattern (context, pattern);
}

/*  the active pattern disappeared  */
static void
gimp_context_pattern_removed (GimpContainer *container,
			      GimpPattern   *pattern,
			      GimpContext   *context)
{
  if (pattern == context->pattern)
    {
      context->pattern = NULL;

      g_signal_handlers_disconnect_by_func (pattern,
					    gimp_context_pattern_dirty,
					    context);
      g_object_unref (pattern);

      if (! gimp_container_frozen (container))
	gimp_context_pattern_list_thaw (container, context);
    }
}

static void
gimp_context_real_set_pattern (GimpContext *context,
			       GimpPattern *pattern)
{
  GimpBaseConfig *base_config;

  if (! standard_pattern)
    standard_pattern = GIMP_PATTERN (gimp_pattern_get_standard ());

  if (context->pattern == pattern)
    return;

  if (context->pattern_name && pattern != standard_pattern)
    {
      g_free (context->pattern_name);
      context->pattern_name = NULL;
    }

  base_config = GIMP_BASE_CONFIG (context->gimp->config);

  /*  make sure the active pattern is swapped before we get a new one...  */
  if (base_config->stingy_memory_use             &&
      context->pattern && context->pattern->mask &&
      G_OBJECT (context->pattern)->ref_count == 2)
    {
      temp_buf_swap (pattern->mask);
    }

  /*  disconnect from the old pattern's signals  */
  if (context->pattern)
    {
      g_signal_handlers_disconnect_by_func (context->pattern,
					    gimp_context_pattern_dirty,
					    context);
      g_object_unref (context->pattern);
    }

  context->pattern = pattern;

  if (pattern)
    {
      GimpBaseConfig *base_config;

      base_config = GIMP_BASE_CONFIG (context->gimp->config);

      g_object_ref (pattern);

      g_signal_connect_object (pattern, "name_changed",
			       G_CALLBACK (gimp_context_pattern_dirty),
			       context,
			       0);

      /*  Make sure the active pattern is unswapped... */
      if (base_config->stingy_memory_use   &&
	  pattern->mask                    &&
	  G_OBJECT (pattern)->ref_count < 2)
	{
	  temp_buf_unswap (pattern->mask);
	}

      if (pattern != standard_pattern)
	context->pattern_name = g_strdup (GIMP_OBJECT (pattern)->name);
    }

  g_object_notify (G_OBJECT (context), "pattern");
  gimp_context_pattern_changed (context);
}


/*****************************************************************************/
/*  gradient  ****************************************************************/

GimpGradient *
gimp_context_get_gradient (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->gradient;
}

void
gimp_context_set_gradient (GimpContext  *context,
			   GimpGradient *gradient)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_GRADIENT);

  gimp_context_real_set_gradient (context, gradient);
}

void
gimp_context_gradient_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[GRADIENT_CHANGED], 0,
		 context->gradient);
}

/*  the active gradient was modified  */
static void
gimp_context_gradient_dirty (GimpGradient *gradient,
			     GimpContext  *context)
{
  g_free (context->gradient_name);
  context->gradient_name = g_strdup (GIMP_OBJECT (gradient)->name);
}

/*  the global gradient list is there again after refresh  */
static void
gimp_context_gradient_list_thaw (GimpContainer *container,
				 GimpContext   *context)
{
  GimpGradient *gradient;

  if (! context->gradient_name)
    context->gradient_name = g_strdup (context->gimp->config->default_gradient);

  gradient = gimp_context_find_object (context, container,
                                       context->gradient_name,
                                       gimp_gradient_get_standard ());

  gimp_context_real_set_gradient (context, gradient);
}

/*  the active gradient disappeared  */
static void
gimp_context_gradient_removed (GimpContainer *container,
			       GimpGradient  *gradient,
			       GimpContext   *context)
{
  if (gradient == context->gradient)
    {
      context->gradient = NULL;

      g_signal_handlers_disconnect_by_func (gradient,
					    gimp_context_gradient_dirty,
					    context);
      g_object_unref (gradient);

      if (! gimp_container_frozen (container))
	gimp_context_gradient_list_thaw (container, context);
    }
}

static void
gimp_context_real_set_gradient (GimpContext  *context,
				GimpGradient *gradient)
{
  if (! standard_gradient)
    standard_gradient = GIMP_GRADIENT (gimp_gradient_get_standard ());

  if (context->gradient == gradient)
    return;

  if (context->gradient_name && gradient != standard_gradient)
    {
      g_free (context->gradient_name);
      context->gradient_name = NULL;
    }

  /*  disconnect from the old gradient's signals  */
  if (context->gradient)
    {
      g_signal_handlers_disconnect_by_func (context->gradient,
					    gimp_context_gradient_dirty,
					    context);
      g_object_unref (context->gradient);
    }

  context->gradient = gradient;

  if (gradient)
    {
      g_object_ref (gradient);

      g_signal_connect_object (gradient, "name_changed",
			       G_CALLBACK (gimp_context_gradient_dirty),
			       context,
			       0);

      if (gradient != standard_gradient)
	context->gradient_name = g_strdup (GIMP_OBJECT (gradient)->name);
    }

  g_object_notify (G_OBJECT (context), "gradient");
  gimp_context_gradient_changed (context);
}


/*****************************************************************************/
/*  palette  *****************************************************************/

GimpPalette *
gimp_context_get_palette (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->palette;
}

void
gimp_context_set_palette (GimpContext *context,
			  GimpPalette *palette)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_PALETTE);

  gimp_context_real_set_palette (context, palette);
}

void
gimp_context_palette_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[PALETTE_CHANGED], 0,
		 context->palette);
}

/*  the active palette was modified  */
static void
gimp_context_palette_dirty (GimpPalette *palette,
			    GimpContext *context)
{
  g_free (context->palette_name);
  context->palette_name = g_strdup (GIMP_OBJECT (palette)->name);
}

/*  the global palette list is there again after refresh  */
static void
gimp_context_palette_list_thaw (GimpContainer *container,
				GimpContext   *context)
{
  GimpPalette *palette;

  if (! context->palette_name)
    context->palette_name = g_strdup (context->gimp->config->default_palette);

  palette = gimp_context_find_object (context, container,
                                      context->palette_name,
                                      gimp_palette_get_standard ());

  gimp_context_real_set_palette (context, palette);
}

/*  the active palette disappeared  */
static void
gimp_context_palette_removed (GimpContainer *container,
			      GimpPalette   *palette,
			      GimpContext   *context)
{
  if (palette == context->palette)
    {
      context->palette = NULL;

      g_signal_handlers_disconnect_by_func (palette,
					    gimp_context_palette_dirty,
					    context);
      g_object_unref (palette);

      if (! gimp_container_frozen (container))
	gimp_context_palette_list_thaw (container, context);
    }
}

static void
gimp_context_real_set_palette (GimpContext *context,
			       GimpPalette *palette)
{
  if (! standard_palette)
    standard_palette = GIMP_PALETTE (gimp_palette_get_standard ());

  if (context->palette == palette)
    return;

  if (context->palette_name && palette != standard_palette)
    {
      g_free (context->palette_name);
      context->palette_name = NULL;
    }

  /*  disconnect from the old palette's signals  */
  if (context->palette)
    {
      g_signal_handlers_disconnect_by_func (context->palette,
					    gimp_context_palette_dirty,
					    context);
      g_object_unref (context->palette);
    }

  context->palette = palette;

  if (palette)
    {
      g_object_ref (palette);

      g_signal_connect_object (palette, "name_changed",
			       G_CALLBACK (gimp_context_palette_dirty),
			       context,
			       0);

      if (palette != standard_palette)
	context->palette_name = g_strdup (GIMP_OBJECT (palette)->name);
    }

  g_object_notify (G_OBJECT (context), "palette");
  gimp_context_palette_changed (context);
}


/*****************************************************************************/
/*  font     *****************************************************************/

GimpFont *
gimp_context_get_font (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->font;
}

void
gimp_context_set_font (GimpContext *context,
                       GimpFont    *font)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_FONT);

  gimp_context_real_set_font (context, font);
}

void
gimp_context_font_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[FONT_CHANGED], 0,
		 context->font);
}

/*  the active palette was modified  */
static void
gimp_context_font_dirty (GimpFont    *font,
                         GimpContext *context)
{
  g_free (context->font_name);
  context->font_name = g_strdup (GIMP_OBJECT (font)->name);
}

/*  the global font list is there again after refresh  */
static void
gimp_context_font_list_thaw (GimpContainer *container,
                             GimpContext   *context)
{
  GimpFont *font;

  if (! context->font_name)
    context->font_name = g_strdup (context->gimp->config->default_font);

  font = gimp_context_find_object (context, container,
                                   context->font_name,
                                   gimp_font_get_standard ());

  gimp_context_real_set_font (context, font);
}

/*  the active font disappeared  */
static void
gimp_context_font_removed (GimpContainer *container,
                           GimpFont      *font,
                           GimpContext   *context)
{
  if (font == context->font)
    {
      context->font = NULL;

      g_signal_handlers_disconnect_by_func (font,
					    gimp_context_font_dirty,
					    context);
      g_object_unref (font);

      if (! gimp_container_frozen (container))
	gimp_context_font_list_thaw (container, context);
    }
}

static void
gimp_context_real_set_font (GimpContext *context,
                            GimpFont    *font)
{
  if (! standard_font)
    standard_font = GIMP_FONT (gimp_font_get_standard ());

  if (context->font == font)
    return;

  if (context->font_name && font != standard_font)
    {
      g_free (context->font_name);
      context->font_name = NULL;
    }

  /*  disconnect from the old font's signals  */
  if (context->font)
    {
      g_signal_handlers_disconnect_by_func (context->font,
					    gimp_context_font_dirty,
					    context);
      g_object_unref (context->font);
    }

  context->font = font;

  if (font)
    {
      g_object_ref (font);

      g_signal_connect_object (font, "name_changed",
			       G_CALLBACK (gimp_context_font_dirty),
			       context,
			       0);

      if (font != standard_font)
	context->font_name = g_strdup (GIMP_OBJECT (font)->name);
    }

  g_object_notify (G_OBJECT (context), "font");
  gimp_context_font_changed (context);
}


/*****************************************************************************/
/*  buffer  ******************************************************************/

/*
static GimpBuffer *standard_buffer = NULL;
*/

GimpBuffer *
gimp_context_get_buffer (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->buffer;
}

void
gimp_context_set_buffer (GimpContext *context,
			 GimpBuffer *buffer)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_BUFFER);

  gimp_context_real_set_buffer (context, buffer);
}

void
gimp_context_buffer_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[BUFFER_CHANGED], 0,
		 context->buffer);
}

/*  the active buffer was modified  */
static void
gimp_context_buffer_dirty (GimpBuffer  *buffer,
			   GimpContext *context)
{
  g_free (context->buffer_name);
  context->buffer_name = g_strdup (GIMP_OBJECT (buffer)->name);
}

/*  the global buffer list is there again after refresh  */
static void
gimp_context_buffer_list_thaw (GimpContainer *container,
			       GimpContext   *context)
{
  GimpBuffer *buffer;

  /*
  if (! context->buffer_name)
    context->buffer_name = g_strdup (context->gimp->config->default_buffer);
  */

  buffer = gimp_context_find_object (context, container,
                                     context->buffer_name,
                                     NULL /* gimp_buffer_get_standard () */);

  if (buffer)
    {
      gimp_context_real_set_buffer (context, buffer);
    }
  else
    {
      g_object_notify (G_OBJECT (context), "buffer");
      gimp_context_buffer_changed (context);
    }
}

/*  the active buffer disappeared  */
static void
gimp_context_buffer_removed (GimpContainer *container,
			     GimpBuffer    *buffer,
			     GimpContext   *context)
{
  if (buffer == context->buffer)
    {
      context->buffer = NULL;

      g_signal_handlers_disconnect_by_func (buffer,
					    gimp_context_buffer_dirty,
					    context);
      g_object_unref (buffer);

      if (! gimp_container_frozen (container))
	gimp_context_buffer_list_thaw (container, context);
    }
}

static void
gimp_context_real_set_buffer (GimpContext *context,
			      GimpBuffer  *buffer)
{
  /*
  if (! standard_buffer)
    standard_buffer = GIMP_BUFFER (gimp_buffer_get_standard ());
  */

  if (context->buffer == buffer)
    return;

  if (context->buffer_name /* && buffer != standard_buffer */)
    {
      g_free (context->buffer_name);
      context->buffer_name = NULL;
    }

  /*  disconnect from the old buffer's signals  */
  if (context->buffer)
    {
      g_signal_handlers_disconnect_by_func (context->buffer,
					    gimp_context_buffer_dirty,
					    context);
      g_object_unref (context->buffer);
    }

  context->buffer = buffer;

  if (buffer)
    {
      g_object_ref (buffer);

      g_signal_connect_object (buffer, "name_changed",
			       G_CALLBACK (gimp_context_buffer_dirty),
			       context,
			       0);

      /*
      if (buffer != standard_buffer)
      */
      context->buffer_name = g_strdup (GIMP_OBJECT (buffer)->name);
    }

  g_object_notify (G_OBJECT (context), "buffer");
  gimp_context_buffer_changed (context);
}


/*****************************************************************************/
/*  imagefile  ***************************************************************/

/*
static GimpImagefile *standard_imagefile = NULL;
*/

GimpImagefile *
gimp_context_get_imagefile (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->imagefile;
}

void
gimp_context_set_imagefile (GimpContext   *context,
                            GimpImagefile *imagefile)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_IMAGEFILE);

  gimp_context_real_set_imagefile (context, imagefile);
}

void
gimp_context_imagefile_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[IMAGEFILE_CHANGED], 0,
		 context->imagefile);
}

/*  the active imagefile was modified  */
static void
gimp_context_imagefile_dirty (GimpImagefile *imagefile,
                              GimpContext   *context)
{
  g_free (context->imagefile_name);
  context->imagefile_name = g_strdup (GIMP_OBJECT (imagefile)->name);
}

/*  the global imagefile list is there again after refresh  */
static void
gimp_context_imagefile_list_thaw (GimpContainer *container,
                                  GimpContext   *context)
{
  GimpImagefile *imagefile;

  /*
  if (! context->imagefile_name)
    context->imagefile_name = g_strdup (context->gimp->config->default_imagefile);
  */

  imagefile = gimp_context_find_object (context, container,
                                        context->imagefile_name,
                                        NULL /* gimp_imagefile_get_standard () */);

  if (imagefile)
    {
      gimp_context_real_set_imagefile (context, imagefile);
    }
  else
    {
      g_object_notify (G_OBJECT (context), "imagefile");
      gimp_context_imagefile_changed (context);
    }
}

/*  the active imagefile disappeared  */
static void
gimp_context_imagefile_removed (GimpContainer *container,
                                GimpImagefile *imagefile,
                                GimpContext   *context)
{
  if (imagefile == context->imagefile)
    {
      context->imagefile = NULL;

      g_signal_handlers_disconnect_by_func (imagefile,
					    gimp_context_imagefile_dirty,
					    context);
      g_object_unref (imagefile);

      if (! gimp_container_frozen (container))
	gimp_context_imagefile_list_thaw (container, context);
    }
}

static void
gimp_context_real_set_imagefile (GimpContext   *context,
                                 GimpImagefile *imagefile)
{
  /*
  if (! standard_imagefile)
    standard_imagefile = GIMP_IMAGEFILE (gimp_imagefile_get_standard ());
  */

  if (context->imagefile == imagefile)
    return;

  if (context->imagefile_name /* && imagefile != standard_imagefile */)
    {
      g_free (context->imagefile_name);
      context->imagefile_name = NULL;
    }

  /*  disconnect from the old imagefile's signals  */
  if (context->imagefile)
    {
      g_signal_handlers_disconnect_by_func (context->imagefile,
					    gimp_context_imagefile_dirty,
					    context);
      g_object_unref (context->imagefile);
    }

  context->imagefile = imagefile;

  if (imagefile)
    {
      g_object_ref (imagefile);

      g_signal_connect_object (imagefile, "name_changed",
			       G_CALLBACK (gimp_context_imagefile_dirty),
			       context,
			       0);

      /*
      if (imagefile != standard_imagefile)
      */
      context->imagefile_name = g_strdup (GIMP_OBJECT (imagefile)->name);
    }

  g_object_notify (G_OBJECT (context), "imagefile");
  gimp_context_imagefile_changed (context);
}


/*****************************************************************************/
/*  template  ***************************************************************/

/*
static GimpTemplate *standard_template = NULL;
*/

GimpTemplate *
gimp_context_get_template (GimpContext *context)
{
  g_return_val_if_fail (GIMP_IS_CONTEXT (context), NULL);

  return context->template;
}

void
gimp_context_set_template (GimpContext  *context,
                           GimpTemplate *template)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));
  context_find_defined (context, GIMP_CONTEXT_PROP_TEMPLATE);

  gimp_context_real_set_template (context, template);
}

void
gimp_context_template_changed (GimpContext *context)
{
  g_return_if_fail (GIMP_IS_CONTEXT (context));

  g_signal_emit (context,
		 gimp_context_signals[TEMPLATE_CHANGED], 0,
		 context->template);
}

/*  the active template was modified  */
static void
gimp_context_template_dirty (GimpTemplate *template,
                             GimpContext  *context)
{
  g_free (context->template_name);
  context->template_name = g_strdup (GIMP_OBJECT (template)->name);
}

/*  the global template list is there again after refresh  */
static void
gimp_context_template_list_thaw (GimpContainer *container,
                                 GimpContext   *context)
{
  GimpTemplate *template;

  /*
  if (! context->template_name)
    context->template_name = g_strdup (context->gimp->config->default_template);
  */

  template = gimp_context_find_object (context, container,
                                       context->template_name,
                                        NULL /* gimp_template_get_standard () */);

  if (template)
    {
      gimp_context_real_set_template (context, template);
    }
  else
    {
      g_object_notify (G_OBJECT (context), "template");
      gimp_context_template_changed (context);
    }
}

/*  the active template disappeared  */
static void
gimp_context_template_removed (GimpContainer *container,
                               GimpTemplate  *template,
                               GimpContext   *context)
{
  if (template == context->template)
    {
      context->template = NULL;

      g_signal_handlers_disconnect_by_func (template,
					    gimp_context_template_dirty,
					    context);
      g_object_unref (template);

      if (! gimp_container_frozen (container))
	gimp_context_template_list_thaw (container, context);
    }
}

static void
gimp_context_real_set_template (GimpContext  *context,
                                GimpTemplate *template)
{
  /*
  if (! standard_template)
    standard_template = GIMP_TEMPLATE (gimp_template_get_standard ());
  */

  if (context->template == template)
    return;

  if (context->template_name /* && template != standard_template */)
    {
      g_free (context->template_name);
      context->template_name = NULL;
    }

  /*  disconnect from the old template's signals  */
  if (context->template)
    {
      g_signal_handlers_disconnect_by_func (context->template,
					    gimp_context_template_dirty,
					    context);
      g_object_unref (context->template);
    }

  context->template = template;

  if (template)
    {
      g_object_ref (template);

      g_signal_connect_object (template, "name_changed",
			       G_CALLBACK (gimp_context_template_dirty),
			       context,
			       0);

      /*
      if (template != standard_template)
      */
      context->template_name = g_strdup (GIMP_OBJECT (template)->name);
    }

  g_object_notify (G_OBJECT (context), "template");
  gimp_context_template_changed (context);
}


/*****************************************************************************/
/*  utility functions  *******************************************************/

static gpointer
gimp_context_find_object (GimpContext   *context,
                          GimpContainer *container,
                          const gchar   *object_name,
                          gpointer       standard_object)
{
  GimpObject *object = NULL;

  if (object_name)
    object = gimp_container_get_child_by_name (container, object_name);

  if (! object && gimp_container_num_children (container) > 0)
    object = gimp_container_get_child_by_index (container, 0);

  if (! object)
    object = standard_object;

  return object;
}
