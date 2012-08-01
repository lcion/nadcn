/* A box with two buttons and a string. Simple boxes like this
   are very usefull for asking questions
*/

#include "forms.h"

void yes_callback(FL_OBJECT *ob, long user_data){
	printf("Yes was called, so Bye!\n");
	fl_finish();
	exit(0);
}

void no_callback(FL_OBJECT *ob, long user_data){
	printf("No was called, so Try Again\n");
}


void main(int argc, char *argv[]){

   FL_FORM *form;
   FL_OBJECT *obj;
  
  fl_initialize(&argc, argv, "FormDemo", 0, 0);
  form = fl_bgn_form(FL_UP_BOX,320,120);
    fl_add_box(FL_NO_BOX,160,40,0,0,"Do you want to Quit?");
    obj = fl_add_button(FL_NORMAL_BUTTON,40,70,80,30,"Yes");
    fl_set_object_callback(obj, yes_callback,0);
    obj = fl_add_button(FL_NORMAL_BUTTON,200,70,80,30,"No");
    fl_set_object_callback(obj, no_callback,0);
  fl_end_form();

  fl_show_form(form,FL_PLACE_MOUSE,FL_TRANSIENT,"Question");

  fl_do_forms();
}

