#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "huffman.h"
#include "file_utils.h"

/**************************** DEFINES ****************************/
#define ASCII_SIZE 128
#define CHUNK_SIZE 64
#define BYTE_SIZE 8

/**************************** INTERFACE FUNCTION ****************************/
app_status_t app(arg_t args) {
      /* Compress data */
    if (args.operation == COMPRESSION) {
        /* Read content to compress */
        char* content = NULL;

        /* Read data in chunks from file and decode it */
        if (args.input_type == INPUT_FILE) {
            file_status_t status = read_from_file(args.input_val, &content, args.operation);

            if (status == FILE_READ_NOT_FOUND) {
                printf("Error! Status code %d: File not found!\n", status);
                return APP_STATUS_ERROR;
            }

            /* Encode content */
            canonical_huff_table_t huff = huffman_encode(content);
            /* Write Huffman code to file */
            (void)save_encoded(&huff, content, args.output_val);
            /* Save metadata to file */
            (void)save_metadata(&huff);
        /* Read data from command line */
        } else {
          content = args.input_val;
        }
    /* Decompress data */
    } else {
        /* Read metadata for decoding */
        decode_metadata_t metadata[ASCII_SIZE] = { { .code = 0, .code_len = 0, .shift = 0 } };
        (void)read_metadata(metadata);

        /* Read data in chunks from file and decode it */
        if (args.input_type == INPUT_FILE) {
            char data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
            read_content_t read_content = { .content = data, .content_size = 0, .is_eof = 0, .file = NULL };
            int undecoded_code = 0;
            int undecoded_code_len = 0;

            while (read_content.is_eof != 1) {
                (void)read_chunk_to_decode(args.input_val, &read_content, CHUNK_SIZE);
                /* Decode content */
                char encoded_data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
                decoded_content_t decoded_content = huffman_decode(read_content, metadata, encoded_data, undecoded_code, undecoded_code_len);
                undecoded_code = decoded_content.undecoded_code;
                undecoded_code_len = decoded_content.undecoded_code_len;
                /* Save decoded content */
                (void)save_decoded(args.output_val, &decoded_content);
            }
        }  else {
            char* content = NULL;
            content = args.input_val;
        }
    }

    return APP_STATUS_SUCCESS;
}