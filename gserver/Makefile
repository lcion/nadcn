OBJS = ex_dem.o server.o fix_str.o cryptl.o
CC = gcc
CFLAGS = -c -Wall
LDFAGS = -o $@

all : CFLAGS += -o3 -o $@
all : LDFAGS += -o3 
all : server

debug : CFLAGS += -DDEBUG -g3 -o $@
debug : server

server : $(OBJS)
	$(CC) $(LDFAGS) $(OBJS) -lcrypt

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
