#include "server.h"
#define QLEN 0

int	errno;


// u_short	htons(), ntohs();
void fix_string(char *, int);
void err_sys(const char *fmt, ...);
u_short	portbase = 0;		/* port base, for non-root servers	*/

/* ------------------------------------------------------------------------
 * passivesock - aloc si inregistrez un socket server folosind  TCP sau UDP
 * ------------------------------------------------------------------------
 */
int
passivesock( service, protocol, qlen )
char	*service;	// serviciul asociat cu protocolul dorit
char	*protocol;	// numele protocolului folosit ("tcp" sau "udp")
int	qlen;		// lungimea cozii de asteptare pentru clienti
{
	struct servent	*pse;	// pointer la o entitate cu informati despre serviciu
	struct protoent *ppe;	// pointer la o entitate cu informati despre protocol
	struct sockaddr_in sin;	// adresa a unui server in Internet
	int	s, type;	// descriptorul de socket si tipul socket-ului

	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;

    /* Asociez serviciul unui numar de port */
	if ( (pse = getservbyname(service, protocol)) != 0)
		sin.sin_port = htons(ntohs((u_short)pse->s_port) + portbase);

	else if ( (sin.sin_port = htons((u_short)atoi(service))) == 0 )
		err_sys("can't get \"%s\" service entry\n", service);	//?????

    /* Asociez numele protocolului la un numar protocol */
	if ( (ppe = getprotobyname(protocol)) == 0)
		err_sys("can't get \"%s\" protocol entry\n", protocol);

    /* Folosesc protocolul pentru a alege tipul socket-ului */
	if (strcmp(protocol, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /* Alloc un socket */
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0)
		err_sys("can't create socket: %s\n", sys_errlist[errno]);

    /* inregistrez  socket-ul */
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		return -1;

	if (type == SOCK_STREAM && listen(s, qlen) < 0)
		err_sys("can't listen on %s port: %s\n", service,
			sys_errlist[errno]);
	return s;
}

/* ------------------------------------------------------------------------
 * scriu mesajul de eroare la iesirea standard pentru erori
 * ------------------------------------------------------------------------
*/

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

/* ------------------------------------------------------------------------
 * apelez functia ce aloca un socket folosind protocolul TCP
 * ------------------------------------------------------------------------
*/

int passiveTCP(char *service, int qlen){
	return passivesock(service, "tcp", qlen);
}

/*
 * Scriu "n" bytes intr-un descriptor de socket
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
			return(nwritten);		/* eroare */

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(nbytes - nleft);
}

/* ------------------------------------------------------------------------
 * apelez functia ce scrie eroarea la standard error si parasesc programul
 * ------------------------------------------------------------------------
*/

void err_dump(const char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	abort();
	exit(1);
}

/* ------------------------------------------------------------------------
 * atata timp cat nu primesc sesiune incheiata de la client tratez cererile
 * acestuia si i-i raspund apeland functia reply_c
 * ------------------------------------------------------------------------
*/

int strecho(int sockfd){
	int	n;
	char	line[MAXLINE];
	extern void reply_c(int, char *);
	line[1] = 0;
	while ( line[1] != END_SESION ) {
		n = readprot(sockfd, line, MAXLINE);	// Citesc cererea de la client
		if (n == 0)
			return 1;		/* conexiune terminata */
		else if (n < 0)
			err_dump("<server> : strecho - eroare in readprot");
		reply_c(sockfd, line); // Raspund clientului la cererea depusa
	}
	smsg("Server: Conexion Closed !\n Acces disabled for one minnute ...");
	return 0;
}

/* ------------------------------------------------------------------------
 * a aparut o eroare in sistem. Parasesc programul fara sa mai golesc
 * bufferele
 * ------------------------------------------------------------------------
*/

void err_sys(const char *fmt, ...){
va_list ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}

/* ------------------------------------------------------------------------
 * bucla principala a server-ului
 * aloca un socket TCP pentru a accepta conexiuni de la client
 * apelul sistem accept va returna un nou socket prin intermediul caruia
 * se va face comunicarea cu clientul
 * Inainte de a trata procesul client inchid socket-ul de ascultare pentru
 * ca alti clienti sa nu reuseasca sa mai contacteze server-ul
 * ------------------------------------------------------------------------
*/

void wait_for_conection(void){

struct sockaddr_in fsin;	//cererea de la adresa
int alen;			//de la lungimea adresei
int tcsock= -1;			//TCP socket for client
	while(1){
		int ssock;	//TCP Client socket
		alen = sizeof(fsin);
		while (tcsock < 0){
			tcsock = passiveTCP(MYPORT, QLEN);
			sleep(20);
		}
		ssock = accept(tcsock, (struct sockaddr *)&fsin, &alen);
		close(tcsock);
		tcsock = -1;
		if(ssock < 0)
			err_sys("<server> : Accept failed\n");
		/* Conexiune acceptata asa ca intru in rutina de deservire */

		strecho(ssock);
		/* Inchide socket-ul de comunicatie cu clientul dupa ce acesta a terminat */
		close(ssock);
	}
	close(tcsock);
}


/* ------------------------------------------------------------------------
 * Functia principala invalideaza mesjele de la procese child si apeleaza
 * functia de asteptare si tratare a conexiunilor
 * ------------------------------------------------------------------------
*/

int main(void){
	signal(SIGCHLD, SIG_IGN); 	//Fara zombies ??
	wait_for_conection();
	return 1;
}
