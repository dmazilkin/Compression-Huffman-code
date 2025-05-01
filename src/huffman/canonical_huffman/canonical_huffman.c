#include <stdlib.h>

#include "canonical_huffman.h"
#include "huffman_utils.h"

/**************************** DEFINES ****************************/
#define START_CODE 0
#define START_CODE_LEN 1

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static int compare_huff_codes(const void* p1, const void* p2);

/**************************** INTERFACE FUNCTIONS ****************************/
canonical_huff_table_t get_canonical_huff(huff_code_t* huff_codes, canonical_huff_code_t* canonical_huff_code, int size)
{
    canonical_huff_table_t canonical_huff = {
      .codes = canonical_huff_code,
      .size = size
    };

    qsort(huff_codes, size, sizeof(huff_code_t), compare_huff_codes);

    int is_initialized = 0;
    int prev_canonical_ind = 0;
    int huff_code_ind = 0;

    while(huff_codes[huff_code_ind].code_len == 0) {
      huff_code_ind++;
    }

    while (huff_code_ind < size) {
        char ind = huff_codes[huff_code_ind].chr;

        if (is_initialized == 0) {
            canonical_huff.codes[(int)ind].chr = huff_codes[huff_code_ind].chr;
            canonical_huff.codes[(int)ind].code = START_CODE;
            canonical_huff.codes[(int)ind].code_len = huff_codes[huff_code_ind].code_len;
            is_initialized = 1;
        }
        else {
            canonical_huff.codes[(int)ind].chr = huff_codes[huff_code_ind].chr;
            canonical_huff.codes[(int)ind].code = canonical_huff.codes[prev_canonical_ind].code + 1;
            int code_len_diff = huff_codes[huff_code_ind].code_len - huff_codes[huff_code_ind-1].code_len;

            if (code_len_diff) {
                canonical_huff.codes[(int)ind].code <<= code_len_diff;
                canonical_huff.codes[(int)ind].code_len = canonical_huff.codes[prev_canonical_ind].code_len + code_len_diff;
            } else {
                canonical_huff.codes[(int)ind].code_len = canonical_huff.codes[prev_canonical_ind].code_len;
            }
        }

        prev_canonical_ind = (int)ind;
        huff_code_ind++;
    }

    return canonical_huff;
}

/**************************** STATIC FUNCTIONS ****************************/
static int compare_huff_codes(const void* p1, const void* p2)
{
    huff_code_t* code1 = (huff_code_t*)p1;
    huff_code_t* code2 = (huff_code_t*)p2;

    if (code1->code_len > code2->code_len) {
        return 1;
    }
    else if (code1->code_len < code2->code_len) {
        return -1;
    }
    else {
        if (code1->chr > code2->chr) {
            return 1;
        }
        else {
            return -1;
        }
    }
}