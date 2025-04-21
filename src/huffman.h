#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct {
    char ch;
    int freq;
} char_freq_t;

typedef struct {
    char_freq_t* frequencies;
    int size;
} freq_table_t;

void huffman_encode(char* content);

void huffman_decode(char* content);

#endif //HUFFMAN_H
