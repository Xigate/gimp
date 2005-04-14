/* LIBGIMP - The GIMP Library
 * Copyright (C) 1995-2003 Peter Mattis and Spencer Kimball
 *
 * gimpimage_pdb.h
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* NOTE: This file is autogenerated by pdbgen.pl */

#ifndef __GIMP_IMAGE_PDB_H__
#define __GIMP_IMAGE_PDB_H__

G_BEGIN_DECLS

/* For information look into the C source or the html documentation */


gint*             gimp_image_list                     (gint                 *num_images);
gint32            gimp_image_new                      (gint                  width,
						       gint                  height,
						       GimpImageBaseType     type);
gint32            gimp_image_duplicate                (gint32                image_ID);
gboolean          gimp_image_delete                   (gint32                image_ID);
GimpImageBaseType gimp_image_base_type                (gint32                image_ID);
gint              gimp_image_width                    (gint32                image_ID);
gint              gimp_image_height                   (gint32                image_ID);
gboolean          gimp_image_free_shadow              (gint32                image_ID);
gboolean          gimp_image_resize                   (gint32                image_ID,
						       gint                  new_width,
						       gint                  new_height,
						       gint                  offx,
						       gint                  offy);
gboolean          gimp_image_resize_to_layers         (gint32                image_ID);
gboolean          gimp_image_scale                    (gint32                image_ID,
						       gint                  new_width,
						       gint                  new_height);
gboolean          gimp_image_crop                     (gint32                image_ID,
						       gint                  new_width,
						       gint                  new_height,
						       gint                  offx,
						       gint                  offy);
gboolean          gimp_image_flip                     (gint32                image_ID,
						       GimpOrientationType   flip_type);
gboolean          gimp_image_rotate                   (gint32                image_ID,
						       GimpRotationType      rotate_type);
gint*             gimp_image_get_layers               (gint32                image_ID,
						       gint                 *num_layers);
gint*             gimp_image_get_channels             (gint32                image_ID,
						       gint                 *num_channels);
gint*             gimp_image_get_vectors              (gint32                image_ID,
						       gint                 *num_vectors);
gint32            gimp_image_get_active_drawable      (gint32                image_ID);
gboolean          gimp_image_unset_active_channel     (gint32                image_ID);
gint32            gimp_image_get_floating_sel         (gint32                image_ID);
gint32            gimp_image_floating_sel_attached_to (gint32                image_ID);
gboolean          gimp_image_pick_color               (gint32                image_ID,
						       gint32                drawable_ID,
						       gdouble               x,
						       gdouble               y,
						       gboolean              sample_merged,
						       gboolean              sample_average,
						       gdouble               average_radius,
						       GimpRGB              *color);
gint32            gimp_image_pick_correlate_layer     (gint32                image_ID,
						       gint                  x,
						       gint                  y);
gboolean          gimp_image_add_layer                (gint32                image_ID,
						       gint32                layer_ID,
						       gint                  position);
gboolean          gimp_image_remove_layer             (gint32                image_ID,
						       gint32                layer_ID);
gboolean          gimp_image_raise_layer              (gint32                image_ID,
						       gint32                layer_ID);
gboolean          gimp_image_lower_layer              (gint32                image_ID,
						       gint32                layer_ID);
gboolean          gimp_image_raise_layer_to_top       (gint32                image_ID,
						       gint32                layer_ID);
gboolean          gimp_image_lower_layer_to_bottom    (gint32                image_ID,
						       gint32                layer_ID);
gboolean          gimp_image_add_channel              (gint32                image_ID,
						       gint32                channel_ID,
						       gint                  position);
gboolean          gimp_image_remove_channel           (gint32                image_ID,
						       gint32                channel_ID);
gboolean          gimp_image_raise_channel            (gint32                image_ID,
						       gint32                channel_ID);
gboolean          gimp_image_lower_channel            (gint32                image_ID,
						       gint32                layer_ID);
gint32            gimp_image_flatten                  (gint32                image_ID);
gint32            gimp_image_merge_visible_layers     (gint32                image_ID,
						       GimpMergeType         merge_type);
gint32            gimp_image_merge_down               (gint32                image_ID,
						       gint32                merge_layer_ID,
						       GimpMergeType         merge_type);
guint8*           _gimp_image_get_colormap            (gint32                image_ID,
						       gint                 *num_bytes);
gboolean          _gimp_image_set_colormap            (gint32                image_ID,
						       gint                  num_bytes,
						       const guint8         *colormap);
gboolean          gimp_image_clean_all                (gint32                image_ID);
gboolean          gimp_image_is_dirty                 (gint32                image_ID);
gboolean          _gimp_image_thumbnail               (gint32                image_ID,
						       gint                  width,
						       gint                  height,
						       gint                 *ret_width,
						       gint                 *ret_height,
						       gint                 *bpp,
						       gint                 *thumbnail_data_count,
						       guint8              **thumbnail_data);
gint32            gimp_image_get_active_layer         (gint32                image_ID);
gboolean          gimp_image_set_active_layer         (gint32                image_ID,
						       gint32                active_layer_ID);
gint32            gimp_image_get_active_channel       (gint32                image_ID);
gboolean          gimp_image_set_active_channel       (gint32                image_ID,
						       gint32                active_channel_ID);
gint32            gimp_image_get_selection            (gint32                image_ID);
gboolean          gimp_image_get_component_active     (gint32                image_ID,
						       GimpChannelType       component);
gboolean          gimp_image_set_component_active     (gint32                image_ID,
						       GimpChannelType       component,
						       gboolean              active);
gboolean          gimp_image_get_component_visible    (gint32                image_ID,
						       GimpChannelType       component);
gboolean          gimp_image_set_component_visible    (gint32                image_ID,
						       GimpChannelType       component,
						       gboolean              visible);
gchar*            gimp_image_get_filename             (gint32                image_ID);
gboolean          gimp_image_set_filename             (gint32                image_ID,
						       const gchar          *filename);
gchar*            gimp_image_get_name                 (gint32                image_ID);
gboolean          gimp_image_get_resolution           (gint32                image_ID,
						       gdouble              *xresolution,
						       gdouble              *yresolution);
gboolean          gimp_image_set_resolution           (gint32                image_ID,
						       gdouble               xresolution,
						       gdouble               yresolution);
GimpUnit          gimp_image_get_unit                 (gint32                image_ID);
gboolean          gimp_image_set_unit                 (gint32                image_ID,
						       GimpUnit              unit);
gint              gimp_image_get_tattoo_state         (gint32                image_ID);
gboolean          gimp_image_set_tattoo_state         (gint32                image_ID,
						       gint                  tattoo_state);
gint32            gimp_image_get_layer_by_tattoo      (gint32                image_ID,
						       gint                  tattoo);
gint32            gimp_image_get_channel_by_tattoo    (gint32                image_ID,
						       gint                  tattoo);


G_END_DECLS

#endif /* __GIMP_IMAGE_PDB_H__ */
