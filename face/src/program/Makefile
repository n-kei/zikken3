# Makefile
CC = gcc -Wall -O3

#bmp.o: bmp.c bitmap.h
#	$(CC) -c bmp.c
#
#main: main.c bmp.o
#	$(CC) bmp main.c bmp.o
#
#add.o: addon.c addon.h
#	$(CC) -c addon.c
#
#add: main.c addon.c
#	$(CC) -o bmp_add main.c addon.c

add: main.c addon.c bmp.c
	$(CC) -o bmp_add main.c bmp.c addon.c
