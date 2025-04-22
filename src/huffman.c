#include <stdlib.h>
#include <stdio.h>

#include "huffman.h"

#define ASCII_TABLE_SIZE 128
#define COMBINED_CHARACTER 0
#define UNDEFINED_CODE -1
#define RIGHT_CODE 0
#define LEFT_CODE 1

static int look_up_table[128] = { 0 };

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static freq_table_t create_freq_table(char* content);

static int compare_freq_table(const void* elem1, const void* elem2);

static int compare_tree_nodes(const void* node1, const void* node2);

static huffman_tree_t make_huffman_tree(freq_table_t* freq_table);

static void set_node_code(huffman_tree_node_t* node);

static huffman_table_t get_huffman_table(huffman_tree_t* tree, int leaf_count);

/**************************** INTERFACE FUNCTIONS ****************************/
void huffman_encode(char* content) {
  freq_table_t freq_table = create_freq_table(content);
  huffman_tree_t tree = make_huffman_tree(&freq_table);
  huffman_table_t huffman_table = get_huffman_table(&tree, freq_table.size);

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

  qsort(freq_table.frequencies, freq_table.size, sizeof(char_freq_t), compare_freq_table);

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
    tree.nodes[i].freq = freq_table->frequencies[i].freq;
    tree.nodes[i].code = UNDEFINED_CODE;
    tree.nodes[i].left = NULL;
    tree.nodes[i].right = NULL;
  }

  int empty_node_index = freq_table->size;

  for (int i = 0; i < tree_size-2; i+=2) {
    tree.nodes[empty_node_index].ch = COMBINED_CHARACTER;
    tree.nodes[empty_node_index].freq = tree.nodes[i].freq + tree.nodes[i+1].freq;
    tree.nodes[empty_node_index].code = UNDEFINED_CODE;
    tree.nodes[empty_node_index].code_len = 0;
    tree.nodes[empty_node_index].left = &tree.nodes[i];
    tree.nodes[empty_node_index].right = &tree.nodes[i+1];
    empty_node_index += 1;
    qsort(tree.nodes, empty_node_index, sizeof(huffman_tree_node_t), compare_tree_nodes);
  }

  huffman_tree_node_t result_node = tree.nodes[tree_size - 1];
  set_node_code(&result_node);

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

static void set_node_code(huffman_tree_node_t* node) {
  huffman_tree_node_t* left = (huffman_tree_node_t*)node->left;
  huffman_tree_node_t* right = (huffman_tree_node_t*)node->right;

  if ((left != NULL) && (right != NULL)) {
    if (node->code != UNDEFINED_CODE) {
      left->code = (node->code << 1) | LEFT_CODE;
      left->code_len += node->code_len + 1;
      right->code = (node->code << 1) | RIGHT_CODE;
      right->code_len += node->code_len + 1;
    } else {
      left->code = LEFT_CODE;
      left->code_len = 1;
      right->code = RIGHT_CODE;
      right->code_len = 1;
    }
    set_node_code(left);
    set_node_code(right);
  }

  return;
}

static huffman_table_t get_huffman_table(huffman_tree_t* tree, int leaf_count) {
  huffman_code_t* huffman_code = (huffman_code_t*)calloc(leaf_count, sizeof(huffman_code_t));
  huffman_table_t huffman_table = { .codes = huffman_code, .size = leaf_count };
  int ind = 0;

  for (int i = 0; i < tree->size; i++) {
     if (tree->nodes[i].left == NULL && tree->nodes[i].right == NULL) {
        huffman_code[ind].ch = tree->nodes[i].ch;
        huffman_code[ind].code = tree->nodes[i].code;
        huffman_code[ind].code_len = tree->nodes[i].code_len;
        ind++;
      }
  }

  return huffman_table;
}