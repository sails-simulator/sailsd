CC=gcc
CFLAGS=-I. -Wall -Werror $(shell pkg-config --cflags --libs gtk+-3.0 librsvg-2.0)
DEPS = sail_view.h sail_viewstate.h sail_boat.h
OBJ = sail_application.o sail_view.o sail_viewstate.o sail_boat.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sail: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm sail *o
