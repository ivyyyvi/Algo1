
DEBUG = y
ifeq ($(DEBUG), y)
  #enale either of the following 3 to perform different test
	CFLAGS = -g -DSMALLSETSTEST
	CFLAGS = -g -DSINGLESETTEST
	CFLAGS = -g -DPIVOTTEST
else
 	CFLAGS = -g
endif

INPUT = c
ifeq ($(INPUT), a)
	CFLAGS += -g -DDEFAULT_INPUT_FILENAME_A

else ifeq ($(INPUT), b)
	CFLAGS += -g -DDEFAULT_INPUT_FILENAME_B

else ifeq ($(INPUT), c)
	CFLAGS += -g -DDEFAULT_INPUT_FILENAME_C
endif

CC = gcc
DEPS = SCC.h
OBJ = SCC.o ProcessInput.o DFS.o
CFLAGS += -I.


#%.o: %.c $(DEPS)
	#$(CC) -c -o $@ $< $(CFLAGS)

SCC: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

#SCC: ProcessInput.o SCC.o
	#gcc -o SCC SCC.o ProcessInput.o $(CFLAGS)

#SCC_simple:
	#gcc -o SCC SCC.c ProcessInput.c # build everything evertime

clean:
	rm SCC *.o

