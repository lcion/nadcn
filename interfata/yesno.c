/* A box with two buttons and a string. Simple boxes like this
   are very usefull for asking questions
*/

#include "forms.h"

int
main(int argc, char *argv[])
{
   FL_FORM *form;
   FL_OBJECT *yes;
  
  fl_initialize(&argc, argv, "FormDemo", 0, 0);
  form = fl_bgn_form(FL_UP_BOX,320,120);
    fl_add_box(FL_NO_BOX,160,40,0,0,"Do you want to Quit?");
    yes = fl_add_button(FL_NORMAL_BUTTON,40,70,80,30,"Yes");
    fl_add_button(FL_NORMAL_BUTTON,200,70,80,30,"No");
  fl_end_form();

  fl_show_form(form,FL_PLACE_MOUSE,FL_TRANSIENT,"Question");

  while (fl_do_forms() != yes)
   ;

  fl_hide_form(form);
  return 0;
}

