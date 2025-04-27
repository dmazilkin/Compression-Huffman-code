#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "canonical_huffman.h"
#include "file_utils.h"

canonical_huff_table_t huffman_encode(char* content);

decoded_content_t huffman_decode(read_content_t content, decode_metadata_t* metadata, char* encoded_data, int undecoded_code, int undecoded_code_len);

#endif //HUFFMAN_H
