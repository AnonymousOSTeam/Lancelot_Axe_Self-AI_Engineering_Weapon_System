CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = bin/system_core

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf src/*.o bin/
