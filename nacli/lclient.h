#include "forms.h"
#include "forms/lhostanduser.h"
#include "fclient.h"
#include "forms/gccommander.h"
#include "forms/cgpassf.h"
#include "forms/inpforocc.h"
#include "forms/landmincp.h"

#define TIMER_SEC_DELAY 10

#define CPU_INFO	100
#define DISK_FREE	101
#define DISK_USAGE	102
#define INTERFACE_CONF	103
#define LOG_HISTORY	104
#define SYS_HALT	105
#define MOUNTED_FS	106
#define NET_STATUS	107
#define PROCESS_VIEW	108
#define SYS_REBOOT	109
#define REP_QUOTA	110
#define ROUTE		111
#define LUSERS_MSG	112
#define CH_USER_PASS	113
#define GRAPH_RAPORT	114
#define FREE_COMMAND	115
#define MSG_LOGINOUT	116
#define END_SESION	117

typedef struct unc_plist{
	FD_gccommander *p_gcom;
	struct unc_plist *next;
	int sockfdex;
	int no_login;
}unc_pl;

unc_pl *crt_head;

FD_landmin *fland = NULL;
FD_hostanduser	*hauf = NULL;
FD_cgpassf	*cgpas = NULL;
FD_inpforocc	*inpf = NULL;
//int no_login = 0;
int new_client_inst = 0;

int	recv_rline(int);
int	recv_reply(int);
//int readline(int , char*, int);
//void err_dump(const char *fmt, ...);
void start_timer(FL_OBJECT *ob);
int recv_gupdate(FD_gccommander *);
int recv_access(int, int *);
void dest_c(FL_OBJECT *);
void erblinec(int sock);

