//
//
//		Algoritmul de criptare folosit pentru transmiterea mesajelor 
//				pe retea.
//
//
char *cryptl(char *, char *);
//static long seed = 123;

#define ECHO 010
#define ROTORSZ 256
#define MASK 0377

void	shuffle(char [], char *, long *);

void setup(char *pw, char *t1, char *t2,  char *t3,  char *buf,  char *deck, long *seed){
	int ic, i, k, temp;
	unsigned random;

	buf[0] = pw[0];
	buf[1] = pw[1];
	buf[2] = 0;
	cryptl(pw, buf);
	for(i = 0; i < 13; i++)
		buf[i] = pw[i];

	*seed = 123;
	for (i=0; i<13; i++)
		*seed = *seed*buf[i] + i;
	for(i=0;i<ROTORSZ;i++) {
		t1[i] = i;
		deck[i] = i;
	}
	for(i=0;i<ROTORSZ;i++) {
		*seed = 5**seed + buf[i%13];
		if( sizeof(long) > 4 )  {
			/* Force seed to stay in 32-bit signed math */
			if( *seed & 0x80000000 )
				*seed = *seed | (-1L & ~0xFFFFFFFFL);
			else
				*seed &= 0x7FFFFFFF;
		}
		random = *seed % 65521;
		k = ROTORSZ-1 - i;
		ic = (random&MASK)%(k+1);
		random >>= 8;
		temp = t1[k];
		t1[k] = t1[ic];
		t1[ic] = temp;
		if(t3[k]!=0) continue;
		ic = (random&MASK) % k;
		while(t3[ic]!=0) ic = (ic+1) % k;
		t3[k] = ic;
		t3[ic] = k;
	}
	for(i=0;i<ROTORSZ;i++)
		t2[t1[i]&MASK] = i;
}

void fix_string(char *strof, int strl){	//binary string with len = srtl
char	t1[ROTORSZ];
char	t2[ROTORSZ];
char	t3[ROTORSZ];
char	deck[ROTORSZ];
char	buf[13], buf1[13];
	long seed;
	register int i, n1, n2, nr1, nr2;
	int secureflg = 1;
	for(i = 0; i < ROTORSZ; i++){
		t1[i] = t2[i] = t3[i] = deck[i] = 0;
	}
	strcpy(buf1, "*@d^_~#d%^7*a");
	setup(buf1, t1, t2, t3, buf, deck, &seed);
//	fprintf(stderr,"");
	n1 = 0;
	n2 = 0;
	nr2 = 0;

	for(i = 0; i < strl; i++){
		if (secureflg) {
			nr1 = deck[n1]&MASK;
			nr2 = deck[nr1]&MASK;
		} else {
			nr1 = n1;
		}
		strof[i] = t2[(t3[(t1[(strof[i] +nr1)&MASK]+nr2)&MASK]-nr2)&MASK]-nr1;
		n1++;
		if(n1==ROTORSZ) {
			n1 = 0;
			n2++;
			if(n2==ROTORSZ) n2 = 0;
			if (secureflg) {
				shuffle(deck, buf, &seed);
			} else {
				nr2 = n2;
			}
		}
	}
}

void shuffle(char deck[], char *buf, long *seed)
{
	int i, ic, k, temp;
	unsigned random;
	*seed = 123;
	for(i=0;i<ROTORSZ;i++) {
		*seed = 5**seed + buf[i%13];
		random = *seed % 65521;
		k = ROTORSZ-1 - i;
		ic = (random&MASK)%(k+1);
		temp = deck[k];
		deck[k] = deck[ic];
		deck[ic] = temp;
	}
}
