#include <stdlib.h>
#include <stdio.h>

#include "file_utils.h"

/**************************** DEFINES ****************************/
#define DEFAULT_SIZE 100

/**************************** STATIC FUNCTION DECLARATIONS ****************************/

/**************************** INTERFACE FUNCTIONS ****************************/
file_status_t read_from_file(char* file_name, char** content) {
    *content = (char*)calloc(DEFAULT_SIZE, sizeof(char));
    int content_size = DEFAULT_SIZE;

    FILE* file = fopen(file_name, "r");

    if (file == NULL) {
        return FILE_READ_NOT_FOUND;
    }

    int symb = fgetc(file);
    int ind = 0;

    while (symb != EOF) {
        if (ind >= content_size - 1) {
            content_size *= 2;
            *content = (char*)realloc(*content, content_size * sizeof(char));
        }
        *(*content+ind) = (char)symb;
        symb = fgetc(file);
        ind++;
    }

    return FILE_READ_SUCCESS;
}

file_status_t write_to_file(canonical_huff_table_t* huff, char* content, char* file_name)
{
    int chr_ind = 0;
    char chr = content[chr_ind];
    char* encoded_text = (char*)calloc(DEFAULT_SIZE, sizeof(char));
    int encoded_ind = 0;
    int encoded_size = DEFAULT_SIZE;

    while (chr != '\0') {
        int code = -1;
        int code_len = -1;
        for (int i = 0; i < huff->size; i++) {
            if (huff->codes[i].chr == chr) {
                code = huff->codes[i].code;
                code_len = huff->codes[i].code_len;
                i = huff->size;
              }
        }

        if (code_len + encoded_ind + 1 > encoded_size) {
            encoded_text = (char*)realloc(encoded_text, 2 * encoded_size * sizeof(char));
        }

        for (int i = 0; i < code_len; i++) {
            int char_ind = i + encoded_ind;
            encoded_text[char_ind] = code & (1 << i) ? '1' : '0';
        }
        encoded_ind += code_len;

        chr_ind++;
        chr = content[chr_ind];
    }

    return FILE_WRITE_SUCCESS;
}

/**************************** STATIC FUNCTIONS ****************************/
