/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpcolorselect.c
 * Copyright (C) 2002 Michael Natterer <mitch@gimp.org>
 *
 * based on color_notebook module
 * Copyright (C) 1998 Austin Donnelly <austin@greenend.org.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#ifdef __GNUC__
#warning GTK_DISABLE_DEPRECATED
#endif
#undef GTK_DISABLE_DEPRECATED

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "libgimpcolor/gimpcolor.h"
#include "libgimpmath/gimpmath.h"

#include "gimpwidgetstypes.h"

#include "gimpcolorselector.h"
#include "gimpcolorselect.h"
#include "gimphelpui.h"
#include "gimpstock.h"

#include "libgimp/libgimp-intl.h"


#define XY_DEF_WIDTH     GIMP_COLOR_SELECTOR_SIZE
#define XY_DEF_HEIGHT    GIMP_COLOR_SELECTOR_SIZE
#define Z_DEF_WIDTH      GIMP_COLOR_SELECTOR_BAR_SIZE
#define Z_DEF_HEIGHT     GIMP_COLOR_SELECTOR_SIZE

#define COLOR_AREA_EVENT_MASK (GDK_EXPOSURE_MASK       | \
                               GDK_BUTTON_PRESS_MASK   | \
                               GDK_BUTTON_RELEASE_MASK | \
			       GDK_BUTTON_MOTION_MASK  | \
                               GDK_ENTER_NOTIFY_MASK)


typedef enum
{
  COLOR_SELECT_HUE = 0,
  COLOR_SELECT_SATURATION,
  COLOR_SELECT_VALUE,
  COLOR_SELECT_RED,
  COLOR_SELECT_GREEN,
  COLOR_SELECT_BLUE,
  COLOR_SELECT_ALPHA,
  COLOR_SELECT_HUE_SATURATION,
  COLOR_SELECT_HUE_VALUE,
  COLOR_SELECT_SATURATION_VALUE,
  COLOR_SELECT_RED_GREEN,
  COLOR_SELECT_RED_BLUE,
  COLOR_SELECT_GREEN_BLUE
} ColorSelectFillType;

typedef enum
{
  UPDATE_VALUES     = 1 << 0,
  UPDATE_POS        = 1 << 1,
  UPDATE_XY_COLOR   = 1 << 2,
  UPDATE_Z_COLOR    = 1 << 3,
  UPDATE_CALLER     = 1 << 6
} ColorSelectUpdateType;


#define GIMP_COLOR_SELECT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GIMP_TYPE_COLOR_SELECT, GimpColorSelectClass))
#define GIMP_IS_COLOR_SELECT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GIMP_TYPE_COLOR_SELECT))
#define GIMP_COLOR_SELECT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GIMP_TYPE_COLOR_SELECT, GimpColorSelectClass))


typedef struct _GimpColorSelectClass GimpColorSelectClass;

struct _GimpColorSelect
{
  GimpColorSelector    parent_instance;

  GtkWidget           *toggle_box;

  GtkWidget           *xy_color;
  GtkWidget           *z_color;

  gint                 pos[3];

  ColorSelectFillType  z_color_fill;
  ColorSelectFillType  xy_color_fill;
  GdkGC               *gc;
};

struct _GimpColorSelectClass
{
  GimpColorSelectorClass  parent_class;
};


typedef struct _ColorSelectFill ColorSelectFill;

typedef void (* ColorSelectFillUpdateProc) (ColorSelectFill *color_select_fill);

struct _ColorSelectFill
{
  guchar  *buffer;
  gint     y;
  gint     width;
  gint     height;
  GimpRGB  rgb;
  GimpHSV  hsv;

  ColorSelectFillUpdateProc update;
};


static void   gimp_color_select_class_init    (GimpColorSelectClass *klass);
static void   gimp_color_select_init          (GimpColorSelect      *select);

static void   gimp_color_select_finalize        (GObject            *object);

static void   gimp_color_select_togg_visible    (GimpColorSelector  *selector,
                                                 gboolean            visible);
static void   gimp_color_select_togg_sensitive  (GimpColorSelector  *selector,
                                                 gboolean            sensitive);
static void   gimp_color_select_set_color       (GimpColorSelector  *selector,
                                                 const GimpRGB      *rgb,
                                                 const GimpHSV      *hsv);
static void   gimp_color_select_set_channel     (GimpColorSelector  *selector,
                                                 GimpColorSelectorChannel  channel);

static void   gimp_color_select_channel_toggled (GtkWidget          *widget,
                                                 GimpColorSelect    *select);

static void   gimp_color_select_update          (GimpColorSelect    *select,
                                                 ColorSelectUpdateType  type);
static void   gimp_color_select_update_values   (GimpColorSelect    *select);
static void   gimp_color_select_update_pos      (GimpColorSelect    *select);

#if 0
static void   gimp_color_select_drop_color      (GtkWidget          *widget,
                                                 const GimpRGB      *color,
                                                 gpointer            data);
#endif

