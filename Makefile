#
# MC326 1s2008
#
# Group 1
# Anderson Phillip Birocchi
# Miguel Francisco Alves de Mattos Gaiowski
# Raphael Kubo da Costa
#

CC=gcc
CFLAGS=-c -Wall -g -ansi -pedantic -DDEBUG
LDFLAGS=-lefence
OFILES=bptree.o file.o io.o main.o mem.o menu.o

TP=tp5

all: art

art: $(OFILES)
	$(CC) $(LDFLAGS) $(OFILES) -o $(TP)

bptree.o: bptree.c bptree.h
	$(CC) $(CFLAGS) $<

file.o: file.c file.h
	$(CC) $(CFLAGS) $<

io.o: io.c io.h
	$(CC) $(CFLAGS) $<

main.o: main.c menu.h io.h
	$(CC) $(CFLAGS) $<

mem.o: mem.c mem.h
	$(CC) $(CFLAGS) $<

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) $<

clean:
	rm -rf *.o $(TP) 0 *.bpx;
