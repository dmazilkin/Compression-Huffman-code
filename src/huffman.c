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
#define NOT_INITIALIZED 0
#define COMBINED_CHARACTER 0
#define RIGHT_CODE 0
#define LEFT_CODE 1

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static int compare_tree_nodes(const void* node1, const void* node2);

static min_heap_node_t* pop_min(min_heap_t* min_heap, min_heap_t* buff_huff_codes);

static void insert_and_update(min_heap_t* huff_tree, min_heap_node_t new_node);

static void set_node_code(min_heap_node_t* node, huff_code_t* codes);

static min_heap_t create_min_heap(freq_table_t* freq_table, min_heap_node_t* nodes, int huff_tree_size);

static void build_huff_tree(min_heap_t* min_heap, min_heap_t* buff_min_heap);

static void bubble_up(min_heap_t* huff_tree, int node_ind);

static void bubble_down(min_heap_t* huff_tree, int node_ind);

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

void calculate_huff_codes(huff_code_t* codes, freq_table_t* freq_table, int huff_tree_size)
{
  /* Initialize Huffman Tree with min-heap */
  min_heap_node_t* nodes = (min_heap_node_t*)calloc(huff_tree_size, sizeof(min_heap_node_t));
  min_heap_t min_heap = create_min_heap(freq_table, nodes, huff_tree_size);

  /* Build huff_tree */
  int buff_nodes_count = 2*huff_tree_size-1;
  min_heap_node_t* buff_nodes = (min_heap_node_t*)calloc(buff_nodes_count, sizeof(min_heap_node_t));
  min_heap_t buff_min_heap = { .nodes=buff_nodes, .size=0 };
  build_huff_tree(&min_heap, &buff_min_heap);

  /* Calculate Huffman code */
  min_heap_node_t* peak = &(min_heap.nodes[0]);
  peak->chr = UNDEFINED_CODE;
  set_node_code(peak, codes);

  return;
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
static void set_node_code(min_heap_node_t* node, huff_code_t* codes)
{
  min_heap_node_t* left = (min_heap_node_t*)node->left;
  min_heap_node_t* right = (min_heap_node_t*)node->right;

  if ((left != NULL) && (right != NULL)) {
    if (node->code != UNDEFINED_CODE) {
      left->code = (node->code << 1) | LEFT_CODE;
      left->code_len = node->code_len + 1;
      right->code = (node->code << 1) | RIGHT_CODE;
      right->code_len = node->code_len + 1;
    } else {
      left->code = LEFT_CODE;
      left->code_len = 1;
      right->code = RIGHT_CODE;
      right->code_len = 1;
    }

    /* Set left parent */
    codes[left->chr].chr = left->chr;
    codes[left->chr].code = left->code;
    codes[left->chr].code_len = left->code_len;
    /* Set right parent */
    codes[right->chr].chr = right->chr;
    codes[right->chr].code = right->code;
    codes[right->chr].code_len = right->code_len;

    set_node_code(left, codes);
    set_node_code(right, codes);
  }

  return;
}

static min_heap_t create_min_heap(freq_table_t* freq_table, min_heap_node_t* nodes, int huff_tree_size)
{
  min_heap_t min_heap = { .nodes=nodes, .size=huff_tree_size };

  int is_initialized = 0;
  int empty_ind = 0;

  for (int i = 0; i < freq_table->size; i++) {
    if (freq_table->frequencies[i].freq > 0) {
      min_heap_node_t node = {
        .chr = freq_table->frequencies[i].chr,
        .freq = freq_table->frequencies[i].freq,
      };
      insert_and_update(&min_heap, node);
    }
  }

  return min_heap;
}

static void build_huff_tree(min_heap_t* min_heap, min_heap_t* buff_min_heap)
{
  while (min_heap->size > 1) {
    min_heap_node_t* left = pop_min(min_heap, buff_min_heap);
    min_heap_node_t* right = pop_min(min_heap, buff_min_heap);

    min_heap_node_t new_node = {
        .chr=COMBINED_CHARACTER,
        .freq=left->freq + right->freq,
        .code = NOT_INITIALIZED,
        .code_len = NOT_INITIALIZED,
        .left = left,
        .right = right,
    };

    insert_and_update(min_heap, new_node);
  }
  return;
}

static min_heap_node_t* pop_min(min_heap_t* min_heap, min_heap_t* buff_huff_codes)
{
  /* Save min node to buffer */
  buff_huff_codes->nodes[buff_huff_codes->size] = min_heap->nodes[0];
  min_heap_node_t* min_node = &buff_huff_codes->nodes[buff_huff_codes->size];
  buff_huff_codes->size++;

  min_heap->nodes[0] = min_heap->nodes[min_heap->size-1];
  min_heap->size--;
  bubble_down(min_heap, 0);

  return min_node;
}

static void insert_and_update(min_heap_t* huff_tree, min_heap_node_t new_node)
{
  huff_tree->nodes[huff_tree->size] = new_node;
  huff_tree->size++;
  bubble_up(huff_tree, huff_tree->size - 1);

  return;
}

static void bubble_up(min_heap_t* huff_tree, int node_ind)
{
  if (node_ind != 0) {
    int parent_ind = (node_ind - 1) / 2;
    min_heap_node_t parent = huff_tree->nodes[parent_ind];
    min_heap_node_t child = huff_tree->nodes[node_ind];

    if (child.freq < parent.freq) {
      huff_tree->nodes[parent_ind] = child;
      huff_tree->nodes[node_ind] = parent;
      bubble_up(huff_tree, parent_ind);
    }
  }

  return;
}

static void bubble_down(min_heap_t* huff_tree, int node_ind)
{
  int left_ind = 2 * node_ind + 1;
  int right_ind = 2 * (node_ind + 1);

  if (right_ind < huff_tree->size) {
    int child_ind = (huff_tree->nodes[left_ind].freq < huff_tree->nodes[right_ind].freq) ? left_ind : right_ind;
    min_heap_node_t child = huff_tree->nodes[child_ind];
    min_heap_node_t parent = huff_tree->nodes[node_ind];

    if (child.freq < parent.freq) {
      huff_tree->nodes[node_ind] = child;
      huff_tree->nodes[child_ind] = parent;
      bubble_down(huff_tree, child_ind);
    }
  } else if (left_ind < huff_tree->size) {
    int child_ind = left_ind;
    min_heap_node_t child = huff_tree->nodes[child_ind];
    min_heap_node_t parent = huff_tree->nodes[node_ind];

    if (child.freq < parent.freq) {
      huff_tree->nodes[node_ind] = child;
      huff_tree->nodes[child_ind] = parent;
      bubble_down(huff_tree, child_ind);
    }
  }

  return;
}