static gboolean   gimp_color_select_xy_expose   (GtkWidget          *widget,
                                                 GdkEventExpose     *eevent,
                                                 GimpColorSelect    *select);
static gboolean   gimp_color_select_xy_events   (GtkWidget          *widget,
                                                 GdkEvent           *event,
                                                 GimpColorSelect    *select);
static gboolean   gimp_color_select_z_expose    (GtkWidget          *widget,
                                                 GdkEventExpose     *eevent,
                                                 GimpColorSelect    *select);
static gboolean   gimp_color_select_z_events    (GtkWidget          *widet,
                                                 GdkEvent           *event,
                                                 GimpColorSelect    *select);

static void   gimp_color_select_image_fill      (GtkWidget          *widget,
                                                 ColorSelectFillType fill_type,
                                                 const GimpHSV      *hsv,
                                                 const GimpRGB      *rgb);

static void   gimp_color_select_draw_z_marker   (GimpColorSelect    *select,
                                                 GdkRectangle       *clip);
static void   gimp_color_select_draw_xy_marker  (GimpColorSelect    *select,
                                                 GdkRectangle       *clip);

static void   color_select_update_red              (ColorSelectFill *csf);
static void   color_select_update_green            (ColorSelectFill *csf);
static void   color_select_update_blue             (ColorSelectFill *csf);
static void   color_select_update_hue              (ColorSelectFill *csf);
static void   color_select_update_saturation       (ColorSelectFill *csf);
static void   color_select_update_value            (ColorSelectFill *csf);
static void   color_select_update_red_green        (ColorSelectFill *csf);
static void   color_select_update_red_blue         (ColorSelectFill *csf);
static void   color_select_update_green_blue       (ColorSelectFill *csf);
static void   color_select_update_hue_saturation   (ColorSelectFill *csf);
static void   color_select_update_hue_value        (ColorSelectFill *csf);
static void   color_select_update_saturation_value (ColorSelectFill *csf);


/*  static variables  */

static GimpColorSelectorClass *parent_class = NULL;

static ColorSelectFillUpdateProc update_procs[] =
{
  color_select_update_hue,
  color_select_update_saturation,
  color_select_update_value,
  color_select_update_red,
  color_select_update_green,
  color_select_update_blue,
  NULL, /* alpha */
  color_select_update_hue_saturation,
  color_select_update_hue_value,
  color_select_update_saturation_value,
  color_select_update_red_green,
  color_select_update_red_blue,
  color_select_update_green_blue,
};


