#include "fclient.h"
#include "forms/gccommander.h"
/* ----------------------------------------------------------------------
 * Functia de criptare/decriptare a sirurilor de caractere pentru a 
 * fi trimise / receptionate pe / de pe retea.
 * ----------------------------------------------------------------------
*/
void fix_string(char *, int);

void err_socket(int sock);
char	*pname = NULL;		/* caller can set this from argv[0] */

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
/*  ------------------------------------------------------------------------
 * 
 * Functie care cripteaza mesajele si le trimite mai departe functiei writen pentru 
 * a le scrie in socket.
 * De ademenea pune in fata sirului un intreg pe doi bytes pentru protocolul de comunicatie
 * 
 * Formatul pachetului ce va fi trimis pe retea va fi:
 *     -------------------------------------------------------------------
 *     |lungime pachet (2 bytes) | date (lungime pachet bytes)           |
 *     -------------------------------------------------------------------
 * 
 *  ------------------------------------------------------------------------
*/
int smsg(int sockfd, char *msg){
int	n; 
char buffer[MAXLINE], b1[MAXLINE+2];
	strcpy(buffer, msg);
	n = strlen(buffer);
	fix_string(buffer, n);
	b1[0] = n >> 8;		//byte-ul cel mai semnificativ
	b1[1] = n << 8 >> 8;	//byte-ul cel mai putin semnificativ
	memcpy(b1+2, buffer, n);
	if (writen(sockfd, b1, n+2) != n+2){
		err_socket(sockfd);
		return 0;
	}
	return 1;	//OK
}

/* ----------------------------------------------------------------------
 * Apelez functia "connectsock" ce stabileste o conexiune folosind protocolul TCP
 * ----------------------------------------------------------------------
*/
int
connectTCP( host, service )
char	*host;		/* name of host to which connection is desired	*/
char	*service;	/* service associated with the desired port	*/
{
	return connectsock( host, service, "tcp");
}

/* ----------------------------------------------------------------------
 * stabileste o conexiune cu un socket TCP al unui server
 * aloca un socket TCP pentru a accepta conexiuni de la client
 * apelul sistem accept va returna un nou socket prin intermediul caruia
 * se va face comunicarea cu clientul
 * Inainte de a trata procesul client inchid socket-ul de ascultare pentru
 * ca alti clienti sa nu reuseasca sa mai contacteze server-ul
 * 
 * ----------------------------------------------------------------------
*/
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
	else if ( (sin.sin_port = htons((u_short)atoi(service))) == 0 ){
		fl_show_messages("can't get service entry");
		return -1;
	}

    /* Map host name to IP address, allowing for dotted decimal */
	if ( (phe = gethostbyname(host)) != NULL )
		bcopy(phe->h_addr, (char *)&sin.sin_addr, phe->h_length);
	else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE ){
		fl_show_messages("can't get host entry");
		return -1;
	}

    /* Map protocol name to protocol number */
	if ( (ppe = getprotobyname(protocol)) == 0){
		fl_show_messages("can't get protocol entry");
		return -1;
	}

    /* Use protocol to choose a socket type */
	if (strcmp(protocol, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /* Allocate a socket */
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0){
		fl_show_messages("can't cerate socket ");
		return -1;
	}

    /* Connect the socket */
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		if(s > 0)
			close(s);
                return -1;
	}
	return s;
}

/* ----------------------------------------------------------------------
 * apelez functia ce scrie eroarea la standard error si parasesc programul
 * ----------------------------------------------------------------------
*/
void err_dump(const char *fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	abort();
	exit(1);
}

