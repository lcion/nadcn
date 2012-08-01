/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "unCeasfrm.h"

FD_unCeas *create_form_unCeas(void)
{
  FL_OBJECT *obj;
  FD_unCeas *fdui = (FD_unCeas *) fl_calloc(1, sizeof(*fdui));

  fdui->unCeas = fl_bgn_form(FL_NO_BOX, 140, 190);
  obj = fl_add_box(FL_UP_BOX,0,0,140,190,"");
  fdui->retOB = obj = fl_add_button(FL_NORMAL_BUTTON,15,150,110,30,"Return");
    fl_set_object_callback(obj,unc_ret_cb,0);
  obj = fl_add_clock(FL_ANALOG_CLOCK,5,10,130,130,"");
    fl_set_object_color(obj,FL_INACTIVE,FL_WHITE);
  fl_end_form();

  fdui->unCeas->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

