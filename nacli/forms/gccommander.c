/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "gccommander.h"

FD_gccommander *create_form_gccommander(void)
{
  FL_OBJECT *obj;
  FD_gccommander *fdui = (FD_gccommander *) fl_calloc(1, sizeof(*fdui));

  fdui->gccommander = fl_bgn_form(FL_NO_BOX, 440, 500);
  obj = fl_add_box(FL_UP_BOX,0,0,440,500,"");
  fdui->retOB = obj = fl_add_button(FL_NORMAL_BUTTON,160,460,260,30,"Disconnect from server");
    fl_set_object_callback(obj,cb_gcom_disconnect,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,40,120,30,"CPU Info");
    fl_set_object_callback(obj,cb_gcom_cpui,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,70,120,30,"Disk Free");
    fl_set_object_callback(obj,cb_gcom_df,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,100,120,30,"Disk Usage");
    fl_set_object_callback(obj,cb_gcom_du,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,130,120,30,"Halt");
    fl_set_object_callback(obj,cb_gcom_halt,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,160,120,30,"InterfaceConfig");
    fl_set_object_callback(obj,cb_gcom_ifconfig,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,190,120,30,"Last");
    fl_set_object_callback(obj,cb_gcom_last,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,220,120,30,"View mtab");
    fl_set_object_callback(obj,cb_gcom_mtab,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,250,120,30,"netstat");
    fl_set_object_callback(obj,cb_gcom_netstat,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,310,120,30,"ps");
    fl_set_object_callback(obj,cb_gcom_ps,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,340,120,30,"reboot");
    fl_set_object_callback(obj,cb_gcom_reboot,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,370,120,30,"repquota");
    fl_set_object_callback(obj,cb_gcom_repq,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,400,120,30,"route");
    fl_set_object_callback(obj,cb_gcom_route,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,430,120,30,"Wall");
    fl_set_object_callback(obj,cb_gcom_wall,0);
  obj = fl_add_text(FL_NORMAL_TEXT,280,10,40,20,"uptime");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,190,285,100,20,"mem / swap Status");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->load_txtOB = obj = fl_add_text(FL_NORMAL_TEXT,335,285,80,20,"load");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_callback(obj,cb_gcom_cddon,0);
  fdui->nolog_btnOB = obj = fl_add_lightbutton(FL_PUSH_BUTTON,20,280,120,30,"No Login");
    fl_set_object_callback(obj,cb_gcom_login,0);
  obj = fl_add_text(FL_NORMAL_TEXT,240,60,120,20,"Logged Users");
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,10,120,30,"Change Password");
    fl_set_object_callback(obj,cb_gcom_cp,0);
  fdui->memsw_chartOB = obj = fl_add_chart(FL_PIE_CHART,165,305,150,140,"");
    fl_set_object_color(obj,FL_WHITE,FL_COL1);
    fl_set_object_callback(obj,cb_gcom_cddon,0);
  fdui->load_chartOB = obj = fl_add_chart(FL_BAR_CHART,330,305,90,140,"");
    fl_set_object_color(obj,FL_WHITE,FL_COL1);
    fl_set_object_callback(obj,cb_gcom_cddon,0);
  fdui->upt_txtOB = obj = fl_add_text(FL_NORMAL_TEXT,190,30,210,20,"");
    fl_set_object_boxtype(obj,FL_DOWN_BOX);
    fl_set_object_color(obj,FL_WHITE,FL_MCOL);
    fl_set_object_lalign(obj,FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    fl_set_object_callback(obj,cb_gcom_cddon,0);
  obj = fl_add_button(FL_NORMAL_BUTTON,20,460,120,30,"Run Command");
    fl_set_object_callback(obj,cb_gcom_command,0);
  fdui->w_txtOB = obj = fl_add_browser(FL_NORMAL_BROWSER,160,80,260,200,"");
    fl_set_object_color(obj,FL_WHITE,FL_YELLOW);
    fl_set_object_callback(obj,cb_gcom_wclk,0);
  fdui->status_timer = obj = fl_add_timer(FL_HIDDEN_TIMER,240,280,90,20,"Timer");
    fl_set_object_callback(obj,cb_gcom_timer,0);
  fl_end_form();

  fdui->gccommander->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

