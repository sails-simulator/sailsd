CC=gcc
CFLAGS=-I. -std=gnu99 $(shell pkg-config --libs --cflags jansson) -lpthread
DEPS =
OBJ = sailsd.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sailsd: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
