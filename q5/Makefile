CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGETS = list countw copy main

all: $(TARGETS)

list: list.c
	$(CC) $(CFLAGS) -o $@ $<

countw: countw.c
	$(CC) $(CFLAGS) -o $@ $<

copy: copy.c
	$(CC) $(CFLAGS) -o $@ $<

main: main.c list countw copy move
	$(CC) $(CFLAGS) -o $@ $<

move: move.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
.PHONY: all clean