# include <shadow.h>
# include <stdio.h>
# define  _XOPEN_SOURCE
# include <unistd.h>
# include <crypt.h>
# include <pwd.h>
# include <string.h>

int cripteaza(char *oldp){	//old password

char key[3], *newpass, *mlp;

	key[0] = oldp[0];
	key[1] = oldp[1];
	

	newpass = getpass("Introduceti Parola :");
	mlp = crypt(newpass, key);
	if(strcmp(mlp, oldp) == 0)
		printf("Parola Valida\n");
	else
		printf("Parola invalida\n");
	return 0;
}

void main(int argn, char **argc){
struct spwd *my_pwent;
	if(argn >= 2)
		my_pwent = getspnam(argc[1]);
	else
		printf("User parameter request!\n");
	if(my_pwent != NULL){
		printf("Utilizatorul %s are parola criptata: %s\n", my_pwent->sp_namp, my_pwent->sp_pwdp);
		cripteaza(my_pwent->sp_pwdp);
	}
	else
		printf("Nu Exista utilizatorul %s\n", argc[1]);

}



/* Recicled Code
# include <sys/types.h>
main:
		printf("Utilizatorul %s are parola criptata: %s\n", my_pwent->sp_namp, my_pwent->sp_pwdp);
cripteaza:
	for(i = 0; i <= 7; i++){
		newpass[i] = getchar();
		if( newpass[i] == ' ')
			newpass[i] = 0;
	}
	newpass[i] = 0;

*/