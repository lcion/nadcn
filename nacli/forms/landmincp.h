#ifndef FD_landmin_h_
#define FD_landmin_h_
/* Header file generated with fdesign. */

/** Callback routines and free object handlers **/

extern void cb_land_quitb(FL_OBJECT *, long);
extern void cb_landseld_hst(FL_OBJECT *, long);
extern void cb_land_nh(FL_OBJECT *, long);
extern void cb_land_dh(FL_OBJECT *, long);


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *landmin;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *host_listOB;
	FL_OBJECT *landnhOB;
	FL_OBJECT *landdhOB;
} FD_landmin;

extern FD_landmin * create_form_landmin(void);

#endif /* FD_landmin_h_ */
