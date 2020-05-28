CC=gcc
OBJ= accountfunc.o accountmanagementsystem.o
CFLAGS=-I.
DEPS = accountvars.h accountheaders.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ams: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
