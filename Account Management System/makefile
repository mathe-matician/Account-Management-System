CC=gcc
OBJ= main.o
CFLAGS=-I. -g
DEPS = funcdef.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ams: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
