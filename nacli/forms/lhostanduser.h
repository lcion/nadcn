#ifndef FD_hostanduser_h_
#define FD_hostanduser_h_
/* Header file generated with fdesign. */

/** Callback routines and free object handlers **/

extern void cb_hau_hapinp(FL_OBJECT *, long);
extern void cb_hau_acc(FL_OBJECT *, long);
extern void cb_hau_quit(FL_OBJECT *, long);
extern void cb_hau_clear(FL_OBJECT *, long);


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *hostanduser;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *ob_tihost;
	FL_OBJECT *ob_tipasswd;
} FD_hostanduser;

extern FD_hostanduser * create_form_hostanduser(void);

#endif /* FD_hostanduser_h_ */
