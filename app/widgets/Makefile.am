## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DG_LOG_DOMAIN=\"Gimp-Widgets\"

INCLUDES = \
	-I$(top_builddir)	\
	-I$(top_srcdir)		\
	-I$(top_builddir)/app	\
	-I$(top_srcdir)/app	\
	$(GTK_CFLAGS)		\
	$(PANGOFT2_CFLAGS)	\
	$(DBUS_GLIB_CFLAGS)	\
	-I$(includedir)

noinst_LIBRARIES = libappwidgets.a

libappwidgets_a_sources = \
	widgets-enums.h			\
	widgets-types.h			\
	gimpaction.c			\
	gimpaction.h			\
	gimpactionfactory.c		\
	gimpactionfactory.h		\
	gimpactiongroup.c		\
	gimpactiongroup.h		\
	gimpactionview.c		\
	gimpactionview.h		\
	gimpblobeditor.c		\
	gimpblobeditor.h		\
	gimpbrusheditor.c		\
	gimpbrusheditor.h		\
	gimpbrushfactoryview.c		\
	gimpbrushfactoryview.h		\
	gimpbrushselect.c		\
	gimpbrushselect.h		\
	gimpbufferview.c		\
	gimpbufferview.h		\
	gimpcellrendererdashes.c	\
	gimpcellrendererdashes.h	\
	gimpcellrendererviewable.c	\
	gimpcellrendererviewable.h	\
	gimpchanneltreeview.c		\
	gimpchanneltreeview.h		\
	gimpclipboard.c			\
	gimpclipboard.h			\
	gimpcolorbar.c			\
	gimpcolorbar.h			\
	gimpcolordialog.c		\
	gimpcolordialog.h		\
	gimpcolordisplayeditor.c	\
	gimpcolordisplayeditor.h	\
	gimpcoloreditor.c		\
	gimpcoloreditor.h		\
	gimpcolorframe.c		\
	gimpcolorframe.h		\
	gimpcolormapeditor.c		\
	gimpcolormapeditor.h		\
	gimpcolorpanel.c		\
	gimpcolorpanel.h		\
	gimpcolorselectorpalette.c	\
	gimpcolorselectorpalette.h	\
	gimpcomponenteditor.c		\
	gimpcomponenteditor.h		\
	gimpcontainerbox.c		\
	gimpcontainerbox.h		\
	gimpcontainercombobox.c		\
	gimpcontainercombobox.h		\
	gimpcontainereditor.c		\
	gimpcontainereditor.h		\
	gimpcontainerentry.c		\
	gimpcontainerentry.h		\
	gimpcontainergridview.c		\
	gimpcontainergridview.h		\
	gimpcontainerpopup.c		\
	gimpcontainerpopup.h		\
	gimpcontainertreeview.c		\
	gimpcontainertreeview.h		\
	gimpcontainertreeview-dnd.c	\
	gimpcontainertreeview-dnd.h	\
	gimpcontainerview.c		\
	gimpcontainerview.h		\
	gimpcontainerview-utils.c	\
	gimpcontainerview-utils.h	\
	gimpcontrollereditor.c		\
	gimpcontrollereditor.h		\
	gimpcontrollerinfo.c		\
	gimpcontrollerinfo.h		\
	gimpcontrollerlist.c		\
	gimpcontrollerlist.h		\
	gimpcontrollers.c		\
	gimpcontrollers.h		\
	gimpcontrollerkeyboard.c	\
	gimpcontrollerkeyboard.h	\
	gimpcontrollerwheel.c		\
	gimpcontrollerwheel.h		\
	gimpcursor.c			\
	gimpcursor.h			\
	gimpcursorview.c		\
	gimpcursorview.h		\
	gimpdasheditor.c		\
	gimpdasheditor.h		\
	gimpdataeditor.c		\
	gimpdataeditor.h		\
	gimpdatafactoryview.c		\
	gimpdatafactoryview.h		\
	gimpdbusservice.c		\
	gimpdbusservice.h		\
	gimpdeviceinfo.c		\
	gimpdeviceinfo.h		\
	gimpdevices.c			\
	gimpdevices.h			\
	gimpdevicestatus.c		\
	gimpdevicestatus.h		\
	gimpdialogfactory.c		\
	gimpdialogfactory.h		\
	gimpdnd.c			\
	gimpdnd.h			\
	gimpdnd-xds.c			\
	gimpdnd-xds.h			\
	gimpdock.c			\
	gimpdock.h			\
	gimpdockable.c			\
	gimpdockable.h			\
	gimpdockbook.c			\
	gimpdockbook.h			\
	gimpdocked.c			\
	gimpdocked.h			\
	gimpdockseparator.c		\
	gimpdockseparator.h		\
	gimpdocumentview.c		\
	gimpdocumentview.h		\
	gimpdrawabletreeview.c		\
	gimpdrawabletreeview.h		\
	gimpeditor.c			\
	gimpeditor.h			\
	gimpenumaction.c		\
	gimpenumaction.h		\
	gimperrorconsole.c		\
	gimperrorconsole.h		\
	gimperrordialog.c		\
	gimperrordialog.h		\
	gimpfgbgeditor.c		\
	gimpfgbgeditor.h		\
	gimpfgbgview.c			\
	gimpfgbgview.h			\
	gimpfiledialog.c		\
	gimpfiledialog.h		\
	gimpfileprocview.c		\
	gimpfileprocview.h		\
	gimpfontselect.c		\
	gimpfontselect.h		\
	gimpfontview.c			\
	gimpfontview.h			\
	gimpgradienteditor.c		\
	gimpgradienteditor.h		\
	gimpgradientselect.c		\
	gimpgradientselect.h		\
	gimpgrideditor.c		\
	gimpgrideditor.h		\
	gimphelp.c			\
	gimphelp.h			\
	gimphelp-ids.h			\
	gimphistogrambox.c		\
	gimphistogrambox.h		\
	gimphistogrameditor.c		\
	gimphistogrameditor.h		\
	gimphistogramview.c		\
	gimphistogramview.h		\
	gimpimagedock.c			\
	gimpimagedock.h			\
	gimpimagecommenteditor.c	\
	gimpimagecommenteditor.h	\
	gimpimageeditor.c		\
	gimpimageeditor.h		\
	gimpimageparasiteview.c		\
	gimpimageparasiteview.h		\
	gimpimageprofileview.c		\
	gimpimageprofileview.h		\
	gimpimagepropview.c		\
	gimpimagepropview.h		\
	gimpimageview.c			\
	gimpimageview.h			\
	gimpitemtreeview.c		\
	gimpitemtreeview.h		\
	gimplayertreeview.c		\
	gimplayertreeview.h		\
	gimpmenudock.c			\
	gimpmenudock.h			\
	gimpmenufactory.c		\
	gimpmenufactory.h		\
	gimpmessagebox.c		\
	gimpmessagebox.h		\
	gimpmessagedialog.c		\
	gimpmessagedialog.h		\
	gimpnavigationview.c		\
	gimpnavigationview.h		\
	gimppaletteeditor.c		\
	gimppaletteeditor.h		\
	gimppaletteselect.c		\
	gimppaletteselect.h		\
	gimppaletteview.c		\
	gimppaletteview.h		\
	gimppatternfactoryview.c	\
	gimppatternfactoryview.h	\
	gimppatternselect.c		\
	gimppatternselect.h		\
	gimppdbdialog.c			\
	gimppdbdialog.h			\
	gimppixbuf.c			\
	gimppixbuf.h			\
	gimppluginaction.c		\
	gimppluginaction.h		\
	gimpprofilechooserdialog.c	\
	gimpprofilechooserdialog.h	\
	gimpprogressbox.c		\
	gimpprogressbox.h		\
	gimpprogressdialog.c		\
	gimpprogressdialog.h		\
	gimppropwidgets.c		\
	gimppropwidgets.h		\
	gimprender.c			\
	gimprender.h			\
	gimpsamplepointeditor.c		\
	gimpsamplepointeditor.h		\
	gimpselectiondata.c		\
	gimpselectiondata.h		\
	gimpselectioneditor.c		\
	gimpselectioneditor.h		\
	gimpsessioninfo.c		\
	gimpsessioninfo.h		\
	gimpsessioninfo-aux.c		\
	gimpsessioninfo-aux.h		\
	gimpsessioninfo-book.c		\
	gimpsessioninfo-book.h		\
	gimpsessioninfo-dock.c		\
	gimpsessioninfo-dock.h		\
	gimpsessioninfo-dockable.c	\
	gimpsessioninfo-dockable.h	\
	gimpsizebox.c			\
	gimpsizebox.h			\
	gimpstringaction.c		\
	gimpstringaction.h		\
	gimpstrokeeditor.c		\
	gimpstrokeeditor.h		\
	gimptemplateeditor.c		\
	gimptemplateeditor.h		\
	gimptemplateview.c		\
	gimptemplateview.h		\
	gimptexteditor.c		\
	gimptexteditor.h		\
	gimpthumbbox.c			\
	gimpthumbbox.h			\
	gimptoolbox.c			\
	gimptoolbox.h			\
	gimptoolbox-color-area.c	\
	gimptoolbox-color-area.h	\
	gimptoolbox-dnd.c		\
	gimptoolbox-dnd.h		\
	gimptoolbox-image-area.c	\
	gimptoolbox-image-area.h	\
	gimptoolbox-indicator-area.c	\
	gimptoolbox-indicator-area.h	\
	gimptooldialog.c		\
	gimptooldialog.h		\
	gimptooloptionseditor.c		\
	gimptooloptionseditor.h		\
	gimptoolview.c			\
	gimptoolview.h			\
	gimpuimanager.c			\
	gimpuimanager.h			\
	gimpundoeditor.c		\
	gimpundoeditor.h		\
	gimpunitcombobox.c		\
	gimpunitcombobox.h		\
	gimpunitstore.c			\
	gimpunitstore.h			\
	gimpvectorstreeview.c		\
	gimpvectorstreeview.h		\
	gimpview.c			\
	gimpview.h			\
	gimpview-popup.c		\
	gimpview-popup.h		\
	gimpviewablebox.c		\
	gimpviewablebox.h		\
	gimpviewablebutton.c		\
	gimpviewablebutton.h		\
	gimpviewabledialog.c		\
	gimpviewabledialog.h		\
	gimpviewrenderer.c		\
	gimpviewrenderer.h		\
	gimpviewrenderer-frame.c	\
	gimpviewrenderer-frame.h	\
	gimpviewrenderer-utils.c	\
	gimpviewrenderer-utils.h	\
	gimpviewrendererbrush.c		\
	gimpviewrendererbrush.h		\
	gimpviewrendererbuffer.c	\
	gimpviewrendererbuffer.h	\
	gimpviewrendererdrawable.c	\
	gimpviewrendererdrawable.h	\
	gimpviewrenderergradient.c	\
	gimpviewrenderergradient.h	\
	gimpviewrendererimage.c 	\
	gimpviewrendererimage.h 	\
	gimpviewrendererimagefile.c	\
	gimpviewrendererimagefile.h	\
	gimpviewrendererlayer.c		\
	gimpviewrendererlayer.h		\
	gimpviewrendererpalette.c	\
	gimpviewrendererpalette.h	\
	gimpviewrenderervectors.c	\
	gimpviewrenderervectors.h	\
	gimpwidgets-constructors.c	\
	gimpwidgets-constructors.h	\
	gimpwidgets-utils.c		\
	gimpwidgets-utils.h		\
	gtkwrapbox.c			\
	gtkwrapbox.h			\
	gtkhwrapbox.c			\
	gtkhwrapbox.h			\
	gtkvwrapbox.c			\
	gtkvwrapbox.h

