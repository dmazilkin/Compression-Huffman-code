#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "canonical_huffman.h"
#include "file_utils.h"

typedef struct {
    char chr;
    int freq;
    int code;
    int code_len;
    void* left;
    void* right;
} min_heap_node_t;

typedef struct {
    min_heap_node_t* nodes;
    int size;
} min_heap_t;

decoded_content_t huffman_decode(read_content_t content, decode_metadata_t* metadata, char* encoded_data, int undecoded_code, int undecoded_code_len);

encoded_content_t huffman_encode(read_content_t content, char* encoded_data, canonical_huff_table_t* huff_table, int* unencoded_code, int* unencoded_code_len);

void update_freq_table(read_content_t* read_content, freq_table_t* freq_table);

freq_table_t create_freq_table(char_freq_t* frequencies);

void calculate_huff_codes(huff_code_t* codes, freq_table_t* freq_table, int huff_tree_size);

#endif //HUFFMAN_H
