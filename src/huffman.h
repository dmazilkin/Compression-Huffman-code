#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "canonical_huffman.h"

canonical_huff_table_t huffman_encode(char* content);

void huffman_decode(char* content);

#endif //HUFFMAN_H
