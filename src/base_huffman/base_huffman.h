#ifndef BASE_HUFFMAN_H
#define BASE_HUFFMAN_H

typedef struct {
    char chr;
    int freq;
} char_freq_t;

typedef struct {
    char_freq_t* frequencies;
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
} huff_code_t;

typedef struct {
    huff_code_t* codes;
    int size;
} huff_table_t;

freq_table_t get_freq_table(char* content);

huff_tree_t get_huff_tree(freq_table_t* freq_table);

huff_table_t get_base_huff(huff_tree_t* tree, int leaf_count);

#endif //BASE_HUFFMAN_H
