OBJS = ex_dem.o server.o
CC = cl
CFLAGS = -c -g -Wall

all : server


server : $(OBJS)
	$(CC) -o $@ $(OBJS) -lcrypt
ex_dem.o : ex_dem.c
	$(CC) $(CFLAGS) ex_dem.c

server.o : server.c
	$(CC) $(CFLAGS) server.c

clean :
	rm -f *.o
	rm -f server
	cld
