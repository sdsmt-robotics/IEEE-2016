CC = gcc
CFLAGS = -Wall -g -I../includes/$(wildcard *.h)

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))


all: main

main : $(objects)
        $(CC) $(CFLAGS)-o main $(OBJS)