/* ----------------------------------------------------------------------
 * 
 * Functie care citeste din socket mesajele primite de la client 
 * si le decripteaza apeland fuctia fix_string
 *
 * Citeste din socket un numar de bytes egal cu numarul citit in primii doi bytes 
 * (lungime pachet)
 * Formatul pachetului ce va fi receptionat din retea va fi:
 *     -------------------------------------------------------------------
 *     |lungime pachet (2 bytes) | date (lungime pachet bytes)           |
 *     -------------------------------------------------------------------
 * 
 * ----------------------------------------------------------------------
*/
int readprot(register int fd, register char *ptr, register int maxlen){
int	n, rc;
char	 buffer[MAXLINE];

	if ( (rc = read(fd, buffer, 2)) == 2){	
		n = (unsigned char)  buffer[0] << 8;	//partea cea mai semnificativa a lui n
		n += (unsigned char) buffer[1];		//partea cea mai putin semnificativa a lui n
	}
	else
		return(-1);
	if ( (rc = read(fd, buffer, n)) == n){
		fix_string(buffer, n);	//decriptez
		buffer[n]=0;		//pun terminator de sir
		strcpy(ptr, buffer);
		return (n);
	}
	return(-1);	//eroare
}


/* ----------------------------------------------------------------------
 * Scriu "n" bytes intr-un descriptor de socket
 * ----------------------------------------------------------------------
*/
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

/* ----------------------------------------------------------------------
 * Functia e folosita pentru a afla daca server-ul a acceptat parola 
 * de utilizator privilegiat pentru acces la monitorizarea si
 * administrarea sistemului
 * ----------------------------------------------------------------------
*/
int recv_access(int sockfd,int *nolo){
int	n;
char recvline[MAXLINE + 1];
	n = readprot(sockfd, recvline, MAXLINE);
	if (n < 0){
		err_socket(sockfd);
		return 0;
	}
//		err_dump("str_cli: readline error");
	recvline[n-1] = 0;	/* null terminate */
 	if(strcmp(recvline, "ACCES_OK") == 0){
		*nolo = 0;
		fl_show_messages("Administrator Loginat");
	}
	else{
		*nolo = 1;
	}
	return 1;
}

/* ----------------------------------------------------------------------
 * functia e folosita pentru a citi un sir de caractere de pe conexiunea 
 * de retea si a-l afisa intr-o fereastra ca mesaj utilizatorului.
 * Fereastra de mesaj are sub mesajul afisat un buton Ok prin care este
 * inchisa.
 * ----------------------------------------------------------------------
*/
int recv_rline(int sockfd){
int	n;
char recvline[MAXLINE + 1];
	n = readprot(sockfd, recvline, MAXLINE);
	if (n < 0){
		err_socket(sockfd);//		err_dump("str_cli: readline error");
		return 0;
	}
	recvline[n] = 0;	/* null terminate */
	fl_show_messages(recvline);
	return 1;
}

/* ----------------------------------------------------------------------
 * functia receptioneaza o redirectare a iesirii din programul server
 * sau un sir mai mare de text. Acesta este afisat in fereastra de log
 * care are doua butoane : Clear si Close. Primul sterge fereastra iar
 * al doilea o inchide dar fara a pierde continutul
 * ----------------------------------------------------------------------
*/
int recv_reply(int sockfd){
int	n;
char recvline[MAXLINE + 1];
	while( recvline[0] != '.' || recvline[1] != 'e'){
		n = readprot(sockfd, recvline, MAXLINE);
		if (n < 0){
			err_socket(sockfd);//			err_dump("str_cli: readline error");
			return 0;
		}
		recvline[n] = 0;	/* null terminate */
		if(recvline[0] == '.' && recvline[1] == 'e');
		else
			fl_addto_command_log(recvline);
	}
	fl_show_command_log(0);
	return 1;

}
/* ----------------------------------------------------------------------
 * functia e folosita la afisarea unei cantitati brute intr-o forma citibila
 * de catre om. De exemplu 1000 este afisat 1K
 * ----------------------------------------------------------------------
*/
char *memformat(float fm,char *buf){
//char buf[20];
	if(fm > 1000){
		sprintf(buf, "%.0fM", fm/1000);
		return buf;
	}
	else{
		sprintf(buf, "%.0fK", fm);
		return buf;
	}
}

