#include "server.h"
#define MAXLINE 512

typedef struct my_struct{
	char cv[MAXLINE];
	char ex[MAXLINE];
	char c;
}t_char;
int sockfdex;
GDBM_FILE bz;
void smsg(char *msg){
int n;
	n = strlen(msg);
	if (writen(sockfdex, msg, n) != n)
			err_dump("<server> : strecho - eroare in writen");

}

int count_cuv(const char *cheie){
int i = 0, found, len_cv, i_defno;
datum key, nextkey;
	len_cv = strlen(cheie); 
	for(found = 1; found == 1;){
		found = 0;
		key = gdbm_firstkey( bz );
		i_defno = 1 + len_cv + i; //am obtinut key.dsize pe care-l caut
		while(key.dptr){
			if(strcmp(cheie, key.dptr) == 0 && key.dsize == i_defno){
				i++;
				found = 1;
			}
			nextkey = gdbm_nextkey(bz, key);
			key = nextkey;
		}
	}
	return i;
	
}

void afisez(void){
datum key, nextkey, content;
	key = gdbm_firstkey( bz );
	while(key.dptr){
		smsg(" Cuvantul : ");
		smsg(key.dptr);
		content = gdbm_fetch(bz, key);
		smsg(" Explicatia : ");
		smsg(content.dptr);
		smsg("\n");
		nextkey = gdbm_nextkey(bz, key);
		key = nextkey;
	}
}

void sterg(void){
datum key, nextkey;
	key = gdbm_firstkey( bz );
	while(key.dptr){
		nextkey = gdbm_nextkey(bz, key);
		gdbm_delete(bz, key);		
		key = nextkey;
	}
	gdbm_reorganize(bz);
	smsg("\n Am initializat dictionarul \n");

}

void inserare( char *cv, char *ex){
datum key, content;

	if(( key.dptr = malloc(strlen(cv)+1)) == NULL){
		smsg("Not enough memory\n");
		exit(1);
	}
	strcpy(key.dptr, cv);
	key.dsize = strlen(key.dptr)+1+count_cuv(key.dptr);	//sax
	if((content.dptr = malloc(strlen(ex)+1)) == NULL){
		smsg("\n Not enough memory");
		exit(1);
	}
	strcpy(content.dptr, ex);
	content.dsize = strlen(content.dptr)+1;
	gdbm_store(bz, key, content, GDBM_INSERT);
		
	free(key.dptr);
	free(content.dptr);
}

void stergc(const char *cv, const char *defno){
datum key, nextkey;
int len_cv, i_defno;
	i_defno = atoi(defno);	//aici prelucrez defno
	len_cv = strlen(cv); 
	i_defno = i_defno + 1 + len_cv; //am obtinut key.dsize pe care-l caut

	key = gdbm_firstkey( bz );
	while(key.dptr){
		if(strcmp(key.dptr,cv) == 0 && key.dsize == i_defno){
			gdbm_delete(bz, key);
			smsg("\nAm sters un cuvant \n");	
		}	
		nextkey = gdbm_nextkey(bz, key);
		key = nextkey;
	}
	gdbm_reorganize(bz);

}

void explic(const char *cv, const char *defno){
datum key, nextkey, content;
int i_defno;
	i_defno = atoi(defno);	//aici prelucrez defno
	key = gdbm_firstkey( bz );
	while(key.dptr){
		if(strcmp(cv, key.dptr) == 0){
			if(i_defno == 0){
				smsg(" Cuvantul : ");
				smsg(key.dptr);
				content = gdbm_fetch(bz, key);
				smsg(" Explicatia : ");
				smsg(content.dptr);
				smsg("\n");
				i_defno = -1;
			}
			else	i_defno--;
		}
		nextkey = gdbm_nextkey(bz, key);
		key = nextkey;
	}
}

int exista(char *cv){
datum key, content;
	if((key.dptr = malloc(strlen(cv)+1)) == NULL){
		smsg(" Out of mem \n");
		exit(1);
	}
	strcpy(key.dptr, cv);
	key.dsize = strlen(key.dptr)+1;
	content = gdbm_fetch(bz, key);
	free(key.dptr);
	if(content.dptr != NULL){
		smsg("\nCuvantul exista !\n");
		return 1;
	}

	smsg("\nCuvantul nu exista !\n");
	return 0;
	
}

int is_command(char *brut, t_char *block){ //prelucreaza brut returnand 
char *t;				//structura block
	strcpy(block->cv, "\0");
	strcpy(block->ex, "\0");
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

void reply_c(int sockfd, char *sir){
t_char bl;
	sockfdex = sockfd;
	bz = gdbm_open("baza.db", 512, GDBM_WRCREAT, 0600, 0);
		if(is_command(sir, &bl)){
			switch (bl.c) {
				case 'I' : sterg();break;
				case 'i' : inserare(bl.cv, bl.ex);break;
				case 's' : stergc(bl.cv, bl.ex);break;
				case 'c' : exista(bl.cv);break;
				case 'e' : explic(bl.cv, bl.ex);break;
				case 'l' : afisez();
				case 'q' : break;
				default : smsg("\nInvalid command\n");

			}

		}
		else smsg("\nInvalid command\n");

	gdbm_close(bz);
}