GType
gimp_color_select_get_type (void)
{
  static GType select_type = 0;

  if (! select_type)
    {
      static const GTypeInfo select_info =
      {
        sizeof (GimpColorSelectClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gimp_color_select_class_init,
	NULL,           /* class_finalize */
	NULL,           /* class_data     */
	sizeof (GimpColorSelect),
	0,              /* n_preallocs    */
	(GInstanceInitFunc) gimp_color_select_init,
      };

      select_type = g_type_register_static (GIMP_TYPE_COLOR_SELECTOR,
                                            "GimpColorSelect",
                                            &select_info, 0);
    }

  return select_type;
}

static void
gimp_color_select_class_init (GimpColorSelectClass *klass)
{
  GObjectClass           *object_class;
  GimpColorSelectorClass *selector_class;

  object_class   = G_OBJECT_CLASS (klass);
  selector_class = GIMP_COLOR_SELECTOR_CLASS (klass);

  parent_class = g_type_class_peek_parent (klass);

  object_class->finalize      = gimp_color_select_finalize;

  selector_class->name                  = "GIMP";
  selector_class->help_page             = "built_in.html";
  selector_class->stock_id              = GIMP_STOCK_WILBER;
  selector_class->set_toggles_visible   = gimp_color_select_togg_visible;
  selector_class->set_toggles_sensitive = gimp_color_select_togg_sensitive;
  selector_class->set_color             = gimp_color_select_set_color;
  selector_class->set_channel           = gimp_color_select_set_channel;
}

static void
gimp_color_select_init (GimpColorSelect *select)
{
  GtkWidget *main_hbox;
  GtkWidget *hbox;
  GtkWidget *hbox2;
  GtkWidget *xy_frame;
  GtkWidget *z_frame;

  select->z_color_fill  = COLOR_SELECT_HUE;
  select->xy_color_fill = COLOR_SELECT_SATURATION_VALUE;
  select->gc            = NULL;

  main_hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (select), main_hbox, TRUE, FALSE, 0);
  gtk_widget_show (main_hbox);

  hbox = gtk_hbox_new (FALSE, 4);
  gtk_box_pack_start (GTK_BOX (main_hbox), hbox, TRUE, FALSE, 0);
  gtk_widget_show (hbox);

  /*  The x/y component preview  */
  xy_frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (xy_frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX (hbox), xy_frame, FALSE, FALSE, 0);
  gtk_widget_show (xy_frame);

  select->xy_color = gtk_preview_new (GTK_PREVIEW_COLOR);
  gtk_preview_set_dither (GTK_PREVIEW (select->xy_color), GDK_RGB_DITHER_MAX);
  gtk_preview_size (GTK_PREVIEW (select->xy_color),
                    XY_DEF_WIDTH, XY_DEF_HEIGHT);
  gtk_widget_set_events (select->xy_color, COLOR_AREA_EVENT_MASK);
  gtk_container_add (GTK_CONTAINER (xy_frame), select->xy_color);
  gtk_widget_show (select->xy_color);

  g_signal_connect_after (select->xy_color, "expose_event",
			  G_CALLBACK (gimp_color_select_xy_expose),
			  select);
  g_signal_connect (select->xy_color, "event",
		    G_CALLBACK (gimp_color_select_xy_events),
		    select);

#if 0
  gimp_dnd_color_dest_add (select->xy_color, gimp_color_select_drop_color,
                           select);
#endif

  hbox2 = gtk_hbox_new (FALSE, 2);
  gtk_box_pack_start (GTK_BOX (hbox), hbox2, TRUE, FALSE, 0);
  gtk_widget_show (hbox2);

  /*  The z component preview  */
  z_frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (z_frame), GTK_SHADOW_IN);
  gtk_box_pack_start (GTK_BOX (hbox2), z_frame, FALSE, FALSE, 0);
  gtk_widget_show (z_frame);

  select->z_color = gtk_preview_new (GTK_PREVIEW_COLOR);
  gtk_preview_set_dither (GTK_PREVIEW (select->z_color), GDK_RGB_DITHER_MAX);
  gtk_preview_size (GTK_PREVIEW (select->z_color), Z_DEF_WIDTH, Z_DEF_HEIGHT);
  gtk_widget_set_events (select->z_color, COLOR_AREA_EVENT_MASK);
  gtk_container_add (GTK_CONTAINER (z_frame), select->z_color);
  gtk_widget_show (select->z_color);

  g_signal_connect_after (select->z_color, "expose_event",
			  G_CALLBACK (gimp_color_select_z_expose),
			  select);
  g_signal_connect (select->z_color, "event",
		    G_CALLBACK (gimp_color_select_z_events),
		    select);

  select->toggle_box = gtk_vbox_new (FALSE, 2);
  gtk_box_pack_start (GTK_BOX (hbox2), select->toggle_box, FALSE, FALSE, 0);
  gtk_widget_show (select->toggle_box);

  /*  channel toggles  */
  {
    GimpColorSelectorChannel  channel;
    GSList                   *group = NULL;

    static const gchar *labels[] =
    {
      N_("_H"), N_("_S"), N_("_V"), N_("_R"), N_("_G"), N_("_B")
    };
    static const gchar *tips[7] =
    {
      N_("Hue"),
      N_("Saturation"),
      N_("Value"),
      N_("Red"),
      N_("Green"),
      N_("Blue"),
      N_("Alpha")
    };

    for (channel = GIMP_COLOR_SELECTOR_HUE;
         channel < GIMP_COLOR_SELECTOR_ALPHA;
         channel++)
      {
        GtkWidget *button;

        button = gtk_radio_button_new_with_mnemonic (group,
                                                     gettext (labels[channel]));
        group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));
        gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON (button), FALSE);
        gtk_box_pack_start (GTK_BOX (select->toggle_box), button,
                            TRUE, TRUE, 0);
        gtk_widget_show (button);

        g_object_set_data (G_OBJECT (button), "channel",
                           GINT_TO_POINTER (channel));

        if (channel == GIMP_COLOR_SELECTOR_HUE)
          gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);

        gimp_help_set_help_data (button, gettext (tips[channel]), NULL);

        g_signal_connect (button, "toggled",
                          G_CALLBACK (gimp_color_select_channel_toggled),
                          select);
      }
  }
}

