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

#include "gui-types.h"

#include "widgets/gimpcontainerview.h"
#include "widgets/gimpcontainerview-utils.h"
#include "widgets/gimpdialogfactory.h"
#include "widgets/gimpdockable.h"
#include "widgets/gimpdockbook.h"
#include "widgets/gimpimagedock.h"
#include "widgets/gimpitemfactory.h"

#include "dialogs-commands.h"
#include "dialogs-menu.h"
#include "menus.h"

#include "gimp-intl.h"


#define ADD_TAB(path,id,type,stock_id) \
  { { (path), "foo", dialogs_add_tab_cmd_callback, 0, (type), (stock_id) }, \
    (id), NULL, NULL }
#define PREVIEW_SIZE(path,size) \
  { { (path), NULL, dialogs_preview_size_cmd_callback, \
      (size), "/Preview Size/Tiny" }, NULL, NULL, NULL }
#define TAB_STYLE(path,style) \
  { { (path), NULL, dialogs_tab_style_cmd_callback, \
      (style), "/Tab Style/Icon" }, NULL, NULL, NULL }


GimpItemFactoryEntry dialogs_menu_entries[] =
{
  { { N_("/_Select Tab"), NULL, NULL, 0 },
    NULL, NULL, NULL },

  MENU_BRANCH ("/_Add Tab"),

  ADD_TAB (N_("/Add Tab/Tool _Options..."),     "gimp-tool-options",
           "<StockItem>",                      GIMP_STOCK_TOOL_OPTIONS),
  ADD_TAB (N_("/Add Tab/_Device Status..."),    "gimp-device-status",
           "<StockItem>",                      GIMP_STOCK_DEVICE_STATUS),

  MENU_SEPARATOR ("/Add Tab/---"),

  ADD_TAB (N_("/Add Tab/_Layers..."),           "gimp-layer-list",
           "<StockItem>",                      GIMP_STOCK_LAYERS),
  ADD_TAB (N_("/Add Tab/_Channels..."),         "gimp-channel-list",
           "<StockItem>",                      GIMP_STOCK_CHANNELS),
  ADD_TAB (N_("/Add Tab/_Paths..."),            "gimp-vectors-list",
           "<StockItem>",                      GIMP_STOCK_PATHS),
  ADD_TAB (N_("/Add Tab/_Indexed Palette..."),  "gimp-indexed-palette",
           "<StockItem>",                      GTK_STOCK_SELECT_COLOR),
  ADD_TAB (N_("/Add Tab/_Selection Editor..."), "gimp-selection-editor",
           "<StockItem>",                      GIMP_STOCK_TOOL_RECT_SELECT),
  ADD_TAB (N_("/Add Tab/Na_vigation..."),       "gimp-navigation-view",
           "<StockItem>",                      GIMP_STOCK_NAVIGATION),
  ADD_TAB (N_("/Add Tab/_Undo History..."),     "gimp-undo-history",
           "<StockItem>",                      GTK_STOCK_UNDO),

  MENU_SEPARATOR ("/Add Tab/---"),

  ADD_TAB (N_("/Add Tab/Colo_rs..."),           "gimp-color-editor",
           "<StockItem>",                      GIMP_STOCK_DEFAULT_COLORS),
  ADD_TAB (N_("/Add Tab/Brus_hes..."),          "gimp-brush-grid",
           "<StockItem>",                      GIMP_STOCK_TOOL_PAINTBRUSH),
  ADD_TAB (N_("/Add Tab/P_atterns..."),         "gimp-pattern-grid",
           "<StockItem>",                      GIMP_STOCK_TOOL_BUCKET_FILL),
  ADD_TAB (N_("/Add Tab/_Gradients..."),        "gimp-gradient-list",
           "<StockItem>",                      GIMP_STOCK_TOOL_BLEND),
  ADD_TAB (N_("/Add Tab/Pal_ettes..."),         "gimp-palette-list",
           "<StockItem>",                      GTK_STOCK_SELECT_COLOR),
  ADD_TAB (N_("/Add Tab/_Fonts..."),            "gimp-font-list",
           "<StockItem>",                      GTK_STOCK_SELECT_FONT),
  ADD_TAB (N_("/Add Tab/_Buffers..."),          "gimp-buffer-list",
           "<StockItem>",                      GTK_STOCK_PASTE),

  MENU_SEPARATOR ("/Add Tab/---"),

  ADD_TAB (N_("/Add Tab/I_mages..."),           "gimp-image-list",
           "<StockItem>",                      GIMP_STOCK_IMAGES),
  ADD_TAB (N_("/Add Tab/Document Histor_y..."), "gimp-document-list",
           "<StockItem>",                      GTK_STOCK_OPEN),
  ADD_TAB (N_("/Add Tab/_Templates..."),        "gimp-template-list",
           "<StockItem>",                      GIMP_STOCK_TEMPLATE),
  ADD_TAB (N_("/Add Tab/Error Co_nsole..."),    "gimp-error-console",
           "<StockItem>",                      GIMP_STOCK_WARNING),

  MENU_SEPARATOR ("/Add Tab/---"),

  ADD_TAB (N_("/Add Tab/Tools..."),            "gimp-tool-list", NULL, NULL),

  { { N_("/_Remove Tab"), NULL,
      dialogs_remove_tab_cmd_callback, 0,
      "<StockItem>", GTK_STOCK_REMOVE },
    NULL, NULL, NULL },

  MENU_SEPARATOR ("/---"),

  MENU_BRANCH ("/Preview Si_ze"),

  { { N_("/Preview Size/_Tiny"), NULL,
      dialogs_preview_size_cmd_callback,
      GIMP_PREVIEW_SIZE_TINY, "<RadioItem>" },
    NULL, NULL, NULL },

  PREVIEW_SIZE (N_("/Preview Size/E_xtra Small"), GIMP_PREVIEW_SIZE_EXTRA_SMALL),
  PREVIEW_SIZE (N_("/Preview Size/_Small"),       GIMP_PREVIEW_SIZE_SMALL),
  PREVIEW_SIZE (N_("/Preview Size/_Medium"),      GIMP_PREVIEW_SIZE_MEDIUM),
  PREVIEW_SIZE (N_("/Preview Size/_Large"),       GIMP_PREVIEW_SIZE_LARGE),
  PREVIEW_SIZE (N_("/Preview Size/Ex_tra Large"), GIMP_PREVIEW_SIZE_EXTRA_LARGE),
  PREVIEW_SIZE (N_("/Preview Size/_Huge"),        GIMP_PREVIEW_SIZE_HUGE),
  PREVIEW_SIZE (N_("/Preview Size/_Enormous"),    GIMP_PREVIEW_SIZE_ENORMOUS),
  PREVIEW_SIZE (N_("/Preview Size/_Gigantic"),    GIMP_PREVIEW_SIZE_GIGANTIC),

  MENU_BRANCH ("/_Tab Style"),

  { { N_("/Tab Style/_Icon"), NULL,
      dialogs_tab_style_cmd_callback,
      GIMP_TAB_STYLE_ICON, "<RadioItem>" },
    NULL, NULL, NULL },

  TAB_STYLE (N_("/Tab Style/Current _Status"), GIMP_TAB_STYLE_PREVIEW),
  TAB_STYLE (N_("/Tab Style/_Text"),           GIMP_TAB_STYLE_NAME),
  TAB_STYLE (N_("/Tab Style/I_con & Text"),    GIMP_TAB_STYLE_ICON_NAME),
  TAB_STYLE (N_("/Tab Style/St_atus & Text"),  GIMP_TAB_STYLE_PREVIEW_NAME),

  { { N_("/View as _List"), NULL,
      dialogs_toggle_view_cmd_callback, GIMP_VIEW_TYPE_LIST, "<RadioItem>" },
    NULL, NULL, NULL },
  { { N_("/View as _Grid"), NULL,
      dialogs_toggle_view_cmd_callback, GIMP_VIEW_TYPE_GRID, "/View as List" },
    NULL, NULL, NULL },

  MENU_SEPARATOR ("/image-menu-separator"),

  { { N_("/Show Image _Menu"), NULL,
      dialogs_toggle_image_menu_cmd_callback, 0, "<ToggleItem>" },
    NULL, NULL, NULL },
  { { N_("/Auto Follow Active _Image"), NULL,
      dialogs_toggle_auto_cmd_callback, 0, "<ToggleItem>" },
    NULL, NULL, NULL }
};

