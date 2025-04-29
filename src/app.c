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

/**************************** STATIC VARIABLES ****************************/

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static app_status_t compress_from_file(char* input_file_name, char* output_file_name);

static app_status_t compress_from_cli(char* data);

static app_status_t decompress_from_file(char* input_file_name, char* output_file_name, decode_metadata_t* metadata);

static app_status_t decompress_from_cli(char* data);

/**************************** INTERFACE FUNCTIONS ****************************/
app_status_t app(arg_t args) {
  switch (args.operation) {
    case COMPRESSION: {
            /* Read data in chunks from file and decode it */
            if (args.input_type == INPUT_FILE) {
                app_status_t app_status = compress_from_file(args.input_val, args.output_val);
            } else {
                app_status_t app_status = compress_from_cli(args.input_val);
            }
            break;
        }
    case DECOMPRESSION: {
            /* Read metadata for decoding */
            decode_metadata_t metadata[ASCII_SIZE] = { { .code = 0, .code_len = 0, .shift = 0 } };
            (void)read_metadata(metadata);

            /* Read data in chunks from file and decode it */
            if (args.input_type == INPUT_FILE) {
                decompress_from_file(args.input_val, args.output_val, metadata);
            }  else {
              (void)decompress_from_cli(args.input_val);
            }
            break;
        }
    case OPERATION_UNDEFINED: {
            /* Should never happen */
            break;
        }
    }

    return APP_STATUS_SUCCESS;
}

/**************************** STATIC FUNCTION DEFINITIONS ****************************/
static app_status_t compress_from_file(char* input_file_name, char* output_file_name)
{
    /* Read content to compress */
    char* content = NULL;

    /* Read data in chunks from file and decode it */
    file_status_t status = read_to_encode_from_file(input_file_name, &content);

    if (status == FILE_READ_NOT_FOUND) {
        printf("Error! Status code %d: File not found!\n", status);
        return APP_STATUS_ERROR;
    }

    /* Encode content */
    canonical_huff_table_t huff = huffman_encode(content);
    /* Write Huffman code to file */
    (void)save_encoded(&huff, content, output_file_name);
    /* Save metadata to file */
    (void)save_metadata(&huff);

    return APP_STATUS_SUCCESS;
}

static app_status_t decompress_from_file(char* input_file_name, char* output_file_name, decode_metadata_t* metadata)
{
    char data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
    read_content_t read_content = { .content = data, .content_size = 0, .is_eof = 0, .file = NULL };
    int undecoded_code = 0;
    int undecoded_code_len = 0;

    while (read_content.is_eof != 1) {
      (void)read_chunk_to_decode(input_file_name, &read_content, CHUNK_SIZE);
      /* Decode content */
      char encoded_data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
      decoded_content_t decoded_content = huffman_decode(read_content, metadata, encoded_data, undecoded_code, undecoded_code_len);
      undecoded_code = decoded_content.undecoded_code;
      undecoded_code_len = decoded_content.undecoded_code_len;
      /* Save decoded content */
      (void)save_decoded(output_file_name, &decoded_content);
    }

    return APP_STATUS_SUCCESS;
}

static app_status_t compress_from_cli(char* data)
{

  return APP_STATUS_SUCCESS;
}

static app_status_t decompress_from_cli(char* data)
{

    return APP_STATUS_SUCCESS;
}