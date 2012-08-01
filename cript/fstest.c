#define MAXS 255
#include <stdio.h>

void fix_string(char *, int);	//functie de criptare definita in sursa fix_string.c

void main(){
	char buf[MAXS];		//aloc o zona de memorie pentru sir de criptare
	int i, n = 0, slt = 255;
	while((i = getc(stdin)) != EOF){
		if(n >=  slt-1){	// daca s-au citit slt char din fisier
			buf[n] = i;
			fix_string(buf, n+1);	//criptez slt charactere
			n = 0;
			for(i = 0; i < slt; i++)
				putchar( buf[i] );	//afisez sirul criptat
			if (slt > 10)		// modific saltul pentru testul de salt variabil
				slt -= 5;
			else
				slt = 255;
		}
		else{
			buf[n] = i;
			n++;
		}
	}
	if(n > 0){	//a mai ramas ceva in buffer ce nu am criptat
		fix_string(buf, n);
		for(i = 0; i < n; i++)
			putchar( buf[i] );
	}
}
