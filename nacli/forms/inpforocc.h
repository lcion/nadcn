#ifndef FD_inpforocc_h_
#define FD_inpforocc_h_
/* Header file generated with fdesign. */

/** Callback routines and free object handlers **/

extern void cb_inpfo_ok(FL_OBJECT *, long);
extern void cb_inpfo_clear(FL_OBJECT *, long);
extern void cb_inpfo_cancel(FL_OBJECT *, long);
extern void cb_inpfo_edit(FL_OBJECT *, long);


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *inpforocc;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *inpfo_inpOb;
} FD_inpforocc;

extern FD_inpforocc * create_form_inpforocc(void);

#endif /* FD_inpforocc_h_ */
