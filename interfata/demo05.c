/* This demo shows the use of a slider. Every time the slider changes
   position it is returned by do_forms() and the text field showing its
   value is adapted.
*/

#include <stdio.h>
#include "forms.h"

void slider_cb(FL_OBJECT *ob, long data)
{
    char str[30];
    sprintf(str,"%f",fl_get_slider_value(ob));
    fl_set_object_label((FL_OBJECT *)data,str);
}

int
main(int argc, char *argv[])
{
  FL_FORM *form;
  FL_OBJECT *slider, *value;

  fl_initialize(&argc, argv, "FormDemo", 0, 0);
  form = fl_bgn_form(FL_UP_BOX,240,400);
     value = fl_add_box(FL_DOWN_BOX,120,180,100,30,"");
     fl_set_object_lalign(value,FL_ALIGN_CENTER);
     slider = fl_add_slider(FL_VERT_SLIDER,40,40,60,320,"");
     fl_set_slider_bounds(slider,-1,1);
     fl_set_slider_value(slider,0);
     fl_set_object_color(slider,FL_SLIDER_COL1,FL_GREEN);
     fl_set_object_callback(slider,slider_cb,(long)value);
     fl_add_button(FL_RETURN_BUTTON,120,290,100,30,"Exit");
  fl_end_form();

  fl_set_object_label(value, "0.000000");

  fl_show_form(form,FL_PLACE_CENTER,FL_NOBORDER,"Slider");
  fl_do_forms();

  return 0;
}
