

CC=			gcc
CFLAGS=		-g -Wall -Wc++-compat -std=c99 -O2
CPPFLAGS=
INCLUDES=


bio:
	$(CC) -o bio main.c $(CFLAGS)




