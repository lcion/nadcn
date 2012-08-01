#include "tipag.h"

long end_sec, start_sec, end_usec , start_usec;

void start_timer(FL_OBJECT *ob, long data){
FD_tipagf *fd = ob->form->fdui;
char buf[128];

//	fd->ldata += 200;
	sprintf(buf,"Timer accuracy testing %.1f sec ...",fd->ldata*0.001);
	fl_set_object_label(fd->report1, buf);
	fl_gettime(&start_sec, &start_usec);
	fl_set_timer(fd->my_timer, fd->ldata * 0.001);
}

void timer_cb(FL_OBJECT *ob, long data){
char buf[128];
float df;
	FD_tipagf *fd = ob->form->fdui;
	float timerval =  fd->ldata * 0.001;
	
	fl_gettime(&end_sec, &end_usec);
	
	df = end_sec - start_sec + (end_usec - start_usec) * 1.e-6;
	fl_insert_chart_value(fd->chartOB, 1, (double)df, "",0);
	fl_replace_chart_value(fd->chartOB, 100, 0, "",0);
	sprintf(buf,"Timeout:%.2f  Actual:%.2f  DeltaE=%.2f",
		             timerval, df, df - timerval);
	
	fl_set_object_label(fd->report2,buf);
	start_timer(ob, data);
}


void cb_exit(FL_OBJECT *ob, long data){
	fl_finish();
	exit(0);
}

void main(int argc, char **argv){
FD_tipagf *mainfrm;

	fl_initialize(&argc, argv, "Tipagf", 0, 0);
	mainfrm = create_form_tipagf();
	mainfrm->ldata = 1000;
//	fl_set_chart_autosize(mainfrm->chartOB, 0);
	fl_set_chart_maxnumb(mainfrm->chartOB, 100);
	fl_insert_chart_value(mainfrm->chartOB, 1, 0, "",0);
	start_timer(mainfrm->my_timer,0);
	fl_show_form(mainfrm->tipagf, FL_PLACE_MOUSE,FL_TRANSIENT,"The Tipag");
	fl_do_forms();
}
