

CC=			gcc
CFLAGS=		-g -Wall -Wc++-compat -std=c99 -O2
CPPFLAGS=
INCLUDES=


all: build/bio build/lsb


build/bio:
	$(CC) -o build/bio main.c $(CFLAGS)

build/lsb:
	$(CC) -o build/lsb lsb.c $(CFLAGS)

