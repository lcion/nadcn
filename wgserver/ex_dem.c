/* Executa cererea clientului si-i trimite clientului rezultatul */
#include "server.h"


typedef struct my_struct{
	char cv[MAXLINE];
	char ex[MAXLINE];
	char c;
}t_char;

int sockfdex, acces;
void fix_string(char *, int);

void smsg(char *msg){
int	n; 
char buffer[MAXLINE], b1[MAXLINE+2];
	strcpy_s(buffer, MAXLINE, msg);
	n = strlen(buffer);
	fix_string(buffer, n);
	b1[0] = n>>8;		//byte-ul cel mai semnificativ
	b1[1] = n<<8>>8;	//byte-ul cel mai putin semnificativ
	memcpy(b1+2, buffer, n);
	if (writen(sockfdex, b1, n+2) != n+2)
		err_dump("<server> : strecho - eroare in writen");

}

int readprot(register int fd, register char *ptr, register int maxlen){
int	n, rc;
char    buffer[MAXLINE];
	if ( (rc = recv(fd, buffer, 2, 0)) == 2){
		n = (unsigned char)  buffer[0] << 8;	//partea cea mai semnificativa a lui n
		n += (unsigned char) buffer[1];		//partea cea mai putin semnificativa a lui n
	}
	else
		return(-1);
	if ( (rc = recv(fd, buffer, n, 0)) == n){
		fix_string(buffer, n);	//decriptez
		buffer[n]=0;		//pun terminator de sir
		strcpy_s(ptr, maxlen, buffer);
		return (n);
	}
	return(-1);	//eroare
}


int is_command(char *brut, t_char *block){ //prelucreaza brut returnand 
char *token, *nex_token;
	strcpy_s(block->cv, MAXLINE, "\0");
	strcpy_s(block->ex, MAXLINE, "\0");
	token = _strdup(brut);
	if((brut[0] == '.') && strlen(brut) >=2 ){
		block->c = brut[1];
		token = strtok_s(brut, " \t", &nex_token);
		token = strtok_s(NULL, " \t", &nex_token);
		if(token != NULL){
			strcpy_s(block->cv, MAXLINE, token);
			token = strtok_s(NULL, " \t", &nex_token);
			while(token != NULL){
				strcat_s(block->ex, MAXLINE, token);
				token = strtok_s(NULL, " \t", &nex_token);
				if(token != NULL)
					strcat_s(block->ex, MAXLINE, " ");
			}
		}
		return 1;
	}
	else
	return 0;
}


void edisk_fspace(){	//informatii de baza despre driverele de disk din sistem
unsigned long SecpClust;
unsigned long BytespSector;
unsigned long freeClust;
unsigned long TotClust;
int drive;
char rpn[5], buf[MAXLINE];
		strcpy_s(rpn, 5, "C:\\");
		for( drive = 3; drive <= 26; drive++){
			rpn[0] = 'A' + drive - 1;
			if(GetDiskFreeSpace( rpn, &SecpClust, &BytespSector, &freeClust, &TotClust)){
				sprintf_s(buf, MAXLINE, "\n Drive %s has parameters:\n",rpn);
				smsg(buf);
				sprintf_s(buf, MAXLINE, "Sectors per cluster: %u\nBytes per Sector: %u\nFree Clusters: %u\nTotal Clusters: %u\nFree Space: %15u\n",
					SecpClust, BytespSector, freeClust, TotClust, freeClust*SecpClust*BytespSector);
				smsg(buf);
			}
		}

	smsg(".e");
}

void eifconfig(){	//Ofer info despre placa de retea 
	smsg("Ofer info despre placa de retea \n");
	smsg(".e");
}

void elast(){	//Ofer info despre ultimi user ce s-au conectat la sistem
	smsg("Ofer info despre ultimi user ce s-au conectat la sistem\n");
	smsg(".e");
}

void eamounted_fs(){	//Ofer info despre sistemele de fisiere mapate ...
	smsg("Ofer info despre sistemele de fisiere mapate ...\n");
	smsg(".e");
}

void enetstat(){	//Ofer info despre starea interfetelor de retea 
	smsg("Ofer info despre starea interfetelor de retea \n");
	smsg(".e");
}

void eproces_list(){		//Ofer info despre starea proceselor active 
	smsg("Ofer info despre starea proceselor active \n");
	smsg(".e");
}

void erepuqota(){		//Ofer info spatiul inpartit al utilizatorilor 
	smsg("Ofer info spatiul inpartit al utilizatorilor \n");
	smsg(".e");
}

void eroute(){		//Ofer info despre interfetele ce permit rutare
	smsg("Ofer info despre interfetele ce permit rutare\n");
	smsg(".e");
}

