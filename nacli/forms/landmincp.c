/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "landmincp.h"

static FL_PUP_ENTRY fdchoice_0[] =
{ 
    /*  itemtext   callback  shortcut   mode */
    { "new host",	0,	"",	 FL_PUP_NONE},
    {0}
};

static FL_PUP_ENTRY fdchoice_1[] =
{ 
    /*  itemtext   callback  shortcut   mode */
    { "host",	0,	"",	 FL_PUP_NONE},
    {0}
};

FD_landmin *create_form_landmin(void)
{
  FL_OBJECT *obj;
  FD_landmin *fdui = (FD_landmin *) fl_calloc(1, sizeof(*fdui));

  fdui->landmin = fl_bgn_form(FL_NO_BOX, 330, 280);
  obj = fl_add_box(FL_UP_BOX,0,0,330,280,"");
  obj = fl_add_text(FL_NORMAL_TEXT,10,10,70,30,"Connect to :");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,10,50,90,30,"Disconect from:");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_button(FL_NORMAL_BUTTON,60,250,220,20,"Quit");
    fl_set_object_callback(obj,cb_land_quitb,0);
  obj = fl_add_clock(FL_DIGITAL_CLOCK,140,225,70,20,"");
    fl_set_object_color(obj,FL_INACTIVE,FL_CYAN);
  fdui->host_listOB = obj = fl_add_browser(FL_NORMAL_BROWSER,40,100,270,120,"Connected to:");
    fl_set_object_color(obj,FL_WHITE,FL_YELLOW);
    fl_set_object_lalign(obj,FL_ALIGN_TOP);
    fl_set_object_callback(obj,cb_landseld_hst,0);
  fdui->landnhOB = obj = fl_add_choice(FL_NORMAL_CHOICE2,110,10,200,30,"");
    fl_set_object_callback(obj,cb_land_nh,0);
   fl_set_choice_entries(obj, fdchoice_0);
   fl_set_choice(obj,1);
  fdui->landdhOB = obj = fl_add_choice(FL_NORMAL_CHOICE2,110,50,200,30,"");
    fl_set_object_callback(obj,cb_land_dh,0);
   fl_set_choice_entries(obj, fdchoice_1);
   fl_set_choice(obj,1);
  fl_end_form();

  fdui->landmin->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

