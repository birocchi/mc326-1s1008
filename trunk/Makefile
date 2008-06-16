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
LDFLAGS=-ljpeg -lungif -lpng
OFILES=adapter.o avail.o base.o descriptor.o file.o io.o hash.o html.o main.o mem.o memindex.o menu.o secindex.o libimg.o

TP=tp4
BASE=base01.dat

.PHONY: all clean

all: art

art: $(OFILES)
	$(CC) $(LDFLAGS) $(OFILES) -o $(TP)

adapter.o: adapter.c adapter.h
	$(CC) $(CFLAGS) $<

avail.o: avail.c avail.h
	$(CC) $(CFLAGS) $<

base.o: base.c base.h
	$(CC) $(CFLAGS) $<

descriptor.o: descriptor.c descriptor.h
	$(CC) $(CFLAGS) $<

file.o: file.c file.h
	$(CC) $(CFLAGS) $<

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) $<

html.o: html.c html.h
	$(CC) $(CFLAGS) $<

io.o: io.c io.h
	$(CC) $(CFLAGS) $<

libimg.o: libimg/libimg.c libimg/libimg.h
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
	rm -rf doc/ *.o $(TP) *.av descriptor.desc* *.sk.h* *.sl pkfile.pk.h*
