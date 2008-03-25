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

all: art

art: data.o io.o main.o menu.o
	$(CC) $(LDFLAGS) data.o io.o main.o menu.o -o art 

data.o: data.c data.h
	$(CC) $(CFLAGS) $<

io.o: io.c io.h
	$(CC) $(CFLAGS) $<

main.o: main.c data.h menu.h io.h
	$(CC) $(CFLAGS) $<

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) $<

report: report.tex
	pdflatex report.tex

clean:
	cp -v base01.dat base01.dat.back ; \
	rm -rf *.o art base01.dat ; \
	rm -rf *.aux *.log
