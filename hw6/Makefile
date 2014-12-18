#
# Makefile that builds sortfile.c and fileio.c
# 
CC = gcc
CFLAGS = -O -Wall 
DEPS = fileio.h


all: sortfile

sortfile: sortfile.c fileio.c
	$(CC) $(CFLAGS) -o sortfile sortfile.c fileio.c

fileio: fileio.h fileio.c sortfile.c
	$(CC) $(CFLAGS) -o sortfile sortfile.c fileio.c

clean:
	rm sortfile