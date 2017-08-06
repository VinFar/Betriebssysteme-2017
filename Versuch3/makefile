GCCFLAGS = -Wall -Wconversion -Wextra -g -m64 -std=c99 -pedantic

all:
	gcc -o Versuch2 $(GCCFLAGS) Versuch1.c OSMPLib.c OSMPLib.h -lrt
	gcc -o echoall $(GCCFLAGS) echoall.c OSMPLib.c OSMPLib.h -lrt 
	./Versuch2 9

