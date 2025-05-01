#ifndef CANONICAL_HUFFMAN_H
#define CANONICAL_HUFFMAN_H

#include "base_huffman.h"

typedef struct {
    char chr;
    int code;
    int code_len;
} canonical_huff_code_t;

typedef struct {
    canonical_huff_code_t* codes;
    int size;
    int shift;
} canonical_huff_table_t;

canonical_huff_table_t get_canonical_huff(huff_code_t* huff_table, canonical_huff_code_t* canonical_huff_code, int size);

#endif //CANONICAL_HUFFMAN_H