INCLD = -I/usr/include
LDIR = -L/usr/X11R6/lib/
CC = gcc
XFFLAGS = -lforms -lX11 -lm -lcrypt
CFLAGS = -c -Wall -o $@

OBJS = lclient.o lhostanduser.o fclient.o gccommander.o cgpassf.o inpforocc.o landmincp.o fix_str.o cryptl.o

all: CFLAGS += -O3 
all: lacli

debug: CFLAGS += -g
debug: lacli

lacli : $(OBJS)
	$(CC) -o $@ $(OBJS) $(XFFLAGS) $(LDIR)

lclient.o : lclient.c
	$(CC) $(CFLAGS) lclient.c $(INCLD)

fix_str.o: ../cript/fix_string.c
	$(CC) $(CFLAGS) ../cript/fix_string.c

cryptl.o: ../cript/cryptl.c
	$(CC) $(CFLAGS) ../cript/cryptl.c

lhostanduser.o : forms/lhostanduser.c
	$(CC) $(CFLAGS) forms/lhostanduser.c $(INCLD)

fclient.o : fclient.c
	$(CC) $(CFLAGS) fclient.c $(INCLD)

gccommander.o : forms/gccommander.c
	$(CC) $(CFLAGS) forms/gccommander.c $(INCLD)

cgpassf.o : forms/cgpassf.c
	$(CC) $(CFLAGS) forms/cgpassf.c $(INCLD)

inpforocc.o : forms/inpforocc.c
	$(CC) $(CFLAGS) forms/inpforocc.c $(INCLD)

landmincp.o : forms/landmincp.c
	$(CC) $(CFLAGS) forms/landmincp.c $(INCLD)

clean:
	rm -f lacli
	rm -f *.o *~
