CC=gcc
CFLAGS=-O9

all: cosine rmhub

cosine : cosine.c
	$(CC) $(CFLAGS) cosine.c -o cosine -lm -fopenmp

rmhub : rmhub.c
	$(CC) $(CFLAGS) rmhub.c -o rmhub

clean:
	rm cosine rmhub
