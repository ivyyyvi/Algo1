
DEBUG = y
ifeq ($(DEBUG), y)
  #enale either of the following 3 to perform different test
	CFLAGS = -g -DSMALLSETSTEST
	CFLAGS = -g -DSINGLESETTEST
	CFLAGS = -g -DPIVOTTEST
else
 	CFLAGS = -g 
endif

CC = gcc
DEPS = SCC.h
OBJ = SCC.o ProcessInput.o
CFLAGS += -I.


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

SCC: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

