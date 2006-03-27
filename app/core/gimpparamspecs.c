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

#include "libgimpbase/gimpbase.h"

#include "core-types.h"

#include "gimp.h"
#include "gimpimage.h"
#include "gimpitem.h"
#include "gimpparamspecs.h"


/*
 * GIMP_TYPE_PARAM_STRING
 */

#define GIMP_PARAM_SPEC_STRING(pspec) (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_STRING, GimpParamSpecString))

static void       gimp_param_string_class_init  (GParamSpecClass *class);
static void       gimp_param_string_init        (GParamSpec      *pspec);
static gboolean   gimp_param_string_validate    (GParamSpec      *pspec,
                                                 GValue          *value);

typedef struct _GimpParamSpecString GimpParamSpecString;

struct _GimpParamSpecString
{
  GParamSpecString parent_instance;

  guint            no_validate : 1;
  guint            null_ok     : 1;
};

GType
gimp_param_string_get_type (void)
{
  static GType type = 0;

  if (! type)
    {
      static const GTypeInfo type_info =
      {
        sizeof (GParamSpecClass),
        NULL, NULL,
        (GClassInitFunc) gimp_param_string_class_init,
        NULL, NULL,
        sizeof (GimpParamSpecString),
        0,
        (GInstanceInitFunc) gimp_param_string_init
      };

      type = g_type_register_static (G_TYPE_PARAM_STRING,
                                     "GimpParamString",
                                     &type_info, 0);
    }

  return type;
}

static void
gimp_param_string_class_init (GParamSpecClass *class)
{
  class->value_type     = G_TYPE_STRING;
  class->value_validate = gimp_param_string_validate;
}

static void
gimp_param_string_init (GParamSpec *pspec)
{
  GimpParamSpecString *sspec = GIMP_PARAM_SPEC_STRING (pspec);

  sspec->no_validate = FALSE;
  sspec->null_ok     = FALSE;
}

static gboolean
gimp_param_string_validate (GParamSpec *pspec,
                            GValue     *value)
{
  GimpParamSpecString *sspec  = GIMP_PARAM_SPEC_STRING (pspec);
  gchar               *string = value->data[0].v_pointer;

  if (string)
    {
      gchar *s;

      if (! sspec->no_validate &&
          ! g_utf8_validate (string, -1, (const gchar **) &s))
        {
          for (; *s; s++)
            if (*s < ' ')
              *s = '?';

          return TRUE;
        }
    }
  else if (! sspec->null_ok)
    {
      value->data[0].v_pointer = g_strdup ("");
      return TRUE;
    }

  return FALSE;
}

GParamSpec *
gimp_param_spec_string (const gchar *name,
                        const gchar *nick,
                        const gchar *blurb,
                        const gchar *default_value,
                        gboolean     no_validate,
                        gboolean     null_ok,
                        GParamFlags  flags)
{
  GimpParamSpecString *sspec;

  sspec = g_param_spec_internal (GIMP_TYPE_PARAM_STRING,
                                 name, nick, blurb, flags);

  if (sspec)
    {
      g_free (G_PARAM_SPEC_STRING (sspec)->default_value);
      G_PARAM_SPEC_STRING (sspec)->default_value = g_strdup (default_value);

      sspec->no_validate = no_validate ? TRUE : FALSE;
      sspec->null_ok     = null_ok     ? TRUE : FALSE;
    }

  return G_PARAM_SPEC (sspec);
}


/*
 * GIMP_TYPE_PARAM_IMAGE_ID
 */

#define GIMP_PARAM_SPEC_IMAGE_ID(pspec) (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_IMAGE_ID, GimpParamSpecImageID))

static void       gimp_param_image_id_class_init  (GParamSpecClass *class);
static void       gimp_param_image_id_init        (GParamSpec      *pspec);
static void       gimp_param_image_id_set_default (GParamSpec      *pspec,
                                                   GValue          *value);
static gboolean   gimp_param_image_id_validate    (GParamSpec      *pspec,
                                                   GValue          *value);
static gint       gimp_param_image_id_values_cmp  (GParamSpec      *pspec,
                                                   const GValue    *value1,
                                                   const GValue    *value2);

