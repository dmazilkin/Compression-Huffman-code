CC = gcc
CFLAGS = -Wall -Wextra -std=c23 -O2

SRC = \
	src/*.c \
	src/app/*.c \
	src/arg_parser/*.c \
	src/huffman/*c \
	src/huffman/huffman_utils/*.c \
	src/huffman/canonical_huffman/*.c \
	src/huffman/file_utils/*.c
INC = \
	-Isrc/app \
	-Isrc/arg_parser \
	-Isrc/huffman \
	-Isrc/huffman/huffman_utils \
	-Isrc/huffman/canonical_huffman \
	-Isrc/huffman/file_utils

TARGET = build/huffman

all: $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $(INC) -o $(TARGET) $^

clean:
	rm -rf build