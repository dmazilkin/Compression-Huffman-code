#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_utils.h"
#include "canonical_huffman.h"

/**************************** DEFINES ****************************/
#define BYTE_SIZE 8
#define METADATA ".metadata"

/**************************** STATIC VARIABLES ****************************/
static int prev_chr = 0;

/**************************** STATIC FUNCTION DECLARATIONS ****************************/

/**************************** INTERFACE FUNCTIONS ****************************/
file_status_t read_chunk_to_encode(char* file_name, read_content_t* read_content, int chunk_size)
{
    if (read_content->file == NULL) {
        read_content->file = fopen(file_name, "rb");
    }

    int chr = prev_chr;

    if (chr == 0) {
        chr = fgetc(read_content->file);
    }

    int ind = 0;

    while ((chr != EOF) && (ind < chunk_size)) {
        read_content->content[ind] = chr;
        ind++;

        chr = fgetc(read_content->file);
        if (chr != EOF) {
            prev_chr = chr;
        }
    }

    if (chr == EOF) {
        read_content->is_eof = 1;
        prev_chr = 0;
        fclose(read_content->file);
    }

    read_content->content_size = ind;

  return FILE_READ_SUCCESS;
}

file_status_t read_chunk_to_decode(char* file_name, read_content_t* read_content, int chunk_size)
{
    if (read_content->file == NULL) {
        read_content->file = fopen(file_name, "rb");
    }

    int chr = prev_chr;

    if (chr == 0) {
        chr = fgetc(read_content->file);
    }

    int ind = 0;

    while ((chr != EOF) && (ind < chunk_size)) {
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

file_status_t save_encoded(char* file_name, encoded_content_t* content)
{
    FILE* file = fopen(file_name, "ab");

    if (file == NULL) {
      return FILE_WRITE_ERROR;
    }

    fwrite(content->content, sizeof(content->content[0]), content->content_size, file);

    fclose(file);

    return FILE_WRITE_SUCCESS;
}

file_status_t save_metadata(canonical_huff_table_t* huff)
{
    FILE* file = fopen(".metadata", "wb");

    if (file == NULL) {
      return FILE_WRITE_ERROR;
    }

    encode_metadata_t metadata[huff->size];

    fprintf(file, "shift[%d]", huff->shift);

    for (int i = 0; i < huff->size; i++) {
        if (huff->codes[i].code_len != 0) {
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
    }

    fclose(file);

    return FILE_WRITE_SUCCESS;
}

file_status_t read_metadata(decode_metadata_t* metadata)
{
    FILE* file = fopen(METADATA, "rb");

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
    FILE* file = fopen(file_name, "ab");

    if (file == NULL) {
      return FILE_WRITE_ERROR;
    }

    fwrite(decoded_content->content, sizeof(char), decoded_content->content_size, file);
    fclose(file);

    return FILE_WRITE_SUCCESS;
}

/**************************** STATIC FUNCTIONS ****************************/