CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-Wall -ansi

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
