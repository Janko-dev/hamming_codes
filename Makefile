CC = gcc
CFLAGS = -Wall -Wextra
IN = main.c la.c
OUT = out

default:
	$(CC) $(IN) -o $(OUT) $(CFLAGS)
	./$(OUT)