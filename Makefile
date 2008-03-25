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

data.o: data.c
	$(CC) $(CFLAGS) data.c

io.o: io.c
	$(CC) $(CFLAGS) io.c

main.o: main.c 
	$(CC) $(CFLAGS) main.c

menu.o: menu.c
	$(CC) $(CFLAGS) menu.c

report: report.tex
	pdflatex report.tex

clean:
	cp -v base01.dat base01.dat.back ; \
	rm -rf *.o art base01.dat ; \
	rm -rf *.aux *.log
