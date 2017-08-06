GCCFLAGS = -Wall -Wconversion -Wextra -g -m64 -std=c99 -pedantic -D_XOPEN_SOURCE=500

all:
	sudo gcc -o OSMP_Run $(GCCFLAGS) Versuch1.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o echoall $(GCCFLAGS) echoall.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test00 $(GCCFLAGS) test_00.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test01 $(GCCFLAGS) test_01.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test02 $(GCCFLAGS) test_02.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test03 $(GCCFLAGS) test_03.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test04 $(GCCFLAGS) test_04.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test05 $(GCCFLAGS) test_05.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test06 $(GCCFLAGS) test_06.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test07 $(GCCFLAGS) test_07.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test08 $(GCCFLAGS) test_08.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test09 $(GCCFLAGS) test_09.c OSMPLib.c OSMPLib.h -lrt -pthread
	sudo gcc -o test10 $(GCCFLAGS) test_10.c OSMPLib.c OSMPLib.h -lrt -pthread

	
	sudo ./OSMP_Run 43 ./echoall [3z9] ./echoall 
