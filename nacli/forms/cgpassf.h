#ifndef FD_cgpassf_h_
#define FD_cgpassf_h_
/* Header file generated with fdesign. */

/** Callback routines and free object handlers **/

extern void cb_cgp_change(FL_OBJECT *, long);
extern void cb_cgp_nochange(FL_OBJECT *, long);
extern void cb_cgp_upcinp(FL_OBJECT *, long);
extern void cb_cgp_clear(FL_OBJECT *, long);


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *cgpassf;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *cgp_userOb;
	FL_OBJECT *cgp_pass0Ob;
	FL_OBJECT *cgp_pass1Ob;
} FD_cgpassf;

extern FD_cgpassf * create_form_cgpassf(void);

#endif /* FD_cgpassf_h_ */
