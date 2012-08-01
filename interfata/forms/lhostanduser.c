/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "lhostanduser.h"

FD_hostanduser *create_form_hostanduser(void)
{
  FL_OBJECT *obj;
  FD_hostanduser *fdui = (FD_hostanduser *) fl_calloc(1, sizeof(*fdui));

  fdui->hostanduser = fl_bgn_form(FL_NO_BOX, 300, 170);
  obj = fl_add_box(FL_UP_BOX,0,0,300,170,"Login");
  obj = fl_add_frame(FL_ENGRAVED_FRAME,20,20,260,130,"");
  obj = fl_add_input(FL_NORMAL_INPUT,95,40,140,30,"Host:");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
  obj = fl_add_input(FL_SECRET_INPUT,95,80,140,30,"Password:");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
  obj = fl_add_button(FL_NORMAL_BUTTON,40,120,80,20,"Accept");
    fl_set_object_callback(obj,cb_hau_acc,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,180,120,70,20,"Cancel");
    fl_set_object_callback(obj,cb_hau_quit,0);
  fl_end_form();

  fdui->hostanduser->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

