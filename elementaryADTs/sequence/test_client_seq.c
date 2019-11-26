#include <stdio.h>
#include <assert.h>
#include "sequence.h"

// a simple test client for sequence

int main(void) {
  struct sequence *seq = sequence_create();
  sequence_print(seq);
  assert(sequence_length(seq) == 0);
  sequence_insert_at(seq, 0, 27);
  sequence_insert_at(seq, 0, 17);
  sequence_insert_at(seq, 0, 37);
  sequence_insert_at(seq, 3, 7);
  sequence_insert_at(seq, 2, 47);
  sequence_insert_at(seq, 2, 57);
  sequence_print(seq);
  assert(sequence_item_at(seq, 0) == 37);
  sequence_remove_at(seq, 0);
  sequence_remove_at(seq, 4);
  assert(sequence_length(seq) == 4);
  assert(sequence_item_at(seq, 3) == 27);
  sequence_remove_at(seq, 2);
  sequence_print(seq);
  sequence_destroy(seq);
}
