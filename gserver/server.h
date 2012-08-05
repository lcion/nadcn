# include <signal.h>
# include <stdio.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdarg.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <netdb.h>
# include <errno.h>
# include <unistd.h>
# include <string.h>
// # include <gdbm.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <shadow.h>
# include <crypt.h>
# include <pwd.h>

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

# define  _XOPEN_SOURCE
#define MAXLINE 512
#define MYPORT "8002"
#define SERVER_HOST_ADDR "127.0.0.1"

int readprot(int , char *, int );
void smsg(char *msg);
int writen(int, char *, int);
void err_dump(const char *, ...);
