/* gimpparasite.h: Copyright 1998 Jay Cox <jaycox@earthlink.net> 
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

#ifndef __APP_GIMP_PARASITE_H__
#define __APP_GIMP_PARASITE_H__


void            gimp_parasites_init  (Gimp         *gimp);
void            gimp_parasites_exit  (Gimp         *gimp);

void            gimp_parasite_attach (Gimp         *gimp,
				      GimpParasite *parasite);
void            gimp_parasite_detach (Gimp         *gimp,
				      const gchar  *name);

GimpParasite  * gimp_parasite_find   (Gimp         *gimp,
				      const gchar  *name);
gchar        ** gimp_parasite_list   (Gimp         *gimp,
				      gint         *count);

void            gimp_parasiterc_load (Gimp         *gimp);
void            gimp_parasiterc_save (Gimp         *gimp);


#endif  /*  __APP_GIMP_PARASITE_H__  */
