#ifndef IO_H
#define IO_H

#include <stdio.h>

typedef enum {
  FILE_READ_SUCCESS=0,
  FILE_READ_ERROR,
} read_status_t;

typedef enum {
  FILE_WRITE_SUCCESS=0,
  FILE_WRITE_ERROR,
} write_status_t;

typedef struct {
  char* content;
  int content_size;
} encoded_content_t;

typedef struct {
  int code;
  int code_len;
} metadata_codes_t;

typedef struct {
  metadata_codes_t* codes;
  int shift;
} metadata_t;

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

read_status_t read_chunk_to_encode(char* file_name, read_content_t* read_content, int chunk_size);

read_status_t read_chunk_to_decode(char* file_name, read_content_t* read_content, int chunk_size);

write_status_t save_encoded_chunk(char* file_name, encoded_content_t* content);

write_status_t save_decoded_chunk(char* file_name, decoded_content_t* decoded_content);

write_status_t save_metadata(metadata_t* metadata, int metadata_size);

read_status_t read_metadata(metadata_t* metadata);

#endif //IO_H
