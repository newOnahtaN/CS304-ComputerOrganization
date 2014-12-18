#
# Makefile that builds sortfile.c and fileio.c
# 
CC = gcc
CFLAGS = -O -Wall 
#-m32

all: sortfile

sortfile: fileio.h sortfile.c fileio.c
	$(CC) $(CFLAGS) -o sortfile sortfile.c fileio.h fileio.c

fileio: fileio.h fileio.c sortfile.c
	$(CC) $(CFLAGS) -o sortfile sortfile.c fileio.h fileio.c

clean:
	rm sortfile