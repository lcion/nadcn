/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "inpforocc.h"

FD_inpforocc *create_form_inpforocc(void)
{
  FL_OBJECT *obj;
  FD_inpforocc *fdui = (FD_inpforocc *) fl_calloc(1, sizeof(*fdui));

  fdui->inpforocc = fl_bgn_form(FL_NO_BOX, 320, 110);
  obj = fl_add_box(FL_UP_BOX,0,0,320,110,"");
  obj = fl_add_frame(FL_ENGRAVED_FRAME,30,20,255,50,"");
  obj = fl_add_button(FL_RETURN_BUTTON,10,80,70,20,"Ok");
    fl_set_object_callback(obj,cb_inpfo_ok,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,110,80,90,20,"Clear");
    fl_set_button_shortcut(obj,"^L",1);
    fl_set_object_callback(obj,cb_inpfo_clear,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,230,80,80,20,"Cancel");
    fl_set_button_shortcut(obj,"^C",1);
    fl_set_object_callback(obj,cb_inpfo_cancel,0);
  fdui->inpfo_inpOb = obj = fl_add_input(FL_SECRET_INPUT,40,40,235,20,"Introduceti parola root pentru identificare:");
    fl_set_object_lalign(obj,FL_ALIGN_TOP_LEFT);
    fl_set_object_callback(obj,cb_inpfo_edit,0);
  fl_end_form();

  fdui->inpforocc->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

