#ifndef IO_H
#define IO_H

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
} decode_metadata_t;

file_status_t read_from_file(char* file_name, char** content, operation_t option);

file_status_t save_encoded(canonical_huff_table_t* huff, char* content, char* file_name);

file_status_t save_metadata(canonical_huff_table_t* huff);

file_status_t read_metadata(decode_metadata_t* metadata);

file_status_t save_decoded(char* content, char* file_name);

#endif //IO_H
