#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"
#include "arg_parser.h"
#include "file_utils.h"

/**************************** DEFINES ****************************/
#define ASCII_SIZE 128
#define DEFAULT_SIZE 64
#define BYTE_SIZE 8

/**************************** STATIC FUNCTION DECLARATIONS ****************************/

/**************************** MAIN FUNCTION ****************************/
int main(int argc, char* argv[]) {
    /* Parse command line arguments */
    arg_t arguments = {
        .input_type = INPUT_UNDEFINED,
        .input_val = NULL,
        .output_type = OUTPUT_UNDEFINED,
        .output_val = NULL,
        .operation = OPERATION_UNDEFINED,
    };

    parser_status_t status = parse_args(argc, argv, &arguments);

    /* Check if provided option is available */
    if (status == PARSE_ERROR_UNKNOWN_OPTION) {
        printf("Error! Status code %d: Unknown option!\n", status);
        return PARSE_ERROR_UNKNOWN_OPTION;
    }

    /* Check if provided input source type is available*/
    if (status == PARSE_ERROR_UNDEFINED_INPUT) {
      printf("Error! Status code %d: Undefined input!\n", status);
      return PARSE_ERROR_UNDEFINED_INPUT;
    }

    /* Compress data */
    if (arguments.operation == COMPRESSION) {
        /* Read content to compress */
        char* content = NULL;

        /* Read data in chunks from file and decode it */
        if (arguments.input_type == INPUT_FILE) {
            file_status_t status = read_from_file(arguments.input_val, &content, arguments.operation);

            if (status == FILE_READ_NOT_FOUND) {
                printf("Error! Status code %d: File not found!\n", status);
                return FILE_READ_NOT_FOUND;
            }

            /* Encode content */
            canonical_huff_table_t huff = huffman_encode(content);
            /* Write Huffman code to file */
            save_encoded(&huff, content, arguments.output_val);
            /* Save metadata to file */
            save_metadata(&huff);
        /* Read data from command line */
        } else {
          content = arguments.input_val;
        }
    /* Decompress data */
    } else {
        /* Read metadata for decoding */
        decode_metadata_t metadata[ASCII_SIZE] = { { .code = 0, .code_len = 0, .shift = 0 } };
        read_metadata(metadata);

        /* Read data in chunks from file and decode it */
        if (arguments.input_type == INPUT_FILE) {
            char data[DEFAULT_SIZE*BYTE_SIZE] = { 0 };
            read_content_t read_content = { .content = data, .content_size = 0, .is_eof = 0, .file = NULL };
            int undecoded_code = 0;
            int undecoded_code_len = 0;

            while (read_content.is_eof != 1) {
                read_content_to_decode(arguments.input_val, &read_content);
                /* Decode content */
                char encoded_data[DEFAULT_SIZE*BYTE_SIZE] = { 0 };
                decoded_content_t decoded_content = huffman_decode(read_content, metadata, encoded_data, undecoded_code, undecoded_code_len);
                undecoded_code = decoded_content.undecoded_code;
                undecoded_code_len = decoded_content.undecoded_code_len;
                /* Save decoded content */
                save_decoded(arguments.output_val, &decoded_content);
            }
        }  else {
            char* content = NULL;
            content = arguments.input_val;
        }
    }

    return 0;
}

/**************************** STATIC FUNCTION DEFINITIONS ****************************/