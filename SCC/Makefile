
DEBUG = y
ifeq ($(DEBUG), y)
	CFLAGS = -g
	CFLAGS += -D_DEBUG
	CFLAGS += -D_DEBUG_MAIN
	CFLAGS += -D_DEBUG_DFS
	#CFLAGS += -D_DEBUG_PROCESS_INPUT
else
 	CFLAGS = -g
endif

INPUT = H
ifeq ($(INPUT), Z)
	CFLAGS += -DDEFAULT_INPUT_FILENAME_Z
else ifeq ($(INPUT), A)
	CFLAGS += -DDEFAULT_INPUT_FILENAME_A
else ifeq ($(INPUT), B)
	CFLAGS += -DDEFAULT_INPUT_FILENAME_B
else ifeq ($(INPUT), C)
	CFLAGS += -DDEFAULT_INPUT_FILENAME_C
else ifeq ($(INPUT), D)
	CFLAGS += -DDEFAULT_INPUT_FILENAME_D
else ifeq ($(INPUT), E)
	CFLAGS += -DDEFAULT_INPUT_FILENAME_E
else ifeq ($(INPUT), F)
	CFLAGS += -DDEFAULT_INPUT_FILENAME_F
else ifeq ($(INPUT), G)
	CFLAGS += -DDEFAULT_INPUT_FILENAME_G
else ifeq ($(INPUT), H)
	CFLAGS += -DDEFAULT_INPUT_FILENAME_H
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

