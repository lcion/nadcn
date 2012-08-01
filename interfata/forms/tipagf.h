#ifndef FD_tipagf_h_
#define FD_tipagf_h_
/* Header file generated with fdesign. */

/** Callback routines and free object handlers **/

extern void timer_cb(FL_OBJECT *, long);
extern void cb_exit(FL_OBJECT *, long);


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *tipagf;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *my_timer;
	FL_OBJECT *report1;
	FL_OBJECT *report2;
	FL_OBJECT *chartOB;
} FD_tipagf;

extern FD_tipagf * create_form_tipagf(void);

#endif /* FD_tipagf_h_ */
