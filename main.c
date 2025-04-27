#include <stdio.h>
#include <stdlib.h>

#include "huffman.h"
#include "arg_parser.h"
#include "file_utils.h"

/**************************** DEFINES ****************************/
#define ASCII_SIZE 128

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
int main(int argc, char* argv[]) {
    /* Parse command line arguments */
    arg_t arguments = {
        .input_type = INPUT_UNDEFINED,
        .input_val = NULL,
        .output_type = OUTPUT_UNDEFINED,
        .output_val = NULL,
        .operation = OPERATION_UNDEFINED,
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
        file_status_t status = read_from_file(arguments.input_val, &content, arguments.operation);

        if (status == FILE_READ_NOT_FOUND) {
            printf("Error! Status code %d: File not found!\n", status);
            return FILE_READ_NOT_FOUND;
        }
    } else {
      content = arguments.input_val;
    }

    /* Compression operation */
    if (arguments.operation == COMPRESSION) {
        /* Encode content */
        canonical_huff_table_t huff = huffman_encode(content);
        /* Write Huffman code to file */
        save_encoded_to_file(&huff, content, arguments.output_val);
        /* Save metadata to file */
        save_metadata(&huff);
    /* Decomression operation */
    } else {
        /* Read metadata for decoding */
        decode_metadata_t* metadata = (decode_metadata_t*)calloc(ASCII_SIZE, sizeof(decode_metadata_t));
        read_metadata(metadata);
        for (int i = 0; i < ASCII_SIZE; i++) {
          if (metadata[i].code_len != 0) {
                printf("%c: code=%d, len=%d\n", i, metadata[i].code, metadata[i].code_len);
            }
        }
        printf("------------\n");
        /* Read content */
        char* content = NULL;
        file_status_t status = read_from_file(arguments.input_val, &content, arguments.operation);
        /* Decode content */
        char* decoded_content = NULL;
        huffman_decode(content, metadata, &decoded_content);
        printf("Decoded content: %s\n", decoded_content);
    }

    return 0;
}