static void
gimp_color_select_finalize (GObject *object)
{
  GimpColorSelect *select;

  select = GIMP_COLOR_SELECT (object);

  if (select->gc)
    {
      g_object_unref (select->gc);
      select->gc = NULL;
    }

  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gimp_color_select_togg_visible (GimpColorSelector *selector,
                                gboolean           visible)
{
  GimpColorSelect *select;

  select = GIMP_COLOR_SELECT (selector);

  if (visible)
    gtk_widget_show (select->toggle_box);
  else
    gtk_widget_hide (select->toggle_box);
}

static void
gimp_color_select_togg_sensitive (GimpColorSelector *selector,
                                  gboolean           sensitive)
{
  GimpColorSelect *select;

  select = GIMP_COLOR_SELECT (selector);

  gtk_widget_set_sensitive (select->toggle_box, sensitive);
}

static void
gimp_color_select_set_color (GimpColorSelector *selector,
                             const GimpRGB     *rgb,
                             const GimpHSV     *hsv)
{
  GimpColorSelect *select;

  select = GIMP_COLOR_SELECT (selector);

  gimp_color_select_update (select,
                            UPDATE_POS | UPDATE_XY_COLOR | UPDATE_Z_COLOR);
}

static void
gimp_color_select_set_channel (GimpColorSelector        *selector,
                               GimpColorSelectorChannel  channel)
{
  GimpColorSelect *select;

  select = GIMP_COLOR_SELECT (selector);

  switch ((ColorSelectFillType) channel)
    {
    case COLOR_SELECT_HUE:
      select->z_color_fill  = COLOR_SELECT_HUE;
      select->xy_color_fill = COLOR_SELECT_SATURATION_VALUE;
      break;

    case COLOR_SELECT_SATURATION:
      select->z_color_fill  = COLOR_SELECT_SATURATION;
      select->xy_color_fill = COLOR_SELECT_HUE_VALUE;
      break;

    case COLOR_SELECT_VALUE:
      select->z_color_fill  = COLOR_SELECT_VALUE;
      select->xy_color_fill = COLOR_SELECT_HUE_SATURATION;
      break;

    case COLOR_SELECT_RED:
      select->z_color_fill  = COLOR_SELECT_RED;
      select->xy_color_fill = COLOR_SELECT_GREEN_BLUE;
      break;

    case COLOR_SELECT_GREEN:
      select->z_color_fill  = COLOR_SELECT_GREEN;
      select->xy_color_fill = COLOR_SELECT_RED_BLUE;
      break;

    case COLOR_SELECT_BLUE:
      select->z_color_fill  = COLOR_SELECT_BLUE;
      select->xy_color_fill = COLOR_SELECT_RED_GREEN;
      break;

    default:
      break;
    }

  gimp_color_select_update (select,
                            UPDATE_POS | UPDATE_Z_COLOR | UPDATE_XY_COLOR);
}

static void
gimp_color_select_channel_toggled (GtkWidget       *widget,
                                   GimpColorSelect *select)
{
  if (GTK_TOGGLE_BUTTON (widget)->active)
    {
      GimpColorSelector        *selector;
      GimpColorSelectorChannel  channel;

      selector = GIMP_COLOR_SELECTOR (select);

      channel = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (widget),
                                                    "channel"));

      selector->channel = channel;
      gimp_color_select_set_channel (selector, channel);

      gimp_color_selector_channel_changed (selector);
    }
}

static void
gimp_color_select_update (GimpColorSelect       *select,
                          ColorSelectUpdateType  update)
{
  GimpColorSelector *selector;

  selector = GIMP_COLOR_SELECTOR (select);

  if (update & UPDATE_POS)
    gimp_color_select_update_pos (select);

  if (update & UPDATE_VALUES)
    gimp_color_select_update_values (select);

  if (update & UPDATE_XY_COLOR)
    {
      gimp_color_select_image_fill (select->xy_color, select->xy_color_fill,
                                    &selector->hsv, &selector->rgb);
      gtk_widget_queue_draw (select->xy_color);
    }

  if (update & UPDATE_Z_COLOR)
    {
      gimp_color_select_image_fill (select->z_color, select->z_color_fill,
                                    &selector->hsv, &selector->rgb);
      gtk_widget_queue_draw (select->z_color);
    }

  if (update & UPDATE_CALLER)
    gimp_color_selector_color_changed (GIMP_COLOR_SELECTOR (select));
}

static void
gimp_color_select_update_values (GimpColorSelect *select)
{
  GimpColorSelector *selector;

  selector = GIMP_COLOR_SELECTOR (select);

  switch (select->z_color_fill)
    {
    case COLOR_SELECT_RED:
      selector->rgb.b = select->pos[0] / 255.0;
      selector->rgb.g = select->pos[1] / 255.0;
      selector->rgb.r = select->pos[2] / 255.0;
      break;
    case COLOR_SELECT_GREEN:
      selector->rgb.b = select->pos[0] / 255.0;
      selector->rgb.r = select->pos[1] / 255.0;
      selector->rgb.g = select->pos[2] / 255.0;
      break;
    case COLOR_SELECT_BLUE:
      selector->rgb.g = select->pos[0] / 255.0;
      selector->rgb.r = select->pos[1] / 255.0;
      selector->rgb.b = select->pos[2] / 255.0;
      break;

    case COLOR_SELECT_HUE:
      selector->hsv.v = select->pos[0] / 255.0;
      selector->hsv.s = select->pos[1] / 255.0;
      selector->hsv.h = select->pos[2] / 255.0;
      break;
    case COLOR_SELECT_SATURATION:
      selector->hsv.v = select->pos[0] / 255.0;
      selector->hsv.h = select->pos[1] / 255.0;
      selector->hsv.s = select->pos[2] / 255.0;
      break;
    case COLOR_SELECT_VALUE:
      selector->hsv.s = select->pos[0] / 255.0;
      selector->hsv.h = select->pos[1] / 255.0;
      selector->hsv.v = select->pos[2] / 255.0;
      break;

    default:
      break;
    }

  switch (select->z_color_fill)
    {
    case COLOR_SELECT_RED:
    case COLOR_SELECT_GREEN:
    case COLOR_SELECT_BLUE:
      gimp_rgb_to_hsv (&selector->rgb, &selector->hsv);
      break;

    case COLOR_SELECT_HUE:
    case COLOR_SELECT_SATURATION:
    case COLOR_SELECT_VALUE:
      gimp_hsv_to_rgb (&selector->hsv, &selector->rgb);
      break;

    default:
      break;
    }
}

