/*********************************************************/
/* Image manipulation routines. Calls mapobject_shade.c  */
/* functions to compute the shading of the image at each */
/* pixel. These routines are used by the functions in    */
/* mapobject_preview.c and mapobject_apply.c             */
/*********************************************************/

#include <sys/types.h>

#include <gck/gck.h>

#include <libgimp/gimp.h>

#include "mapobject_main.h"
#include "mapobject_preview.h"
#include "mapobject_shade.h"
#include "mapobject_ui.h"
#include "mapobject_image.h"

GDrawable *input_drawable,*output_drawable;
GPixelRgn source_region,dest_region;

GDrawable *box_drawables[6];
GPixelRgn box_regions[6];

GDrawable *cylinder_drawables[2];
GPixelRgn cylinder_regions[2];

guchar   *preview_rgb_data = NULL;
GdkImage *image = NULL;

glong   maxcounter,old_depth,max_depth;
gint    imgtype,width,height,in_channels,out_channels;
GckRGB  background;
gdouble oldtreshold;

gint border_x1,border_y1,border_x2,border_y2;

/******************/
/* Implementation */
/******************/

GckRGB
peek (gint x,
      gint y)
{
  static guchar data[4];
  GckRGB color;

  gimp_pixel_rgn_get_pixel(&source_region,data,x,y);

  color.r=(gdouble)(data[0])/255.0;
  color.g=(gdouble)(data[1])/255.0;
  color.b=(gdouble)(data[2])/255.0;

  if (input_drawable->bpp==4)
    {
      if (in_channels==4)
        color.a=(gdouble)(data[3])/255.0;
      else
        color.a=1.0;
    }
  else
    color.a=1.0;

  return(color);
}

static GckRGB
peek_box_image (gint image,
		gint x,
		gint y)
{
  static guchar data[4];
  GckRGB color;

  gimp_pixel_rgn_get_pixel(&box_regions[image],data,x,y);

  color.r=(gdouble)(data[0])/255.0;
  color.g=(gdouble)(data[1])/255.0;
  color.b=(gdouble)(data[2])/255.0;

  if (box_drawables[image]->bpp==4)
    {
      if (gimp_drawable_has_alpha(box_drawables[image]->id))
        color.a=(gdouble)(data[3])/255.0;
      else
        color.a=1.0;
    }
  else
    color.a=1.0;

  return(color);
}

static GckRGB
peek_cylinder_image (gint image,
		     gint x,
		     gint y)
{
  static guchar data[4];
  GckRGB color;

  gimp_pixel_rgn_get_pixel(&cylinder_regions[image],data,x,y);

  color.r=(gdouble)(data[0])/255.0;
  color.g=(gdouble)(data[1])/255.0;
  color.b=(gdouble)(data[2])/255.0;

  if (cylinder_drawables[image]->bpp==4)
    {
      if (gimp_drawable_has_alpha(cylinder_drawables[image]->id))
        color.a=(gdouble)(data[3])/255.0;
      else
        color.a=1.0;
    }
  else
    color.a=1.0;

  return(color);
}

void
poke (gint    x,
      gint    y,
      GckRGB *color)
{
  static guchar data[4];
  
  data[0]=(guchar)(color->r*255.0);
  data[1]=(guchar)(color->g*255.0);
  data[2]=(guchar)(color->b*255.0);
  data[3]=(guchar)(color->a*255.0);
  
  gimp_pixel_rgn_set_pixel(&dest_region,data,x,y);
}

gint
checkbounds (gint x,
	     gint y)
{
  if (x<border_x1 || y<border_y1 || x>=border_x2 || y>=border_y2)
    return(FALSE);
  else
    return(TRUE);
}

static gint
checkbounds_box_image (gint image,
		       gint x,
		       gint y)
{
  gint w,h;

  w = box_drawables[image]->width;
  h = box_drawables[image]->height;

  if (x<0 || y<0 || x>=w || y>=h)
    return(FALSE);
  else
    return(TRUE);
}

static gint
checkbounds_cylinder_image (gint image,
			    gint x,
			    gint y)
{
  gint w,h;

  w = cylinder_drawables[image]->width;
  h = cylinder_drawables[image]->height;

  if (x<0 || y<0 || x>=w || y>=h)
    return(FALSE);
  else
    return(TRUE);
}

GimpVector3
int_to_pos (gint x,
	    gint y)
{
  GimpVector3 pos;

  pos.x=(gdouble)x/(gdouble)width;
  pos.y=(gdouble)y/(gdouble)height;
  pos.z=0.0;

  return(pos);
}

void
pos_to_int (gdouble  x,
	    gdouble  y,
	    gint    *scr_x,
	    gint    *scr_y)
{
  *scr_x=(gint)((x*(gdouble)width));
  *scr_y=(gint)((y*(gdouble)height));
}

/**********************************************/
/* Compute the image color at pos (u,v) using */
/* Quartics bilinear interpolation stuff.     */
/**********************************************/

