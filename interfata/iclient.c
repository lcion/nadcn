/* A box with two buttons and a string. Simple boxes like this
 */


#include "forms.h"
#include "lhostanduser.h"

FD_hostanduser *hauf;

void leave_cback(FL_OBJECT *ob, long user_data){
	printf("Leave was called, so Bye!\n");
q	fl_finish();
	exit(0);
}

void cb_hau_acc(FL_OBJECT *ob, long user_data){
   printf("Lansez clientul \n");
}
void cb_hau_quit(FL_OBJECT *ob, long user_data){
   fl_hide_form(hauf->hostanduser);
}

void connect_cback(FL_OBJECT *ob, long user_data){

   hauf = create_form_hostanduser();
   fl_show_form(hauf->hostanduser,FL_PLACE_MOUSE,FL_TRANSIENT,"LOGIN Form");

  fl_do_forms();

   //   printf("Aici va interveni o interfata de conectare\n");
}


void main(int argc, char *argv[]){

   FL_FORM *form;
   FL_OBJECT *obj;
  
  fl_initialize(&argc, argv, "FormDemo", 0, 0);
  form = fl_bgn_form(FL_UP_BOX,320,120);
    fl_add_box(FL_NO_BOX,160,40,0,0,"Welcome to LanAdmin");
    obj = fl_add_button(FL_NORMAL_BUTTON,40,70,80,30,"Connect");
    fl_set_object_callback(obj, connect_cback,0);
    obj = fl_add_button(FL_NORMAL_BUTTON,200,70,80,30,"Leave");
    fl_set_object_callback(obj, leave_cback,0);
  fl_end_form();

  fl_show_form(form,FL_PLACE_MOUSE,FL_TRANSIENT,"LanAdmin");

  fl_do_forms();
}
