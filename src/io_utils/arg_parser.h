#ifndef ARG_PARSER_H
#define ARG_PARSER_H

typedef enum {
  INPUT_UNDEFINED=0,
  INPUT_FILE,
  INPUT_MSG,
} option_input_t;

typedef enum {
  OUTPUT_UNDEFINED=0,
  OUTPUT_FILE,
  OUTPUT_MSG,
} option_output_t;

typedef enum {
  OPERATION_UNDEFINED=0,
  COMPRESSION,
  DECOMPRESSION,
} operation_t;

typedef struct {
  option_input_t input_type;
  char* input_val;
  option_output_t output_type;
  char* output_val;
  operation_t operation;
} arg_t;

typedef enum {
  PARSE_SUCCESS=0,
  PARSE_ERROR_UNDEFINED_INPUT,
  PARSE_ERROR_UNKNOWN_OPTION,
  PARSE_ERROR_UNDEFINED_OPERATION,
} arg_parser_t;

arg_parser_t parse_args(int argc, char* args_src[], arg_t* args_dst);

#endif //ARG_PARSER_H
