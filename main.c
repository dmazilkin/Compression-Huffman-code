#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"
#include "arg_parser.h"
#include "file_utils.h"

int main(int argc, char* argv[]) {
    /* Parse command line arguments */
    arg_t arguments = {
        .input_type = INPUT_UNDEFINED,
        .input_val = NULL,
        .output_type = OUTPUT_UNDEFINED,
        .output_val = NULL,
    };

    arg_parser_t status = parse_args(argc, argv, &arguments);

    if (status == PARSE_ERROR_UNKNOWN_OPTION) {
        printf("Error! Status code %d: Unknown option!\n", status);
        return PARSE_ERROR_UNKNOWN_OPTION;
    }

    if (status == PARSE_ERROR_UNDEFINED_INPUT) {
      printf("Error! Status code %d: Undefined input!\n", status);
      return PARSE_ERROR_UNDEFINED_INPUT;
    }

    /* Read content to compress */
    char* content = NULL;

    if (arguments.input_type == INPUT_FILE) {
        file_status_t status = read_from_file(arguments.input_val, &content);

        if (status == FILE_READ_NOT_FOUND) {
            printf("Error! Status code %d: File not found!\n", status);
            return FILE_READ_NOT_FOUND;
        }
    } else {
      content = arguments.input_val;
    }

    /* Encode content */
    canonical_huff_table_t huff = huffman_encode(content);

    /* Write Huffman code to file */
    write_to_file(&huff, content, arguments.output_val);

    return 0;
}