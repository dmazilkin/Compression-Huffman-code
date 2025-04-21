#include <stdio.h>

#include "huffman.h"
#include "arg_parser.h"

int main(int argc, char* argv[]) {
    arg_t arguments = {
        .input_type = INPUT_UNDEFINED,
        .input_val = NULL,
        .output_type = OUTPUT_UNDEFINED,
        .output_val = NULL,
    };

    arg_parser_t status = arg_parser(argc, argv, &arguments);
    if (status == ERROR_UNKNOWN_OPTION) {
        printf("Error: %d! Unknown option!\n", status);
        return ERROR_UNKNOWN_OPTION;
    }

    printf("-------------------------------\n");
    printf("-o --output type: %d\n", arguments.output_type);
    printf("-o --output value: %s\n", arguments.output_val);
    printf("-------------------------------\n");
    printf("-i --input type: %d\n", arguments.input_type);
    printf("-i --input value: %s\n", arguments.input_val);
    return 0;
}