INCLD = -I/usr/include
LDIR = -L/usr/X11R6/lib/
CC = gcc
XFFLAGS = -lforms -lX11 -lm
CCFLG = -O -Wall -g -o $@

all: demo05 yesno ayesno hostanduser

prob: proba0.c forms/unCeasfrm.c
	$(CC) $(CCFLG) proba0.c forms/lhostanduser.c forms/unCeasfrm.c $(XFFLAGS) $(INCLD) $(LDIR)

tipag: tipag.c forms/tipagf.c
	$(CC) $(CCFLG)  tipag.c forms/tipagf.c $(XFFLAGS) $(INCLD) $(LDIR)

demo05: demo05.c
	$(CC) $(CCFLG) demo05.c $(XFFLAGS) $(INCLD) $(LDIR)

yesno:
	$(CC) $(CCFLG) yesno.c $(XFFLAGS) $(INCLD) $(LDIR)

ayesno:
	$(CC) $(CCFLG) ayesno.c $(XFFLAGS) $(INCLD) $(LDIR)

yesno1:
	$(CC) $(CCFLG) yesno1.c $(XFFLAGS) $(INCLD) $(LDIR)

iclient:
	$(CC) $(CCFLG) iclient.c $(XFFLAGS) $(INCLD) $(LDIR)

hostanduser: forms/lhostanduser.c
	$(CC) $(CCFLG) iclient.c forms/lhostanduser.c $(XFFLAGS) $(INCLD) $(LDIR)

clean:
	rm -f demo05 yesno ayesno yesno1 iclient hostanduser
