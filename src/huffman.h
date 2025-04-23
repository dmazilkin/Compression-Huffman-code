#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct {
    char ch;
    int freq;
} char_freq_t;

typedef struct {
    char_freq_t* frequencies;
    int size;
    int total_freq;
} freq_table_t;

typedef struct {
    char ch;
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
    char ch;
    int code;
    int code_len;
} huff_code_t;

typedef struct {
  huff_code_t* codes;
  int size;
} huff_table_t;

void huffman_encode(char* content);

void huffman_decode(char* content);

#endif //HUFFMAN_H
