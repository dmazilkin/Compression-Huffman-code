#ifndef IO_H
#define IO_H

#include <stdio.h>

#include "canonical_huffman.h"
#include "arg_parser.h"

typedef enum {
  FILE_READ_SUCCESS=0,
  FILE_WRITE_SUCCESS=1,
  FILE_READ_NOT_FOUND,
  FILE_WRITE_ERROR,
} file_status_t;

typedef struct {
  char* data;
  int count;
} reformatting_data_t;

typedef struct {
  char chr;
  char* code;
} encode_metadata_t;

typedef struct {
  int code;
  int code_len;
  int shift;
} decode_metadata_t;

typedef struct {
  char* content;
  int content_size;
  char is_eof;
  FILE* file;
} read_content_t;

typedef struct {
  char* content;
  int content_size;
  int undecoded_code;
  int undecoded_code_len;
} decoded_content_t;

file_status_t read_from_file(char* file_name, char** content, operation_t option);

file_status_t read_chunk_to_decode(char* file_name, read_content_t* read_content, int chunk_size);

file_status_t save_encoded(canonical_huff_table_t* huff, char* content, char* file_name);

file_status_t save_metadata(canonical_huff_table_t* huff);

file_status_t read_metadata(decode_metadata_t* metadata);

file_status_t save_decoded(char* file_name, decoded_content_t* decoded_content);

#endif //IO_H
