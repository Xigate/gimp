#include "libgimp/gimpintl.h"
#include <locale.h>

#ifndef LOCALEDIR
#define LOCALEDIR g_strconcat (gimp_data_directory (), \
			       G_DIR_SEPARATOR_S, \
			       "locale", \
			       NULL)
#endif

#ifdef HAVE_LC_MESSAGES
#define INIT_I18N() \
  setlocale(LC_MESSAGES, ""); \
  bindtextdomain("gimp-std-plugins", LOCALEDIR); \
  textdomain("gimp-std-plugins")

#else
#define INIT_I18N() \
  bindtextdomain("gimp-std-plugins", LOCALEDIR); \
  textdomain("gimp-std-plugins")
#endif
