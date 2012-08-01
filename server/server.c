#include "server.h"
#define QLEN 5

int	errno;
/* extern char	*sys_errlist[];*/

u_short	htons(), ntohs();

void err_sys(const char *fmt, ...);

u_short	portbase = 0;		/* port base, for non-root servers	*/

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

	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;

    /* Map service name to port number */
	if ( (pse = getservbyname(service, protocol)) != 0)
		sin.sin_port = htons(ntohs((u_short)pse->s_port) + portbase);

	else if ( (sin.sin_port = htons((u_short)atoi(service))) == 0 )
		err_sys("can't get \"%s\" service entry\n", service);	//?????

    /* Map protocol name to protocol number */
	if ( (ppe = getprotobyname(protocol)) == 0)
		err_sys("can't get \"%s\" protocol entry\n", protocol);

    /* Use protocol to choose a socket type */
	if (strcmp(protocol, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /* Allocate a socket */
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0)
		err_sys("can't create socket: %s\n", sys_errlist[errno]);

    /* Bind the socket */
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		err_sys("can't bind to %s port: %s\n", service,
			sys_errlist[errno]);
	if (type == SOCK_STREAM && listen(s, qlen) < 0)
		err_sys("can't listen on %s port: %s\n", service,
			sys_errlist[errno]);
	return s;
}

void err_doit(int errnoflag, const char *fmt, va_list ap){
int errno_save;
char buf[MAXLINE];
	errno_save = errno;
	vsprintf(buf, fmt, ap);
	if(errnoflag)
		sprintf(buf+strlen(buf), ": %s", strerror(errno_save));
	strcat(buf, "\n");
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
		nwritten = write(fd, ptr, nleft);
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

int
readline(fd, ptr, maxlen)
register int	fd;
register char	*ptr;
register int	maxlen;
{
	int	n, rc;
	char	c;

	for (n = 1; n < maxlen; n++) {
		if ( (rc = read(fd, &c, 1)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
			if (n == 1)
				return(0);	/* EOF, no data read */
			else
				break;		/* EOF, some data was read */
		} else
			return(-1);	/* error */
	}

	*ptr = 0;
	return(n);
}


int strecho(int sockfd){
	int	n;
	char	line[MAXLINE];
	extern reply_c(int, char *);
	line[1] = 0;
	while ( line[1] != 'q' ) {
		n = readline(sockfd, line, MAXLINE);	// Citesc cererea de la client
		if (n == 0)
			return 1;		/* conexiune terminata */
		else if (n < 0)
			err_dump("<server> : strecho - eroare in readline");
		reply_c(sockfd, line); // Raspund clientului la cererea depusa

	}
	line[2] = '\n';
	line[3] = '\0';
	if (writen(sockfd, line, n) != n) //trimit clientului ACKnoledge la .q
		err_dump("<server> : strecho - eroare in writen");

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
		
		close(ssock);
	}
}


void main(void){
	signal(SIGCHLD, SIG_IGN); 	//Fara zombies ??
	wait_for_conection();
}
