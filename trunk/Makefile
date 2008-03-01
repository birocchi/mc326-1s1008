CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-Wall -ansi

all: art

art: io.o main.o 
	$(CC) $(LDFLAGS) io.o main.o -o art 

main.o: main.c 
	$(CC) $(CFLAGS) main.c

io.o: io.c
	$(CC) $(CFLAGS) io.c

clean:
	rm -rf *.o art