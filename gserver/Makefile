OBJS = ex_dem.o server.o fix_str.o cryptl.o
CC = gcc
CFLAGS = -c -Wall
LDFAGS = -o $@

all : CFLAGS += -O3 -o $@
all : LDFAGS += 
all : server

debug : CFLAGS += -DDEBUG -g3 -o $@
debug : server

server : $(OBJS)
	$(CC) $(LDFAGS) $(OBJS) -lcrypt

%.o: %.c
	$(CC) $(CFLAGS) $<

fix_str.o: ../cript/fix_string.c
	$(CC) $(CFLAGS) $<

cryptl.o: ../cript/cryptl.c
	$(CC) $(CFLAGS) $<

clean :
	rm -f *.o *~
	rm -f server
