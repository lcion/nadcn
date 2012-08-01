/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "cgpassf.h"

FD_cgpassf *create_form_cgpassf(void)
{
  FL_OBJECT *obj;
  FD_cgpassf *fdui = (FD_cgpassf *) fl_calloc(1, sizeof(*fdui));

  fdui->cgpassf = fl_bgn_form(FL_NO_BOX, 290, 150);
  obj = fl_add_box(FL_UP_BOX,0,0,290,150,"");
  obj = fl_add_button(FL_RETURN_BUTTON,10,105,70,30,"Ok");
    fl_set_object_callback(obj,cb_cgp_change,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,200,105,70,30,"Cancel");
    fl_set_button_shortcut(obj,"^C",1);
    fl_set_object_callback(obj,cb_cgp_nochange,0);
  fdui->cgp_userOb = obj = fl_add_input(FL_NORMAL_INPUT,100,20,90,20,"User : ");
    fl_set_object_callback(obj,cb_cgp_upcinp,0);
  fdui->cgp_pass0Ob = obj = fl_add_input(FL_SECRET_INPUT,100,50,90,20,"Pasword : ");
    fl_set_object_callback(obj,cb_cgp_upcinp,0);
  fdui->cgp_pass1Ob = obj = fl_add_input(FL_SECRET_INPUT,100,80,90,20,"Check Password : ");
    fl_set_object_callback(obj,cb_cgp_upcinp,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,110,105,70,30,"Clear");
    fl_set_button_shortcut(obj,"^L",1);
    fl_set_object_callback(obj,cb_cgp_clear,0);
  fl_end_form();

  fdui->cgpassf->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

