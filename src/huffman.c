#include <stdlib.h>
#include <stdio.h>

#include "huffman.h"
#include "base_huffman.h"
#include "canonical_huffman.h"
#include "file_utils.h"

/**************************** DEFINES ****************************/
#define ASCII_SIZE 128
#define BYTE_SIZE 8

/**************************** INTERFACE FUNCTIONS ****************************/
canonical_huff_table_t huffman_encode(char* content)
{
  freq_table_t freq_table = get_freq_table(content);
  huff_tree_t tree = get_huff_tree(&freq_table);
  huff_table_t huff_table = get_base_huff(&tree, freq_table.size);
  canonical_huff_table_t canonical_huff_table = get_canonical_huff(&huff_table);

  return canonical_huff_table;
}

decoded_content_t huffman_decode(read_content_t content, decode_metadata_t* metadata, char* encoded_data, int undecoded_code, int undecoded_code_len)
{
  decoded_content_t decoded_content = { .content = encoded_data, .content_size = 0, .undecoded_code = undecoded_code, .undecoded_code_len = undecoded_code_len };
  int decoded_ind = 0;

  int size = content.content_size;
  if ((content.is_eof) & (metadata->shift)) {
    size -= metadata->shift;
  }

  int content_ind = 0;
  char chr = content.content[0];

  int code_len = 0;
  int code = 0;

  if (decoded_content.undecoded_code_len > 0) {
    code |= decoded_content.undecoded_code;
    code_len = decoded_content.undecoded_code_len;
  }

  while (content_ind < size) {
    int bit = (chr == '1' ? 1 : 0);
//    printf("bit=%d\n", bit);

    code = (code << 1) | bit;
    code_len++;
//    printf("Try to decode: code=%d, len=%d\n", code, code_len);

    for (char i = 0; i < ASCII_SIZE - 1; i++) {
      if ((code == metadata[(int)i].code) && (code_len == metadata[(int)i].code_len) && (metadata[(int)i].code_len != 0)) {
        decoded_content.content[decoded_ind] = i;
//        printf("Decoded: %c!\n", i);
        decoded_ind++;
        code = 0;
        code_len = 0;
      }
    }

    content_ind++;
    chr = content.content[content_ind];
  }

  if (code_len != 0) {
    decoded_content.undecoded_code = code;
    decoded_content.undecoded_code_len = code_len;
  } else {
    decoded_content.undecoded_code = 0;
    decoded_content.undecoded_code_len = 0;
  }

  decoded_content.content_size = decoded_ind;

  return decoded_content;
}

/**************************** STATIC FUNCTIONS ****************************/
