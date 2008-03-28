#
# MC326 1s2008
#
# Group 1
# Anderson Phillip Birocchi
# Miguel Francisco Alves de Mattos Gaiowski
# Raphael Kubo da Costa
#

CC=gcc
CFLAGS=-c -Wall -g -ansi -pedantic
LDFLAGS=-lefence
OFILES=data.o file.o io.o html.o main.o mem.o menu.o pk.o

all: art

art: $(OFILES)
	$(CC) $(LDFLAGS) $(OFILES) -o art 

data.o: data.c data.h
	$(CC) $(CFLAGS) $<

file.o: file.c file.h
	$(CC) $(CFLAGS) $<

html.o: html.c html.h
	$(CC) $(CFLAGS) $<

io.o: io.c io.h
	$(CC) $(CFLAGS) $<

main.o: main.c data.h menu.h io.h
	$(CC) $(CFLAGS) $<

mem.o: mem.c mem.h
	$(CC) $(CFLAGS) $<

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) $<

pk.o: pk.c pk.h
	$(CC) $(CFLAGS) $<

clean:
	cp -v base01.dat base01.dat.back ; \
	rm -rf *.o art base01.dat ; 
