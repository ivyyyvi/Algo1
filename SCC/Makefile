
DEBUG = y
ifeq ($(DEBUG), y)
  #enale either of the following 3 to perform different test
	CFLAGS = -g -DSMALLSETSTEST
	CFLAGS = -g -DSINGLESETTEST
	CFLAGS = -g -DPIVOTTEST
else
 	CFLAGS = -g 
endif

INPUT = a
ifeq ($(INPUT), a)
	CFLAGS += -g -DDEFAULT_INPUT_FILENAME_A
endif

CC = gcc
DEPS = SCC.h
OBJ = SCC.o ProcessInput.o
CFLAGS += -I.


#%.o: %.c $(DEPS)
	#$(CC) -c -o $@ $< $(CFLAGS)

SCC_simple:
	gcc -o SCC SCC.c ProcessInput.c # build everything evertime

SCC: ProcessInput.o SCC.o 
	gcc -o SCC SCC.o ProcessInput.o $(CFLAGS)

SCC_advanced: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
clean:
	rm SCC *.o

