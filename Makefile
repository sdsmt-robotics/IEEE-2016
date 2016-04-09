#find any .c file in src/
SRC = $(wildcard src/*.c)
#make an object file for every .c file
OBJ = $(SRC:.c=.o)

CC = gcc
LINK = g++

CFLAGS =  -O -Wall
CXXFLAGS = $(CFLAGS)

TARGET = c_main_exec

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LINK) -o $@ $^ $(CFLAGS)
#	rm -rf src/*.o

.c:
	$(CC) -o $@ $@.c

clean:
	rm -rf $(TARGET) src/*.o
