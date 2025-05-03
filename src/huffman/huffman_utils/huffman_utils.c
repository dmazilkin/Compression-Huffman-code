#include <stdlib.h>

#include "huffman_utils.h"

/**************************** DEFINES ****************************/

/**************************** STATIC VARIABLES ****************************/

/**************************** STATIC FUNCTION DECLARATIONS ****************************/

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
freq_table_t create_freq_table(char_freq_t* frequencies, int size)
{
    freq_table_t freq_table = { .frequencies = frequencies, .size = size, .non_zero_count = 0, .total_freq = 0 };

    for (int i = 0; i < freq_table.size; i++) {
        freq_table.frequencies[i].chr = (char)i;
        freq_table.frequencies[i].freq = 0;
    }

    return freq_table;
}

/**************************** STATIC FUNCTIONS ****************************/