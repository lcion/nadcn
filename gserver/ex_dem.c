/* ------------------------------------------------------------------------
 * fiser sursa : ex_dem.c
 * Inclus in cadrul programului server
 * Apartine proiectului de administrare a unei retele locale de sisteme Unix
 * 
 * ------------------------------------------------------------------------
 * 
 * Autor: Ion Lucian
 * ------------------------------------------------------------------------
*/

#include "server.h"
char *cryptl(const char *key, const char *salt);

typedef struct my_struct{
	char cv[MAXLINE];
	char ex[MAXLINE];
	char c;
}t_char;

int sockfdex, acces;

/* ------------------------------------------------------------------------
 * 
 * Functie de criptare/decryptare a mesajului pentru a fi trimis/receptionat pe retea
 * definita in fisierul ../cript/fix_string.c
 * ------------------------------------------------------------------------ 
*/
void fix_string(char *, int);

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

void smsg(char *msg){
unsigned int	n; 
char buffer[MAXLINE], b1[MAXLINE+2];
	strcpy(buffer, msg);
#ifdef DEBUG
	printf("%s",buffer);
#endif
	n = strlen(buffer);
	fix_string(buffer, n);
	b1[0] = n>>8;		//byte-ul cel mai semnificativ
	b1[1] = n<<8>>8;	//byte-ul cel mai putin semnificativ
	memcpy(b1+2, buffer, n);
	if (writen(sockfdex, b1, n+2) != n+2)
		err_dump("<server> : strecho - eroare in writen");
}

/* ------------------------------------------------------------------------
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
 *  ------------------------------------------------------------------------
*/

