#include <stdlib.h>

#include "huffman_utils.h"
#include "canonical_huffman.h"
#include "huffman.h"

/**************************** DEFINES ****************************/
#define ASCII_TABLE_SIZE 128
#define LEFT_CODE 1
#define COMBINED_CHARACTER 0
#define RIGHT_CODE 0
#define UNDEFINED_CODE -1

/**************************** STATIC VARIABLES ****************************/
static int look_up_table[ASCII_TABLE_SIZE] = { 0 };

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static int compare_huff_code(const void* code1, const void* code2);

static int compare_freq_table(const void* elem1, const void* elem2);

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
freq_table_t create_freq_table(char_freq_t* frequencies)
{
    freq_table_t freq_table = { .frequencies = frequencies, .size = ASCII_SIZE, .non_zero_count = 0, .total_freq = 0 };

    for (int i = 0; i < freq_table.size; i++) {
        freq_table.frequencies[i].chr = (char)i;
        freq_table.frequencies[i].freq = 0;
    }

    return freq_table;
}

void update_freq_table(read_content_t* read_content, freq_table_t* freq_table)
{
    for (int i = 0; i < read_content->content_size; i++) {

        if (freq_table->frequencies[(int)read_content->content[i]].freq == 0) {
            freq_table->non_zero_count++;
        }

        freq_table->frequencies[(int)read_content->content[i]].freq += 1;
        freq_table->total_freq += 1;
    }

    return;
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