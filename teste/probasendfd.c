
#include "srfd.h"

int fd[2];   //pipe-ul folosit la transmiterea descriptorului

int fiul1(){
   int fdfs[2];  //File Descriptorul de trimis ...
     if(pipe(fdfs) < 0){
	perror("fiul1: Can not create testpipe");
	return 1;
     }
   send_fd(fd[1],fdfs[0]);   //trimit pipe-descriptorul pentru citire
   write(fdfs[1], "12345", 5);
   return 0;
}

int fiul2(){
   int gfds;
   char buf[5];
   gfds = recv_fd(fd[0]);   //primeste descriptorul de comunicatie prin pipe
   read(gfds, buf, 5);       //citeste intr-un buffer cinci caractere
   printf("\nFrom fiul2: %s", buf);
   return 0;
}

int main(){
//   int n;
   if(pipe(fd) < 0){
      perror("pipe creator ?...");
      return 1;
   }
   switch(fork()){          //lansez primul proces care va creea un descriptor
    case 0: exit(fiul1()); break;
    case -1: perror("fork ...");
      return 1;
   }
   switch(fork()){         // lansez al doilea proces care va primi un descriptor prin pipe
    case 0: exit(fiul2());break;
    case 1: perror("fork...");
      return 1;
   }
   
   getchar();
   return 0;
}
