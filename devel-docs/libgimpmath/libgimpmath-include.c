/*  
 *   gtk-doc can't build libgimpmath-scan.c without PLUG_IN_INFO being defined
 *   so we include this file as a workaround 
 */

#include <glib.h>
#include <libgimp/gimp.h>

GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,
  NULL, 
  NULL, 
  NULL,
};
