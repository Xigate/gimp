/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-1997 Peter Mattis and Spencer Kimball
 *
 * gimpdrawable.c
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

#include "gimp.h"


#define TILE_WIDTH  gimp_tile_width()
#define TILE_HEIGHT gimp_tile_height()


GimpDrawable *
gimp_drawable_get (gint32 drawable_ID)
{
  GimpDrawable *drawable;
  gint          width;
  gint          height;
  gint          bpp;

  width  = gimp_drawable_width  (drawable_ID);
  height = gimp_drawable_height (drawable_ID);
  bpp    = gimp_drawable_bpp    (drawable_ID);

  g_return_val_if_fail (width > 0 && height > 0 && bpp > 0, NULL);

  drawable = g_new0 (GimpDrawable, 1);

  drawable->drawable_id  = drawable_ID;
  drawable->width        = width;
  drawable->height       = height;
  drawable->bpp          = bpp;
  drawable->ntile_rows   = (height + TILE_HEIGHT - 1) / TILE_HEIGHT;
  drawable->ntile_cols   = (width  + TILE_WIDTH  - 1) / TILE_WIDTH;

  return drawable;
}

void
gimp_drawable_detach (GimpDrawable *drawable)
{
  g_return_if_fail (drawable != NULL);

  gimp_drawable_flush (drawable);

  if (drawable->tiles)
    g_free (drawable->tiles);
  if (drawable->shadow_tiles)
    g_free (drawable->shadow_tiles);

  g_free (drawable);
}

void
gimp_drawable_flush (GimpDrawable *drawable)
{
  GimpTile *tiles;
  gint      n_tiles;
  gint      i;

  g_return_if_fail (drawable != NULL);

  if (drawable->tiles)
    {
      tiles   = drawable->tiles;
      n_tiles = drawable->ntile_rows * drawable->ntile_cols;

      for (i = 0; i < n_tiles; i++)
        if ((tiles[i].ref_count > 0) && tiles[i].dirty)
          gimp_tile_flush (&tiles[i]);
    }

  if (drawable->shadow_tiles)
    {
      tiles   = drawable->shadow_tiles;
      n_tiles = drawable->ntile_rows * drawable->ntile_cols;

      for (i = 0; i < n_tiles; i++)
        if ((tiles[i].ref_count > 0) && tiles[i].dirty)
          gimp_tile_flush (&tiles[i]);
    }
}

GimpTile *
gimp_drawable_get_tile (GimpDrawable *drawable,
			gboolean      shadow,
			gint          row,
			gint          col)
{
  GimpTile *tiles;
  guint     right_tile;
  guint     bottom_tile;
  gint      n_tiles;
  gint      tile_num;
  gint      i, j, k;

  g_return_val_if_fail (drawable != NULL, NULL);

  if (shadow)
    tiles = drawable->shadow_tiles;
  else
    tiles = drawable->tiles;

  if (! tiles)
    {
      n_tiles = drawable->ntile_rows * drawable->ntile_cols;
      tiles = g_new (GimpTile, n_tiles);

      right_tile  = drawable->width  - ((drawable->ntile_cols - 1) * TILE_WIDTH);
      bottom_tile = drawable->height - ((drawable->ntile_rows - 1) * TILE_HEIGHT);

      for (i = 0, k = 0; i < drawable->ntile_rows; i++)
        {
          for (j = 0; j < drawable->ntile_cols; j++, k++)
            {
              tiles[k].bpp       = drawable->bpp;
              tiles[k].tile_num  = k;
              tiles[k].ref_count = 0;
              tiles[k].dirty     = FALSE;
              tiles[k].shadow    = shadow;
              tiles[k].data      = NULL;
              tiles[k].drawable  = drawable;

              if (j == (drawable->ntile_cols - 1))
                tiles[k].ewidth  = right_tile;
              else
                tiles[k].ewidth  = TILE_WIDTH;

              if (i == (drawable->ntile_rows - 1))
                tiles[k].eheight = bottom_tile;
              else
                tiles[k].eheight = TILE_HEIGHT;
            }
        }

      if (shadow)
        drawable->shadow_tiles = tiles;
      else
        drawable->tiles = tiles;
    }

  tile_num = row * drawable->ntile_cols + col;

  return &tiles[tile_num];
}

GimpTile *
gimp_drawable_get_tile2 (GimpDrawable *drawable,
			 gint          shadow,
			 gint          x,
			 gint          y)
{
  gint row;
  gint col;

  g_return_val_if_fail (drawable != NULL, NULL);

  col = x / TILE_WIDTH;
  row = y / TILE_HEIGHT;

  return gimp_drawable_get_tile (drawable, shadow, row, col);
}

guchar *
gimp_drawable_get_thumbnail_data (gint32  drawable_ID,
				  gint   *width,
				  gint   *height,
				  gint   *bpp)
{
  gint    ret_width;
  gint    ret_height;
  guchar *image_data;
  gint    data_size;

  _gimp_drawable_thumbnail (drawable_ID,
			    *width,
			    *height,
			    &ret_width,
			    &ret_height,
			    bpp,
			    &data_size,
			    &image_data);

  *width  = ret_width;
  *height = ret_height;

  return image_data;
}

/**
 * gimp_drawable_attach_new_parasite:
 * @drawable_ID: the ID of the #GimpDrawable to attach the #GimpParasite to.
 * @name: the name of the #GimpParasite to create and attach.
 * @flags: the flags set on the #GimpParasite.
 * @size: the size of the parasite data in bytes.
 * @data: a pointer to the data attached with the #GimpParasite.
 *
 * Convenience function that creates a parasite and attaches it
 * to the GIMP.
 *
 * See Also: gimp_drawable_parasite_attach()
 */
void
gimp_drawable_attach_new_parasite (gint32          drawable_ID,
				   const gchar    *name,
				   gint            flags,
				   gint            size,
				   gconstpointer   data)
{
  GimpParasite *parasite = gimp_parasite_new (name, flags, size, data);

  gimp_drawable_parasite_attach (drawable_ID, parasite);

  gimp_parasite_free (parasite);
}