GckRGB
get_image_color (gdouble  u,
		 gdouble  v,
		 gint    *inside)
{
  gint   x1, y1, x2, y2;
  GckRGB p[4];
 
  pos_to_int(u,v,&x1,&y1);

  if (mapvals.tiled==TRUE)
    {
      *inside=TRUE;
      if (x1 < 0) x1 = (width-1) - (-x1 % width);
      else        x1 = x1 % width;
  
      if (y1 < 0) y1 = (height-1) - (-y1 % height);
      else        y1 = y1 % height;

      x2 = (x1 + 1) % width;
      y2 = (y1 + 1) % height;

      p[0] = peek(x1, y1);
      p[1] = peek(x2, y1);
      p[2] = peek(x1, y2);
      p[3] = peek(x2, y2);
      return(gck_bilinear_rgba(u * width, v * height, p));
    }

  if (checkbounds(x1,y1)==FALSE)
    {
      *inside=FALSE;
      return(background);
    }

  x2 = (x1 + 1);
  y2 = (y1 + 1);

  if (checkbounds(x2,y2)==FALSE)
   {
     *inside=TRUE;
     return(peek(x1,y1));
   }

  *inside=TRUE;
  p[0] = peek(x1, y1);
  p[1] = peek(x2, y1);
  p[2] = peek(x1, y2);
  p[3] = peek(x2, y2);
  return(gck_bilinear_rgba(u * width, v * height, p));
}

GckRGB
get_box_image_color (gint    image,
		     gdouble u,
		     gdouble v)
{
  gint   w,h, x1, y1, x2, y2;
  GckRGB p[4];
 
  w = box_drawables[image]->width;
  h = box_drawables[image]->height;

  x1 = (gint)((u*(gdouble)w));
  y1 = (gint)((v*(gdouble)h));

  if (checkbounds_box_image(image, x1,y1)==FALSE)
    return(background);

  x2 = (x1 + 1);
  y2 = (y1 + 1);

  if (checkbounds_box_image(image, x2,y2)==FALSE)
    return(peek_box_image(image, x1,y1));

  p[0] = peek_box_image(image, x1, y1);
  p[1] = peek_box_image(image, x2, y1);
  p[2] = peek_box_image(image, x1, y2);
  p[3] = peek_box_image(image, x2, y2);

  return(gck_bilinear_rgba(u*w, v*h, p));
}

GckRGB
get_cylinder_image_color (gint    image,
			  gdouble u,
			  gdouble v)
{
  gint   w,h, x1, y1, x2, y2;
  GckRGB p[4];
 
  w = cylinder_drawables[image]->width;
  h = cylinder_drawables[image]->height;

  x1 = (gint)((u*(gdouble)w));
  y1 = (gint)((v*(gdouble)h));

  if (checkbounds_cylinder_image(image, x1,y1)==FALSE)
    return(background);

  x2 = (x1 + 1);
  y2 = (y1 + 1);

  if (checkbounds_cylinder_image(image, x2,y2)==FALSE)
    return(peek_cylinder_image(image, x1,y1));

  p[0] = peek_cylinder_image(image, x1, y1);
  p[1] = peek_cylinder_image(image, x2, y1);
  p[2] = peek_cylinder_image(image, x1, y2);
  p[3] = peek_cylinder_image(image, x2, y2);

  return(gck_bilinear_rgba(u*w, v*h, p));
}

/****************************************/
/* Allocate memory for temporary images */
/****************************************/

gint
image_setup (GDrawable *drawable,
	     gint       interactive)
{
  glong numbytes;

  /* Set the tile cache size */
  /* ======================= */

  gimp_tile_cache_ntiles ((drawable->width + gimp_tile_width() - 1) /
			  gimp_tile_width());
 
  /* Get some useful info on the input drawable */
  /* ========================================== */

  input_drawable=drawable;
  output_drawable=drawable;

  gimp_drawable_mask_bounds (drawable->id,
			     &border_x1, &border_y1, &border_x2, &border_y2);

  width=input_drawable->width;
  height=input_drawable->height;

  gimp_pixel_rgn_init (&source_region, input_drawable,
		       0, 0, width, height, FALSE, FALSE);

  maxcounter=(glong)width*(glong)height;

  /* Assume at least RGB */
  /* =================== */

  in_channels=3;
  if (gimp_drawable_has_alpha(input_drawable->id)==TRUE)
    in_channels++;

  if (interactive==TRUE)
    {
      /* Allocate memory for temp. images */
      /* ================================ */
    
      numbytes=PREVIEW_HEIGHT*PREVIEW_WIDTH*3;
    
      image=gdk_image_new(GDK_IMAGE_FASTEST,visinfo->visual,
			  PREVIEW_WIDTH,PREVIEW_HEIGHT);
      if (image==NULL)
        return(FALSE);

      preview_rgb_data=(guchar *)g_malloc((size_t)numbytes);
      if (preview_rgb_data==NULL)
        return(FALSE);
      memset(preview_rgb_data,0,numbytes);
    
      /* Convert from raw RGB to GdkImage */
      /* ================================ */

      gck_rgb_to_gdkimage(visinfo,preview_rgb_data,image,
			  PREVIEW_WIDTH,PREVIEW_HEIGHT);
    }

  return(TRUE);
}