static void
gimp_color_select_update_pos (GimpColorSelect *select)
{
  GimpColorSelector *selector;

  selector = GIMP_COLOR_SELECTOR (select);

  switch (select->z_color_fill)
    {
    case COLOR_SELECT_RED:
      select->pos[0] = ROUND (selector->rgb.b * 255.0);
      select->pos[1] = ROUND (selector->rgb.g * 255.0);
      select->pos[2] = ROUND (selector->rgb.r * 255.0);
      break;
    case COLOR_SELECT_GREEN:
      select->pos[0] = ROUND (selector->rgb.b * 255.0);
      select->pos[1] = ROUND (selector->rgb.r * 255.0);
      select->pos[2] = ROUND (selector->rgb.g * 255.0);
      break;
    case COLOR_SELECT_BLUE:
      select->pos[0] = ROUND (selector->rgb.g * 255.0);
      select->pos[1] = ROUND (selector->rgb.r * 255.0);
      select->pos[2] = ROUND (selector->rgb.b * 255.0);
      break;

    case COLOR_SELECT_HUE:
      select->pos[0] = ROUND (selector->hsv.v * 255.0);
      select->pos[1] = ROUND (selector->hsv.s * 255.0);
      select->pos[2] = ROUND (selector->hsv.h * 255.0);
      break;
    case COLOR_SELECT_SATURATION:
      select->pos[0] = ROUND (selector->hsv.v * 255.0);
      select->pos[1] = ROUND (selector->hsv.h * 255.0);
      select->pos[2] = ROUND (selector->hsv.s * 255.0);
      break;
    case COLOR_SELECT_VALUE:
      select->pos[0] = ROUND (selector->hsv.s * 255.0);
      select->pos[1] = ROUND (selector->hsv.h * 255.0);
      select->pos[2] = ROUND (selector->hsv.v * 255.0);
      break;

    default:
      break;
    }
}

#if 0
static void
gimp_color_select_drop_color (GtkWidget     *widget,
                              const GimpRGB *color,
                              gpointer       data)
{
  GimpColorSelect *select;

  select = GIMP_COLOR_SELECT (data);

  select->rgb = *color;

  gimp_color_select_update_hsv_values (select);

  gimp_color_select_update (select,
                            UPDATE_POS | UPDATE_XY_COLOR | UPDATE_Z_COLOR |
                            UPDATE_CALLER);
}
#endif

static gboolean
gimp_color_select_xy_expose (GtkWidget       *widget,
                             GdkEventExpose  *event,
                             GimpColorSelect *select)
{
  if (! select->gc)
    select->gc = gdk_gc_new (widget->window);

  gimp_color_select_draw_xy_marker (select, &event->area);

  return TRUE;
}

static gboolean
gimp_color_select_xy_events (GtkWidget       *widget,
                             GdkEvent        *event,
                             GimpColorSelect *select)
{
  GdkEventButton *bevent;
  GdkEventMotion *mevent;
  gint            x, y;

  switch (event->type)
    {
    case GDK_BUTTON_PRESS:
      bevent = (GdkEventButton *) event;
      x = bevent->x;
      y = bevent->y;

      gdk_pointer_grab (select->xy_color->window, FALSE,
			GDK_POINTER_MOTION_HINT_MASK |
			GDK_BUTTON_MOTION_MASK       |
			GDK_BUTTON_RELEASE_MASK,
			NULL, NULL, bevent->time);
      break;

    case GDK_BUTTON_RELEASE:
      bevent = (GdkEventButton *) event;
      x = bevent->x;
      y = bevent->y;

      gdk_display_pointer_ungrab (gtk_widget_get_display (widget),
                                  bevent->time);
      break;

    case GDK_MOTION_NOTIFY:
      mevent = (GdkEventMotion *) event;
      if (mevent->is_hint)
	{
	  gdk_window_get_pointer (widget->window, &x, &y, NULL);
	}
      else
        {
          x = mevent->x;
          y = mevent->y;
        }
      break;

    default:
      return FALSE;
    }

  gimp_color_select_draw_xy_marker (select, NULL);

  select->pos[0] = (x * 255) / (XY_DEF_WIDTH - 1);
  select->pos[1] = 255 - (y * 255) / (XY_DEF_HEIGHT - 1);

  select->pos[0] = CLAMP (select->pos[0], 0, 255);
  select->pos[1] = CLAMP (select->pos[1], 0, 255);

  gimp_color_select_draw_xy_marker (select, NULL);
  gimp_color_select_update (select, UPDATE_VALUES | UPDATE_CALLER);

  return TRUE;
}

