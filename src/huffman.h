#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "canonical_huffman.h"
#include "file_utils.h"

canonical_huff_table_t huffman_encode(char* content);

void huffman_decode(reformatting_data_t* metadata);

#endif //HUFFMAN_H
