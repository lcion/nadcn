OBJS = ex_dem.o server.o fix_str.o cryptl.o
CC = gcc
CFLAGS = -c -g -Wall -o $@

all : server
#	server &

server : $(OBJS)
	$(CC) -o $@ $(OBJS) -lcrypt

ex_dem.o : ex_dem.c
	$(CC) $(CFLAGS) ex_dem.c

server.o : server.c
	$(CC) $(CFLAGS) server.c

fix_str.o: ../cript/fix_string.c
	$(CC) $(CFLAGS) ../cript/fix_string.c

cryptl.o: ../cript/cryptl.c
	$(CC) $(CFLAGS) ../cript/cryptl.c

clean :
	rm -f *.o *~
	rm -f server
#	cld
