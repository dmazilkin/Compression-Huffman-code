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

typedef struct {
  option_input_t input_type;
  char* input_val;
  option_output_t output_type;
  char* output_val;
} arg_t;

typedef enum {
  SUCCESS=0,
  ERROR_UNDEFINED_INPUT,
  ERROR_UNKNOWN_OPTION,
} arg_parser_t;

arg_parser_t arg_parser(int argc, char* args_src[], arg_t* args_dst);

#endif //ARG_PARSER_H