typedef struct _GimpParamSpecImageID GimpParamSpecImageID;

struct _GimpParamSpecImageID
{
  GParamSpecInt  parent_instance;

  Gimp          *gimp;
};

GType
gimp_param_image_id_get_type (void)
{
  static GType type = 0;

  if (! type)
    {
      static const GTypeInfo type_info =
      {
        sizeof (GParamSpecClass),
        NULL, NULL,
        (GClassInitFunc) gimp_param_image_id_class_init,
        NULL, NULL,
        sizeof (GimpParamSpecImageID),
        0,
        (GInstanceInitFunc) gimp_param_image_id_init
      };

      type = g_type_register_static (G_TYPE_PARAM_INT,
                                     "GimpParamImageID",
                                     &type_info, 0);
    }

  return type;
}

static void
gimp_param_image_id_class_init (GParamSpecClass *class)
{
  class->value_type        = G_TYPE_INT;
  class->value_set_default = gimp_param_image_id_set_default;
  class->value_validate    = gimp_param_image_id_validate;
  class->values_cmp        = gimp_param_image_id_values_cmp;
}

static void
gimp_param_image_id_init (GParamSpec *pspec)
{
  GimpParamSpecImageID *ispec = GIMP_PARAM_SPEC_IMAGE_ID (pspec);

  ispec->gimp = NULL;
}

static void
gimp_param_image_id_set_default (GParamSpec *pspec,
                                 GValue     *value)
{
  value->data[0].v_int = -1;
}

static gboolean
gimp_param_image_id_validate (GParamSpec *pspec,
                              GValue     *value)
{
  GimpParamSpecImageID *ispec = GIMP_PARAM_SPEC_IMAGE_ID (pspec);
  gint                  image_id;
  GimpImage            *image;

  image_id = value->data[0].v_int;
  image    = gimp_image_get_by_ID (ispec->gimp, image_id);

  return GIMP_IS_IMAGE (image);
}

static gint
gimp_param_image_id_values_cmp (GParamSpec   *pspec,
                                const GValue *value1,
                                const GValue *value2)
{
  gint image_id1;
  gint image_id2;

  image_id1 = value1->data[0].v_int;
  image_id2 = value2->data[0].v_int;

  /*  try to return at least *something*, it's useless anyway...  */

  if (image_id1 < image_id2)
    return -1;
  else if (image_id1 > image_id2)
    return 1;
  else
    return 0;
}

GParamSpec *
gimp_param_spec_image_id (const gchar *name,
                          const gchar *nick,
                          const gchar *blurb,
                          Gimp        *gimp,
                          GParamFlags  flags)
{
  GimpParamSpecImageID *ispec;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);

  ispec = g_param_spec_internal (GIMP_TYPE_PARAM_IMAGE_ID,
                                 name, nick, blurb, flags);

  if (ispec)
    ispec->gimp = gimp;

  return G_PARAM_SPEC (ispec);
}

GimpImage *
gimp_value_get_image (const GValue *value,
                      Gimp         *gimp)
{
  g_return_val_if_fail (G_VALUE_HOLDS_INT (value), NULL);
  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);

  return gimp_image_get_by_ID (gimp, value->data[0].v_int);
}

void
gimp_value_set_image (GValue    *value,
                      GimpImage *image)
{
  g_return_if_fail (G_VALUE_HOLDS_INT (value));
  g_return_if_fail (image == NULL || GIMP_IS_IMAGE (image));

  value->data[0].v_int = image ? gimp_image_get_ID (image) : -1;
}


/*
 * GIMP_TYPE_PARAM_ITEM_ID
 */

#define GIMP_PARAM_SPEC_ITEM_ID(pspec) (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_ITEM_ID, GimpParamSpecItemID))

static void       gimp_param_item_id_class_init  (GParamSpecClass *class);
static void       gimp_param_item_id_init        (GParamSpec      *pspec);
static void       gimp_param_item_id_set_default (GParamSpec      *pspec,
                                                   GValue          *value);
static gboolean   gimp_param_item_id_validate    (GParamSpec      *pspec,
                                                   GValue          *value);
static gint       gimp_param_item_id_values_cmp  (GParamSpec      *pspec,
                                                   const GValue    *value1,
                                                   const GValue    *value2);

