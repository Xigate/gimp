/* The GIMP -- an image manipulation program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimphelp-ids.h
 * Copyright (C) 2003 Michael Natterer <mitch@gimp.org>
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

#ifndef __GIMP_HELP_IDS_H__
#define __GIMP_HELP_IDS_H__


#define GIMP_HELP_MAIN                            "gimp-main"

#define GIMP_HELP_FILE_NEW                        "gimp-file-new"
#define GIMP_HELP_FILE_NEW_CONFIRM                "gimp-file-new-confirm"
#define GIMP_HELP_FILE_OPEN                       "gimp-file-open"
#define GIMP_HELP_FILE_OPEN_BY_EXTENSION          "gimp-file-open-by-extension"
#define GIMP_HELP_FILE_OPEN_XCF                   "gimp-file-open-xcf"
#define GIMP_HELP_FILE_OPEN_RECENT                "gimp-file-open-recent"
#define GIMP_HELP_FILE_SAVE                       "gimp-file-save"
#define GIMP_HELP_FILE_SAVE_AS                    "gimp-file-save-as"
#define GIMP_HELP_FILE_SAVE_A_COPY                "gimp-file-save-a-copy"
#define GIMP_HELP_FILE_SAVE_AS_TEMPLATE           "gimp-file-save-as-template"
#define GIMP_HELP_FILE_SAVE_BY_EXTENSION          "gimp-file-save-by-extension"
#define GIMP_HELP_FILE_SAVE_XCF                   "gimp-file-save-xcf"
#define GIMP_HELP_FILE_SAVE_OVERWRITE             "gimp-file-save-overwrite"
#define GIMP_HELP_FILE_REVERT                     "gimp-file-revert"
#define GIMP_HELP_FILE_CLOSE                      "gimp-file-close"
#define GIMP_HELP_FILE_CLOSE_CONFIRM              "gimp-file-close-confirm"
#define GIMP_HELP_FILE_QUIT                       "gimp-file-quit"
#define GIMP_HELP_FILE_QUIT_CONFIRM               "gimp-file-quit-confirm"

#define GIMP_HELP_EDIT_UNDO                       "gimp-edit-undo"
#define GIMP_HELP_EDIT_REDO                       "gimp-edit-redo"
#define GIMP_HELP_EDIT_CUT                        "gimp-edit-cut"
#define GIMP_HELP_EDIT_COPY                       "gimp-edit-copy"
#define GIMP_HELP_EDIT_PASTE                      "gimp-edit-paste"
#define GIMP_HELP_EDIT_PASTE_INTO                 "gimp-edit-paste-into"
#define GIMP_HELP_EDIT_PASTE_AS_NEW               "gimp-edit-paste-as-new"
#define GIMP_HELP_EDIT_CLEAR                      "gimp-edit-clear"
#define GIMP_HELP_EDIT_FILL_FG                    "gimp-edit-fill-fg"
#define GIMP_HELP_EDIT_FILL_BG                    "gimp-edit-fill-bg"
#define GIMP_HELP_EDIT_FILL_PATTERN               "gimp-edit-fill-pattern"

#define GIMP_HELP_SELECTION_DIALOG                "gimp-selection-dialog"
#define GIMP_HELP_SELECTION_ALL                   "gimp-selection-all"
#define GIMP_HELP_SELECTION_NONE                  "gimp-selection-none"
#define GIMP_HELP_SELECTION_INVERT                "gimp-selection-invert"
#define GIMP_HELP_SELECTION_FLOAT                 "gimp-selection-float"
#define GIMP_HELP_SELECTION_FEATHER               "gimp-selection-feather"
#define GIMP_HELP_SELECTION_SHARPEN               "gimp-selection-sharpen"
#define GIMP_HELP_SELECTION_SHRINK                "gimp-selection-shrink"
#define GIMP_HELP_SELECTION_GROW                  "gimp-selection-grow"
#define GIMP_HELP_SELECTION_BORDER                "gimp-selection-border"
#define GIMP_HELP_SELECTION_STROKE                "gimp-selection-stroke"
#define GIMP_HELP_SELECTION_TO_CHANNEL            "gimp-selection-to-channel"
#define GIMP_HELP_SELECTION_TO_PATH               "gimp-selection-to-path"

#define GIMP_HELP_VIEW_NEW                        "gimp-view-new"
#define GIMP_HELP_VIEW_DOT_FOR_DOT                "gimp-view-dot-for-dot"
#define GIMP_HELP_VIEW_ZOOM_OUT                   "gimp-view-zoom-out"
#define GIMP_HELP_VIEW_ZOOM_IN                    "gimp-view-zoom-in"
#define GIMP_HELP_VIEW_ZOOM_100                   "gimp-view-zoom-100"
#define GIMP_HELP_VIEW_ZOOM_FIT                   "gimp-view-zoom-fit"
#define GIMP_HELP_VIEW_ZOOM_OTHER                 "gimp-view-zoom-other"
#define GIMP_HELP_VIEW_SHOW_SELECTION             "gimp-view-show-selection"
#define GIMP_HELP_VIEW_SHOW_LAYER_BOUNDARY        "gimp-view-show-layer-boundary"
#define GIMP_HELP_VIEW_SHOW_GUIDES                "gimp-view-show-guides"
#define GIMP_HELP_VIEW_SNAP_TO_GUIDES             "gimp-view-snap-to-guides"
#define GIMP_HELP_VIEW_SHOW_GRID                  "gimp-view-show-grid"
#define GIMP_HELP_VIEW_SNAP_TO_GRID               "gimp-view-snap-to-grid"
#define GIMP_HELP_VIEW_SHOW_MENUBAR               "gimp-view-show-menubar"
#define GIMP_HELP_VIEW_SHOW_RULERS                "gimp-view-show-rulers"
#define GIMP_HELP_VIEW_SHOW_SCROLLBARS            "gimp-view-show-scrollbars"
#define GIMP_HELP_VIEW_SHOW_STATUSBAR             "gimp-view-show-statusbar"
#define GIMP_HELP_VIEW_SHRINK_WRAP                "gimp-view-shrink-wrap"
#define GIMP_HELP_VIEW_FULLSCREEN                 "gimp-view-fullscreen"

#define GIMP_HELP_IMAGE_WINDOW                    "gimp-image-window"
#define GIMP_HELP_IMAGE_DIALOG                    "gimp-image-dialog"
#define GIMP_HELP_IMAGE_CONVERT_RGB               "gimp-image-convert-rgb"
#define GIMP_HELP_IMAGE_CONVERT_GRAYSCALE         "gimp-image-convert-grayscale"
#define GIMP_HELP_IMAGE_CONVERT_INDEXED           "gimp-image-convert-indexed"
#define GIMP_HELP_IMAGE_FLIP_HORIZONTAL           "gimp-image-flip-horizontal"
#define GIMP_HELP_IMAGE_FLIP_VERTICAL             "gimp-image-flip-vertical"
#define GIMP_HELP_IMAGE_ROTATE_90                 "gimp-image-rotate-90"
#define GIMP_HELP_IMAGE_ROTATE_180                "gimp-image-rotate-180"
#define GIMP_HELP_IMAGE_ROTATE_270                "gimp-image-rotate-270"
#define GIMP_HELP_IMAGE_RESIZE                    "gimp-image-resize"
#define GIMP_HELP_IMAGE_SCALE                     "gimp-image-scale"
#define GIMP_HELP_IMAGE_SCALE_WARNING             "gimp-image-scale-warning"
#define GIMP_HELP_IMAGE_CROP                      "gimp-image-crop"
#define GIMP_HELP_IMAGE_DUPLICATE                 "gimp-image-duplicate"
#define GIMP_HELP_IMAGE_MERGE_LAYERS              "gimp-image-merge-layers"
#define GIMP_HELP_IMAGE_FLATTEN                   "gimp-image-flatten"
#define GIMP_HELP_IMAGE_GRID                      "gimp-image-grid"

#define GIMP_HELP_LAYER_DIALOG                    "gimp-layer-dialog"
#define GIMP_HELP_LAYER_NEW                       "gimp-layer-new"
#define GIMP_HELP_LAYER_DUPLICATE                 "gimp-layer-duplicate"
#define GIMP_HELP_LAYER_ANCHOR                    "gimp-layer-anchor"
#define GIMP_HELP_LAYER_MERGE_DOWN                "gimp-layer-merge-down"
#define GIMP_HELP_LAYER_DELETE                    "gimp-layer-delete"
#define GIMP_HELP_LAYER_PREVIOUS                  "gimp-layer-previous"
#define GIMP_HELP_LAYER_NEXT                      "gimp-layer-next"
#define GIMP_HELP_LAYER_TOP                       "gimp-layer-top"
#define GIMP_HELP_LAYER_BOTTOM                    "gimp-layer-bottom"
#define GIMP_HELP_LAYER_RAISE                     "gimp-layer-raise"
#define GIMP_HELP_LAYER_RAISE_TO_TOP              "gimp-layer-raise-to-top"
#define GIMP_HELP_LAYER_LOWER                     "gimp-layer-lower"
#define GIMP_HELP_LAYER_LOWER_TO_BOTTOM           "gimp-layer-lower-to-bottom"
#define GIMP_HELP_LAYER_DESATURATE                "gimp-layer-desaturate"
#define GIMP_HELP_LAYER_INVERT                    "gimp-layer-invert"
#define GIMP_HELP_LAYER_EQUALIZE                  "gimp-layer-equalize"
#define GIMP_HELP_LAYER_MASK_ADD                  "gimp-layer-mask-add"
#define GIMP_HELP_LAYER_MASK_APPLY                "gimp-layer-mask-apply"
#define GIMP_HELP_LAYER_MASK_DELETE               "gimp-layer-mask-delete"
#define GIMP_HELP_LAYER_MASK_SELECTION_REPLACE    "gimp-layer-mask-selection-replace"
#define GIMP_HELP_LAYER_MASK_SELECTION_ADD        "gimp-layer-mask-selection-add"
#define GIMP_HELP_LAYER_MASK_SELECTION_SUBTRACT   "gimp-layer-mask-selection-subtract"
#define GIMP_HELP_LAYER_MASK_SELECTION_INTERSECT  "gimp-layer-mask-selection-intersect"
#define GIMP_HELP_LAYER_ALPHA_ADD                 "gimp-layer-alpha-add"
#define GIMP_HELP_LAYER_ALPHA_SELECTION_REPLACE   "gimp-layer-alpha-selection-replace"
#define GIMP_HELP_LAYER_ALPHA_SELECTION_ADD       "gimp-layer-alpha-selection-add"
#define GIMP_HELP_LAYER_ALPHA_SELECTION_SUBTRACT  "gimp-layer-alpha-selection-subtract"
#define GIMP_HELP_LAYER_ALPHA_SELECTION_INTERSECT "gimp-layer-alpha-selection-intersect"
#define GIMP_HELP_LAYER_FLIP_HORIZONTAL           "gimp-layer-flip-horizontal"
#define GIMP_HELP_LAYER_FLIP_VERTICAL             "gimp-layer-flip-vertical"
#define GIMP_HELP_LAYER_ROTATE_90                 "gimp-layer-rotate-90"
#define GIMP_HELP_LAYER_ROTATE_180                "gimp-layer-rotate-180"
#define GIMP_HELP_LAYER_ROTATE_270                "gimp-layer-rotate-270"
#define GIMP_HELP_LAYER_OFFSET                    "gimp-layer-offset"
#define GIMP_HELP_LAYER_RESIZE                    "gimp-layer-resize"
#define GIMP_HELP_LAYER_RESIZE_TO_IMAGE           "gimp-layer-resize-to-image"
#define GIMP_HELP_LAYER_SCALE                     "gimp-layer-scale"
#define GIMP_HELP_LAYER_CROP                      "gimp-layer-crop"
#define GIMP_HELP_LAYER_EDIT                      "gimp-layer-edit"

#define GIMP_HELP_CHANNEL_DIALOG                  "gimp-channel-dialog"
#define GIMP_HELP_CHANNEL_NEW                     "gimp-channel-new"
#define GIMP_HELP_CHANNEL_RAISE                   "gimp-channel-raise"
#define GIMP_HELP_CHANNEL_RAISE_TO_TOP            "gimp-channel-raise-to-top"
#define GIMP_HELP_CHANNEL_LOWER                   "gimp-channel-lower"
#define GIMP_HELP_CHANNEL_LOWER_TO_BOTTOM         "gimp-channel-lower-to-bottom"
#define GIMP_HELP_CHANNEL_DUPLICATE               "gimp-channel-duplicate"
#define GIMP_HELP_CHANNEL_DELETE                  "gimp-channel-delete"
#define GIMP_HELP_CHANNEL_SELECTION_REPLACE       "gimp-channel-selection-replace"
#define GIMP_HELP_CHANNEL_SELECTION_ADD           "gimp-channel-selection-add"
#define GIMP_HELP_CHANNEL_SELECTION_SUBTRACT      "gimp-channel-selection-subtract"
#define GIMP_HELP_CHANNEL_SELECTION_INTERSECT     "gimp-channel-selection-intersect"
#define GIMP_HELP_CHANNEL_EDIT                    "gimp-channel-edit"

#define GIMP_HELP_QMASK                           "gimp-qmask"
#define GIMP_HELP_QMASK_TOGGLE                    "gimp-qmask-toggle"
#define GIMP_HELP_QMASK_INVERT                    "gimp-qmask-invert"
#define GIMP_HELP_QMASK_EDIT                      "gimp-qmask-edit"

#define GIMP_HELP_PATH_DIALOG                     "gimp-path-dialog"
#define GIMP_HELP_PATH_NEW                        "gimp-path-new"
#define GIMP_HELP_PATH_RAISE                      "gimp-path-raise"
#define GIMP_HELP_PATH_RAISE_TO_TOP               "gimp-path-raise-to-top"
#define GIMP_HELP_PATH_LOWER                      "gimp-path-lower"
#define GIMP_HELP_PATH_LOWER_TO_BOTTOM            "gimp-path-lower-to-bottom"
#define GIMP_HELP_PATH_DUPLICATE                  "gimp-path-duplicate"
#define GIMP_HELP_PATH_DELETE                     "gimp-path-delete"
#define GIMP_HELP_PATH_MERGE_VISIBLE              "gimp-path-merge-visible"
#define GIMP_HELP_PATH_SELECTION_REPLACE          "gimp-path-selection-replace"
#define GIMP_HELP_PATH_SELECTION_ADD              "gimp-path-selection-add"
#define GIMP_HELP_PATH_SELECTION_SUBTRACT         "gimp-path-selection-subtract"
#define GIMP_HELP_PATH_SELECTION_INTERSECT        "gimp-path-selection-intersect"
#define GIMP_HELP_PATH_STROKE                     "gimp-path-stroke"
#define GIMP_HELP_PATH_COPY                       "gimp-path-copy"
#define GIMP_HELP_PATH_PASTE                      "gimp-path-paste"
#define GIMP_HELP_PATH_IMPORT                     "gimp-path-import"
#define GIMP_HELP_PATH_EXPORT                     "gimp-path-export"
#define GIMP_HELP_PATH_EDIT                       "gimp-path-edit"

#define GIMP_HELP_TOOLS_DIALOG                    "gimp-tools-dialog"

#define GIMP_HELP_TOOL_AIRBRUSH                   "gimp-tool-airbrush"
#define GIMP_HELP_TOOL_BLEND                      "gimp-tool-blend"
#define GIMP_HELP_TOOL_BRIGHTNESS_CONTRAST        "gimp-tool-brightness-contrast"
#define GIMP_HELP_TOOL_BUCKET_FILL                "gimp-tool-bucket-fill"
#define GIMP_HELP_TOOL_BY_COLOR_SELECT            "gimp-tool-by-color-select"
#define GIMP_HELP_TOOL_CLONE                      "gimp-tool-clone"
#define GIMP_HELP_TOOL_COLORIZE                   "gimp-tool-colorize"
#define GIMP_HELP_TOOL_COLOR_BALANCE              "gimp-tool-color-balance"
#define GIMP_HELP_TOOL_COLOR_PICKER               "gimp-tool-color-picker"
#define GIMP_HELP_TOOL_CONVOLVE                   "gimp-tool-convolve"
#define GIMP_HELP_TOOL_CROP                       "gimp-tool-crop"
#define GIMP_HELP_TOOL_CURVES                     "gimp-tool-curves"
#define GIMP_HELP_TOOL_CURVES                     "gimp-tool-curves"
#define GIMP_HELP_TOOL_DODGE_BURN                 "gimp-tool-dodge-burn"
#define GIMP_HELP_TOOL_ELLIPSE_SELECT             "gimp-tool-ellipse-select"
#define GIMP_HELP_TOOL_ERASER                     "gimp-tool-eraser"
#define GIMP_HELP_TOOL_FLIP                       "gimp-tool-flip"
#define GIMP_HELP_TOOL_FREE_SELECT                "gimp-tool-free-select"
#define GIMP_HELP_TOOL_FUZZY_SELECT               "gimp-tool-fuzzy-select"
#define GIMP_HELP_TOOL_HUE_SATURATION             "gimp-tool-hue-saturation"
#define GIMP_HELP_TOOL_INK                        "gimp-tool-ink"
#define GIMP_HELP_TOOL_ISCISSORS                  "gimp-tool-iscissors"
#define GIMP_HELP_TOOL_LEVELS                     "gimp-tool-levels"
#define GIMP_HELP_TOOL_MEASURE                    "gimp-tool-measure"
#define GIMP_HELP_TOOL_MOVE                       "gimp-tool-move"
#define GIMP_HELP_TOOL_PATH                       "gimp-tool-path"
#define GIMP_HELP_TOOL_PAINTBRUSH                 "gimp-tool-paintbrush"
#define GIMP_HELP_TOOL_PENCIL                     "gimp-tool-pencil"
#define GIMP_HELP_TOOL_PERSPECTIVE                "gimp-tool-perspective"
#define GIMP_HELP_TOOL_POSTERIZE                  "gimp-tool-posterize"
#define GIMP_HELP_TOOL_RECT_SELECT                "gimp-tool-rect-select"
#define GIMP_HELP_TOOL_ROTATE                     "gimp-tool-rotate"
#define GIMP_HELP_TOOL_SCALE                      "gimp-tool-scale"
#define GIMP_HELP_TOOL_SHEAR                      "gimp-tool-shear"
#define GIMP_HELP_TOOL_SMUDGE                     "gimp-tool-smudge"
#define GIMP_HELP_TOOL_TEXT                       "gimp-tool-text"
#define GIMP_HELP_TOOL_THRESHOLD                  "gimp-tool-threshold"
#define GIMP_HELP_TOOL_VECTORS                    "gimp-tool-vectors"
#define GIMP_HELP_TOOL_ZOOM                       "gimp-tool-zoom"

#define GIMP_HELP_FILTER_REPEAT                   "gimp-filter-repeat"
#define GIMP_HELP_FILTER_RESHOW                   "gimp-filter-reshow"

#define GIMP_HELP_TOOLBOX                         "gimp-toolbox"
#define GIMP_HELP_TOOLBOX_COLOR_AREA              "gimp-toolbox-color-area"
#define GIMP_HELP_TOOLBOX_INDICATOR_AREA          "gimp-toolbox-indicator-area"
#define GIMP_HELP_TOOLBOX_DEFAULT_COLORS          "gimp-toolbox-default-colors"
#define GIMP_HELP_TOOLBOX_SWAP_COLORS             "gimp-toolbox-swap-colors"

#define GIMP_HELP_BRUSH_DIALOG                    "gimp-brush-dialog"
#define GIMP_HELP_BRUSH_NEW                       "gimp-brush-new"
#define GIMP_HELP_BRUSH_DUPLICATE                 "gimp-brush-duplicate"
#define GIMP_HELP_BRUSH_EDIT                      "gimp-brush-edit"
#define GIMP_HELP_BRUSH_DELETE                    "gimp-brush-delete"
#define GIMP_HELP_BRUSH_REFRESH                   "gimp-brush-refresh"

#define GIMP_HELP_BRUSH_EDITOR_DIALOG             "gimp-brush-editor-dialog"

#define GIMP_HELP_PATTERN_DIALOG                  "gimp-pattern-dialog"
#define GIMP_HELP_PATTERN_NEW                     "gimp-pattern-new"
#define GIMP_HELP_PATTERN_DUPLICATE               "gimp-pattern-duplicate"
#define GIMP_HELP_PATTERN_EDIT                    "gimp-pattern-edit"
#define GIMP_HELP_PATTERN_DELETE                  "gimp-pattern-delete"
#define GIMP_HELP_PATTERN_REFRESH                 "gimp-pattern-refresh"

#define GIMP_HELP_GRADIENT_DIALOG                 "gimp-gradient-dialog"
#define GIMP_HELP_GRADIENT_NEW                    "gimp-gradient-new"
#define GIMP_HELP_GRADIENT_DUPLICATE              "gimp-gradient-duplicate"
#define GIMP_HELP_GRADIENT_EDIT                   "gimp-gradient-edit"
#define GIMP_HELP_GRADIENT_DELETE                 "gimp-gradient-delete"
#define GIMP_HELP_GRADIENT_REFRESH                "gimp-gradient-refresh"
#define GIMP_HELP_GRADIENT_SAVE_AS_POV            "gimp-gradient-save-as-pov"

#define GIMP_HELP_GRADIENT_EDITOR_DIALOG          "gimp-gradient-editor-dialog"
#define GIMP_HELP_GRADIENT_EDITOR_LEFT_COLOR      "gimp-gradient-editor-left-color"
#define GIMP_HELP_GRADIENT_EDITOR_LEFT_LOAD       "gimp-gradient-editor-left-load"
#define GIMP_HELP_GRADIENT_EDITOR_LEFT_SAVE       "gimp-gradient-editor-left-save"
#define GIMP_HELP_GRADIENT_EDITOR_RIGHT_COLOR     "gimp-gradient-editor-right-color"
#define GIMP_HELP_GRADIENT_EDITOR_RIGHT_LOAD      "gimp-gradient-editor-right-load"
#define GIMP_HELP_GRADIENT_EDITOR_RIGHT_SAVE      "gimp-gradient-editor-right-save"
#define GIMP_HELP_GRADIENT_EDITOR_BLENDING        "gimp-gradient-editor-blending"
#define GIMP_HELP_GRADIENT_EDITOR_COLORING        "gimp-gradient-editor-coloring"
#define GIMP_HELP_GRADIENT_EDITOR_FLIP            "gimp-gradient-editor-flip"
#define GIMP_HELP_GRADIENT_EDITOR_REPLICATE       "gimp-gradient-editor-replicate"
#define GIMP_HELP_GRADIENT_EDITOR_SPLIT_MIDPOINT  "gimp-gradient-editor-split-midpoint"
#define GIMP_HELP_GRADIENT_EDITOR_SPLIT_UNIFORM   "gimp-gradient-editor-split-uniform"
#define GIMP_HELP_GRADIENT_EDITOR_DELETE          "gimp-gradient-editor-delete"
#define GIMP_HELP_GRADIENT_EDITOR_RECENTER        "gimp-gradient-editor-recenter"
#define GIMP_HELP_GRADIENT_EDITOR_REDISTRIBUTE    "gimp-gradient-editor-redistribute"
#define GIMP_HELP_GRADIENT_EDITOR_BLEND_COLOR     "gimp-gradient-editor-blend-color"
#define GIMP_HELP_GRADIENT_EDITOR_BLEND_OPACITY   "gimp-gradient-editor-blend-opacity"
#define GIMP_HELP_GRADIENT_EDITOR_ZOOM_OUT        "gimp-gradient-editor-zoom-out"
#define GIMP_HELP_GRADIENT_EDITOR_ZOOM_IN         "gimp-gradient-editor-zoom-in"
#define GIMP_HELP_GRADIENT_EDITOR_ZOOM_ALL        "gimp-gradient-editor-zoom-all"

#define GIMP_HELP_PALETTE_DIALOG                  "gimp-palette-dialog"
#define GIMP_HELP_PALETTE_NEW                     "gimp-palette-new"
#define GIMP_HELP_PALETTE_DUPLICATE               "gimp-palette-duplicate"
#define GIMP_HELP_PALETTE_EDIT                    "gimp-palette-edit"
#define GIMP_HELP_PALETTE_DELETE                  "gimp-palette-delete"
#define GIMP_HELP_PALETTE_REFRESH                 "gimp-palette-refresh"
#define GIMP_HELP_PALETTE_IMPORT                  "gimp-palette-import"
#define GIMP_HELP_PALETTE_MERGE                   "gimp-palette-merge"

#define GIMP_HELP_PALETTE_EDITOR_DIALOG           "gimp-palette-editor-dialog"
#define GIMP_HELP_PALETTE_EDITOR_NEW              "gimp-palette-editor-new"
#define GIMP_HELP_PALETTE_EDITOR_EDIT             "gimp-palette-editor-edit"
#define GIMP_HELP_PALETTE_EDITOR_DELETE           "gimp-palette-editor-delete"
#define GIMP_HELP_PALETTE_EDITOR_ZOOM_OUT         "gimp-palette-editor-zoom-out"
#define GIMP_HELP_PALETTE_EDITOR_ZOOM_IN          "gimp-palette-editor-zoom-in"
#define GIMP_HELP_PALETTE_EDITOR_ZOOM_ALL         "gimp-palette-editor-zoom-all"

#define GIMP_HELP_FONT_DIALOG                     "gimp-font-dialog"
#define GIMP_HELP_FONT_REFRESH                    "gimp-font-refresh"

#define GIMP_HELP_BUFFER_DIALOG                   "gimp-buffer-dialog"
#define GIMP_HELP_BUFFER_CUT                      "gimp-buffer-cut"
#define GIMP_HELP_BUFFER_COPY                     "gimp-buffer-copy"
#define GIMP_HELP_BUFFER_PASTE                    "gimp-buffer-paste"
#define GIMP_HELP_BUFFER_PASTE_INTO               "gimp-buffer-paste-into"
#define GIMP_HELP_BUFFER_PASTE_AS_NEW             "gimp-buffer-paste-as-new"
#define GIMP_HELP_BUFFER_DELETE                   "gimp-buffer-delete"

#define GIMP_HELP_DOCUMENT_DIALOG                 "gimp-document-dialog"
#define GIMP_HELP_DOCUMENT_OPEN                   "gimp-document-open"
#define GIMP_HELP_DOCUMENT_REMOVE                 "gimp-document-remove"
#define GIMP_HELP_DOCUMENT_REFRESH                "gimp-document-refresh"

#define GIMP_HELP_TEMPLATE_DIALOG                 "gimp-template-dialog"
#define GIMP_HELP_TEMPLATE_NEW                    "gimp-template-new"
#define GIMP_HELP_TEMPLATE_DUPLICATE              "gimp-template-duplicate"
#define GIMP_HELP_TEMPLATE_EDIT                   "gimp-template-edit"
#define GIMP_HELP_TEMPLATE_IMAGE_NEW              "gimp-template-image-new"
#define GIMP_HELP_TEMPLATE_DELETE                 "gimp-template-delete"

#define GIMP_HELP_TOOL_OPTIONS_DIALOG             "gimp-tool-options-dialog"
#define GIMP_HELP_TOOL_OPTIONS_SAVE               "gimp-tool-options-save"
#define GIMP_HELP_TOOL_OPTIONS_RESTORE            "gimp-tool-options-restore"
#define GIMP_HELP_TOOL_OPTIONS_RENAME             "gimp-tool-options-rename"
#define GIMP_HELP_TOOL_OPTIONS_DELETE             "gimp-tool-options-delete"
#define GIMP_HELP_TOOL_OPTIONS_RESET              "gimp-tool-options-reset"

#define GIMP_HELP_ERRORS_DIALOG                   "gimp-errors-dialog"
#define GIMP_HELP_ERRORS_CLEAR                    "gimp-errors-clear"
#define GIMP_HELP_ERRORS_SAVE                     "gimp-errors-save"

#define GIMP_HELP_PREFS_DIALOG                    "gimp-prefs-dialog"
#define GIMP_HELP_PREFS_NEW_IMAGE                 "gimp-prefs-new-image"
#define GIMP_HELP_PREFS_NEW_IMAGE_COMMENT         "gimp-prefs-new-image-comment"
#define GIMP_HELP_PREFS_NEW_IMAGE_GRID            "gimp-prefs-new-image-grid"
#define GIMP_HELP_PREFS_INTERFACE                 "gimp-prefs-interface"
#define GIMP_HELP_PREFS_HELP                      "gimp-prefs-help"
#define GIMP_HELP_PREFS_TOOL_OPTIONS              "gimp-prefs-tool-options"
#define GIMP_HELP_PREFS_INPUT_DEVICES             "gimp-prefs-input-devices"
#define GIMP_HELP_PREFS_IMAGE_WINDOW              "gimp-prefs-image-window"
#define GIMP_HELP_PREFS_IMAGE_WINDOW_APPEARANCE   "gimp-prefs-image-window-appearance"
#define GIMP_HELP_PREFS_IMAGE_WINDOW_TITLE        "gimp-prefs-image-window-title"
#define GIMP_HELP_PREFS_DISPLAY                   "gimp-prefs-display"
#define GIMP_HELP_PREFS_MONITOR                   "gimp-prefs-monitor"
#define GIMP_HELP_PREFS_ENVIRONMENT               "gimp-prefs-environment"
#define GIMP_HELP_PREFS_SESSION                   "gimp-prefs-session"
#define GIMP_HELP_PREFS_FOLDERS                   "gimp-prefs-folders"
#define GIMP_HELP_PREFS_FOLDERS_BRUSHES           "gimp-prefs-folders-brushes"
#define GIMP_HELP_PREFS_FOLDERS_PATTERNS          "gimp-prefs-folders-patterns"
#define GIMP_HELP_PREFS_FOLDERS_PALETTES          "gimp-prefs-folders-palettes"
#define GIMP_HELP_PREFS_FOLDERS_GRADIENTS         "gimp-prefs-folders-gradients"
#define GIMP_HELP_PREFS_FOLDERS_PLUG_INS          "gimp-prefs-folders-plug-ins"
#define GIMP_HELP_PREFS_FOLDERS_SCRIPTS           "gimp-prefs-folders-scripts"
#define GIMP_HELP_PREFS_FOLDERS_MODULES           "gimp-prefs-folders-modules"
#define GIMP_HELP_PREFS_FOLDERS_ENVIRONMENT       "gimp-prefs-folders-environment"
#define GIMP_HELP_PREFS_FOLDERS_THEMES            "gimp-prefs-folders-themes"

#define GIMP_HELP_INDEXED_PALETTE_DIALOG          "gimp-indexed-palette-dialog"
#define GIMP_HELP_INDEXED_PALETTE_EDIT            "gimp-indexed-palette-edit"
#define GIMP_HELP_INDEXED_PALETTE_ADD             "gimp-indexed-palette-add"

#define GIMP_HELP_DOCK                            "gimp-dock"
#define GIMP_HELP_DOCK_IMAGE_MENU                 "gimp-dock-image-menu"
#define GIMP_HELP_DOCK_AUTO_BUTTON                "gimp-dock-auto-button"
#define GIMP_HELP_DOCK_TAB_ADD                    "gimp-dock-tab-add"
#define GIMP_HELP_DOCK_TAB_CLOSE                  "gimp-dock-tab-close"
#define GIMP_HELP_DOCK_TAB_DETACH                 "gimp-dock-tab-detach"
#define GIMP_HELP_DOCK_PREVIEW_SIZE               "gimp-dock-preview-size"
#define GIMP_HELP_DOCK_TAB_STYLE                  "gimp-dock-tab-style"
#define GIMP_HELP_DOCK_VIEW_AS_LIST               "gimp-dock-view-as-list"
#define GIMP_HELP_DOCK_VIEW_AS_GRID               "gimp-dock-view-as-grid"

#define GIMP_HELP_ABOUT_DIALOG                    "gimp-about-dialog"
#define GIMP_HELP_COLOR_DIALOG                    "gimp-color-dialog"
#define GIMP_HELP_DEVICE_STATUS_DIALOG            "gimp-device-status-dialog"
#define GIMP_HELP_DISPLAY_FILTER_DIALOG           "gimp-display-filter-dialog"
#define GIMP_HELP_HISTOGRAM_DIALOG                "gimp-histogram-dialog"
#define GIMP_HELP_INFO_DIALOG                     "gimp-info-dialog"
#define GIMP_HELP_MODULE_DIALOG                   "gimp-module-dialog"
#define GIMP_HELP_NAVIGATION_DIALOG               "gimp-navigation-dialog"
#define GIMP_HELP_TEXT_EDITOR_DIALOG              "gimp-text-editor-dialog"
#define GIMP_HELP_TIPS_DIALOG                     "gimp-tips-dialog"
#define GIMP_HELP_UNDO_DIALOG                     "gimp-undo-dialog"

#define GIMP_HELP_HELP                            "gimp-help"
#define GIMP_HELP_HELP_CONTEXT                    "gimp-help-context"


#endif /* __GIMP_HELP_IDS_H__ */
