/* Executa cererea clientului si-i trimite clientului rezultatul */
#include "server.h"


typedef struct my_struct{
	char cv[MAXLINE];
	char ex[MAXLINE];
	char c;
}t_char;

int sockfdex, acces;

void smsg(char *msg){
int n;
	n = strlen(msg);
	if (writen(sockfdex, msg, n) != n)
			err_dump("<server> : strecho - eroare in writen");

}

int is_command(char *brut, t_char *block){ //prelucreaza brut returnand 
char *t;
int brut_len;				//structura block
	strcpy(block->cv, "\0");
	strcpy(block->ex, "\0");
	brut_len = strlen(brut);
	brut_len -= 1;
	brut[brut_len] = 0;
	t=strdup(brut);
	if((brut[0] == '.') && strlen(brut) >=2 ){
		block->c=brut[1];
		t=strtok(brut," \t");
		t = strtok(NULL, " \t");
		if(t != NULL){
			strcpy(block->cv, t);
			t = strtok(NULL, " \t");
			while(t != NULL){
				strcat(block->ex, t);
				strcat(block->ex, " ");
				t = strtok(NULL, " \t");
			}
		}
		return 1;
	}
	else
	return 0;
}
void sendtoc(){	//trimite iesirea comenzii catre client si sterge fisierul.
	FILE *fd;
	char mybuf[MAXLINE];
	if( (fd = fopen("fsfile00","rt")) != NULL){	//deschid fisierul nou creat
		while( !feof(fd) ){
			fscanf(fd, "%c", mybuf);
			smsg(mybuf);
		
		}
		smsg("\n");
		fclose(fd);
		remove("fsfile00");
	}
}

void lans_ex(char *comanda, char *parametru){
	int wst;
	char redar[255];
	if( acces == 1 ){
		redar[0] = '\0';
		strcat(redar, comanda);
		strcat(redar, " ");
		if( strlen(parametru) > 0){
			strcat(redar, parametru);
			strcat(redar, " ");
		}
		strcat(redar, ">fsfile00");
		if(fork() == 0) 
			wst = execl("/bin/sh", "sh", "-c", redar, NULL);
		if(wst < 0)
			perror("Eroare in execl");
		
		
		wait(&wst);
	
		sendtoc();
	}
	else
		smsg("First login with root passwd: hit .p\n");
}
void sendmyrp(char *aut){
struct passwd *my_pwent;
struct spwd *my_spwent;
char key[18], *oldp;
	if(strcmp(aut,"PASSWDOK") != 0){
		my_pwent = getpwnam("root");
		if(my_pwent == NULL){
			smsg("Eroare la accesarea fisierului passwd, nu exista root\n");
			return;
		}
		if(strcmp(my_pwent->pw_passwd, "x") == 0){	//exista shadow
			my_spwent = getspnam("root");
			oldp = my_spwent->sp_pwdp;
		}
		else{
			oldp = my_pwent->pw_passwd;
		}
		strcpy(key,".p");
		strcat(key,oldp);
		strcat(key,"\n");
		smsg(key);
	}
	else{
		smsg("Acces OK! \n");
		acces = 1;
	}
}

void reply_c(int sockfd, char *sir){
t_char bl;
	sockfdex = sockfd;
		if(is_command(sir, &bl)){
			switch (bl.c) {
				case 'e' : lans_ex(bl.cv, bl.ex);break;
				case 'q' : break;
				case 'p' : sendmyrp(bl.cv);break;
				default  : smsg("Comanda nu este suportata de versiunea actuala a programului\nIncercati:\n.q - Quit\n.p - Password .e <comanda+parametrii> - Executia unei comenzi");

			}

		}
		else smsg("\nInvalid command\n");

}
