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
OFILES=adapter.o avail.o base.o file.o io.o html.o main.o mem.o memindex.o menu.o secindex.o

TP=tp3

all: art

art: $(OFILES)
	$(CC) $(LDFLAGS) $(OFILES) -o $(TP)

adapter.o: adapter.c adapter.h
	$(CC) $(CFLAGS) $<

avail.o: avail.c avail.h
	$(CC) $(CFLAGS) $<

base.o: base.c base.h
	$(CC) $(CFLAGS) $<

file.o: file.c file.h
	$(CC) $(CFLAGS) $<

html.o: html.c html.h
	$(CC) $(CFLAGS) $<

io.o: io.c io.h
	$(CC) $(CFLAGS) $<

main.o: main.c base.h menu.h io.h
	$(CC) $(CFLAGS) $<

mem.o: mem.c mem.h
	$(CC) $(CFLAGS) $<

memindex.o: memindex.c memindex.h
	$(CC) $(CFLAGS) $<

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) $<

secindex.o: secindex.c secindex.h
	$(CC) $(CFLAGS) $<

clean:
	cp -v base01.dat base01.dat.back ; \
	rm -rf doc/ *.o $(TP) base01.dat *.av *.sk *.sl pkfile.pk ;
