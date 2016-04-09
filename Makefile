#find any .c file in src/
SRC = $(wildcard src/*.cpp)
#make an object file for every .c file
OBJ = $(SRC:.cpp=.o)

CC = g++
LINK = g++

CFLAGS =  -O -g -lopencv_core -lopencv_highgui -lopencv_imgproc -std=c++11 #-Wall
CXXFLAGS = $(CFLAGS)

TARGET = cam_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LINK) -o $@ $^ $(CFLAGS)
#	rm -rf src/*.o

.cpp:
	$(CC) -o $@ $@.cpp

clean:
	rm -rf $(TARGET) src/*.o