libappwidgets_a_built_sources = gimpdbusservice-glue.h widgets-enums.c

libappwidgets_a_SOURCES = \
	$(libappwidgets_a_built_sources) $(libappwidgets_a_sources)

EXTRA_DIST = \
	dbus-service.xml	\
	makefile.msc


#
# rules to generate built sources
#
# setup autogeneration dependencies
gen_sources = xgen-wec gimpdbusservice-glue.h
CLEANFILES = $(gen_sources)


$(srcdir)/gimpdbusservice.c: gimpdbusservice-glue.h

gimpdbusservice-glue.h: $(srcdir)/dbus-service.xml
	$(DBUS_BINDING_TOOL) --mode=glib-server --prefix=gimp $< > $(@F)


widgets-enums.c: $(srcdir)/widgets-enums.h $(GIMP_MKENUMS)
	$(GIMP_MKENUMS) \
		--fhead "#include \"config.h\"\n#include <gtk/gtk.h>\n#include \"libgimpbase/gimpbase.h\"\n#include \"widgets-enums.h\"\n#include \"gimp-intl.h\"" \
		--fprod "\n/* enumerations from \"@filename@\" */" \
		--vhead "GType\n@enum_name@_get_type (void)\n{\n  static const G@Type@Value values[] =\n  {" \
		--vprod "    { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
		--vtail "    { 0, NULL, NULL }\n  };\n" \
		--dhead "  static const Gimp@Type@Desc descs[] =\n  {" \
		--dprod "    { @VALUENAME@, @valuedesc@, @valuehelp@ }," \
		--dtail "    { 0, NULL, NULL }\n  };\n\n  static GType type = 0;\n\n  if (! type)\n    {\n      type = g_@type@_register_static (\"@EnumName@\", values);\n      gimp_@type@_set_value_descriptions (type, descs);\n    }\n\n  return type;\n}\n" \
		$(srcdir)/widgets-enums.h > xgen-wec \
	&& cp xgen-wec $(@F) \
	&& rm -f xgen-wec
