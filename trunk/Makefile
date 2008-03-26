#
# MC326 1s2008
#
# Group 1
# Anderson Phillip Birocchi
# Miguel Francisco Alves de Mattos Gaiowski
# Raphael Kubo da Costa
#

CC=gcc
CFLAGS=-c -Wall -g -ansi -pedantic -lefence
LDFLAGS=
OFILES=data.o io.o main.o menu.o db.o

all: art

art: $(OFILES)
	$(CC) $(LDFLAGS) $(OFILES) -o art 

data.o: data.c data.h
	$(CC) $(CFLAGS) $<

io.o: io.c io.h
	$(CC) $(CFLAGS) $<

main.o: main.c data.h menu.h io.h db.h
	$(CC) $(CFLAGS) $<

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) $<

db.o: db.c db.h
	$(CC) $(CFLAGS) $<

clean:
	cp -v base01.dat base01.dat.back ; \
	rm -rf *.o art base01.dat ; \
	rm -rf *.aux *.log
