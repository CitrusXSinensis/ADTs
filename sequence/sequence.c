#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "sequence.h"
#include "cs136-trace.h"


// DO NOT MODIFY THIS STRUCTURE
struct llnode {
  int item;
  struct llnode *next;
};

// you can modify this structure
struct sequence {
  int len;
  struct llnode *data;
};

// sequence_create() returns a pointer to a new (empty) sequence
// effects: allocates memory (caller must call sequence_destroy)
// time: O(1)
struct sequence *sequence_create(void) {
  struct sequence *seq = malloc(sizeof(struct sequence));
  seq->len = 0;
  seq->data = NULL;
  return seq;
}

// free_seq(seq) frees all dynamically allocated memory used by sequence except
//   the pointer to its front
// effects: all the memory at seq except its front is invalid (freed)
// time: O(n)
void free_seq(struct llnode *node) { 
  if (node) { 
    free_seq(node->next);
    free(node); 
  } 
}

// sequence_destroy(seq) frees all dynamically allocated memory 
// effects: the memory at seq is invalid (freed)
// time: O(n)
void sequence_destroy(struct sequence *seq) {
  assert(seq);
  free_seq(seq->data);
  free(seq);
}

// sequence_length(seq) returns the number of items in seq
// time: O(1)
int sequence_length(const struct sequence *seq) {
  assert(seq);
  return seq->len;
}

// sequence_item_at(seq, pos) returns the item in seq at position pos
// requires: 0 <= pos < sequence_length(seq)
// time: O(i) where i is pos described above
int sequence_item_at(const struct sequence *seq, int pos) {
  assert(seq);
  assert(0 <= pos);
  assert(pos < sequence_length(seq));
  struct llnode *node = seq->data;
  for (int i = 0; i < pos; ++i) {
    node = node->next;
  }
  return node->item;
}

// new_node(i, pnext) returns a pointer to a new node with node->item is i 
//   and node->next is pnext
// effects: allocates memory (caller must free the memory at the end)
// time: O(1)
struct llnode *new_node(int i, struct llnode *pnext) { 
  struct llnode *node = malloc(sizeof(struct llnode)); 
  node->item = i; 
  node->next = pnext; 
  return node; 
}

// sequence_insert_at(seq, pos, val) inserts a new item with value val
// at position pos in seq
// (changing the position of items at position >= pos)
// requires: 0 <= pos <=  sequence_length(seq)
// effects: seq is modified
// time: O(i) where i is pos described above
void sequence_insert_at(struct sequence *seq, int pos, int val) {
  assert(seq);
  assert(0 <= pos);
  assert(pos <= sequence_length(seq));
  struct llnode *node = seq->data;
  if (pos == 0) {
    seq->data = new_node(val, node);
  } else {
    for (int i = 0; i < pos - 1; ++i) {
      node = node->next;
    }
    node->next = new_node(val, node->next);
  }
  seq->len += 1;
}

// sequence_remove_at(seq, pos) removes the item at position pos in seq
// and returns the removed value
// (changing the position of items > pos)
// requires: 0 <= pos <  sequence_length(seq)
// effects: seq is modified
// time: O(i) where i is pos described above
int sequence_remove_at(struct sequence *seq, int pos) {
  assert(seq);
  assert(0 <= pos);
  assert(pos < sequence_length(seq));
  struct llnode *node = seq->data;
  for (int i = 0; i < pos - 1; ++i) {
    node = node->next;
  }
  if (pos == 0) {
    int num = node->item;
    seq->data = node->next;
    free(node);
    seq->len -= 1;
    return num;
  }
  struct llnode *old_node = node->next;
  int num = old_node->item;
  if (node->next->next == NULL) {
    free(old_node);
    node->next=NULL;
  } else {
    node->next = node->next->next;
    free(old_node);
  }
  seq->len -= 1;
  return num;
}

// sequence_print(seq) prints out the items in seq
//   using the format: "[item_0,item_1,...,item_last]\n"
//   or "[empty]\n"
// effects: prints out a message
// time : O(n)
void sequence_print(const struct sequence *seq) {
  assert(seq);
  if (seq->len == 0) printf("[empty]\n");
  else {
    printf("[");
    struct llnode *node = seq->data;
    for (int i = 0; i < seq->len - 1; ++i) {
      printf("%d,",node->item);
      node = node->next;
    }
    printf("%d]\n",node->item);
  }
}
