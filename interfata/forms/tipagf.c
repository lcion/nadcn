/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "tipagf.h"

FD_tipagf *create_form_tipagf(void)
{
  FL_OBJECT *obj;
  FD_tipagf *fdui = (FD_tipagf *) fl_calloc(1, sizeof(*fdui));

  fdui->tipagf = fl_bgn_form(FL_NO_BOX, 250, 180);
  obj = fl_add_box(FL_UP_BOX,0,0,250,180,"");
  fdui->my_timer = obj = fl_add_timer(FL_HIDDEN_TIMER,5,5,70,30,"Timer");
    fl_set_object_callback(obj,timer_cb,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,90,5,70,30,"Done");
    fl_set_object_callback(obj,cb_exit,0);
  fdui->report1 = obj = fl_add_text(FL_NORMAL_TEXT,10,35,230,20,"report1");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->report2 = obj = fl_add_text(FL_NORMAL_TEXT,10,55,230,20,"report2");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->chartOB = obj = fl_add_chart(FL_BAR_CHART,30,80,90,90,"");
  fl_end_form();

  fdui->tipagf->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

