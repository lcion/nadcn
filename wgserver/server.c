#include "server.h"
#define QLEN 5

int	errno;
/* extern char	*sys_errlist[];*/
//typedef unsigned short u_short;
//u_short	htons(), ntohs();

void err_sys(const char *fmt, ...);

u_short	portbase = 0;		/* port base, for non-root servers	*/
int init_proto(){

WORD wVersionRequested;
WSADATA wsaData;
int err; 
	wVersionRequested = MAKEWORD( 2, 2 ); 
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
    /* Tell the user that we could not find a usable */
    /* WinSock DLL.                                  */
		return 0;
	}
/* Confirm that the WinSock DLL supports 2.2.*/
/* Note that if the DLL supports versions greater    */
/* than 2.2 in addition to 2.2, it will still return */
/* 2.2 in wVersion since that is the version we      */
/* requested.                                        */
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
    /* Tell the user that we could not find a usable */
    /* WinSock DLL.                                  */
		WSACleanup( );
	    return 0;
	}
	return 1;
}


/*------------------------------------------------------------------------
 * passivesock - allocate & bind a server socket using TCP or UDP
 *------------------------------------------------------------------------
 */
int
passivesock( service, protocol, qlen )
char	*service;	/* service associated with the desired port	*/
char	*protocol;	/* name of protocol to use ("tcp" or "udp")	*/
int	qlen;		/* maximum length of the server request queue	*/
{
	struct servent	*pse;	/* pointer to service information entry	*/
	struct protoent *ppe;	/* pointer to protocol information entry*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, type;	/* socket descriptor and socket type	*/
	char errBuffer[MAX_PATH];

//	bzero((char *)&sin, sizeof(sin));
	ZeroMemory((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;

    /* Map service name to port number */
	if ( (pse = getservbyname(service, protocol)) != 0)
		sin.sin_port = htons(ntohs((u_short)pse->s_port) + portbase);

	else if ( (sin.sin_port = htons((u_short)atoi(service))) == 0 )
		err_sys("can't get \"%s\" service entry\n", service);	//?????

    /* Map protocol name to protocol number */
	if ( !init_proto() || (ppe = getprotobyname(protocol)) == 0)
		err_sys("can't get \"%s\" protocol entry\n", protocol);

    /* Use protocol to choose a socket type */
	if (strcmp(protocol, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /* Allocate a socket */
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0){
		strerror_s(errBuffer, MAX_PATH, errno);
		err_sys("can't create socket: %s\n", errBuffer);
	}

    /* Bind the socket */
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		strerror_s(errBuffer, MAX_PATH, errno);
		err_sys("can't bind to port %s: %s\n", service, errBuffer);
	}
	if (type == SOCK_STREAM && listen(s, qlen) < 0)
		strerror_s(errBuffer, MAX_PATH, errno);
		err_sys("can't listen on %s port: %s\n", service, errBuffer);
	return s;
}

void err_doit(int errnoflag, const char *fmt, va_list ap){
int errno_save;
char buf[MAXLINE];
	errno_save = errno;
	vsprintf_s(buf, MAXLINE, fmt, ap);
	if(errnoflag){
		char errBuffer[MAX_PATH];
		strerror_s(errBuffer, MAX_PATH, errno_save);
		//sprintf(buf+strlen(buf), ": %s", strerror(errno_save));
		strcat_s(buf, MAXLINE, ": ");
		strcat_s(buf, MAXLINE, errBuffer);
	}
	strcat_s(buf, MAXLINE, "\n");
	fflush(stdout);
	fputs(buf, stderr);
	fflush(NULL);
	return;
}

int passiveTCP(char *service, int qlen){
	return passivesock(service, "tcp", qlen);
}

/*
 * Write "n" bytes to a descriptor.
 * Use in place of write() when fd is a stream socket.
 */

int
writen(fd, ptr, nbytes)
register int	fd;
register char	*ptr;
register int	nbytes;
{
	int	nleft, nwritten;

	nleft = nbytes;
	while (nleft > 0) {
		nwritten = send (fd, ptr, nleft, 0 );
		if (nwritten <= 0)
			return(nwritten);		/* error */

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(nbytes - nleft);
}

void err_dump(const char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	abort();
	exit(1);
}


int strecho(int sockfd){
	int	n;
	char	line[MAXLINE];
	extern reply_c(int, char *);
	line[1] = 0;
	while ( line[1] != END_SESION ) {
		n = readprot(sockfd, line, MAXLINE);	// Citesc cererea de la client
		if (n == 0)
			return 1;		/* conexiune terminata */
		else if (n < 0)
			err_dump("<server> : strecho - eroare in readprot");
		reply_c(sockfd, line); // Raspund clientului la cererea depusa
	}
	smsg("Server: Conexion Closed !\n");
	return 0;
}


void err_sys(const char *fmt, ...){
va_list ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}

void wait_for_conection(void){

struct sockaddr_in fsin;	//cererea de la adresa
int alen;			//de la lungimea adresei
int tcsock;			//TCP socket for client
	tcsock = passiveTCP(MYPORT, QLEN);
//	DEBUG("<SERVER> Asteptare de noi conexiuni\n");
	while(1){
		int ssock;	//TCP Client socket
		alen = sizeof(fsin);
		ssock = accept(tcsock, (struct sockaddr *)&fsin, &alen);
		if(ssock < 0)
			err_sys("<server> : Accept failed\n");
			
		/* Conexiune acceptata asa ca intru in rutina de deservire */
		
		strecho(ssock);
		
		/* Inchide socket-ul de comunicatie cu clientul dupa ce acesta a terminat */
		
		_close(ssock);
	}
}


void main(void){
//	signal(SIGCHLD, SIG_IGN); 	//Fara zombies ??
	wait_for_conection();
}