#undef ADD_TAB
#undef PREVIEW_SIZE
#undef TAB_STYLE

gint n_dialogs_menu_entries = G_N_ELEMENTS (dialogs_menu_entries);


void
dialogs_menu_update (GtkItemFactory *factory,
                     gpointer        data)
{
  GimpDockbook *dockbook;

  dockbook = GIMP_DOCKBOOK (data);

  if (dockbook)
    {
      GimpDockable           *dockable;
      gint                    page_num;
      GimpDialogFactoryEntry *entry;
      GimpContainerView      *view;
      GimpViewType            view_type           = -1;
      gboolean                list_view_available = FALSE;
      gboolean                grid_view_available = FALSE;
      GimpPreviewSize         preview_size        = -1;
      GimpTabStyle            tab_style;

      page_num = gtk_notebook_get_current_page (GTK_NOTEBOOK (dockbook));

      dockable = (GimpDockable *)
	gtk_notebook_get_nth_page (GTK_NOTEBOOK (dockbook), page_num);

      entry = g_object_get_data (G_OBJECT (dockable),
                                 "gimp-dialog-factory-entry");

      if (entry)
        {
          gchar *identifier;
          gchar *substring = NULL;

          identifier = g_strdup (entry->identifier);

          if ((substring = strstr (identifier, "grid")))
            view_type = GIMP_VIEW_TYPE_GRID;
          else if ((substring = strstr (identifier, "list")))
            view_type = GIMP_VIEW_TYPE_LIST;

          if (substring)
            {
              memcpy (substring, "list", 4);
              if (gimp_dialog_factory_find_entry (dockbook->dock->dialog_factory,
                                                  identifier))
                list_view_available = TRUE;

              memcpy (substring, "grid", 4);
              if (gimp_dialog_factory_find_entry (dockbook->dock->dialog_factory,
                                                  identifier))
                grid_view_available = TRUE;
            }

          g_free (identifier);
        }

      view = gimp_container_view_get_by_dockable (dockable);

      if (view)
        preview_size = view->preview_size;

      tab_style = dockable->tab_style;

#define SET_ACTIVE(path,active) \
        gimp_item_factory_set_active (factory, (path), (active) != 0)
#define SET_VISIBLE(path,active) \
        gimp_item_factory_set_visible (factory, (path), (active) != 0)
#define SET_SENSITIVE(path,sensitive) \
        gimp_item_factory_set_sensitive (factory, (path), (sensitive) != 0)

      SET_VISIBLE ("/Preview Size", preview_size != -1);

      if (preview_size != -1)
        {
          if (preview_size >= GIMP_PREVIEW_SIZE_GIGANTIC)
            {
              SET_ACTIVE ("/Preview Size/Gigantic", TRUE);
            }
          else if (preview_size >= GIMP_PREVIEW_SIZE_ENORMOUS)
            {
              SET_ACTIVE ("/Preview Size/Enormous", TRUE);
            }
          else if (preview_size >= GIMP_PREVIEW_SIZE_HUGE)
            {
              SET_ACTIVE ("/Preview Size/Huge", TRUE);
            }
          else if (preview_size >= GIMP_PREVIEW_SIZE_EXTRA_LARGE)
            {
              SET_ACTIVE ("/Preview Size/Extra Large", TRUE);
            }
          else if (preview_size >= GIMP_PREVIEW_SIZE_LARGE)
            {
              SET_ACTIVE ("/Preview Size/Large", TRUE);
            }
          else if (preview_size >= GIMP_PREVIEW_SIZE_MEDIUM)
            {
              SET_ACTIVE ("/Preview Size/Medium", TRUE);
            }
          else if (preview_size >= GIMP_PREVIEW_SIZE_SMALL)
            {
              SET_ACTIVE ("/Preview Size/Small", TRUE);
            }
          else if (preview_size >= GIMP_PREVIEW_SIZE_EXTRA_SMALL)
            {
              SET_ACTIVE ("/Preview Size/Extra Small", TRUE);
            }
          else if (preview_size >= GIMP_PREVIEW_SIZE_TINY)
            {
              SET_ACTIVE ("/Preview Size/Tiny", TRUE);
            }
        }

      if (tab_style == GIMP_TAB_STYLE_ICON)
        SET_ACTIVE ("/Tab Style/Icon", TRUE);
      else if (tab_style == GIMP_TAB_STYLE_PREVIEW)
        SET_ACTIVE ("/Tab Style/Current Status", TRUE);
      else if (tab_style == GIMP_TAB_STYLE_NAME)
        SET_ACTIVE ("/Tab Style/Text", TRUE);
      else if (tab_style == GIMP_TAB_STYLE_ICON_NAME)
        SET_ACTIVE ("/Tab Style/Icon & Text", TRUE);
      else if (tab_style == GIMP_TAB_STYLE_PREVIEW_NAME)
        SET_ACTIVE ("/Tab Style/Status & Text", TRUE);

      SET_SENSITIVE ("/Tab Style/Current Status", dockable->get_preview_func);
      SET_SENSITIVE ("/Tab Style/Status & Text",  dockable->get_preview_func);

      SET_VISIBLE ("/View as Grid", view_type != -1);
      SET_VISIBLE ("/View as List", view_type != -1);

      if (view_type != -1)
        {
          if (view_type == GIMP_VIEW_TYPE_LIST)
            SET_ACTIVE ("/View as List", TRUE);
          else
            SET_ACTIVE ("/View as Grid", TRUE);

          SET_SENSITIVE ("/View as Grid", grid_view_available);
          SET_SENSITIVE ("/View as List", list_view_available);
        }

      if (GIMP_IS_IMAGE_DOCK (dockbook->dock))
        {
          SET_VISIBLE ("/image-menu-separator",     TRUE);
          SET_VISIBLE ("/Show Image Menu",          TRUE);
          SET_VISIBLE ("/Auto Follow Active Image", TRUE);

          SET_ACTIVE ("/Show Image Menu",
                      GIMP_IMAGE_DOCK (dockbook->dock)->show_image_menu);
          SET_ACTIVE ("/Auto Follow Active Image",
                      GIMP_IMAGE_DOCK (dockbook->dock)->auto_follow_active);
        }
      else
        {
          SET_VISIBLE ("/image-menu-separator",     FALSE);
          SET_VISIBLE ("/Show Image Menu",          FALSE);
          SET_VISIBLE ("/Auto Follow Active Image", FALSE);
        }

#undef SET_ACTIVE
#undef SET_VISIBLE
#undef SET_SENSITIVE
    }
}
