#include <stdlib.h>
#include <stdio.h>

#include "huffman.h"
#include "base_huffman.h"
#include "canonical_huffman.h"

/**************************** INTERFACE FUNCTIONS ****************************/
canonical_huff_table_t huffman_encode(char* content)
{
  freq_table_t freq_table = get_freq_table(content);
  huff_tree_t tree = get_huff_tree(&freq_table);
  huff_table_t huff_table = get_base_huff(&tree, freq_table.size);
  canonical_huff_table_t canonical_huff_table = get_canonical_huff(&huff_table);

  return canonical_huff_table;
}

void huffman_decode(char* content)
{
  return;
}

/**************************** STATIC FUNCTIONS ****************************/
