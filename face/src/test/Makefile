CC = gcc
CFLAGS = -Wall -O3

all: main

main: main.c bitmap.o
	$(CC) $(CFLAGS) -o main main.c bitmap.o

bitmap.o: bitmap.c bitmap.h
	$(CC) $(CFLAGS) -c bitmap.c

clean:
	$(RM) *.o *~ main
