#include <string.h>

#include "arg_parser.h"

/**************************** DEFINES ****************************/

/**************************** STATIC VARIABLES ****************************/

/**************************** STATIC FUNCTION DECLARATIONS ****************************/

/**************************** INTERFACE FUNCTIONS ****************************/
parser_status_t parse_args(int argc, char* args_src[], arg_t* args_dst) {
    for (int i = 1; i < argc; i++) {
        if (args_src[i][0] == '-') {
            if (args_src[i][1] == '-') {
                if (!strcmp(&args_src[i][2], "input")) {
                    args_dst->input_type = INPUT_FILE;
                    args_dst->input_val = args_src[i+1];
                }
                else if (!strcmp(&args_src[i][2], "output")) {
                    args_dst->output_type = OUTPUT_FILE;
                    args_dst->output_val = args_src[i+1];
                }
                else {
                    return PARSE_ERROR_UNKNOWN_OPTION;
                }
            } else {
                if (args_src[i][1] == 'i') {
                    args_dst->input_type = INPUT_FILE;
                    args_dst->input_val = args_src[i+1];
                }
                else if (args_src[i][1] == 'o') {
                    args_dst->output_type = OUTPUT_FILE;
                    args_dst->output_val = args_src[i+1];
                }
                else if (args_src[i][1] == 'c') {
                    args_dst->operation = COMPRESSION;
                }
                else if (args_src[i][1] == 'd') {
                    args_dst->operation = DECOMPRESSION;
                }
                else {
                    return PARSE_ERROR_UNKNOWN_OPTION;
                }
            }
        }
    }

    /* check if intput_type is provided, otherwise return error */
    if (args_dst->input_type == INPUT_UNDEFINED) {
        return PARSE_ERROR_UNDEFINED_INPUT;
    }

    /* check if output_type is provided, otherwise return error */
    if (args_dst->output_type == OUTPUT_UNDEFINED) {
        return PARSE_ERROR_UNDEFINED_OUTPUT;
    }

    /* check if operation type is set, otherwise return error */
    if (args_dst->operation == OPERATION_UNDEFINED) {
      return PARSE_ERROR_UNDEFINED_OPERATION;
    }

    return PARSE_SUCCESS;
}
/**************************** STATIC FUNCTION DEFINITIONS ****************************/