typedef struct _GimpParamSpecItemID GimpParamSpecItemID;

struct _GimpParamSpecItemID
{
  GParamSpecInt  parent_instance;

  Gimp          *gimp;
  GType          item_type;
};

GType
gimp_param_item_id_get_type (void)
{
  static GType type = 0;

  if (! type)
    {
      static const GTypeInfo type_info =
      {
        sizeof (GParamSpecClass),
        NULL, NULL,
        (GClassInitFunc) gimp_param_item_id_class_init,
        NULL, NULL,
        sizeof (GimpParamSpecItemID),
        0,
        (GInstanceInitFunc) gimp_param_item_id_init
      };

      type = g_type_register_static (G_TYPE_PARAM_INT,
                                     "GimpParamItemID",
                                     &type_info, 0);
    }

  return type;
}

static void
gimp_param_item_id_class_init (GParamSpecClass *class)
{
  class->value_type        = G_TYPE_INT;
  class->value_set_default = gimp_param_item_id_set_default;
  class->value_validate    = gimp_param_item_id_validate;
  class->values_cmp        = gimp_param_item_id_values_cmp;
}

static void
gimp_param_item_id_init (GParamSpec *pspec)
{
  GimpParamSpecItemID *ispec = GIMP_PARAM_SPEC_ITEM_ID (pspec);

  ispec->gimp      = NULL;
  ispec->item_type = G_TYPE_NONE;
}

static void
gimp_param_item_id_set_default (GParamSpec *pspec,
                                 GValue     *value)
{
  value->data[0].v_int = -1;
}

static gboolean
gimp_param_item_id_validate (GParamSpec *pspec,
                              GValue     *value)
{
  GimpParamSpecItemID *ispec = GIMP_PARAM_SPEC_ITEM_ID (pspec);
  gint                 item_id;
  GimpItem            *item;

  item_id = value->data[0].v_int;
  item    = gimp_item_get_by_ID (ispec->gimp, item_id);

  return g_type_is_a (G_TYPE_FROM_INSTANCE (item), ispec->item_type);
}

static gint
gimp_param_item_id_values_cmp (GParamSpec   *pspec,
                               const GValue *value1,
                               const GValue *value2)
{
  gint item_id1;
  gint item_id2;

  item_id1 = value1->data[0].v_int;
  item_id2 = value2->data[0].v_int;

  /*  try to return at least *something*, it's useless anyway...  */

  if (item_id1 < item_id2)
    return -1;
  else if (item_id1 > item_id2)
    return 1;
  else
    return 0;
}

GParamSpec *
gimp_param_spec_item_id (const gchar *name,
                         const gchar *nick,
                         const gchar *blurb,
                         Gimp        *gimp,
                         GType        item_type,
                         GParamFlags  flags)
{
  GimpParamSpecItemID *ispec;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);
  g_return_val_if_fail (g_type_is_a (item_type, GIMP_TYPE_ITEM), NULL);

  ispec = g_param_spec_internal (GIMP_TYPE_PARAM_ITEM_ID,
                                 name, nick, blurb, flags);

  if (ispec)
    {
      ispec->gimp      = gimp;
      ispec->item_type = item_type;
    }

  return G_PARAM_SPEC (ispec);
}

GimpItem *
gimp_value_get_item (const GValue *value,
                     Gimp         *gimp,
                     GType         item_type)
{
  GimpItem *item;

  g_return_val_if_fail (G_VALUE_HOLDS_INT (value), NULL);
  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);
  g_return_val_if_fail (g_type_is_a (item_type, GIMP_TYPE_ITEM), NULL);

  item = gimp_item_get_by_ID (gimp, value->data[0].v_int);

  if (item && ! g_type_is_a (G_TYPE_FROM_INSTANCE (item), item_type))
    return NULL;

  return item;
}

void
gimp_value_set_item (GValue   *value,
                     GimpItem *item)
{
  g_return_if_fail (G_VALUE_HOLDS_INT (value));
  g_return_if_fail (item == NULL || GIMP_IS_ITEM (item));

  value->data[0].v_int = item ? gimp_item_get_ID (item) : -1;
}


