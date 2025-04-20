#include <stdio.h>

#include "huffman.h"
#include "arg_parser.h"

int main(int argc, char* argv[]) {
    char* arguments[2];

    arg_parser_t status = arg_parser(argc, argv, arguments);
    if (status == ERROR_UNKNOWN_OPTION) {
        return ERROR_UNKNOWN_OPTION;
    }

    printf("status: %d\n", status);
    printf("-i --input value: %s\n", arguments[INPUT]);
    printf("-o --output value: %s\n", arguments[OUTPUT]);
    return 0;
}