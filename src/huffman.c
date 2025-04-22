#include <stdlib.h>
#include <stdio.h>

#include "huffman.h"

#define ASCII_TABLE_SIZE 128

static int look_up_table[128] = { 0 };

/**************************** STATIC FUNCTION DECLARATIONS ****************************/
static freq_table_t create_freq_table(char* content);

static int compare_char_freq(const void* elem1, const void* elem2);

/**************************** INTERFACE FUNCTIONS ****************************/
void huffman_encode(char* content) {
  freq_table_t freq_table = create_freq_table(content);
  qsort(freq_table.frequencies, freq_table.size, sizeof(char_freq_t), compare_char_freq);
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

  freq_table_t freq_table = { .frequencies = NULL, .size = unique_count };
  freq_table.frequencies = (char_freq_t*)calloc(unique_count, sizeof(char_freq_t));
  ind = 0;

  for (int i = 0; i < ASCII_TABLE_SIZE; i++) {
    if (look_up_table[i] != 0) {
      freq_table.frequencies[ind].ch = i;
      freq_table.frequencies[ind].freq = look_up_table[i];
      ind++;
    }
  }

  return freq_table;
}

static int compare_char_freq(const void* elem1, const void* elem2) {
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
