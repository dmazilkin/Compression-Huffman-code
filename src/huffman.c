#include <stdlib.h>
#include <stdio.h>

#include "huffman.h"
#include "base_huffman.h"
#include "canonical_huffman.h"
#include "file_utils.h"

/**************************** DEFINES ****************************/
#define ASCII_SIZE 128
#define BYTE_SIZE 8
#define UNDEFINED_CODE -1
#define COMBINED_CHARACTER 0
#define RIGHT_CODE 0
#define LEFT_CODE 1

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static int compare_tree_nodes(const void* node1, const void* node2);

static void set_node_code(huff_tree_node_t* node);

static min_heap_t create_min_heap(freq_table_t* freq_table, char_freq_t* nodes, int huff_tree_size);

static void bubble_up(min_heap_t* min_heap, int node_ind);

/**************************** INTERFACE FUNCTIONS ****************************/
freq_table_t create_freq_table(char_freq_t* frequencies)
{
  freq_table_t freq_table = { .frequencies = frequencies, .size = ASCII_SIZE, .non_zero_count = 0, .total_freq = 0 };

  for (int i = 0; i < freq_table.size; i++) {
    freq_table.frequencies[i].chr = (char)i;
    freq_table.frequencies[i].freq = 0;
  }

  return freq_table;
}

void update_freq_table(read_content_t* read_content, freq_table_t* freq_table)
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

min_heap_t get_huff_tree(freq_table_t* freq_table, char_freq_t* nodes, int huff_tree_size)
{
  min_heap_t min_heap = create_min_heap(freq_table, nodes, huff_tree_size);

  return min_heap;
}

decoded_content_t huffman_decode(read_content_t content, decode_metadata_t* metadata, char* encoded_data, int undecoded_code, int undecoded_code_len)
{
  decoded_content_t decoded_content = { .content = encoded_data, .content_size = 0, .undecoded_code = undecoded_code, .undecoded_code_len = undecoded_code_len };
  int decoded_ind = 0;

  int size = content.content_size;
  if ((content.is_eof == 1) && (metadata->shift > 0)) {
    size -= metadata->shift;
  }

  int content_ind = 0;
  char chr = content.content[0];

  int code_len = 0;
  int code = 0;

  if (decoded_content.undecoded_code_len > 0) {
    code = decoded_content.undecoded_code;
    code_len = decoded_content.undecoded_code_len;
  }

  while (content_ind < size) {
    int bit = (chr == '1' ? 1 : 0);

    code = (code << 1) | bit;
    code_len++;

    for (char i = 0; i < ASCII_SIZE - 1; i++) {
      if ((code == metadata[(int)i].code) && (code_len == metadata[(int)i].code_len) && (metadata[(int)i].code_len != 0)) {
        decoded_content.content[decoded_ind] = i;
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
static void set_node_code(huff_tree_node_t* node)
{
  huff_tree_node_t* left = (huff_tree_node_t*)node->left;
  huff_tree_node_t* right = (huff_tree_node_t*)node->right;

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

static int compare_tree_nodes(const void* node1, const void* node2)
{
  huff_tree_node_t* tree_node1 = (huff_tree_node_t*)node1;
  huff_tree_node_t* tree_node2 = (huff_tree_node_t*)node2;

  if (tree_node1->freq < tree_node2->freq) {
    return -1;
  } else {
    return 1;
  }
}

static min_heap_t create_min_heap(freq_table_t* freq_table, char_freq_t* nodes, int huff_tree_size)
{
  min_heap_t min_heap = { .nodes = nodes, .size = 0 };
  int empty_node_ind = 0;

  for (int i = 0; i < freq_table->size; i++) {
    if (freq_table->frequencies[i].freq > 0) {
      if (min_heap.size == 0) {
        min_heap.nodes[0].freq = freq_table->frequencies[i].freq;
        min_heap.nodes[0].chr = freq_table->frequencies[i].chr;
        min_heap.size++;
      }
      else if (min_heap.nodes[2*empty_node_ind + 1].freq == 0) {
        int left_ind = 2*empty_node_ind + 1;
        min_heap.nodes[left_ind].chr = freq_table->frequencies[i].chr;
        min_heap.nodes[left_ind].freq = freq_table->frequencies[i].freq;
        min_heap.size++;
        bubble_up(&min_heap, left_ind);
      } else {
        int right_ind = 2*(empty_node_ind + 1);
        min_heap.nodes[right_ind].chr = freq_table->frequencies[i].chr;
        min_heap.nodes[right_ind].freq = freq_table->frequencies[i].freq;
        min_heap.size++;
        empty_node_ind++;
        bubble_up(&min_heap, right_ind);
      }
    }
  }

  return min_heap;
}

static void bubble_up(min_heap_t* min_heap, int node_ind)
{
  if (node_ind != 0) {
    int parent_ind = (node_ind - 1) / 2;
    char_freq_t parent = min_heap->nodes[parent_ind];
    char_freq_t child = min_heap->nodes[node_ind];
    if (child.freq < parent.freq) {
      min_heap->nodes[parent_ind] = child;
      min_heap->nodes[node_ind] = parent;
      bubble_up(min_heap, parent_ind);
    }
  }
}