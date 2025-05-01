#include <stdlib.h>

#include "base_huffman.h"

/**************************** DEFINES ****************************/
#define ASCII_TABLE_SIZE 128
#define UNDEFINED_CODE -1
#define COMBINED_CHARACTER 0
#define RIGHT_CODE 0
#define LEFT_CODE 1

/**************************** STATIC VARIABLES ****************************/
static int look_up_table[ASCII_TABLE_SIZE] = { 0 };

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static int compare_huff_code(const void* code1, const void* code2);

static int compare_freq_table(const void* elem1, const void* elem2);

static int compare_tree_nodes(const void* node1, const void* node2);

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
freq_table_t get_freq_table(char* content)
{
    int ind = 0;
    int unique_count = 0;
    char chr = content[ind];

    while (chr != '\0') {
        if (look_up_table[(int)chr] == 0) {
            unique_count++;
        }
        look_up_table[(int)chr] += 1;
        ind++;
        chr = content[ind];
    }

    freq_table_t freq_table = { .frequencies = NULL, .size = unique_count, .total_freq = 0 };
    freq_table.frequencies = (char_freq_t*)calloc(unique_count, sizeof(char_freq_t));
    ind = 0;

    for (int i = 0; i < ASCII_TABLE_SIZE; i++) {
        if (look_up_table[i] != 0) {
            freq_table.frequencies[ind].chr = i;
            freq_table.frequencies[ind].freq = look_up_table[i];
            freq_table.total_freq += freq_table.frequencies[ind].freq;
            ind++;
        }
    }

    qsort(freq_table.frequencies, freq_table.size, sizeof(char_freq_t), compare_freq_table);

    return freq_table;
}

huff_table_t get_base_huff(huff_tree_t* tree, int leaf_count)
{
    huff_code_t* huffman_code = (huff_code_t*)calloc(leaf_count, sizeof(huff_code_t));
    huff_table_t huffman_table = { .codes = huffman_code, .size = leaf_count };
    int ind = 0;

    for (int i = 0; i < tree->size; i++) {
        if (tree->nodes[i].left == NULL && tree->nodes[i].right == NULL) {
            huffman_code[ind].chr = tree->nodes[i].chr;
            huffman_code[ind].code = tree->nodes[i].code;
            huffman_code[ind].code_len = tree->nodes[i].code_len;
            ind++;
        }
    }

    qsort(huffman_table.codes, huffman_table.size, sizeof(huff_code_t), compare_huff_code);

    return huffman_table;
}

/**************************** STATIC FUNCTIONS ****************************/
static int compare_freq_table(const void* elem1, const void* elem2)
{
    char_freq_t* chr1 = (char_freq_t*)elem1;
    char_freq_t* chr2 = (char_freq_t*)elem2;

    if (chr1->freq < chr2->freq) {
        return -1;
    } else if (chr1->freq > chr2->freq) {
        return 1;
    } else {
        if ((int)chr1->chr < (int)chr2->chr) {
            return -1;
        } else {
            return 1;
        }
    }
}

static int compare_tree_nodes(const void* node1, const void* node2)
{
    huff_tree_node_t* tree_node1 = (huff_tree_node_t*)node1;
    huff_tree_node_t* tree_node2 = (huff_tree_node_t*)node2;

    if (tree_node1->freq < tree_node2->freq) {
        return -1;
    } else {
        return 1;
    }
}

static int compare_huff_code(const void* code1, const void* code2)
{
    huff_code_t* huff_code1 = (huff_code_t*)code1;
    huff_code_t* huff_code2 = (huff_code_t*)code2;

    if (huff_code1->code_len < huff_code2->code_len) {
        return -1;
    } else if (huff_code1->code_len > huff_code2->code_len) {
        return 1;
    } else {
        if (huff_code1->chr < huff_code2->chr) {
            return -1;
        } else {
            return 1;
        }
    }
}