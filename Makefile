
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

CC = gcc
LINK = g++

VPATH = src

CFLAGS = -Wall -O -g
CXXFLAGS = $(CFLAGS)

TARGET = test_locomote

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LINK) -o $@ $^
	rm -rf src/*.o

.c:
	$(CC) -o $@ $@.c

clean:
	rm -rf $(TARGET) src/*.o
