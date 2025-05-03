#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_utils.h"

/**************************** DEFINES ****************************/
#define ASCII_UPPER_BOUND 127
#define BYTE_SIZE 8
#define METADATA ".metadata"

/**************************** STATIC VARIABLES ****************************/
static int prev_chr = 0;

/**************************** STATIC FUNCTION DECLARATIONS ****************************/

/**************************** INTERFACE FUNCTIONS ****************************/
read_status_t read_chunk_to_encode(char* file_name, read_content_t* read_content, int chunk_size)
{
    if (read_content->file == NULL) {
        read_content->file = fopen(file_name, "rb");

        if (read_content->file == NULL) {
          printf("ERROR! Could not open file %s!\n", file_name);
          return FILE_READ_ERROR;
        }
    }

    int chr = prev_chr;

    if (chr == 0) {
        chr = fgetc(read_content->file);
    }

    int ind = 0;

    while ((chr != EOF) && (ind < chunk_size)) {

        if (chr > ASCII_UPPER_BOUND) {
            printf("ERROR! Invalid ASCII character found: %c[%d]!\n", chr, chr);
            return FILE_READ_ERROR;
        }

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

read_status_t read_chunk_to_decode(char* file_name, read_content_t* read_content, int chunk_size)
{
    if (read_content->file == NULL) {
        read_content->file = fopen(file_name, "rb");

        if (read_content->file == NULL) {
            printf("ERROR! Could not open file %s!\n", file_name);
            return FILE_READ_ERROR;
        }
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

write_status_t save_encoded_chunk(char* file_name, encoded_content_t* content)
{
    FILE* file = fopen(file_name, "ab");

    if (file == NULL) {
        printf("ERROR! Could not open file %s!\n", file_name);
        return FILE_WRITE_ERROR;
    }

    fwrite(content->content, sizeof(content->content[0]), content->content_size, file);

    fclose(file);

    return FILE_WRITE_SUCCESS;
}

write_status_t save_decoded_chunk(char* file_name, decoded_content_t* decoded_content)
{
    FILE* file = fopen(file_name, "ab");

    if (file == NULL) {
        printf("ERROR! Could not open file %s!\n", file_name);
        return FILE_WRITE_ERROR;
    }

    fwrite(decoded_content->content, sizeof(char), decoded_content->content_size, file);
    fclose(file);

    return FILE_WRITE_SUCCESS;
}

write_status_t save_metadata(metadata_t* metadata, int metadata_size)
{
    FILE* file = fopen(".metadata", "wb");

    if (file == NULL) {
        printf("ERROR! Metadata can not be saved!\n");
        return FILE_WRITE_ERROR;
    }

    fprintf(file, "shift[%d]", metadata->shift);

    for (int i = 0; i < metadata_size; i++) {
        if (metadata->codes[i].code_len != 0) {
            fprintf(file, "%c", (char)i);
            fprintf(file, "[%d]", metadata->codes[i].code_len);
            fprintf(file, "%d", metadata->codes[i].code);
        }
    }

    fclose(file);

    return FILE_WRITE_SUCCESS;
}

read_status_t read_metadata(metadata_t* metadata)
{
    FILE* file = fopen(METADATA, "rb");

    if (file == NULL) {
        printf("ERROR! File \".metadata\" not found!\n");
        return FILE_READ_ERROR;
    }

    int shift = 0;
    fscanf(file, "shift[%d]", &shift);
    metadata->shift = shift;

    char chr = 0;
    int status = fscanf(file, "%c", &chr);

    while (status != EOF) {
        fscanf(file, "[%d]", &metadata->codes[(int)chr].code_len);
        fscanf(file, "%d", &metadata->codes[(int)chr].code);
        status = fscanf(file, "%c", &chr);
    }

    return FILE_READ_SUCCESS;
}

/**************************** STATIC FUNCTIONS ****************************/