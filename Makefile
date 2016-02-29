CC=gcc
CFLAGS=-I. -Ilibsailing -std=gnu99 $(shell pkg-config --libs --cflags jansson) -lpthread
DEPS =
OBJ = sailsd.o
LIBSAILING_OBJ = libsailing/sailing.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sailsd: $(OBJ) $(LIBSAILING_OBJ)
	gcc -o $@ $^ $(CFLAGS)
