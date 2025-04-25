#include <stdlib.h>

#include "canonical_huffman.h"
#include "base_huffman.h"

/**************************** DEFINES ****************************/
#define START_CODE 0
#define START_CODE_LEN 1

/**************************** INTERFACE FUNCTIONS ****************************/
canonical_huff_table_t get_canonical_huff(huff_table_t* huff_table)
{
    int size = huff_table->size;
    canonical_huff_code_t* codes = (canonical_huff_code_t*)calloc(size, sizeof(canonical_huff_code_t));
    canonical_huff_table_t canonical_huff = { .codes = codes, .size = size };

    canonical_huff.codes[0].chr = huff_table->codes[0].chr;
    canonical_huff.codes[0].code = START_CODE;
    canonical_huff.codes[0].code_len = huff_table->codes[0].code_len;

    for (int i = 1; i < size; i++) {
        canonical_huff.codes[i].chr = huff_table->codes[i].chr;
        canonical_huff.codes[i].code = canonical_huff.codes[i-1].code + 1;
        int code_len_diff = huff_table->codes[i].code_len - huff_table->codes[i-1].code_len;

        if (code_len_diff) {
            canonical_huff.codes[i].code <<= code_len_diff;
            canonical_huff.codes[i].code_len = canonical_huff.codes[i-1].code_len + 1;
        } else {
            canonical_huff.codes[i].code_len = canonical_huff.codes[i-1].code_len;
        }
    }

    return canonical_huff;
}

/**************************** STATIC FUNCTIONS ****************************/