CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic -g
LFLAGS=-lpthread
BIN=proj2
SOURCE=proj2.c

all:
	$(CC) $(CFLAGS) $(SOURCE) -o $(BIN) $(LFLAGS)

run: all
	./$(BIN)

clean:
	rm $(BIN)
