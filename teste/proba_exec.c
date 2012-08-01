#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void main(int darg, char **args){
	int nw;
	if( fork()==0 )execl( "/bin/sh", "sh", "-c", "ps aux >myfis", NULL);
	wait(&nw);
	printf("Success\n");
}