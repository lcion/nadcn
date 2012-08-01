#include "client.h"
#define QLEN 5
#define	MAXLINE	512
char	*pname = NULL;		/* caller can set this from argv[0] */


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

void smsg(char *msg){
int n;
	n = strlen(msg);
	if (writen(sockfdex, msg, n) != n)
			err_sys("str_cli: writen error on socket");
/*			err_dump("<server> : strecho - eroare in writen");*/

}

int
connectTCP( host, service )
char	*host;		/* name of host to which connection is desired	*/
char	*service;	/* service associated with the desired port	*/
{
	return connectsock( host, service, "tcp");
}

int
connectsock( host, service, protocol )
char	*host;		/* name of host to which connection is desired	*/
char	*service;	/* service associated with the desired port	*/
char	*protocol;	/* name of protocol to use ("tcp" or "udp")	*/
{
	struct hostent	*phe;	/* pointer to host information entry	*/
	struct servent	*pse;	/* pointer to service information entry	*/
	struct protoent *ppe;	/* pointer to protocol information entry*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, type;	/* socket descriptor and socket type	*/


	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;

    /* Map service name to port number */
	if ( (pse = getservbyname(service, protocol)) != NULL )
		sin.sin_port = pse->s_port;
	else if ( (sin.sin_port = htons((u_short)atoi(service))) == 0 )
		err_sys("can't get \"%s\" service entry\n", service);

    /* Map host name to IP address, allowing for dotted decimal */
	if ( (phe = gethostbyname(host)) == NULL )
		bcopy(phe->h_addr, (char *)&sin.sin_addr, phe->h_length);
	else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
		err_sys("can't get \"%s\" host entry\n", host);

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

    /* Connect the socket */
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		err_sys("can't connect to %s.%s: %s\n", host, service,
			sys_errlist[errno]);
	return s;
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


ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft, nwritten;
	const char	*ptr;

	ptr = vptr;	/* can't do pointer arithmetic on void* */
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0)
			return(nwritten);		/* error */

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return n;
}
void askfp(char *oldp){
char key[3], *newpass, *mlp;
int i;
	for(i=0; i < 13; i++) oldp[i] = oldp[i+2];	//fixez parola primita
	oldp[i] = '\0';
	newpass = getpass("Introduceti Parola Supervizor (root) :");
	key[0] = oldp[0];
	key[1] = oldp[1];
	mlp = crypt(newpass, key);
	if(strcmp(mlp, oldp) == 0)
		smsg(".p PASSWDOK\n");
	else
		printf("Password Not Accept!\n");

}

int asculta_s(fp, sockfd)
register FILE	*fp;
register int	sockfd;
{
int	n;
char recvline[MAXLINE + 1];
	while(recvline[0] != '.' && recvline[1] != 'q'){
		n = readline(sockfd, recvline, MAXLINE);
		if (n < 0)
			err_dump("str_cli: readline error");
		recvline[n] = 0;	/* null terminate */
		if(recvline[0] == '.' && recvline[1] == 'p')
			askfp(recvline);
		else
			fputs(recvline, stdout);
	}
	return 1;
}

void str_cli(fp)
register FILE	*fp;
{
char	sendline[MAXLINE];

	while ( sendline[1] != 'q') {
		fgets(sendline, MAXLINE, fp);
		smsg(sendline);
	}

	if (ferror(fp))
		err_sys("str_cli: error reading file");
}

void err_sys(const char *fmt, ...){
va_list ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}


int main(int argc, char *argv[])
{
	int			sockfd;
/*	struct sockaddr_in	serv_addr;*/
	int n;
	pname = argv[0];
/*
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	serv_addr.sin_port        = htons(SERV_TCP_PORT);

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("<client> : can't open stream socket");

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		err_sys("<client> : can't connect to server");
*/
	if( argc < 2)
		sockfd = connectTCP(SERVER_HOST_ADDR, MYPORT);
	else
		sockfd = connectTCP(argv[1], MYPORT);
	sockfdex = sockfd;	
	switch(fork()){
		case 0 : exit(asculta_s(stdin, sockfd));break;

		case -1: err_sys("<client> : Error fork") ;
	}
	if( argc < 3)
		str_cli(stdin);		/* do it all */

	else{
		n = strlen(argv[2]);
		if (writen(sockfd, argv[2], n) != n)
			err_sys("str_cli: writen error on socket");
		writen(sockfd, "\n",1);
		strcpy(argv[2], ".q\n");
		n = strlen(argv[2]);
		if (writen(sockfd, argv[2], n) != n)
			err_sys("str_cli: writen error on socket");
 
	}
	wait( &n );
	close(sockfd);
	exit(0);
}
