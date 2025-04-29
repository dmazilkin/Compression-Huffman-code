#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "canonical_huffman.h"
#include "file_utils.h"

typedef struct {
    char_freq_t* nodes;
    int size;
} min_heap_t;

decoded_content_t huffman_decode(read_content_t content, decode_metadata_t* metadata, char* encoded_data, int undecoded_code, int undecoded_code_len);

void update_freq_table(read_content_t* read_content, freq_table_t* freq_table);

freq_table_t create_freq_table(char_freq_t* frequencies);

min_heap_t get_huff_tree(freq_table_t* freq_table, char_freq_t* nodes, int huff_tree_size);

#endif //HUFFMAN_H
