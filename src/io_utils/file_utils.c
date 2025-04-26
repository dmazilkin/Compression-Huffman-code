#include <stdlib.h>
#include <stdio.h>

#include "file_utils.h"

/**************************** DEFINES ****************************/
#define DEFAULT_SIZE 100
#define BYTE_SIZE 8

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
reformatting_data_t get_reformatting_data(canonical_huff_table_t* huff, char* content);

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
    reformatting_data_t reformatting_data = get_reformatting_data(huff, content);

    FILE* file = fopen(file_name, "w+");

    if (file == NULL) {
      return FILE_WRITE_ERROR;
    }


    for (int i = 0; i < reformatting_data.count / BYTE_SIZE; i++) {
        unsigned char byte_to_write = 0b0;

      for (int j = 0; j < BYTE_SIZE; j++) {
        unsigned char byte = ((reformatting_data.data[i * BYTE_SIZE + j] == '1')) << (BYTE_SIZE - j - 1);
        byte_to_write |= byte;
      }

      fwrite(&byte_to_write, sizeof(char), 1, file);
    }

    fclose(file);

    return FILE_WRITE_SUCCESS;
}

/**************************** STATIC FUNCTIONS ****************************/
reformatting_data_t get_reformatting_data(canonical_huff_table_t* huff, char* content)
{
    int chr_ind = 0;
    char chr = content[chr_ind];
    reformatting_data_t encoded_text = { .data=(char*)calloc(DEFAULT_SIZE, sizeof(char)), .count = 0 };
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

        if (code_len + encoded_text.count + 1 > encoded_size) {
            encoded_text.data = (char*)realloc(encoded_text.data, 2 * encoded_size * sizeof(char));
        }

        for (int i = code_len - 1; i >= 0; i--) {
            encoded_text.data[encoded_text.count + code_len - i - 1] = code & (1 << i) ? '1' : '0';
        }

        encoded_text.count += code_len;
        chr_ind++;
        chr = content[chr_ind];
    }

    return encoded_text;
}