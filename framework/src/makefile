CC=g++
INC_DIR = ../includes
CFLAGS=-c -Wall -I$(INC_DIR)
DEPS = *.h

all: *.o

%.o: %.cpp $(DEPS)
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm -rf *o all
