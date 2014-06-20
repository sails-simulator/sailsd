CC=gcc
CFLAGS=-I. -Wall -Werror $(shell pkg-config --cflags --libs gtk+-3.0)
DEPS = sim_view.h sim_viewstate.h
OBJ = sim_application.o sim_view.o sim_viewstate.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sail: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
