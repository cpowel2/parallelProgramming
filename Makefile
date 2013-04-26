#Nathan Broemm & Chris Powell
#CMSC 483
#Project findmin Makefile

all: findmin

findmin: findmin.c
	gcc -pthread  -lm findmin.c -o findmin -lm
clean:
	rm -f *.o
	rm -f findmin
