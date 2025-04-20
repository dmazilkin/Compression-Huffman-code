#ifndef ARG_PARSER_H
#define ARG_PARSER_H

typedef enum {
    INPUT=0,
    OUTPUT,
} option_t;

typedef enum {
  SUCCESS=0,
  ERROR_UNKNOWN_OPTION,
} arg_parser_t;

arg_parser_t arg_parser(int argc, char* args_src[], char* args_dst[]);

#endif //ARG_PARSER_H
