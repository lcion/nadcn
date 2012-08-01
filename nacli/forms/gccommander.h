#ifndef FD_gccommander_h_
#define FD_gccommander_h_
/* Header file generated with fdesign. */

/** Callback routines and free object handlers **/

extern void cb_gcom_disconnect(FL_OBJECT *, long);
extern void cb_gcom_cpui(FL_OBJECT *, long);
extern void cb_gcom_df(FL_OBJECT *, long);
extern void cb_gcom_du(FL_OBJECT *, long);
extern void cb_gcom_halt(FL_OBJECT *, long);
extern void cb_gcom_ifconfig(FL_OBJECT *, long);
extern void cb_gcom_last(FL_OBJECT *, long);
extern void cb_gcom_mtab(FL_OBJECT *, long);
extern void cb_gcom_netstat(FL_OBJECT *, long);
extern void cb_gcom_ps(FL_OBJECT *, long);
extern void cb_gcom_reboot(FL_OBJECT *, long);
extern void cb_gcom_repq(FL_OBJECT *, long);
extern void cb_gcom_route(FL_OBJECT *, long);
extern void cb_gcom_wall(FL_OBJECT *, long);
extern void cb_gcom_cddon(FL_OBJECT *, long);
extern void cb_gcom_login(FL_OBJECT *, long);
extern void cb_gcom_cp(FL_OBJECT *, long);
extern void cb_gcom_command(FL_OBJECT *, long);
extern void cb_gcom_wclk(FL_OBJECT *, long);
extern void cb_gcom_timer(FL_OBJECT *, long);


/**** Forms and Objects ****/

typedef struct {
	FL_FORM *gccommander;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *retOB;
	FL_OBJECT *load_txtOB;
	FL_OBJECT *nolog_btnOB;
	FL_OBJECT *memsw_chartOB;
	FL_OBJECT *load_chartOB;
	FL_OBJECT *upt_txtOB;
	FL_OBJECT *w_txtOB;
	FL_OBJECT *status_timer;
} FD_gccommander;

extern FD_gccommander * create_form_gccommander(void);

#endif /* FD_gccommander_h_ */
