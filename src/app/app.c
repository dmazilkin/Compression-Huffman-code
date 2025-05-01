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

static app_status_t TEST_compress_from_file_chunk(char* input_file_name, char* output_file_name);

static app_status_t decompress_from_file(char* input_file_name, char* output_file_name, decode_metadata_t* metadata);

static app_status_t compress_from_cli(char* data);

static app_status_t decompress_from_cli(char* data);

/**************************** INTERFACE FUNCTIONS ****************************/
app_status_t app(arg_t args) {
  app_status_t app_status = APP_STATUS_UNDEFINED;

  switch (args.operation) {
    case COMPRESSION: {
            if (args.input_type == INPUT_FILE) {
                /* Read data from file and encode it */
                app_status = TEST_compress_from_file_chunk(args.input_val, args.output_val);
            } else {
                /* Read and encode data from cli */
                app_status = compress_from_cli(args.input_val);
            }
            break;
        }
    case DECOMPRESSION: {
            /* Read metadata for decoding */
            decode_metadata_t metadata[ASCII_SIZE] = { { .code = 0, .code_len = 0, .shift = 0 } };
            (void)read_metadata(metadata);

            if (args.input_type == INPUT_FILE) {
                /* Read data from file and decode it */
                app_status = decompress_from_file(args.input_val, args.output_val, metadata);
            } else {
                /* Read and decode data from cli */
                app_status = decompress_from_cli(args.input_val);
            }
            break;
        }
    case OPERATION_UNDEFINED: {
            /* Should never happen */
            break;
        }
    }

    if (app_status == APP_STATUS_ERROR) {
        printf("Error! Application status code: %d\n", app_status);
        return APP_STATUS_ERROR;
    }

    return APP_STATUS_SUCCESS;
}

/**************************** STATIC FUNCTION DEFINITIONS ****************************/
//static app_status_t compress_from_file(char* input_file_name, char* output_file_name)
//{
//    /* Read content to compress */
//    char* content = NULL;
//
//    /* Read data from file and encode it */
//    file_status_t status = read_to_encode_from_file(input_file_name, &content);
//
//    if (status == FILE_READ_NOT_FOUND) {
//        printf("Error! Status code %d: File not found!\n", status);
//        return APP_STATUS_ERROR;
//    }
//
//    /* Encode content */
//    canonical_huff_table_t huff = huffman_encode(content);
//    /* Write Huffman code to file */
//    (void)save_encoded(&huff, content, output_file_name);
//    /* Save metadata to file */
//    (void)save_metadata(&huff);
//
//    return APP_STATUS_SUCCESS;
//}

static app_status_t TEST_compress_from_file_chunk(char* input_file_name, char* output_file_name)
{
    char data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
    read_content_t read_content = { .content = data, .content_size = 0, .is_eof = 0, .file = NULL };
    char_freq_t frequencies[ASCII_SIZE] = { { .chr = 0, .freq = 0 } };
    freq_table_t freq_table = create_freq_table(frequencies);

    while (read_content.is_eof != 1) {
        /* Read data in chunks */
        (void)read_chunk_to_encode(input_file_name, &read_content, CHUNK_SIZE);
        /* Create and calculate frequency table */
        update_freq_table(&read_content, &freq_table);
    }

    int huff_tree_size = freq_table.non_zero_count;
    huff_code_t codes[ASCII_SIZE] = { { .chr=0, .code=0, .code_len=0 } };
    calculate_huff_codes(codes, &freq_table, huff_tree_size);

    for (int i = 0; i < ASCII_SIZE; i++) {
        if (codes[i].code_len != 0) {
            printf("%c[%d]: code=%d, len=%d\n", codes[i].chr, (int)codes[i].chr, codes[i].code, codes[i].code_len);
        }
    }

    return APP_STATUS_SUCCESS;
}

static app_status_t decompress_from_file(char* input_file_name, char* output_file_name, decode_metadata_t* metadata)
{
    char data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
    read_content_t read_content = { .content = data, .content_size = 0, .is_eof = 0, .file = NULL };
    int undecoded_code = 0;
    int undecoded_code_len = 0;

    while (read_content.is_eof != 1) {
        /* Read data in chunks */
        (void)read_chunk_to_decode(input_file_name, &read_content, CHUNK_SIZE);
        /* Decode data */
        char encoded_data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
        decoded_content_t decoded_content = huffman_decode(read_content, metadata, encoded_data, undecoded_code, undecoded_code_len);
        undecoded_code = decoded_content.undecoded_code;
        undecoded_code_len = decoded_content.undecoded_code_len;
        /* Save decoded data */
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