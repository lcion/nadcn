INCLD = -I/usr/include
LDIR = -L/usr/X11R6/lib/
CC = gcc
XFFLAGS = -lforms -lX11 -lm
CCFLG = -g -c -O -Wall -o $@

OBJS = lclient.o lhostanduser.o fclient.o gccommander.o cgpassf.o inpforocc.o landmincp.o fix_str.o cryptl.o

all: lacli

lacli : $(OBJS)
	$(CC) -o $@ $(OBJS) $(XFFLAGS) $(LDIR)
	lacli &

lclient.o : lclient.c
	$(CC) $(CCFLG) lclient.c $(INCLD)

fix_str.o: ../cript/fix_string.c
	$(CC) $(CCFLG) ../cript/fix_string.c

cryptl.o: ../cript/cryptl.c
	$(CC) $(CCFLG) ../cript/cryptl.c

lhostanduser.o : forms/lhostanduser.c
	$(CC) $(CCFLG) forms/lhostanduser.c $(INCLD)

fclient.o : fclient.c
	$(CC) $(CCFLG) fclient.c $(INCLD)

gccommander.o : forms/gccommander.c
	$(CC) $(CCFLG) forms/gccommander.c $(INCLD)

cgpassf.o : forms/cgpassf.c
	$(CC) $(CCFLG) forms/cgpassf.c $(INCLD)

inpforocc.o : forms/inpforocc.c
	$(CC) $(CCFLG) forms/inpforocc.c $(INCLD)

landmincp.o : forms/landmincp.c
	$(CC) $(CCFLG) forms/landmincp.c $(INCLD)

clean:
	rm -f lacli
	rm -f *.o
	cld
