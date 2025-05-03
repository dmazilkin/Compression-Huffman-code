#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "huffman.h"
#include "file_utils.h"

/**************************** DEFINES ****************************/
#define ASCII_SIZE 128
#define CHUNK_SIZE 128
#define BYTE_SIZE 8

/**************************** STATIC VARIABLES ****************************/

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static app_status_t compress_from_file(char* input_file_name, char* output_file_name);

static app_status_t decompress_from_file(char* input_file_name, char* output_file_name, metadata_t* metadata);

static metadata_t create_metadata(metadata_codes_t* codes, int metadata_size, canonical_huff_table_t* canonical_huff_table);

static void update_freq_table(read_content_t* read_content, freq_table_t* freq_table);

/**************************** INTERFACE FUNCTIONS ****************************/
app_status_t app(arg_t args) {
  switch (args.operation) {
    case COMPRESSION: {
            if (args.input_type == INPUT_FILE) {
                /* Read data from file and encode it */
                app_status_t app_status = compress_from_file(args.input_val, args.output_val);

                if (app_status == APP_STATUS_ERROR) {
                  printf("ERROR! Failed to compress file!\n");
                  return APP_STATUS_ERROR;
                }
            }

            break;
        }
    case DECOMPRESSION: {
            /* Read metadata for decoding */
            metadata_codes_t codes[ASCII_SIZE] = { { .code = 0, .code_len = 0 } };
            metadata_t metadata = { .codes = codes, .shift = 0 };
            read_status_t status = read_metadata(&metadata);

            if (status == FILE_READ_ERROR) {
                printf("ERROR! Failed to read metadata!\n");
                return APP_STATUS_ERROR;
            }

            if (args.input_type == INPUT_FILE) {
                /* Read data from file and decode it */
                app_status_t app_status = decompress_from_file(args.input_val, args.output_val, &metadata);

                if (app_status == APP_STATUS_ERROR) {
                  printf("ERROR! Failed to decompress file!\n");
                  return APP_STATUS_ERROR;
                }
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
    /* Read data in chunks and update Frequency table */
    char data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
    read_content_t read_content = { .content = data, .content_size = 0, .is_eof = 0, .file = NULL };
    char_freq_t frequencies[ASCII_SIZE] = { { .chr = 0, .freq = 0 } };
    freq_table_t freq_table = create_freq_table(frequencies, ASCII_SIZE);

    while (read_content.is_eof != 1) {
        read_status_t file_read_status = read_chunk_to_encode(input_file_name, &read_content, CHUNK_SIZE);

        if (file_read_status == FILE_READ_ERROR) {
            printf("ERROR! Failed to read data chunk from file!\n");
            return APP_STATUS_ERROR;
        }

        update_freq_table(&read_content, &freq_table);
    }

    /* Build Huffman Tree and calculate Huffman codes */
    int huff_tree_size = freq_table.non_zero_count;
    huff_code_t huff_codes[ASCII_SIZE] = { { .chr = 0, .code = 0, .code_len = 0 } };
    calculate_huff_codes(huff_codes, &freq_table, huff_tree_size);

    /* Calculate Canonical Huffman code */
    canonical_huff_code_t* canonical_codes = (canonical_huff_code_t*)calloc(ASCII_SIZE, sizeof(canonical_huff_code_t));
    canonical_huff_table_t canonical_huff_codes = get_canonical_huff(huff_codes, canonical_codes, ASCII_SIZE);

    /* Read chunk of data, encode it with Canonical Huffman code and save */
    char data_to_encode[CHUNK_SIZE*BYTE_SIZE] = { 0 };
    read_content_t read_content_to_encode = { .content = data_to_encode, .content_size = 0, .is_eof = 0, .file = NULL };
    int unencoded_code = 0;
    int unencoded_code_len = 0;

    while (read_content_to_encode.is_eof != 1) {
        /* Read chunk of data */
        read_status_t file_read_status = read_chunk_to_encode(input_file_name, &read_content_to_encode, CHUNK_SIZE);

        if (file_read_status == FILE_READ_ERROR) {
            printf("ERROR! Failed to read data chunk from file during compression!\n");
            return APP_STATUS_ERROR;
        }
        /* Encode chunk of data */
        char encoded_data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
        encoded_content_t encoded_content = huffman_encode(read_content_to_encode, encoded_data, &canonical_huff_codes, &unencoded_code, &unencoded_code_len);
        /* Sava chunk of data */
        write_status_t file_write_status = save_encoded_chunk(output_file_name, &encoded_content);

        if (file_write_status == FILE_WRITE_ERROR) {
            printf("ERROR! Failed to save data chunk to output file during compression!\n");
            return APP_STATUS_ERROR;
        }
    }

    /* Create and save metadata */
    metadata_codes_t codes[ASCII_SIZE] = { { .code = 0, .code_len = 0 } };
    metadata_t metadata = create_metadata(codes, ASCII_SIZE, &canonical_huff_codes);
    write_status_t file_write_status = save_metadata(&metadata, ASCII_SIZE);

    if (file_write_status == FILE_WRITE_ERROR) {
        printf("ERROR! Failed to save metadata during compression!\n");
        return APP_STATUS_ERROR;
    }

    free(canonical_codes);

    return APP_STATUS_SUCCESS;
}

static app_status_t decompress_from_file(char* input_file_name, char* output_file_name, metadata_t* metadata)
{
    char data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
    read_content_t read_content = { .content = data, .content_size = 0, .is_eof = 0, .file = NULL };
    int undecoded_code = 0;
    int undecoded_code_len = 0;

    while (read_content.is_eof != 1) {
        /* Read data in chunks */
        read_status_t file_read_status = read_chunk_to_decode(input_file_name, &read_content, CHUNK_SIZE);

        if (file_read_status == FILE_READ_ERROR) {
            printf("ERROR! Failed to read data chunk from file during decompression!\n");
            return APP_STATUS_ERROR;
        }
        /* Decode data */
        char encoded_data[CHUNK_SIZE*BYTE_SIZE] = { 0 };
        decoded_content_t decoded_content = huffman_decode(read_content, metadata, encoded_data, undecoded_code, undecoded_code_len);
        undecoded_code = decoded_content.undecoded_code;
        undecoded_code_len = decoded_content.undecoded_code_len;
        /* Save decoded data */
        write_status_t file_write_status = save_decoded_chunk(output_file_name, &decoded_content);

        if (file_write_status == FILE_WRITE_ERROR) {
            printf("ERROR! Failed to save data chunk to output file during decompression!\n");
            return APP_STATUS_ERROR;
        }
    }

    return APP_STATUS_SUCCESS;
}

static metadata_t create_metadata(metadata_codes_t* codes, int metadata_size, canonical_huff_table_t* canonical_huff_table)
{
    metadata_t metadata = { .codes = codes, .shift = 0 };
    for (int i = 0; i < metadata_size; i++) {
        if (canonical_huff_table->codes[i].code_len != 0) {
            metadata.codes[i].code = canonical_huff_table->codes[i].code;
            metadata.codes[i].code_len = canonical_huff_table->codes[i].code_len;
        }
    }
    metadata.shift = canonical_huff_table->shift;

    return metadata;
}

static void update_freq_table(read_content_t* read_content, freq_table_t* freq_table)
{
    for (int i = 0; i < read_content->content_size; i++) {

        if (freq_table->frequencies[(int)read_content->content[i]].freq == 0) {
            freq_table->non_zero_count++;
        }

        freq_table->frequencies[(int)read_content->content[i]].freq += 1;
        freq_table->total_freq += 1;
    }

    return;
}