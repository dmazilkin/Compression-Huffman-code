#ifndef IO_H
#define IO_H

#include "canonical_huffman.h"

typedef enum {
  FILE_READ_SUCCESS=0,
  FILE_WRITE_SUCCESS=1,
  FILE_READ_NOT_FOUND,
  FILE_WRITE_ERROR,
} file_status_t;

file_status_t read_from_file(char* file_name, char** content);

file_status_t write_to_file(canonical_huff_table_t* huff, char* content, char* file_name);

#endif //IO_H
