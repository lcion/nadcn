test: cryptl.c fix_string.c
	gcc -g -o fstest cryptl.c fix_string.c fstest.c

fix_str: cryptl.c fix_string.c
	gcc -c -o fix_str.o cryptl.c fix_string.c

debug: cryptl.c fix_string.c
	gcc -g -c -o fix_str.o cryptl.c fix_string.c
	
clean:
	cld
	rm -f *.o
fstest <fstest.c >fstest.crypt
