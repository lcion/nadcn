# include <pwd.h>
# include <sys/types.h>
# include <stdio.h>

void main(int argn, char **argc){
struct passwd *my_pwent;
	if(argn >= 2)
		my_pwent = getpwnam(argc[1]);
	else
		printf("User parameter request!\n");
	if(my_pwent != NULL)
		printf("Utilizatorul %s are parola %s\n", my_pwent->pw_name, my_pwent->pw_passwd);
	else
		printf("Nu Exista utilizatorul %s\n", argc[1]);
}