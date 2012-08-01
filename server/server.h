# include <signal.h>
# include <stdio.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdarg.h>
# include <sys/types.h>
# include <netdb.h>
# include <errno.h>
# include <unistd.h>
# include <string.h>
# include <gdbm.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <shadow.h>
# include <crypt.h>
# include <pwd.h>


# define  _XOPEN_SOURCE
#define MAXLINE 512
#define MYPORT "8002"
#define SERVER_HOST_ADDR "127.0.0.1"

int writen(int, char *, int);
void err_dump(const char *, ...);