static gboolean
gimp_color_select_z_expose (GtkWidget       *widget,
                            GdkEventExpose  *event,
                            GimpColorSelect *select)
{
  if (! select->gc)
    select->gc = gdk_gc_new (widget->window);

  gimp_color_select_draw_z_marker (select, &event->area);

  return TRUE;
}

static gboolean
gimp_color_select_z_events (GtkWidget       *widget,
                            GdkEvent        *event,
                            GimpColorSelect *select)
{
  GdkEventButton *bevent;
  GdkEventMotion *mevent;
  gint            z;

  switch (event->type)
    {
    case GDK_BUTTON_PRESS:
      bevent = (GdkEventButton *) event;
      z = bevent->y;

      gdk_pointer_grab (select->z_color->window, FALSE,
			GDK_POINTER_MOTION_HINT_MASK |
			GDK_BUTTON_MOTION_MASK       |
			GDK_BUTTON_RELEASE_MASK,
			NULL, NULL, bevent->time);
      break;

    case GDK_BUTTON_RELEASE:
      bevent = (GdkEventButton *) event;
      z = bevent->y;

      gdk_display_pointer_ungrab (gtk_widget_get_display (widget),
                                  bevent->time);
      break;

    case GDK_MOTION_NOTIFY:
      mevent = (GdkEventMotion *) event;
      if (mevent->is_hint)
	{
	  gdk_window_get_pointer (widget->window, NULL, &z, NULL);
	}
      else
        {
          z = mevent->y;
        }
      break;

    default:
      return FALSE;
    }

  gimp_color_select_draw_z_marker (select, NULL);

  select->pos[2] = 255 - (z * 255) / (Z_DEF_HEIGHT - 1);
  select->pos[2] = CLAMP (select->pos[2], 0, 255);

  gimp_color_select_draw_z_marker (select, NULL);
  gimp_color_select_update (select,
                            UPDATE_VALUES | UPDATE_XY_COLOR | UPDATE_CALLER);

  return TRUE;
}

static void
gimp_color_select_image_fill (GtkWidget           *preview,
                              ColorSelectFillType  fill_type,
                              const GimpHSV       *hsv,
                              const GimpRGB       *rgb)
{
  ColorSelectFill csf;
  gint            height;

  csf.buffer = g_malloc (preview->requisition.width * 3);

  csf.update = update_procs[fill_type];

  csf.y      = -1;
  csf.width  = preview->requisition.width;
  csf.height = preview->requisition.height;
  csf.hsv    = *hsv;
  csf.rgb    = *rgb;

  height = csf.height;
  if (height > 0)
    while (height--)
      {
	if (csf.update)
	  (* csf.update) (&csf);

	gtk_preview_draw_row (GTK_PREVIEW (preview),
			      csf.buffer, 0, csf.y, csf.width);
      }

  g_free (csf.buffer);
}

static void
gimp_color_select_draw_z_marker (GimpColorSelect *select,
                                 GdkRectangle    *clip)
{
  gint width;
  gint height;
  gint y;
  gint minx;
  gint miny;

  if (! select->gc)
    return;

  y = (Z_DEF_HEIGHT - 1) - ((Z_DEF_HEIGHT - 1) * select->pos[2]) / 255;
  width  = select->z_color->requisition.width;
  height = select->z_color->requisition.height;
  minx = 0;
  miny = 0;
  if (width <= 0)
    return;

  if (clip)
    {
      width  = MIN (width,  clip->x + clip->width);
      height = MIN (height, clip->y + clip->height);
      minx   = MAX (0, clip->x);
      miny   = MAX (0, clip->y);
    }

  if (y >= miny && y < height)
    {
      gdk_gc_set_function (select->gc, GDK_INVERT);
      gdk_draw_line (select->z_color->window,
                     select->gc, minx, y, width - 1, y);
      gdk_gc_set_function (select->gc, GDK_COPY);
    }
}

static void
gimp_color_select_draw_xy_marker (GimpColorSelect *select,
                                  GdkRectangle    *clip)
{
  gint width;
  gint height;
  gint x, y;
  gint minx, miny;

  if (! select->gc)
    return;

  x = ((XY_DEF_WIDTH - 1) * select->pos[0]) / 255;
  y = (XY_DEF_HEIGHT - 1) - ((XY_DEF_HEIGHT - 1) * select->pos[1]) / 255;
  width = select->xy_color->requisition.width;
  height = select->xy_color->requisition.height;
  minx = 0;
  miny = 0;
  if ((width <= 0) || (height <= 0))
    return;

  gdk_gc_set_function (select->gc, GDK_INVERT);

  if (clip)
    {
      width  = MIN (width,  clip->x + clip->width);
      height = MIN (height, clip->y + clip->height);
      minx   = MAX (0, clip->x);
      miny   = MAX (0, clip->y);
    }

  if (y >= miny && y < height)
    gdk_draw_line (select->xy_color->window,
                   select->gc, minx, y, width - 1, y);

  if (x >= minx && x < width)
    gdk_draw_line (select->xy_color->window,
                   select->gc, x, miny, x, height - 1);

  gdk_gc_set_function (select->gc, GDK_COPY);
}

