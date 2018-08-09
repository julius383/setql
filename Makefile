CC=gcc
CFLAGS=-g -Wall -Wextra -std=c99

set:set.c
	$(CC) $(CFLAGS) -o $@ $^
