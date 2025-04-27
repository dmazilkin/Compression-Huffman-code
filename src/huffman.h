#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "canonical_huffman.h"
#include "file_utils.h"

canonical_huff_table_t huffman_encode(char* content);

void huffman_decode(char* content, decode_metadata_t* metadata, char** decoded_content);

#endif //HUFFMAN_H