int readprot(register int fd, register char *ptr, register int maxlen){
int	n, rc;
char    buffer[MAXLINE];
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
/* ------------------------------------------------------------------------
 * Verific daca sirul de caractere primit de la client este 
 * un sir de comanda si i-l inpart in cuvant de comanda,
 * primul cuvant date, si restul datelor completand
 * structura t_char a carui pointer i-l primesc ca
 * parametru
 *  ------------------------------------------------------------------------
*/
  
int is_command(char *brut, t_char *block){ //prelucreaza brut returnand 
char *t;
	strcpy(block->cv, "\0");
	strcpy(block->ex, "\0");
	t=strdup(brut);
	if((brut[0] == '.') && strlen(brut) >= 2 ){
		block->c=brut[1];
		t=strtok(brut," \t");
		t = strtok(NULL, " \t");
		if(t != NULL){
			strcpy(block->cv, t);
			t = strtok(NULL, " \t");
			while(t != NULL){
				strcat(block->ex, t);
				t = strtok(NULL, " \t");
				if(t != NULL)
					strcat(block->ex, " ");
			}
		}
		return 1;
	}
	else
	return 0;
}

/* ------------------------------------------------------------------------
 * lansez in executie un proces a carui iesire o redirectez spre
 * un pipe. Apoi citesc din pipe pana ajung la capat si trimit
 * procesului client in pachete de lungime maxima MAXLINE
 * ------------------------------------------------------------------------
*/

void sendtoc(char *ftoop){	//trimite iesirea comenzii catre client si sterge fisierul.
	FILE *fd;
	char mybuf[MAXLINE];
	int j = 0;
	if( (fd = popen(ftoop, "r")) != NULL){	//deschid fisierul nou creat

		while( !feof(fd) ){
			mybuf[j++]=fgetc(fd);
			if(j >= MAXLINE-1 || feof(fd)){
				mybuf[j-1] = 0;
				smsg(mybuf);
				j = 0;
			}
		}

		smsg("\n");
		smsg(".e\n");
		if(fd != NULL)
			pclose(fd);
	}
}

/* ------------------------------------------------------------------------
 * rearanjez sirul de caractere pentru a forma o comanda executabila
 * de catre functia sendtoc. Mai verific daca cel care a trimis comanda
 * are dreptul ca sa o execute (este administratorul)
 * ------------------------------------------------------------------------
*/

void lans_ex(char *comanda, char *parametru){

	char redar[255];
	if( acces == 1 ){
		strcpy(redar, comanda);
		strcat(redar, " ");
		if( strlen(parametru) > 0){
			strcat(redar, parametru);
			strcat(redar, " ");
		}
	sendtoc(redar);
	}
	else{
		smsg("First login with root passwd: hit No login button");
		smsg(".e");
	}
}

/* ------------------------------------------------------------------------
 * Functia este creata pentru actualizarea obiectelor grafice ale clientului
 * obtinand aceste informatii captand iesirile unor procese
 * care le lanseaza ca free si w. Aceste iesiri sunt filtrate fiind 
 * trimise astfel:
 * (memorie folosita) (memorie libera) (swap folosit) (swap liber)
 * (up_time) (load_average)
 * -- restul reprezentand lista utilizatorilor loghinati in sistem ---
 * ------------------------------------------------------------------------
*/

void repgrafst(){
	FILE *fd;
	char mb[MAXLINE], sb[MAXLINE];
	int i = 0;
	int scRes = 0;
	mb[0] = 0;
	if( (fd = popen("/usr/bin/free", "r")) != NULL){	//deschid fisierul nou creat
		while( !feof(fd) ){
			scRes = fscanf(fd, "%s", sb);
			if(scRes > 3 && ( strcmp(sb, "Mem:") == 0 || strcmp(sb, "Swap:") == 0 ) ){
				scRes = fscanf(fd, "%s", sb);	//M/S Totala
				scRes = fscanf(fd, "%s", sb);	//M/S Folosit
				strcat(sb, " ");
				strcat(mb, sb);
				scRes = fscanf(fd, "%s", sb);	//M/S liber
				strcat(sb, " ");
				strcat(mb, sb);
			}
		}
		if(fd != NULL)
			pclose(fd);
		smsg(mb);
	}
	mb[0] = 0;
	if( (fd = popen("/usr/bin/w", "r")) != NULL){	//deschid fisierul nou creat
		scRes = fscanf(fd, "%s", sb);
		while( !feof(fd) ){
			if(strcmp( sb, "up") == 0 || strcmp( sb, "average:") == 0 ){
				scRes = fscanf(fd, "%s", sb);	//timpul de cand a fost pornit
				i++;
				if(i == 2){
					strcat(mb, " ");
					strcat(mb, sb);
					smsg(mb);	//trimit sirul format din uptime + load average
				}
				else
					strcpy(mb, sb);
				scRes = fscanf(fd, "%s", sb);	// ignor
				scRes = fscanf(fd, "%s", sb);	//doua sirurui
			}
			if(i > 1){
				int j  = 0;	//aici trebuie sa trimit un buffer odata
				while(!feof(fd)){
					mb[j++] = fgetc(fd);
					if(j >= MAXLINE-1 || feof(fd)){
						mb[j] = 0;
						smsg(mb);
						j = 0;
					}
				}
			}else
				scRes = fscanf(fd, "%s", sb);
		}
		if(fd != NULL)
			pclose(fd);
	}
	smsg(".e\n");
}

/* ------------------------------------------------------------------------
 * Verific daca parola trimisa de client este parola
 * utilizatorului root.
 * requires server to be run as root, don't use this function
 *  ------------------------------------------------------------------------
*/
  
void sendmyrpr(char *aut){
   struct passwd *my_pwent;
   struct spwd *my_spwent;
   char *oldp, key[12], *mlp;

   my_pwent = getpwnam("lucian");
   if(my_pwent == NULL){
      smsg("Eroare la accesarea fisierului passwd, nu exista user root\n");
      return;
   }
   if(strlen(my_pwent->pw_passwd) < 13){	//exista shadow
      my_spwent = getspnam("root");
	if(my_spwent == NULL){
	  printf("Eroare la accesarea fisierului passwd, nu exista root\n");
		smsg("Eroare la accesarea fisierului passwd, nu exista root\n");
		return;
	}
	oldp = my_spwent->sp_pwdp;
   }
   else{
      oldp = my_pwent->pw_passwd;
   }	//PANA AICI AM OBTINUT PAROLA ROOT A SISTEMULUI GAZDA
   strncpy(key, oldp, 11);

   mlp = cryptl(aut, key);

   if( strcmp(mlp, oldp) == 0){
      acces = 1;
      smsg("ACCES_OK\n");
   }
   else{
      acces = 0;
      smsg("ACCES_NOOK\n");
   }
}

/*  ------------------------------------------------------------------------
 * use file in user home directory
 *  ------------------------------------------------------------------------
 */
void sendmyrp(char *aut){
   char key[12], *mlp;

   FILE *fp = fopen(".nasrv","rt");
   if(fp == NULL)
   {
     //generate the password file and allow access
     FILE *fp = fopen(".nasrv","wt");
     if(fp != NULL){
      mlp = cryptl(aut, "$1$MtbV6gOn$");
      fprintf(fp, "%s\n", mlp);
      fclose(fp);
      //change permissions to user read only
      chmod(".nasrv", S_IRUSR);

      //done, let the user in
      acces = 1;
      smsg("ACCES_OK\n");
      return;
     }
   }else
   {//file already exists, read and compare the result
     char encStr[40];
     int scanRes = 0;
     scanRes = fscanf(fp, "%s\n", encStr);
     fclose(fp);
     if(scanRes)
       strncpy(key, encStr, 11);
     else
       bzero (key, 11);
     mlp = cryptl(aut, key);
     if( strcmp(mlp, encStr) == 0){
      //done, let the user in
      acces = 1;
      smsg("ACCES_OK\n");       
     }
     else{
      //failed the password
      acces = 0;
      smsg("ACCES_NOOK\n");
     }
   }
}


/* ------------------------------------------------------------------------
 * Prin aceasta functie schimb parola utilizatorului str_user in
 * sirul de charactere mlp. In functie de characterul primit ca
 * parametru, shadow, aceasta este schimbata in fisierul 
 * /etc/shadow respectiv /etc/passwd
 * ------------------------------------------------------------------------
*/

void changepas(char shadow, char *str_user, char *mlp){
struct passwd *my_pwent;
struct spwd *my_spwent;
FILE *psfp, *pdfp;	//password file pointer
	if(shadow == 1){	//shadow
		if((psfp  = fopen("/etc/shadow", "rt")) == NULL){
			smsg("Password NOT Changed \n");
			return;
		}
		if((pdfp  = fopen("/etc/shadow+", "wt")) == NULL){
			smsg("Password NOT Changed \n");
			fclose(psfp);
			return;
		}
		my_spwent = fgetspent(psfp);
		while(my_spwent != NULL){

			if(strcmp(str_user, my_spwent->sp_namp) == 0){
				strcpy( my_spwent->sp_pwdp, mlp);
				smsg("Password Changed \n");
			}
			putspent(my_spwent, pdfp);
			my_spwent = fgetspent(psfp);
		}
		fclose(psfp);
		fclose(pdfp);
		chmod("/etc/shadow+", S_IRUSR);
		rename("/etc/shadow", "/etc/shadow-");
		rename("/etc/shadow+", "/etc/shadow");
		return;
	}
	else{
		if((psfp  = fopen("/etc/passwd", "rt")) == NULL){
			smsg("Password NOT Changed \n");
			return;
		}
		if((pdfp  = fopen("/etc/passwd+", "wt")) == NULL){
			smsg("Password NOT Changed \n");
			fclose(psfp);
			return;
		}
		my_pwent = fgetpwent(psfp);
		while(my_pwent != NULL){
			if(strcmp(str_user, my_pwent->pw_name) == 0){
				strcpy( my_pwent->pw_passwd, mlp);
				smsg("Password Changed \n");
			}
			putpwent(my_pwent, pdfp);
			my_pwent = fgetpwent(psfp);
		}
		fclose(psfp);
		fclose(pdfp);
		chmod("/etc/passwd+", S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR);
		rename("/etc/passwd", "/etc/passwd-");
		rename("/etc/passwd+", "/etc/passwd");
		chmod("/etc/passwd-", S_IRUSR);
		return;
	}
}

/* ------------------------------------------------------------------------
 * Functia apeleaza changepas cu parametrul shadow = 1
 * daca parolele sunt ascunse in fisierul /etc/shadow
 * altfel cu shadow = 0. De asemenea verifica daca 
 * utilizatorul user exista. parola trimisa mai departe
 * functiei changepass o cripteaza mai inainte.
 * ------------------------------------------------------------------------
*/

void cup(char *str_user, char *str_password){ //char* aut
struct passwd *my_pwent;
struct spwd *my_spwent;
char *oldp, key[3], *mlp, shadow;
	if( acces == 1 ){
		my_pwent = getpwnam(str_user);
		if(my_pwent == NULL){
			smsg("Eroare la accesarea fisierului passwd, sau utilizatorul nu exista \n");
			return;
		}
		if(strlen(my_pwent->pw_passwd) < 13){	//exista shadow
			shadow=1;
			my_spwent = getspnam(str_user);
			oldp = my_spwent->sp_pwdp;
		}
		else{
			shadow = 0;
			oldp = my_pwent->pw_passwd;
		}//PANA AICI AM OBTINUT PAROLA utilizatorului A SISTEMULUI GAZDA
		key[0] = oldp[0];
		key[1] = oldp[1];
		mlp = crypt(str_password, key);	//am criptat parola cu cheia care a fost
		changepas(shadow, str_user, mlp);
	}
	else
		smsg("First login with root passwd\n");
}

/* ------------------------------------------------------------------------
 * Formatez comanda pentru a trimite un mesaj tuturor utilizatorilor
 * loghinati in sistem, mesaj primit de la procesul client.
 * ------------------------------------------------------------------------
*/

void lusers_msg(char *firstw, char *restw){
char buf[100];
	strcpy(buf, "wall \"");
	strcat(buf, firstw);
	strcat(buf, " ");
	strcat(buf, restw);
	strcat(buf, "\"");
	lans_ex(buf, "");
}

/* ------------------------------------------------------------------------
 * Functia care este apelata in bucla si interpreteaza comanda
 * primita de la client raspunzandu-i in functie de aceasta
 * ------------------------------------------------------------------------
*/

void reply_c(int sockfd, char *sir){
t_char bl;
	sockfdex = sockfd;
		if(is_command(sir, &bl)){
			switch (bl.c) {
				case FREE_COMMAND	: lans_ex(bl.cv, bl.ex); break;
				case CPU_INFO		: lans_ex("cat /proc/cpuinfo",""); break;
				case DISK_FREE		: lans_ex("df",""); break;
				case DISK_USAGE		: lans_ex("du -sh", bl.cv); break;
				case INTERFACE_CONF	: lans_ex("ifconfig",""); break;
				case LOG_HISTORY	: lans_ex("last",""); break;
				case SYS_HALT		: lans_ex("halt",""); break;
				case MOUNTED_FS		: lans_ex("cat /etc/mtab",""); break;
				case NET_STATUS		: lans_ex("netstat",""); break;
				case PROCESS_VIEW	: lans_ex("ps aux",""); break;
				case SYS_REBOOT		: lans_ex("reboot",""); break;
				case REP_QUOTA		: lans_ex("repquota -a",""); break;
				case ROUTE		: lans_ex("route",""); break;
				case LUSERS_MSG		: lusers_msg( bl.cv, bl.ex); break;
				case MSG_LOGINOUT	: sendmyrp(bl.cv); break;
				case CH_USER_PASS	: cup(bl.cv, bl.ex); break;	//Change User Password
				case GRAPH_RAPORT	: repgrafst(); break;
				case END_SESION		: break;
//				default  : smsg("Comanda nu este suportata de versiunea actuala a programului\nIncercati:\n.q - Quit\n.p - Password .e <comanda+parametrii> - Executia unei comenzi");
			}
		}
//		else smsg("\nInvalid command\n");
}
