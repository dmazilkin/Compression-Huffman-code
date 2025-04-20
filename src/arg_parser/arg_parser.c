#include <string.h>

#include "arg_parser.h"

arg_parser_t arg_parser(int argc, char* args_src[], char* args_dst[]) {
    for (int i = 1; i < argc; i++) {
        if (args_src[i][0] == '-') {
            if (args_src[i][1] == '-') {
                if (!strcmp(&args_src[i][2], "input")) {
                    args_dst[INPUT] = args_src[i+1];
                } else if (!strcmp(&args_src[i][2], "output")) {
                    args_dst[OUTPUT] = args_src[i+1];
                } else {
                    return ERROR_UNKNOWN_OPTION;
                }
            } else {
                if (args_src[i][1] == 'i') {
                    args_dst[INPUT] = args_src[i+1];
                } else if (args_src[i][1] == 'o') {
                    args_dst[OUTPUT] = args_src[i+1];
                } else {
                    return ERROR_UNKNOWN_OPTION;
                }
            }
        }
    }
    return SUCCESS;
}
