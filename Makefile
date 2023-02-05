

CC=			gcc
CFLAGS=		-g -Wall -Wc++-compat -std=c99 -O2
CPPFLAGS=
INCLUDES=


all: build/bio build/lsb


build/bio:
	$(CC) -o build/bio main.c $(CFLAGS)

build/lsb:
	$(CC) -o build/lsb lsb.c $(CFLAGS)

build/chtax:
	$(CC) -o build/chtax utils.c chtax.c $(CFLAGS)

build/rmtax:
	$(CC) -o build/rmtax utils.c rmtax.c $(CFLAGS)


db/taxons/taxons:
	cd db/taxons; virtualenv -p python3 .venv; source .venv/bin/activate ; pip3 install pywget tqdm; python3 ./update-taxons.py

