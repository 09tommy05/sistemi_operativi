CC=gcc
CFLAGS=-Wall -Wextra
SRC := $(wildcard *.c)
DEBUG=-DDEBUG
OBJ = main.o stats.o
DEBUG_OBJ = main_debug.o stats_debug.o

%.o: %.c stats.h
	$(CC) $(CFLAGS) -c $< -o $@

%_debug.o: %.c stats.h
	$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

all: analyzer

analyzer: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

debug: $(DEBUG_OBJ)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o analyzer
clean:
	rm -f $(OBJ) $(DEBUG_OBJ) analyzer