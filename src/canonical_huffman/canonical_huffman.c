#include <stdlib.h>

#include "canonical_huffman.h"
#include "base_huffman.h"

/**************************** INTERFACE FUNCTIONS ****************************/
canonical_huff_table_t get_canonical_huff(huff_table_t* huff_table)
{
    canonical_huff_table_t canonical_huff = { .codes = NULL , .size = 0 };
    return canonical_huff;
}
