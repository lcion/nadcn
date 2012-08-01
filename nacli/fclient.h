#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <crypt.h>
#include "forms.h"

#define QLEN 5
#define	MAXLINE	512
#define MYPORT "8002"
#define SERVER_HOST_ADDR "127.0.0.1"

//int sockfdex;
int connectsock( char *, char *, char * );
void err_sys(const char *fmt, ...);
int connectTCP( char *, char * );
ssize_t writen(int fd, const void *vptr, size_t n);
int find_sock(FL_FORM *);
int asculta_s(register FILE *, register int);
void str_cli( register FILE *);
int smsg(int ,char *);

