#include <stdlib.h>
#include <stdio.h>

#include "huffman.h"
#include "base_huffman.h"
#include "canonical_huffman.h"
#include "file_utils.h"

/**************************** DEFINES ****************************/
#define DEFAULT_SIZE 128
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

void huffman_decode(char* content, decode_metadata_t* metadata, char** decoded_content)
{
  int decoded_ind = 0;
  int decoded_size = DEFAULT_SIZE;
  *decoded_content = (char*)calloc(decoded_size, sizeof(char));

  int content_ind = 0;
  char chr = content[0];

  int code_len = 0;
  int code = 0;

  while (chr != '\0') {
    int bit = (chr == '1' ? 1 : 0);
    printf("bit=%d\n", bit);

    code = (code << 1) | bit;
    code_len++;
    printf("Try to decode: code=%d, len=%d\n", code, code_len);

    for (char i = 0; i < ASCII_SIZE - 1; i++) {
      if ((code == metadata[(int)i].code) && (code_len == metadata[(int)i].code_len) && (metadata[(int)i].code_len != 0)) {
        (*decoded_content)[decoded_ind] = i;
        content[decoded_ind] = i;
        printf("Decoded: %c!\n", i);
        decoded_ind++;
        code = 0;
        code_len = 0;

        if (decoded_ind >= decoded_size - 1) {
          decoded_size += DEFAULT_SIZE;
          printf("size = %d\n", decoded_size);
          *decoded_content = (char*)realloc(decoded_content, decoded_size * sizeof(char));
        }
      }
    }

    content_ind++;
    chr = content[content_ind];
  }
  printf("size=%d\n", decoded_ind);
//  printf("shift=%d\n", (*metadata).shift);
  if ((*metadata).shift) {
    *decoded_content = (char*)realloc(*decoded_content, (decoded_ind - (*metadata).shift) * sizeof(char));
  }

  return;
}

/**************************** STATIC FUNCTIONS ****************************/
