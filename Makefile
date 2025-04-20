CC = gcc
CFLAGS = -Wall -Wextra -std=c23 -O2

SRC = *.c src/*.c
INC = src/

TARGET = build/main

all: $(SRC)
	mkdir build
	$(CC) $(CFLAGS) -I$(INC) -o $(TARGET) $^

clean:
	rm -rf build