static void
color_select_update_red (ColorSelectFill *csf)
{
  guchar *p;
  gint    i, r;

  p = csf->buffer;

  csf->y += 1;
  r = (csf->height - csf->y + 1) * 255 / csf->height;

  if (r < 0)
    r = 0;
  if (r > 255)
    r = 255;

  for (i = 0; i < csf->width; i++)
    {
      *p++ = r;
      *p++ = 0;
      *p++ = 0;
    }
}

static void
color_select_update_green (ColorSelectFill *csf)
{
  guchar *p;
  gint    i, g;

  p = csf->buffer;

  csf->y += 1;
  g = (csf->height - csf->y + 1) * 255 / csf->height;

  if (g < 0)
    g = 0;
  if (g > 255)
    g = 255;

  for (i = 0; i < csf->width; i++)
    {
      *p++ = 0;
      *p++ = g;
      *p++ = 0;
    }
}

static void
color_select_update_blue (ColorSelectFill *csf)
{
  guchar *p;
  gint    i, b;

  p = csf->buffer;

  csf->y += 1;
  b = (csf->height - csf->y + 1) * 255 / csf->height;

  if (b < 0)
    b = 0;
  if (b > 255)
    b = 255;

  for (i = 0; i < csf->width; i++)
    {
      *p++ = 0;
      *p++ = 0;
      *p++ = b;
    }
}

static void
color_select_update_hue (ColorSelectFill *csf)
{
  guchar *p;
  gfloat  h, f;
  gint    r, g, b;
  gint    i;

  p = csf->buffer;

  csf->y += 1;
  h = csf->y * 360.0 / csf->height;

  h = CLAMP (360 - h, 0, 360);

  h /= 60;
  f = (h - (int) h) * 255;

  r = g = b = 0;

  switch ((int) h)
    {
    case 0:
      r = 255;
      g = f;
      b = 0;
      break;
    case 1:
      r = 255 - f;
      g = 255;
      b = 0;
      break;
    case 2:
      r = 0;
      g = 255;
      b = f;
      break;
    case 3:
      r = 0;
      g = 255 - f;
      b = 255;
      break;
    case 4:
      r = f;
      g = 0;
      b = 255;
      break;
    case 5:
      r = 255;
      g = 0;
      b = 255 - f;
      break;
    }

  for (i = 0; i < csf->width; i++)
    {
      *p++ = r;
      *p++ = g;
      *p++ = b;
    }
}

static void
color_select_update_saturation (ColorSelectFill *csf)
{
  guchar *p;
  gint    s;
  gint    i;

  p = csf->buffer;

  csf->y += 1;
  s = csf->y * 255 / csf->height;

  if (s < 0)
    s = 0;
  if (s > 255)
    s = 255;

  s = 255 - s;

  for (i = 0; i < csf->width; i++)
    {
      *p++ = s;
      *p++ = s;
      *p++ = s;
    }
}

static void
color_select_update_value (ColorSelectFill *csf)
{
  guchar *p;
  gint    v;
  gint    i;

  p = csf->buffer;

  csf->y += 1;
  v = csf->y * 255 / csf->height;

  if (v < 0)
    v = 0;
  if (v > 255)
    v = 255;

  v = 255 - v;

  for (i = 0; i < csf->width; i++)
    {
      *p++ = v;
      *p++ = v;
      *p++ = v;
    }
}

static void
color_select_update_red_green (ColorSelectFill *csf)
{
  guchar *p;
  gint    i, r, b;
  gfloat  g, dg;

  p = csf->buffer;

  csf->y += 1;
  b = ROUND (csf->rgb.b * 255.0);
  r = (csf->height - csf->y + 1) * 255 / csf->height;

  if (r < 0)
    r = 0;
  if (r > 255)
    r = 255;

  g = 0;
  dg = 255.0 / csf->width;

  for (i = 0; i < csf->width; i++)
    {
      *p++ = r;
      *p++ = g;
      *p++ = b;

      g += dg;
    }
}

static void
color_select_update_red_blue (ColorSelectFill *csf)
{
  guchar *p;
  gint    i, r, g;
  gfloat  b, db;

  p = csf->buffer;

  csf->y += 1;
  g = ROUND (csf->rgb.g * 255.0);
  r = (csf->height - csf->y + 1) * 255 / csf->height;

  if (r < 0)
    r = 0;
  if (r > 255)
    r = 255;

  b = 0;
  db = 255.0 / csf->width;

  for (i = 0; i < csf->width; i++)
    {
      *p++ = r;
      *p++ = g;
      *p++ = b;

      b += db;
    }
}

static void
color_select_update_green_blue (ColorSelectFill *csf)
{
  guchar *p;
  gint    i, g, r;
  gfloat  b, db;

  p = csf->buffer;

  csf->y += 1;
  r = ROUND (csf->rgb.r * 255.0);
  g = (csf->height - csf->y + 1) * 255 / csf->height;

  if (g < 0)
    g = 0;
  if (g > 255)
    g = 255;

  b = 0;
  db = 255.0 / csf->width;

  for (i = 0; i < csf->width; i++)
    {
      *p++ = r;
      *p++ = g;
      *p++ = b;

      b += db;
    }
}

