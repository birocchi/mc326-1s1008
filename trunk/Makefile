#
# MC326 1s2008
#
# Group 1
# Anderson Phillip Birocchi
# Miguel Francisco Alves de Mattos Gaiowski
# Raphael Kubo da Costa
#

CC=gcc
CFLAGS=-c -Wall -g -ansi
LDFLAGS=

all: art

art: io.o main.o menu.o
	$(CC) $(LDFLAGS) io.o main.o menu.o -o art 

main.o: main.c 
	$(CC) $(CFLAGS) main.c

menu.o: menu.c
	$(CC) $(CFLAGS) menu.c

io.o: io.c
	$(CC) $(CFLAGS) io.c

clean:
	rm -rf *.o art base01.dat
