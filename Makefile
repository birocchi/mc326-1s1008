CC = gcc

CCFLAGS = -Wall -pedantic -ansi -O0
LDFLAGS =

SOURCES = io.c parser.c main.c
FINALNAME = foo

all:
	$(CC) $(CCFLAGS) $(SOURCES) -o $(FINALNAME)
