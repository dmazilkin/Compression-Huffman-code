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
    int is_leaf;
    int freq;
    int code;
} huffman_tree_node_t;

typedef struct {
    huffman_tree_node_t* nodes;
    int size;
} huffman_tree_t;

void huffman_encode(char* content);

void huffman_decode(char* content);

#endif //HUFFMAN_H
