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

#define MYPORT "8002"
#define SERVER_HOST_ADDR "127.0.0.1"

int sockfdex;
int connectsock( char *, char *, char * );
void err_sys(const char *fmt, ...);
ssize_t writen(int fd, const void *vptr, size_t n);

