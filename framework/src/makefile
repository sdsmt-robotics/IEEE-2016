IDIR =../includes
CC=gcc
CFLAGS=-I $(IDIR) -Wall -g

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = *.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = *.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

SOURCES = *.c

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS) $(SOURCES)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