void lusers_msg(char *cv, char *ex){
	smsg("Masina Windows si nu se conecteaza nimeni pe ea inafara de utilizatorul curent :)\n");
	smsg(".e");
}

void sendtoc(char *ftoop){
FILE *fd;
	char mybuf[MAXLINE];
	if( (fd = _popen(ftoop, "rt")) != NULL){	//deschid fisierul nou creat
		while( !feof(fd) ){
			mybuf[0]=mybuf[1]=0;
			fscanf_s(fd, "%c", mybuf, MAXLINE);
			smsg(mybuf);

		}
		smsg("\n");
		smsg(".e\n");
		_pclose(fd);
	}
}
void cpuinf(){
	char mybuf[MAXLINE];
	//? sscanf(ftoop, "%s", mybuf);
	strcpy_s(mybuf, MAXLINE, "C:\\TEMP\\Luci\\gserver\\Debug\\cpuid3.exe");
	sendtoc(mybuf);
	return;
}

void disk_usage(char *primc, char *rest){
	char mybuf[MAXLINE];
//?	sscanf(ftoop, "%s", mybuf);
//?		char *tmp;
//?		tmp = ftoop + 6;
		strcpy_s(mybuf, MAXLINE, "dir /s /a /w");
		strcat_s(mybuf, MAXLINE, primc);
		strcat_s(mybuf, MAXLINE, rest);
		sendtoc(mybuf);
}

void lans_ex(char *comanda, char *parametru){

	char redar[255];
	if( acces == 1 ){
		redar[0] = '\0';
		strcat_s(redar, 255, comanda);
		strcat_s(redar, 255, " ");
		if( strlen(parametru) > 0){
			strcat_s(redar, 255, parametru);
			strcat_s(redar, 255, " ");
		}
	   sendtoc(redar);
	}
	else
		smsg("First login with root passwd: hit .p\n.e\n");
}

void repgrafst(){
char mybuf[MAXLINE];
MEMORYSTATUS lpmemStat;
DWORD dwUptime;
int ore = 0, minute = 0;
float load, f1;
	GlobalMemoryStatus(&lpmemStat);
	sprintf_s(mybuf, MAXLINE, "%ld %ld %ld %ld \n", (lpmemStat.dwTotalPhys - lpmemStat.dwAvailPhys)/1000,
		lpmemStat.dwAvailPhys/1000, (lpmemStat.dwTotalVirtual - lpmemStat.dwAvailVirtual)/1000,
		lpmemStat.dwAvailVirtual/1000);
	smsg(mybuf);
	dwUptime = SnmpSvcGetUptime();
	ore = dwUptime/360000;
	minute = dwUptime/6000 - ore*60;
	f1 = (float)lpmemStat.dwMemoryLoad ;
	load = f1 / 100;
	sprintf_s(mybuf, MAXLINE, "%ld:%ld %f\n", ore, minute, load);
	smsg(mybuf);
	smsg(".e\n");
}

void sendmyrp(char *aut){
	acces = 1;
	smsg("ACCES_OK\n");

}

void changepas(char shadow, char *str_user, char *mlp){
}

void cup(char *str_user, char *str_password){ //char* aut
	smsg("Windows Machine Option not inplemennted\n");
}

void reply_c(int sockfd, char *sir){
t_char bl;
	sockfdex = sockfd;
		if(is_command(sir, &bl)){
			switch (bl.c) {
				case FREE_COMMAND	: lans_ex(bl.cv, bl.ex); break;
				case CPU_INFO		: cpuinf(); break;
				case DISK_FREE		: edisk_fspace(); break;
				case DISK_USAGE		: disk_usage(bl.cv, bl.ex); break;
				case INTERFACE_CONF	: eifconfig(); break;
				case LOG_HISTORY	: elast(); break;
				case SYS_HALT		: lans_ex("halt",""); break;
				case MOUNTED_FS		: eamounted_fs(); break;
				case NET_STATUS		: enetstat(); break;
				case PROCESS_VIEW	: eproces_list(); break;
				case SYS_REBOOT		: lans_ex("reboot",""); break;
				case REP_QUOTA		: erepuqota(); break;
				case ROUTE			: eroute(); break;
				case LUSERS_MSG		: lusers_msg( bl.cv, bl.ex); break;
				case MSG_LOGINOUT	: sendmyrp(bl.cv); break;
				case CH_USER_PASS	: cup(bl.cv, bl.ex); break;	//Change User Password
				case GRAPH_RAPORT	: repgrafst(); break;
				case END_SESION		: break;
				default  : smsg("Comanda nu este suportata de versiunea actuala a programului\nIncercati:\n.q - Quit\n.p - Password .e <comanda+parametrii> - Executia unei comenzi");
			}

		}
		else smsg("\nInvalid command\n");
}