static void
color_select_update_hue_saturation (ColorSelectFill *csf)
{
  guchar *p;
  gfloat  h, v, s, ds;
  gint    f;
  gint    i;

  p = csf->buffer;

  csf->y += 1;
  h = 360 - (csf->y * 360 / csf->height);

  if (h < 0)
    h = 0;
  if (h > 359)
    h = 359;

  h /= 60;
  f = (h - (int) h) * 255;

  s = 0;
  ds = 1.0 / csf->width;

  v = csf->hsv.v;

  switch ((int) h)
    {
    case 0:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255;
	  *p++ = v * (255 - (s * (255 - f)));
	  *p++ = v * 255 * (1 - s);

	  s += ds;
	}
      break;
    case 1:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * (255 - s * f);
	  *p++ = v * 255;
	  *p++ = v * 255 * (1 - s);

	  s += ds;
	}
      break;
    case 2:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255 * (1 - s);
	  *p++ = v *255;
	  *p++ = v * (255 - (s * (255 - f)));

	  s += ds;
	}
      break;
    case 3:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255 * (1 - s);
	  *p++ = v * (255 - s * f);
	  *p++ = v * 255;

	  s += ds;
	}
      break;
    case 4:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * (255 - (s * (255 - f)));
	  *p++ = v * (255 * (1 - s));
	  *p++ = v * 255;

	  s += ds;
	}
      break;
    case 5:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255;
	  *p++ = v * 255 * (1 - s);
	  *p++ = v * (255 - s * f);

	  s += ds;
	}
      break;
    }
}

static void
color_select_update_hue_value (ColorSelectFill *csf)
{
  guchar *p;
  gfloat  h, v, dv, s;
  gint    f;
  gint    i;

  p = csf->buffer;

  csf->y += 1;
  h = 360 - (csf->y * 360 / csf->height);

  if (h < 0)
    h = 0;
  if (h > 359)
    h = 359;

  h /= 60;
  f = (h - (int) h) * 255;

  v = 0;
  dv = 1.0 / csf->width;

  s = csf->hsv.s;

  switch ((int) h)
    {
    case 0:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255;
	  *p++ = v * (255 - (s * (255 - f)));
	  *p++ = v * 255 * (1 - s);

	  v += dv;
	}
      break;
    case 1:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * (255 - s * f);
	  *p++ = v * 255;
	  *p++ = v * 255 * (1 - s);

	  v += dv;
	}
      break;
    case 2:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255 * (1 - s);
	  *p++ = v *255;
	  *p++ = v * (255 - (s * (255 - f)));

	  v += dv;
	}
      break;
    case 3:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255 * (1 - s);
	  *p++ = v * (255 - s * f);
	  *p++ = v * 255;

	  v += dv;
	}
      break;
    case 4:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * (255 - (s * (255 - f)));
	  *p++ = v * (255 * (1 - s));
	  *p++ = v * 255;

	  v += dv;
	}
      break;
    case 5:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255;
	  *p++ = v * 255 * (1 - s);
	  *p++ = v * (255 - s * f);

	  v += dv;
	}
      break;
    }
}

static void
color_select_update_saturation_value (ColorSelectFill *csf)
{
  guchar *p;
  gfloat  h, v, dv, s;
  gint    f;
  gint    i;

  p = csf->buffer;

  csf->y += 1;
  s = (gfloat) csf->y / csf->height;

  if (s < 0)
    s = 0;
  if (s > 1)
    s = 1;

  s = 1 - s;

  h = (gfloat) csf->hsv.h * 360.0;
  if (h >= 360)
    h -= 360;
  h /= 60;
  f = (h - (gint) h) * 255;

  v = 0;
  dv = 1.0 / csf->width;

  switch ((gint) h)
    {
    case 0:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255;
	  *p++ = v * (255 - (s * (255 - f)));
	  *p++ = v * 255 * (1 - s);

	  v += dv;
	}
      break;
    case 1:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * (255 - s * f);
	  *p++ = v * 255;
	  *p++ = v * 255 * (1 - s);

	  v += dv;
	}
      break;
    case 2:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255 * (1 - s);
	  *p++ = v *255;
	  *p++ = v * (255 - (s * (255 - f)));

	  v += dv;
	}
      break;
    case 3:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255 * (1 - s);
	  *p++ = v * (255 - s * f);
	  *p++ = v * 255;

	  v += dv;
	}
      break;
    case 4:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * (255 - (s * (255 - f)));
	  *p++ = v * (255 * (1 - s));
	  *p++ = v * 255;

	  v += dv;
	}
      break;
    case 5:
      for (i = 0; i < csf->width; i++)
	{
	  *p++ = v * 255;
	  *p++ = v * 255 * (1 - s);
	  *p++ = v * (255 - s * f);

	  v += dv;
	}
      break;
    }
}
