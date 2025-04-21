#ifndef IO_H
#define IO_H

typedef enum {
  FILE_READ_SUCCESS=0,
  FILE_READ_NOT_FOUND,
} io_error_t;

io_error_t read_file(char* file_name, char** content);

#endif //IO_H
