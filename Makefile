IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR) `pkg-config --cflags --libs glib-2.0`

ODIR=obj
LDIR =./lib

_DEPS = cmdline.h utils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o cmdline.o utils.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

target = main

$(target): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(target)
