CC = gcc
CFLAGS = -Wall -Wextra -std=c23 -O2

SRC = *.c src/*.c src/arg_parser/*.c src/io/*.c
INC = -Isrc/ -Isrc/arg_parser/ -Isrc/io/

TARGET = build/main

all: $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $(INC) -o $(TARGET) $^

clean:
	rm -rf build