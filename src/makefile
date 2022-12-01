IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj

LIBS=-lm -lncurses

_DEPS = locknload.h scorestruct.h blockstruct.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = hellomake.o hellofunc.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.c: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mattris: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
