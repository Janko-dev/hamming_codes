CC = gcc
CFLAGS = -Wall -Wextra
IN = main.c la.c
OUT = hc

default:
	$(CC) $(IN) -o $(OUT) $(CFLAGS)