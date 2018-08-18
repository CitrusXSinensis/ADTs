#include <stdlib.h>
#include <assert.h>
#include "minheap.h"

// minheap_create() creates a new empty minheap
// effects: allocates memory (client must call minheap_destroy)

struct minheap *minheap_create(void) {
  struct minheap *h = malloc(sizeof(struct minheap));
  h->len = 0;
  h->maxlen = 1;
  h->data = malloc(sizeof(int) * h->maxlen);
  return h;
}

// minheap_is_empty(h) determines if heap h is empty
// time: O(1)

bool minheap_is_empty(const struct minheap *h) {
  assert(h);
  return h->len == 0;
}

// minheap_insert(i, h) adds i to heap h
// effects: h is modified
// time:    O(logn), n is the size of the heap

void minheap_insert(int i, struct minheap *h) {
  assert(h);
  if(h->len == h->maxlen) {
    h->maxlen = h->maxlen * 2 + 1;
    h->data = realloc(h->data, h->maxlen * sizeof(int));
  }
  h->data[h->len] = i;
  int pos = h->len;
  while(h->data[pos] < h->data[(pos - 1)/2]) {
    h->data[pos] = h->data[(pos - 1)/2];
    h->data[(pos - 1)/2] = i;
    pos = (pos - 1)/2;
    if (pos == 0) break;
  }
  h->len += 1;
}

// minheap_top(h) returns the top of the heap
// requires: h is not empty
// time:     O(1)

int minheap_top(const struct minheap *h) {
  assert(h);
  assert(h->len);
  return h->data[0];
}

// minheap_remove(h) removes and returns the top of the heap
// requires: h is not empty
// effects:  h is modified
// time:     O(logn), n is the size of the heap

int minheap_remove(struct minheap *h) {
  assert(h);
  assert(h->len);
  int min = h->data[0];
  h->data[0] = h->data[h->len - 1];
  h->len -= 1;
  int pos = 0;
  while ((pos * 2 + 1 < h->len && h->data[pos * 2 + 1] < h->data[pos]) ||
         (pos * 2 + 2 < h->len && h->data[pos * 2 + 2] < h->data[pos])) {
    if (h->data[pos * 2 + 1] <= h->data[pos * 2 + 2]) {
      int temp = h->data[pos * 2 + 1];
      h->data[pos * 2 + 1] = h->data[pos];
      h->data[pos] = temp;
      pos = pos * 2 + 1;
    } else {
      int temp = h->data[pos * 2 + 2];
      h->data[pos * 2 + 2] = h->data[pos];
      h->data[pos] = temp;
      pos = pos * 2 + 2;
    }
  }
  return min;
}

// minheap_destroy(h) frees all memory for h
// effects: h is no longer valid
// time:    O(1)

void minheap_destroy(struct minheap *h) {
  assert(h);
  free(h->data);
  free(h);
}

// heapsort(a, len) sorts a in ascending order
// requires: a is a valid array, len > 0
// effects:  modifies a
// time:     O(nlogn)

void heapsort(int a[], int len) {
  assert(a);
  assert(len > 0);
  struct minheap *h = minheap_create();
  for (int i = 0; i < len; ++i) {
    minheap_insert(a[i], h);
  }
  for (int j = 0; j < len; ++j) {
    a[j] = minheap_remove(h);
  }
  minheap_destroy(h);
}
