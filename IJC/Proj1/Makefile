CC=gcc
CFLAGS=-O2 -std=c99 -lm -Wall -pedantic
SHELL=/bin/bash
all: primes	primes-i steg-decode

steg-decode: ppm.o error.o eratosthenes.o steg-decode.o
	$(CC) $(CFLAGS)	steg-decode.o ppm.o	error.o	-o steg-decode

primes: primes.o error.o eratosthenes.o
	$(CC) $(CFLAGS)	primes.o error.o eratosthenes.o	-o primes

primes-i: primes.o error.o eratosthenes.o
	$(CC) $(CFLAGS) -DUSE_INLINE -lm primes.o error.o eratosthenes.o -o primes-i

primes.o: error.c error.h eratosthenes.h
	$(CC) $(CFLAGS) -c primes.c -o primes.o

eratosthenes.o:	error.c	error.h	bit_array.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

steg-decode.o: ppm.h error.h bit_array.h
	$(CC) $(CFLAGS) -c steg-decode.c -o steg-decode.o

ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c ppm.c -o ppm.o
run: primes primes-i
	time ./primes
	time ./primes-i
clean:
	rm -f steg-decode primes primes-i *.o
zip: *.c *.h Makefile
	zip xzausk00.zip *.c *.h Makefile
