#include <stdlib.h>
#include <stdio.h>

#include "huffman.h"

#define ASCII_TABLE_SIZE 128
#define FIRST_CODE 0

static int look_up_table[128] = { 0 };

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static freq_table_t create_freq_table(char* content);

static int compare_freq_table(const void* elem1, const void* elem2);

static int compare_tree_nodes(const void* node1, const void* node2);

static huffman_tree_t make_huffman_tree(freq_table_t* freq_table);

/**************************** INTERFACE FUNCTIONS ****************************/
void huffman_encode(char* content) {
  freq_table_t freq_table = create_freq_table(content);
  qsort(freq_table.frequencies, freq_table.size, sizeof(char_freq_t), compare_freq_table);
  huffman_tree_t tree = make_huffman_tree(&freq_table);
  return;
}

void huffman_decode(char* content) {
  return;
}

/**************************** STATIC FUNCTIONS ****************************/
static freq_table_t create_freq_table(char* content) {
  int ind = 0;
  int unique_count = 0;
  char ch = content[ind];

  while (ch != '\0') {
    if (look_up_table[(int)ch] == 0) {
      unique_count++;
    }
    look_up_table[(int)ch] += 1;
    ind++;
    ch = content[ind];
  }

  freq_table_t freq_table = { .frequencies = NULL, .size = unique_count, .total_freq = 0 };
  freq_table.frequencies = (char_freq_t*)calloc(unique_count, sizeof(char_freq_t));
  ind = 0;

  for (int i = 0; i < ASCII_TABLE_SIZE; i++) {
    if (look_up_table[i] != 0) {
      freq_table.frequencies[ind].ch = i;
      freq_table.frequencies[ind].freq = look_up_table[i];
      freq_table.total_freq += freq_table.frequencies[ind].freq;
      ind++;
    }
  }

  return freq_table;
}

static int compare_freq_table(const void* elem1, const void* elem2) {
  char_freq_t* char1 = (char_freq_t*)elem1;
  char_freq_t* char2 = (char_freq_t*)elem2;

  if (char1->freq < char2->freq) {
    return -1;
  } else if (char1->freq > char2->freq) {
    return 1;
  } else {
    if ((int)char1->ch < (int)char2->ch) {
      return -1;
    } else {
      return 1;
    }
  }
}

static huffman_tree_t make_huffman_tree(freq_table_t* freq_table) {
  int tree_size = 2 * freq_table->size - 1;
  huffman_tree_node_t* nodes = (huffman_tree_node_t*)calloc(tree_size, sizeof(huffman_tree_node_t));
  huffman_tree_t tree = { .nodes = nodes, .size = tree_size };

  for (int i = 0; i < freq_table->size; i++) {
    tree.nodes[i].ch = freq_table->frequencies[i].ch;
    tree.nodes[i].is_leaf = 1;
    tree.nodes[i].freq = freq_table->frequencies[i].freq;
    tree.nodes[i].code = -1;
  }

  int empty_node_index = freq_table->size;
  for (int i = 0; i < tree_size-2; i+=2) {
    tree.nodes[empty_node_index].is_leaf = 0;
    tree.nodes[empty_node_index].ch = -1;
    tree.nodes[empty_node_index].freq = tree.nodes[i].freq + tree.nodes[i+1].freq;
    tree.nodes[empty_node_index].code = -1;
    empty_node_index += 1;
    qsort(tree.nodes, empty_node_index, sizeof(huffman_tree_node_t), compare_tree_nodes);
  }

  return tree;
}

static int compare_tree_nodes(const void* node1, const void* node2) {
  huffman_tree_node_t* tree_node1 = (huffman_tree_node_t*)node1;
  huffman_tree_node_t* tree_node2 = (huffman_tree_node_t*)node2;

  if (tree_node1->freq < tree_node2->freq) {
    return -1;
  } else {
    return 1;
  }
}