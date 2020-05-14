CC=gcc
CCFLAGS=-I.

all : client server

client : libMultModulo.o libMultModulo.h
	$(CC) -o client libMultModulo.o client.c -lpthread $(CCFLAGS)

server : libMultModulo.o libMultModulo.h
	$(CC) -o server libMultModulo.o server.c -lpthread $(CCFLAGS)

libMultModulo.o : libMultModulo.h
	$(CC) -o libMultModulo.o -c libMultModulo.c $(CCFLAGS)

clean :
	rm client server libMultModulo.o