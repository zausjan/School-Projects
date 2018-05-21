CC=gcc
CXX=g++
CFLAGS=-O2 -std=c99 -g -Wall -pedantic
CXXFLAGS=-O2 -std=c++11 -g -Wall -pedantic
LIBFILES=htab_hash_function.o htab_init.o htab_move.o htab_size.o htab_bucket_count.o htab_lookup_add.o htab_find.o htab_foreach.o htab_remove.o htab_clear.o htab_free.o

all: tail tail2 wordcount wordcount-dynamic

tail: tail.c
	$(CC) $(CFLAGS) -o tail tail.c
tail2: tail2.cc
	$(CXX) $(CXXFLAGS) -o tail2 tail2.cc

wordcount:wordcount.o io.o libhtab.a
	$(CC) $(CFLAGS) -o wordcount io.o wordcount.o libhtab.a

wordcount-dynamic:wordcount.o io.o libhtab.so
	$(CC) $(CFLAGS) -o wordcount-dynamic io.o wordcount.o libhtab.so

wordcount.o:wordcount.c htab.h
	$(CC) $(CFLAGS) -c wordcount.c -o wordcount.o

############################ STATICKA KNIZNICA #################################
libhtab.a:$(LIBFILES)
	ar crs libhtab.a $(LIBFILES)

############################ DYNAMICKA KNIZNICA ################################
libhtab.so:$(LIBFILES)
	gcc $(CFLAGS) -shared -fPIC -o libhtab.so $(LIBFILES)

################################# BINARKY ######################################
io.o:io.c io.h
	$(CC) $(CFLAGS) -c io.c -o io.o

htab_hash_function.o:htab_hash_function.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_hash_function.c -o htab_hash_function.o
htab_init.o:htab_init.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_init.c -o htab_init.o
htab_move.o:htab_move.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_move.c -o htab_move.o
htab_size.o:htab_size.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_size.c -o htab_size.o
htab_bucket_count.o:htab_bucket_count.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_bucket_count.c -o htab_bucket_count.o
htab_lookup_add.o:htab_lookup_add.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_lookup_add.c -o htab_lookup_add.o
htab_find.o:htab_find.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_find.c -o htab_find.o
htab_foreach.o:htab_foreach.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_foreach.c -o htab_foreach.o
htab_remove.o:htab_remove.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_remove.c -o htab_remove.o
htab_clear.o:htab_clear.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_clear.c -o htab_clear.o
htab_free.o:htab_free.c htab.h
	 $(CC) $(CFLAGS) -fPIC -c htab_free.c -o htab_free.o

clean:
	rm tail tail2 wordcount wordcount-dynamic libhtab.a libhtab.so *.o