/*
 * GIMP_TYPE_PARAM_DISPLAY_ID
 */

#define GIMP_PARAM_SPEC_DISPLAY_ID(pspec) (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_DISPLAY_ID, GimpParamSpecDisplayID))

static void       gimp_param_display_id_class_init  (GParamSpecClass *class);
static void       gimp_param_display_id_init        (GParamSpec      *pspec);
static void       gimp_param_display_id_set_default (GParamSpec      *pspec,
                                                     GValue          *value);
static gboolean   gimp_param_display_id_validate    (GParamSpec      *pspec,
                                                     GValue          *value);
static gint       gimp_param_display_id_values_cmp  (GParamSpec      *pspec,
                                                     const GValue    *value1,
                                                     const GValue    *value2);

typedef struct _GimpParamSpecDisplayID GimpParamSpecDisplayID;

struct _GimpParamSpecDisplayID
{
  GParamSpecInt  parent_instance;

  Gimp          *gimp;
};

GType
gimp_param_display_id_get_type (void)
{
  static GType type = 0;

  if (! type)
    {
      static const GTypeInfo type_info =
      {
        sizeof (GParamSpecClass),
        NULL, NULL,
        (GClassInitFunc) gimp_param_display_id_class_init,
        NULL, NULL,
        sizeof (GimpParamSpecDisplayID),
        0,
        (GInstanceInitFunc) gimp_param_display_id_init
      };

      type = g_type_register_static (G_TYPE_PARAM_INT,
                                     "GimpParamDisplayID",
                                     &type_info, 0);
    }

  return type;
}

static void
gimp_param_display_id_class_init (GParamSpecClass *class)
{
  class->value_type        = G_TYPE_INT;
  class->value_set_default = gimp_param_display_id_set_default;
  class->value_validate    = gimp_param_display_id_validate;
  class->values_cmp        = gimp_param_display_id_values_cmp;
}

static void
gimp_param_display_id_init (GParamSpec *pspec)
{
  GimpParamSpecDisplayID *ispec = GIMP_PARAM_SPEC_DISPLAY_ID (pspec);

  ispec->gimp = NULL;
}

static void
gimp_param_display_id_set_default (GParamSpec *pspec,
                                   GValue     *value)
{
  value->data[0].v_int = -1;
}

static gboolean
gimp_param_display_id_validate (GParamSpec *pspec,
                                GValue     *value)
{
  GimpParamSpecDisplayID *ispec = GIMP_PARAM_SPEC_DISPLAY_ID (pspec);
  gint                    display_id;
  GimpObject             *display;

  display_id = value->data[0].v_int;
  display    = gimp_get_display_by_ID (ispec->gimp, display_id);

  return GIMP_IS_OBJECT (display);
}

static gint
gimp_param_display_id_values_cmp (GParamSpec   *pspec,
                                  const GValue *value1,
                                  const GValue *value2)
{
  gint display_id1;
  gint display_id2;

  display_id1 = value1->data[0].v_int;
  display_id2 = value2->data[0].v_int;

  /*  try to return at least *something*, it's useless anyway...  */

  if (display_id1 < display_id2)
    return -1;
  else if (display_id1 > display_id2)
    return 1;
  else
    return 0;
}

GParamSpec *
gimp_param_spec_display_id (const gchar *name,
                            const gchar *nick,
                            const gchar *blurb,
                            Gimp        *gimp,
                            GParamFlags  flags)
{
  GimpParamSpecDisplayID *ispec;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);

  ispec = g_param_spec_internal (GIMP_TYPE_PARAM_DISPLAY_ID,
                                 name, nick, blurb, flags);

  if (ispec)
    ispec->gimp = gimp;

  return G_PARAM_SPEC (ispec);
}

GimpObject *
gimp_value_get_display (const GValue *value,
                        Gimp         *gimp)
{
  g_return_val_if_fail (G_VALUE_HOLDS_INT (value), NULL);
  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);

  return gimp_get_display_by_ID (gimp, value->data[0].v_int);
}

