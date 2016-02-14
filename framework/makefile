CC = gcc
INC_DIR = includes
SRC_DIR = src
OBJ_DIR = obj
CFLAGS  = -c -Wall -I.

SRCS = $(SRC_DIR)/$(wildcard *.c)
OBJS = $(patsubst %.c, %.o, ${SRCFILES})

DEPS = $(INC_DIR)/$(wildcard *.h)

all: $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
  $(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/$(wildcard *.o): $(DEPS)
