#find any .c file in src/
SRC = $(wildcard src/*.c)
#make an object file for every .c file
OBJ = $(SRC:.c=.o)

CC = gcc
#have to use g++ to link
LINK = g++

#VPATH = src

CFLAGS = -Wall -O -g
CXXFLAGS = $(CFLAGS)

TARGET = test_locomote

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LINK) -o $@ $^
#	rm -rf src/*.o

.c:
	$(CC) -o $@ $@.c

clean:
	rm -rf $(TARGET) src/*.o
