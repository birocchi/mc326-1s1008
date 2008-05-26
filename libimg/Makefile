
all: IMG
	gcc -Wall -g -o exemplo libimg.o -lefence -ljpeg  -lgif -lpng exemplo.c -lm

IMG:
	gcc -Wall -g -c libimg.c

clean:
	rm -f *.o
	rm *~
	rm exemplo
