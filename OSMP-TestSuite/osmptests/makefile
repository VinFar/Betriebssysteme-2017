# -g for debugger; -pedantic strictly to the ANSI standard; -std=c99 no C++; -D_XOPEN_SOURCE load c++ sys header

CC      = /usr/bin/gcc
#CFLAGS = -Wall -Wconversion -Wextra -std=c99, -std=gnu99, -std=c11 -pthread -m64 # non ansi
#CFLAGS  = -Wall -Wconversion -Wextra -ansi -pedantic -g -m64 -D_XOPEN_SOURCE=500 -pthread # ansi
CFLAGS  = -Wall -Wconversion -Wextra -ansi -pedantic -g -D_XOPEN_SOURCE=500 -pthread # ansi
LDFLAGS = -lm  -lrt -lpthread

LIST =  test_00 test_01 test_02 test_03 test_04 test_05 test_06 test_07 test_08 test_09 test_10 
#test_11 test_12 test_13 test_14

# OBJ = test_00.o test01.o
# SRC = $(OBJ:%.o=%.c)


# test_00: $(OBJ00)
#	$(CC) $(CFLAGS) -o test_00 $(OBJ00) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<


all:
	for i in $(LIST); do \
            gcc $(CFLAGS) $$i.c ../osmprun/osmplib.c $(LDFLAGS) -o $$i; \
        done
	#gcc -o osmprun $(CFLAGS) ../osmprun/osmprun.c ../osmprun/osmplib.o $(LDFLAGS)

clean:
	for i in $(LIST); do \
            rm -rf $$i $$i.o; \
        done

# valgrind -l
