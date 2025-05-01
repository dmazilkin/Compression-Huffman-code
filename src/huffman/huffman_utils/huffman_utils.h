#ifndef HUFFMAN_UTILS_H
#define HUFFMAN_UTILS_H

#include "file_utils.h"
#include "canonical_huffman.h"

typedef struct {
    char chr;
    int freq;
} char_freq_t;

typedef struct {
    char_freq_t* frequencies;
    int non_zero_count;
    int size;
    int total_freq;
} freq_table_t;

typedef struct {
    char chr;
    int freq;
    int code;
    int code_len;
    void* left;
    void* right;
} huff_tree_node_t;

typedef struct {
    huff_tree_node_t* nodes;
    int size;
    int leaf_cout;
} huff_tree_t;

typedef struct {
    char chr;
    int code;
    int code_len;
    void* left;
    void* right;
} huff_code_t;

typedef struct {
    huff_code_t* codes;
    int size;
} huff_table_t;

freq_table_t create_freq_table(char_freq_t* frequencies);

void update_freq_table(read_content_t* read_content, freq_table_t* freq_table);

#endif //HUFFMAN_UTILS_H
