#ifndef FD_unCeas_h_
#define FD_unCeas_h_
/* Header file generated with fdesign. */

/** Callback routines and free object handlers **/

extern void unc_ret_cb(FL_OBJECT *, long);


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *unCeas;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *retOB;
} FD_unCeas;

extern FD_unCeas * create_form_unCeas(void);

#endif /* FD_unCeas_h_ */