void
gimp_value_set_display (GValue     *value,
                        GimpObject *display)
{
  gint id = -1;

  g_return_if_fail (G_VALUE_HOLDS_INT (value));
  g_return_if_fail (display == NULL || GIMP_IS_OBJECT (display));

  if (display)
    g_object_get (display, "id", &id, NULL);

  value->data[0].v_int = id;
}

/*
 * GIMP_TYPE_PARASITE
 */

GType
gimp_parasite_get_type (void)
{
  static GType type = 0;

  if (! type)
    type = g_boxed_type_register_static ("GimpParasite",
                                         (GBoxedCopyFunc) gimp_parasite_copy,
                                         (GBoxedFreeFunc) gimp_parasite_free);

  return type;
}


/*
 * GIMP_TYPE_PARAM_PARASITE
 */

#define GIMP_PARAM_SPEC_PARASITE(pspec) (G_TYPE_CHECK_INSTANCE_CAST ((pspec), GIMP_TYPE_PARAM_PARASITE, GimpParamSpecParasite))

static void       gimp_param_parasite_class_init  (GParamSpecClass *class);
static void       gimp_param_parasite_init        (GParamSpec      *pspec);
static void       gimp_param_parasite_set_default (GParamSpec      *pspec,
                                                   GValue          *value);
static gboolean   gimp_param_parasite_validate    (GParamSpec      *pspec,
                                                   GValue          *value);
static gint       gimp_param_parasite_values_cmp  (GParamSpec      *pspec,
                                                   const GValue    *value1,
                                                   const GValue    *value2);

typedef struct _GimpParamSpecParasite GimpParamSpecParasite;

struct _GimpParamSpecParasite
{
  GParamSpecBoxed parent_instance;
};

GType
gimp_param_parasite_get_type (void)
{
  static GType type = 0;

  if (! type)
    {
      static const GTypeInfo type_info =
      {
        sizeof (GParamSpecClass),
        NULL, NULL,
        (GClassInitFunc) gimp_param_parasite_class_init,
        NULL, NULL,
        sizeof (GimpParamSpecParasite),
        0,
        (GInstanceInitFunc) gimp_param_parasite_init
      };

      type = g_type_register_static (G_TYPE_PARAM_BOXED,
                                     "GimpParamParasite",
                                     &type_info, 0);
    }

  return type;
}

static void
gimp_param_parasite_class_init (GParamSpecClass *class)
{
  class->value_type        = GIMP_TYPE_PARASITE;
  class->value_set_default = gimp_param_parasite_set_default;
  class->value_validate    = gimp_param_parasite_validate;
  class->values_cmp        = gimp_param_parasite_values_cmp;
}

static void
gimp_param_parasite_init (GParamSpec *pspec)
{
}

static void
gimp_param_parasite_set_default (GParamSpec *pspec,
                                 GValue     *value)
{
}

static gboolean
gimp_param_parasite_validate (GParamSpec *pspec,
                              GValue     *value)
{
  GimpParasite *parasite;

  parasite = value->data[0].v_pointer;

  if (parasite)
    {
      return (parasite->name != NULL                     &&
              g_utf8_validate (parasite->name, -1, NULL) &&
              ((parasite->size == 0 && parasite->data == NULL) ||
               (parasite->size >  0 && parasite->data != NULL)));
    }

  return FALSE;
}

static gint
gimp_param_parasite_values_cmp (GParamSpec   *pspec,
                                const GValue *value1,
                                const GValue *value2)
{
  GimpParasite *parasite1;
  GimpParasite *parasite2;

  parasite1 = value1->data[0].v_pointer;
  parasite2 = value2->data[0].v_pointer;

  /*  try to return at least *something*, it's useless anyway...  */

  if (! parasite1)
    return parasite2 != NULL ? -1 : 0;
  else if (! parasite2)
    return parasite1 != NULL;
  else
    return gimp_parasite_compare (parasite1, parasite2);
}

GParamSpec *
gimp_param_spec_parasite (const gchar *name,
                          const gchar *nick,
                          const gchar *blurb,
                          GParamFlags  flags)
{
  GimpParamSpecParasite *parasite_spec;

  parasite_spec = g_param_spec_internal (GIMP_TYPE_PARAM_PARASITE,
                                         name, nick, blurb, flags);

  return G_PARAM_SPEC (parasite_spec);
}