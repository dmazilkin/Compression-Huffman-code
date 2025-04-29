CC = gcc
CFLAGS = -Wall -Wextra -std=c23 -O2

SRC = *.c src/*.c src/app/*.c src/io_utils/*.c src/canonical_huffman/*.c src/base_huffman/*.c
INC = -Isrc/ -Isrc/app -Isrc/io_utils -Isrc/canonical_huffman/ -Isrc/base_huffman/

TARGET = build/main

all: $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $(INC) -o $(TARGET) $^

clean:
	rm -rf build