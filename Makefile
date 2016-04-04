#find any .c file in src/
SRC = $(wildcard src/*.cpp)
#make an object file for every .c file
OBJ = $(SRC:.cpp=.o)

CC = g++
LINK = g++

CFLAGS = -Wall -O -g -lopencv_core -lopencv_highgui -lopencv_flann -lopencv_imgproc
CXXFLAGS = $(CFLAGS)

TARGET = test_locomote

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LINK) -o $@ $^
#	rm -rf src/*.o

.cpp:
	$(CC) -o $@ $@.cpp

clean:
	rm -rf $(TARGET) src/*.o
