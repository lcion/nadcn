# include <shadow.h>
# include <stdio.h>
# define  _XOPEN_SOURCE
# include <unistd.h>
# include <crypt.h>
# include <pwd.h>
# include <string.h>
# include <pwd.h>
# include <sys/types.h>

int test(){
struct passwd *my_pwent;
struct spwd *my_spwent;
char key[3], *newpass, *mlp, *oldp;

	newpass = getpass("Introduceti Parola Supervizor (root) :");

	my_pwent = getpwnam("root");
	if(my_pwent == NULL){
		printf("Eroare la accesarea fisierului passwd, nu exista root\n");
		return 0;
	}
	if(strcmp(my_pwent->pw_passwd, "x") == 0){	//exista shadow
		my_spwent = getspnam("root");
		oldp = my_spwent->sp_pwdp;
	}
	else{
		oldp = my_pwent->pw_passwd;
	}
	key[0] = oldp[0];
	key[1] = oldp[1];
	mlp = crypt(newpass, key);
	if(strcmp(mlp, oldp) == 0)
		return 1;
		
	return 0;
}

void main(){
	if(test())
		printf("OK !\n");
	else
		printf("No Ok !\n");
}