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

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "display-types.h"

#include "config/gimpdisplayconfig.h"

#include "core/gimp.h"
#include "core/gimpcontainer.h"
#include "core/gimpimage.h"
#include "core/gimpitem.h"

#include "widgets/gimpwidgets-utils.h"

#include "gimpdisplay.h"
#include "gimpdisplayoptions.h"
#include "gimpdisplayshell.h"
#include "gimpdisplayshell-appearance.h"
#include "gimpdisplayshell-callbacks.h"
#include "gimpdisplayshell-draw.h"
#include "gimpdisplayshell-handlers.h"
#include "gimpdisplayshell-scale.h"
#include "gimpdisplayshell-title.h"


#define GIMP_DISPLAY_UPDATE_ICON_TIMEOUT  1000


/*  local function prototypes  */

static void   gimp_display_shell_clean_dirty_handler        (GimpImage        *gimage,
                                                             GimpDirtyMask     dirty_mask,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_undo_event_handler         (GimpImage        *gimage,
                                                             GimpUndoEvent     event,
                                                             GimpUndo         *undo,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_grid_notify_handler        (GimpGrid         *grid,
                                                             GParamSpec       *pspec,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_name_changed_handler       (GimpImage        *gimage,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_selection_control_handler  (GimpImage        *gimage,
                                                             GimpSelectionControl control,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_size_changed_handler       (GimpImage        *gimage,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_resolution_changed_handler (GimpImage        *gimage,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_qmask_changed_handler      (GimpImage        *gimage,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_update_guide_handler       (GimpImage        *gimage,
                                                             GimpGuide        *guide,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_update_sample_point_handler(GimpImage        *gimage,
                                                             GimpSamplePoint  *sample_point,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_invalidate_preview_handler (GimpImage        *gimage,
                                                             GimpDisplayShell *shell);

static void   gimp_display_shell_vectors_freeze_handler     (GimpVectors      *vectors,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_vectors_thaw_handler       (GimpVectors      *vectors,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_vectors_visible_handler    (GimpVectors      *vectors,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_vectors_add_handler        (GimpContainer    *container,
                                                             GimpVectors      *vectors,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_vectors_remove_handler     (GimpContainer    *container,
                                                             GimpVectors      *vectors,
                                                             GimpDisplayShell *shell);

static void   gimp_display_shell_check_notify_handler       (GObject          *config,
                                                             GParamSpec       *param_spec,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_title_notify_handler       (GObject          *config,
                                                             GParamSpec       *param_spec,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_nav_size_notify_handler    (GObject          *config,
                                                             GParamSpec       *param_spec,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_monitor_res_notify_handler (GObject          *config,
                                                             GParamSpec       *param_spec,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_padding_notify_handler     (GObject          *config,
                                                             GParamSpec       *param_spec,
                                                             GimpDisplayShell *shell);
static void   gimp_display_shell_ants_speed_notify_handler  (GObject          *config,
                                                             GParamSpec       *param_spec,
                                                             GimpDisplayShell *shell);

static gboolean   gimp_display_shell_idle_update_icon       (gpointer          data);


/*  public functions  */

void
gimp_display_shell_connect (GimpDisplayShell *shell)
{
  GimpImage         *gimage;
  GimpDisplayConfig *display_config;

  g_return_if_fail (GIMP_IS_DISPLAY_SHELL (shell));
  g_return_if_fail (GIMP_IS_DISPLAY (shell->gdisp));
  g_return_if_fail (GIMP_IS_IMAGE (shell->gdisp->gimage));

  gimage = shell->gdisp->gimage;

  display_config = GIMP_DISPLAY_CONFIG (gimage->gimp->config);

  g_signal_connect (gimage, "clean",
                    G_CALLBACK (gimp_display_shell_clean_dirty_handler),
                    shell);
  g_signal_connect (gimage, "dirty",
                    G_CALLBACK (gimp_display_shell_clean_dirty_handler),
                    shell);
  g_signal_connect (gimage, "undo_event",
                    G_CALLBACK (gimp_display_shell_undo_event_handler),
                    shell);
  g_signal_connect (gimage->grid, "notify",
                    G_CALLBACK (gimp_display_shell_grid_notify_handler),
                    shell);
  g_signal_connect (gimage, "name_changed",
                    G_CALLBACK (gimp_display_shell_name_changed_handler),
                    shell);
  g_signal_connect (gimage, "selection_control",
                    G_CALLBACK (gimp_display_shell_selection_control_handler),
                    shell);
  g_signal_connect (gimage, "size_changed",
                    G_CALLBACK (gimp_display_shell_size_changed_handler),
                    shell);
  g_signal_connect (gimage, "resolution_changed",
                    G_CALLBACK (gimp_display_shell_resolution_changed_handler),
                    shell);
  g_signal_connect (gimage, "qmask_changed",
                    G_CALLBACK (gimp_display_shell_qmask_changed_handler),
                    shell);
  g_signal_connect (gimage, "update_guide",
                    G_CALLBACK (gimp_display_shell_update_guide_handler),
                    shell);
  g_signal_connect (gimage, "update_sample_point",
                    G_CALLBACK (gimp_display_shell_update_sample_point_handler),
                    shell);
  g_signal_connect (gimage, "invalidate_preview",
                    G_CALLBACK (gimp_display_shell_invalidate_preview_handler),
                    shell);

  shell->vectors_freeze_handler =
    gimp_container_add_handler (gimage->vectors, "freeze",
                                G_CALLBACK (gimp_display_shell_vectors_freeze_handler),
                                shell);
  shell->vectors_thaw_handler =
    gimp_container_add_handler (gimage->vectors, "thaw",
                                G_CALLBACK (gimp_display_shell_vectors_thaw_handler),
                                shell);
  shell->vectors_visible_handler =
    gimp_container_add_handler (gimage->vectors, "visibility_changed",
                                G_CALLBACK (gimp_display_shell_vectors_visible_handler),
                                shell);

  g_signal_connect (gimage->vectors, "add",
                    G_CALLBACK (gimp_display_shell_vectors_add_handler),
                    shell);
  g_signal_connect (gimage->vectors, "remove",
                    G_CALLBACK (gimp_display_shell_vectors_remove_handler),
                    shell);

  g_signal_connect (gimage->gimp->config,
                    "notify::transparency-size",
                    G_CALLBACK (gimp_display_shell_check_notify_handler),
                    shell);
  g_signal_connect (gimage->gimp->config,
                    "notify::transparency-type",
                    G_CALLBACK (gimp_display_shell_check_notify_handler),
                    shell);

  g_signal_connect (gimage->gimp->config,
                    "notify::image-title-format",
                    G_CALLBACK (gimp_display_shell_title_notify_handler),
                    shell);
  g_signal_connect (gimage->gimp->config,
                    "notify::image-status-format",
                    G_CALLBACK (gimp_display_shell_title_notify_handler),
                    shell);
  g_signal_connect (gimage->gimp->config,
                    "notify::navigation-preview-size",
                    G_CALLBACK (gimp_display_shell_nav_size_notify_handler),
                    shell);
  g_signal_connect (gimage->gimp->config,
                    "notify::monitor-resolution-from-windowing-system",
                    G_CALLBACK (gimp_display_shell_monitor_res_notify_handler),
                    shell);
  g_signal_connect (gimage->gimp->config,
                    "notify::monitor-xresolution",
                    G_CALLBACK (gimp_display_shell_monitor_res_notify_handler),
                    shell);
  g_signal_connect (gimage->gimp->config,
                    "notify::monitor-yresolution",
                    G_CALLBACK (gimp_display_shell_monitor_res_notify_handler),
                    shell);

  g_signal_connect (display_config->default_view,
                    "notify::padding-mode",
                    G_CALLBACK (gimp_display_shell_padding_notify_handler),
                    shell);
  g_signal_connect (display_config->default_view,
                    "notify::padding-color",
                    G_CALLBACK (gimp_display_shell_padding_notify_handler),
                    shell);
  g_signal_connect (display_config->default_fullscreen_view,
                    "notify::padding-mode",
                    G_CALLBACK (gimp_display_shell_padding_notify_handler),
                    shell);
  g_signal_connect (display_config->default_fullscreen_view,
                    "notify::padding-color",
                    G_CALLBACK (gimp_display_shell_padding_notify_handler),
                    shell);

  g_signal_connect (gimage->gimp->config,
                    "notify::marching-ants-speed",
                    G_CALLBACK (gimp_display_shell_ants_speed_notify_handler),
                    shell);

  gimp_display_shell_invalidate_preview_handler (gimage, shell);
  gimp_display_shell_qmask_changed_handler (gimage, shell);
}

void
gimp_display_shell_disconnect (GimpDisplayShell *shell)
{
  GimpImage         *gimage;
  GimpDisplayConfig *display_config;

  g_return_if_fail (GIMP_IS_DISPLAY_SHELL (shell));
  g_return_if_fail (GIMP_IS_DISPLAY (shell->gdisp));
  g_return_if_fail (GIMP_IS_IMAGE (shell->gdisp->gimage));

  gimage = shell->gdisp->gimage;

  display_config = GIMP_DISPLAY_CONFIG (gimage->gimp->config);

  if (shell->icon_idle_id)
    {
      g_source_remove (shell->icon_idle_id);
      shell->icon_idle_id = 0;
    }

  if (shell->grid_gc)
    {
      g_object_unref (shell->grid_gc);
      shell->grid_gc = NULL;
    }

  g_signal_handlers_disconnect_by_func (gimage->gimp->config,
                                        gimp_display_shell_ants_speed_notify_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (display_config->default_fullscreen_view,
                                        gimp_display_shell_padding_notify_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (display_config->default_view,
                                        gimp_display_shell_padding_notify_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage->gimp->config,
                                        gimp_display_shell_monitor_res_notify_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage->gimp->config,
                                        gimp_display_shell_nav_size_notify_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage->gimp->config,
                                        gimp_display_shell_title_notify_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage->gimp->config,
                                        gimp_display_shell_check_notify_handler,
                                        shell);

  g_signal_handlers_disconnect_by_func (gimage->vectors,
                                        gimp_display_shell_vectors_remove_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage->vectors,
                                        gimp_display_shell_vectors_add_handler,
                                        shell);

  gimp_container_remove_handler (gimage->vectors,
                                 shell->vectors_visible_handler);
  gimp_container_remove_handler (gimage->vectors,
                                 shell->vectors_thaw_handler);
  gimp_container_remove_handler (gimage->vectors,
                                 shell->vectors_freeze_handler);

  g_signal_handlers_disconnect_by_func (gimage,
                                        gimp_display_shell_invalidate_preview_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage,
                                        gimp_display_shell_update_guide_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage,
                                        gimp_display_shell_update_sample_point_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage,
                                        gimp_display_shell_qmask_changed_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage,
                                        gimp_display_shell_resolution_changed_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage,
                                        gimp_display_shell_size_changed_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage,
                                        gimp_display_shell_selection_control_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage,
                                        gimp_display_shell_name_changed_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage->grid,
                                        gimp_display_shell_grid_notify_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage,
                                        gimp_display_shell_undo_event_handler,
                                        shell);
  g_signal_handlers_disconnect_by_func (gimage,
                                        gimp_display_shell_clean_dirty_handler,
                                        shell);
}


/*  private functions  */

static void
gimp_display_shell_clean_dirty_handler (GimpImage        *gimage,
                                        GimpDirtyMask     dirty_mask,
                                        GimpDisplayShell *shell)
{
  gimp_display_shell_title_update (shell);
}

static void
gimp_display_shell_undo_event_handler (GimpImage        *gimage,
                                       GimpUndoEvent     event,
                                       GimpUndo         *undo,
                                       GimpDisplayShell *shell)
{
  gimp_display_shell_title_update (shell);
}

static void
gimp_display_shell_grid_notify_handler (GimpGrid         *grid,
                                        GParamSpec       *pspec,
                                        GimpDisplayShell *shell)
{
  if (shell->grid_gc)
    {
      g_object_unref (shell->grid_gc);
      shell->grid_gc = NULL;
    }

  gimp_display_shell_expose_full (shell);

  /* update item factory */
  gimp_display_flush (shell->gdisp);
}

static void
gimp_display_shell_name_changed_handler (GimpImage        *gimage,
                                         GimpDisplayShell *shell)
{
  gimp_display_shell_title_update (shell);
}

static void
gimp_display_shell_selection_control_handler (GimpImage            *gimage,
                                              GimpSelectionControl  control,
                                              GimpDisplayShell     *shell)
{
  gimp_display_shell_selection_visibility (shell, control);
}

static void
gimp_display_shell_size_changed_handler (GimpImage        *gimage,
                                         GimpDisplayShell *shell)
{
  gimp_display_shell_scale_resize (shell,
                                   GIMP_DISPLAY_CONFIG (gimage->gimp->config)->resize_windows_on_resize,
                                   TRUE);
}

static void
gimp_display_shell_resolution_changed_handler (GimpImage        *gimage,
                                               GimpDisplayShell *shell)
{
  if (shell->dot_for_dot)
    {
      gimp_display_shell_scale_setup (shell);
      gimp_display_shell_scaled (shell);
    }
  else
    {
      gimp_display_shell_size_changed_handler (gimage, shell);
    }
}

static void
gimp_display_shell_qmask_changed_handler (GimpImage        *gimage,
                                          GimpDisplayShell *shell)
{
  GtkImage *image;

  image = GTK_IMAGE (GTK_BIN (shell->qmask_button)->child);

  g_signal_handlers_block_by_func (shell->qmask_button,
                                   gimp_display_shell_qmask_toggled,
                                   shell);

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (shell->qmask_button),
                                shell->gdisp->gimage->qmask_state);

  if (shell->gdisp->gimage->qmask_state)
    gtk_image_set_from_stock (image, GIMP_STOCK_QMASK_ON, GTK_ICON_SIZE_MENU);
  else
    gtk_image_set_from_stock (image, GIMP_STOCK_QMASK_OFF, GTK_ICON_SIZE_MENU);

  g_signal_handlers_unblock_by_func (shell->qmask_button,
                                     gimp_display_shell_qmask_toggled,
                                     shell);
}

static void
gimp_display_shell_update_guide_handler (GimpImage        *gimage,
                                         GimpGuide        *guide,
                                         GimpDisplayShell *shell)
{
  gimp_display_shell_expose_guide (shell, guide);
}

static void
gimp_display_shell_update_sample_point_handler (GimpImage        *gimage,
                                                GimpSamplePoint  *sample_point,
                                                GimpDisplayShell *shell)
{
  gimp_display_shell_expose_sample_point (shell, sample_point);
}

static void
gimp_display_shell_invalidate_preview_handler (GimpImage        *gimage,
                                               GimpDisplayShell *shell)
{
  if (shell->icon_idle_id)
    g_source_remove (shell->icon_idle_id);

  shell->icon_idle_id = g_timeout_add_full (G_PRIORITY_LOW,
                                            GIMP_DISPLAY_UPDATE_ICON_TIMEOUT,
                                            gimp_display_shell_idle_update_icon,
                                            shell,
                                            NULL);
}

static void
gimp_display_shell_vectors_freeze_handler (GimpVectors      *vectors,
                                           GimpDisplayShell *shell)
{
  if (shell->paused_count == 0 && gimp_item_get_visible (GIMP_ITEM (vectors)))
    gimp_display_shell_draw_vector (shell, vectors);
}

static void
gimp_display_shell_vectors_thaw_handler (GimpVectors      *vectors,
                                         GimpDisplayShell *shell)
{
  if (shell->paused_count == 0 && gimp_item_get_visible (GIMP_ITEM (vectors)))
    gimp_display_shell_draw_vector (shell, vectors);
}

static void
gimp_display_shell_vectors_visible_handler (GimpVectors      *vectors,
                                            GimpDisplayShell *shell)
{
  if (shell->paused_count == 0)
    gimp_display_shell_draw_vector (shell, vectors);
}

static void
gimp_display_shell_vectors_add_handler (GimpContainer    *container,
                                        GimpVectors      *vectors,
                                        GimpDisplayShell *shell)
{
  if (shell->paused_count == 0 && gimp_item_get_visible (GIMP_ITEM (vectors)))
    gimp_display_shell_draw_vector (shell, vectors);
}

static void
gimp_display_shell_vectors_remove_handler (GimpContainer    *container,
                                           GimpVectors      *vectors,
                                           GimpDisplayShell *shell)
{
  if (shell->paused_count == 0 && gimp_item_get_visible (GIMP_ITEM (vectors)))
    gimp_display_shell_draw_vector (shell, vectors);
}

static void
gimp_display_shell_check_notify_handler (GObject          *config,
                                         GParamSpec       *param_spec,
                                         GimpDisplayShell *shell)
{
  GimpCanvasPaddingMode padding_mode;
  GimpRGB               padding_color;

  gimp_display_shell_get_padding (shell, &padding_mode, &padding_color);

  switch (padding_mode)
    {
    case GIMP_CANVAS_PADDING_MODE_LIGHT_CHECK:
    case GIMP_CANVAS_PADDING_MODE_DARK_CHECK:
      gimp_display_shell_set_padding (shell, padding_mode, &padding_color);
      break;

    default:
      break;
    }

  gimp_display_shell_expose_full (shell);
}

static void
gimp_display_shell_title_notify_handler (GObject          *config,
                                         GParamSpec       *param_spec,
                                         GimpDisplayShell *shell)
{
  gimp_display_shell_title_update (shell);
}

static void
gimp_display_shell_nav_size_notify_handler (GObject          *config,
                                            GParamSpec       *param_spec,
                                            GimpDisplayShell *shell)
{
  if (shell->nav_popup)
    {
      gtk_widget_destroy (shell->nav_popup);
      shell->nav_popup = NULL;
    }
}

static void
gimp_display_shell_monitor_res_notify_handler (GObject          *config,
                                               GParamSpec       *param_spec,
                                               GimpDisplayShell *shell)
{
  if (GIMP_DISPLAY_CONFIG (config)->monitor_res_from_gdk)
    {
      gimp_get_screen_resolution (gtk_widget_get_screen (GTK_WIDGET (shell)),
                                  &shell->monitor_xres,
                                  &shell->monitor_yres);
    }
  else
    {
      shell->monitor_xres = GIMP_DISPLAY_CONFIG (config)->monitor_xres;
      shell->monitor_yres = GIMP_DISPLAY_CONFIG (config)->monitor_yres;
    }

  if (! shell->dot_for_dot)
    {
      gimp_display_shell_scale_setup (shell);
      gimp_display_shell_scaled (shell);

      gimp_display_shell_expose_full (shell);
    }
}

static void
gimp_display_shell_padding_notify_handler (GObject          *config,
                                           GParamSpec       *param_spec,
                                           GimpDisplayShell *shell)
{
  GimpDisplayConfig     *display_config;
  gboolean               fullscreen;
  GimpCanvasPaddingMode  padding_mode;
  GimpRGB                padding_color;

  display_config = GIMP_DISPLAY_CONFIG (shell->gdisp->gimage->gimp->config);

  fullscreen = gimp_display_shell_get_fullscreen (shell);

  /*  if the user did not set the padding mode for this display explicitely  */
  if (! shell->fullscreen_options->padding_mode_set)
    {
      padding_mode  = display_config->default_fullscreen_view->padding_mode;
      padding_color = display_config->default_fullscreen_view->padding_color;

      if (fullscreen)
        {
          gimp_display_shell_set_padding (shell, padding_mode, &padding_color);
        }
      else
        {
          shell->fullscreen_options->padding_mode  = padding_mode;
          shell->fullscreen_options->padding_color = padding_color;
        }
    }

  /*  if the user did not set the padding mode for this display explicitely  */
  if (! shell->options->padding_mode_set)
    {
      padding_mode  = display_config->default_view->padding_mode;
      padding_color = display_config->default_view->padding_color;

      if (fullscreen)
        {
          shell->options->padding_mode  = padding_mode;
          shell->options->padding_color = padding_color;
        }
      else
        {
          gimp_display_shell_set_padding (shell, padding_mode, &padding_color);
        }
    }
}

static void
gimp_display_shell_ants_speed_notify_handler (GObject          *config,
                                              GParamSpec       *param_spec,
                                              GimpDisplayShell *shell)
{
  gimp_display_shell_selection_visibility (shell, GIMP_SELECTION_PAUSE);
  gimp_display_shell_selection_visibility (shell, GIMP_SELECTION_RESUME);
}

static gboolean
gimp_display_shell_idle_update_icon (gpointer data)
{
  GimpDisplayShell *shell = GIMP_DISPLAY_SHELL (data);

  shell->icon_idle_id = 0;

  gimp_display_shell_update_icon (shell);

  return FALSE;
}
