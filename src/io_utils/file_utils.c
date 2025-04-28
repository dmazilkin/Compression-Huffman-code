#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_utils.h"

/**************************** DEFINES ****************************/
#define DEFAULT_SIZE 64
#define BYTE_SIZE 8
#define METADATA ".metadata"

/**************************** STATIC VARIABLES ****************************/
static int prev_chr = 0;

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static reformatting_data_t reformate_encoded_data(canonical_huff_table_t* huff, char* content);

static file_status_t read_to_encode(char* file_name, char** content);

static file_status_t read_to_decode(char* file_name, char** content);

/**************************** INTERFACE FUNCTIONS ****************************/
file_status_t read_from_file(char* file_name, char** content, operation_t option) {

    if (option == COMPRESSION) {
        return read_to_encode(file_name, content);
    } else {
        return read_to_decode(file_name, content);
    }
}

file_status_t read_content_to_decode(char* file_name, read_content_t* read_content)
{
    if (read_content->file == NULL) {
        read_content->file = fopen(file_name, "r");
    }

    int chr = prev_chr;

    if (chr == 0) {
        chr = fgetc(read_content->file);
    }

    int ind = 0;

    while ((chr != EOF) && (ind < DEFAULT_SIZE)) {
        for (int i = 0; i < BYTE_SIZE; i++) {
            read_content->content[ind*BYTE_SIZE + i] = (0b1 & (chr >> (BYTE_SIZE - i - 1))) ? '1' : '0';
        }
        ind++;

        chr = fgetc(read_content->file);
        if (chr != EOF) {
            prev_chr = chr;
        }
    }

    if (chr == EOF) {
      read_content->is_eof = 1;
      fclose(read_content->file);
    }

    read_content->content_size = BYTE_SIZE*ind;

    return FILE_READ_SUCCESS;
}

file_status_t save_encoded(canonical_huff_table_t* huff, char* content, char* file_name)
{
    reformatting_data_t reformatting_data = reformate_encoded_data(huff, content);

    FILE* file = fopen(file_name, "w");

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

    int rest_ind = reformatting_data.count / BYTE_SIZE;
    unsigned char byte_to_write = 0b0;

    if (reformatting_data.count % BYTE_SIZE) {
        for (int j = 0; j < reformatting_data.count % BYTE_SIZE; j++) {
            unsigned char byte = ((reformatting_data.data[rest_ind * BYTE_SIZE + j] == '1')) << (BYTE_SIZE - j - 1);
            byte_to_write |= byte;
        }

        fwrite(&byte_to_write, sizeof(char), 1, file);
    }

    if (reformatting_data.count % BYTE_SIZE > 0) {
        huff->shift = BYTE_SIZE - reformatting_data.count % BYTE_SIZE;
    }

    fclose(file);

    return FILE_WRITE_SUCCESS;
}

file_status_t save_metadata(canonical_huff_table_t* huff)
{
    FILE* file = fopen(".metadata", "w");

    if (file == NULL) {
      return FILE_WRITE_ERROR;
    }

    encode_metadata_t metadata[huff->size];

    fprintf(file, "shift[%d]", huff->shift);

    for (int i = 0; i < huff->size; i++) {
        metadata[i].chr = huff->codes[i].chr;
        char code[huff->codes[i].code_len];
        metadata[i].code = code;

        for (int j = 0; j < huff->codes[i].code_len; j++) {
            metadata[i].code[huff->codes[i].code_len - j - 1] = huff->codes[i].code & (1 << j) ? '1' : '0';
        }

        fwrite(&metadata[i].chr, sizeof(char), 1, file);
        fprintf(file, "[%d]", huff->codes[i].code_len);
        fwrite(metadata[i].code, sizeof(char), huff->codes[i].code_len, file);
    }

    fclose(file);

    return FILE_WRITE_SUCCESS;
}

file_status_t read_metadata(decode_metadata_t* metadata)
{
    FILE* file = fopen(METADATA, "r");

    if (file == NULL) {
      return FILE_READ_NOT_FOUND;
    }

    int shift = 0;
    fscanf(file, "shift[%d]", &shift);
    metadata->shift = shift;

    char chr = fgetc(file);
    int code_len = 0;
    fscanf(file, "[%d]", &code_len);

    while (chr != EOF) {
        metadata[(int)chr].code_len = code_len;

        for (int i = 0; i < code_len; i++) {
            metadata[(int)chr].code |= (fgetc(file) == '1' ? 1 : 0) << (code_len - i - 1);
        }

        chr = fgetc(file);
        code_len = 0;
        fscanf(file, "[%d]", &code_len);
    }

    return FILE_READ_SUCCESS;
}

file_status_t save_decoded(char* file_name, decoded_content_t* decoded_content)
{
    FILE* file = fopen(file_name, "a+");

    if (file == NULL) {
      return FILE_WRITE_ERROR;
    }

    fwrite(decoded_content->content, sizeof(char), decoded_content->content_size, file);
    fclose(file);

    return FILE_WRITE_SUCCESS;
}

/**************************** STATIC FUNCTIONS ****************************/
static file_status_t read_to_encode(char* file_name, char** content) {
    int content_size = DEFAULT_SIZE;
    *content = (char*)calloc(DEFAULT_SIZE, sizeof(char));

    FILE* file = fopen(file_name, "r");

    if (file == NULL) {
        return FILE_READ_NOT_FOUND;
    }

    int symb = fgetc(file);
    int ind = 0;

    while (symb != EOF) {
        if (ind >= content_size - 1) {
            content_size += DEFAULT_SIZE;
            *content = (char*)realloc(*content, content_size * sizeof(char));
        }
        *(*content+ind) = (char)symb;
        symb = fgetc(file);
        ind++;
    }

    return FILE_READ_SUCCESS;
}

static file_status_t read_to_decode(char* file_name, char** content)
{
    int content_size = DEFAULT_SIZE;
    *content = (char*)calloc(content_size, sizeof(char));

    FILE* file = fopen(file_name, "r");

    if (file == NULL) {
        return FILE_READ_NOT_FOUND;
    }

    int chr = fgetc(file);
    int ind = 0;

    while (chr != EOF) {
      for (int i = 0; i < BYTE_SIZE; i++) {
        (*content)[ind] = (0b1 & (chr >> (BYTE_SIZE - i - 1))) ? '1' : '0';
        ind++;

        if (ind >= content_size - 1) {
          content_size += DEFAULT_SIZE;
          *content = (char*)realloc(*content, content_size * sizeof(char));
        }
      }

      chr = fgetc(file);
    }

    return FILE_READ_SUCCESS;
}

static reformatting_data_t reformate_encoded_data(canonical_huff_table_t* huff, char* content)
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