/* ----------------------------------------------------------------------
 * Functia este creata pentru actualizarea obiectelor grafice din fereastra
 * de monitorizare.
 * Informatiile vin de la server intr-un format bine precizat:
 * (memorie folosita) (memorie libera) (swap folosit) (swap liber)
 * -up_time- (load_average)
 * -- restul reprezentand lista utilizatorilor loghinati in sistem ---
 * .e
 * entitatile din paranteza sunt numere intregi fiind convertite pentru
 * a fi afisate in cele doua grafice, iar cele intre liniute sunt siruri
 * de caractere fiind afisate asa cum vin.
 * ----------------------------------------------------------------------
*/
int recv_gupdate(FD_gccommander *gcomf){
int	n, sockfd, val;
char recvline[MAXLINE + 1], buf[20], *cptr, *initptr;
float numb;
	sockfd = find_sock(gcomf->gccommander);
	n = readprot(sockfd, recvline, MAXLINE);
	if (n < 0){
		err_socket(sockfd);//		err_dump("str_cli: readline error");
		return 0;
	}
	initptr = recvline;
	cptr = strtok(initptr, " \t");
	if(cptr == NULL)
		return 0;
	numb = atof(cptr);			// memorie ocupata
	fl_clear_chart(gcomf->memsw_chartOB);
	fl_add_chart_value(gcomf->memsw_chartOB, numb, memformat(numb, buf), FL_RED);
	cptr = strtok(NULL, " \t");		// memorie libera
	if(cptr == NULL)
		return 0;
	numb = atof(cptr);
	fl_add_chart_value(gcomf->memsw_chartOB, numb, memformat(numb, buf), FL_BLUE);
	cptr = strtok(NULL, " \t");		// swap ocupat
	if(cptr == NULL)
		return 0;
	numb = atof(cptr);
	fl_add_chart_value(gcomf->memsw_chartOB, numb, memformat(numb, buf), FL_RED);
	cptr = strtok(NULL, " \t");		// swap liber
	if(cptr == NULL)
		return 0;
	numb = atof(cptr);
	fl_add_chart_value(gcomf->memsw_chartOB, numb, memformat(numb, buf), FL_BLUE);
	n = readprot(sockfd, recvline, MAXLINE);
	if (n < 0){
		err_socket(sockfd);//		err_dump("str_cli: readline error");
		return 0;
	}
	initptr = recvline;
	cptr = strtok(initptr, " \t");		//UP Time
	if(cptr == NULL)
		return 0;
	fl_set_object_label(gcomf->upt_txtOB, cptr);	//setez label-ul la uptime ...
	cptr = strtok(NULL, " \t");		// Load average
	if(cptr == NULL)
		return 0;
	numb = atof(cptr);
	val =  numb+1;
	fl_set_chart_bounds(gcomf->load_chartOB, 0, val * 100);
	sprintf(buf, "load 0-%d", val*100);
	fl_set_object_label(gcomf->load_txtOB, buf);
	fl_insert_chart_value(gcomf->load_chartOB, 1, numb*100, "", 1);	//desenez pe chart-ul de load ...
	fl_clear_browser(gcomf->w_txtOB);
	while( recvline[0] != '.' || recvline[1] != 'e'){
		n = readprot(sockfd, recvline, MAXLINE);
		if (n < 0){
			err_socket(sockfd);//			err_dump("str_cli: readline error");
			return 0;
		}
		recvline[n-1] = 0;	/* null terminate */
		if(recvline[0] == '.' && recvline[1] == 'e');
		else
			fl_addto_browser(gcomf->w_txtOB, recvline);
	}
	return 1;
}

/* ----------------------------------------------------------------------
 * a aparut o eroare in sistem. Parasesc programul fara sa mai golesc
 * bufferele
 * ----------------------------------------------------------------------
*/
void err_sys(const char *fmt, ...){
va_list ap;
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}
