#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "cqueue.h"

struct cqueue {
  int* data;
  int max_len;
  int size;

  /* index of the front of the queue
     since we are using a circular array
     the front is not always equal to 0
   */
  int front;

  /* index of the back of the queue
     since we are using a circular array
     the back is not always equal to size
   */
  int back;
};

// Make the queue dynamic sized, but only grow the size if
// size == max_len

CQueue cqueue_create(int max_capacity) {
  struct cqueue* q = malloc(sizeof(struct cqueue));
  q->max_len = max_capacity;
  q->data = malloc(max_capacity * sizeof(int));
  q->size = 0;
  q->front = 0;
  q->back = 0;
  return q;
}

void cqueue_destroy(CQueue q) {
  free(q->data);
  free(q);
}

int cqueue_front(CQueue q) {
  assert(q->size != 0);
  return q->data[q->front];
}

int cqueue_back(CQueue q) {
  assert(q->size != 0);
  return q->data[q->back];
}

void cqueue_add_back(CQueue q, int item) {
  if (q->back == q->max_len - 1) {
    if (q->size == q->max_len) {
      q->max_len *= 2;
      q->data = realloc(q->data, sizeof(int) * q->max_len);
      q->back++;
      q->data[q->back] = item;
    } else {
      q->back = 0;
      q->data[0] = item;
    }
    q->size++;
    if (q->size == 1) q->front = q->back;
  } else {
    if (q->size == q->max_len) {
      int *a = malloc(sizeof(int) * (1 + q->size));
      int pos = 0;
      for (int i = 0; i <= q->back; ++i) {
        a[pos] = q->data[i];
        ++pos;
      }
      a[pos] = item;
      ++pos;
      for (int i= q->front; i < q->size; ++i) {
        a[pos] = q->data[i];
        ++pos;
      }
      q->max_len += 1;
      q->data = realloc(q->data, q->max_len * sizeof(int));
      for (int i = 0; i < q->size + 1; ++i) {
        q->data[i] = a[i];
      }
      free(a);
      q->back++;
      q->front++;
    } else {
      q->back++;
      q->data[q->back] = item;
    }
    q->size++;
    if (q->size == 1) q->front = q->back;
  }
}

int cqueue_remove_front(CQueue q) {
  assert(q->size != 0);
  int retval = q->data[q->front];
  if(q->front == q->max_len - 1) {
    q->front = 0;
  }
  else {
    q->front++;
  }
  q->size--;
  if (q->size == 0) {
    q->front =0;
    q->back = 0;
  }
  return retval;
}

int cqueue_size(CQueue q) {
  return q->size;
}

void cqueue_print(CQueue q) {
  for(int i = 0; i < q->max_len; i++) {
    printf("%d ", q->data[i]);
  }
  printf("\n");
  printf("front: %d\n", q->front);
  printf("back : %d\n", q->back);
}

int main()
{
    // Assertion based test cases
    struct cqueue* cq = cqueue_create(5);
    cqueue_add_back(cq, 1);
    assert(cqueue_back(cq) == 1);
    assert(cqueue_front(cq) == 1);

    cqueue_add_back(cq, 2);
    assert(cqueue_back(cq) == 2);
    assert(cqueue_front(cq) == 1);

    cqueue_add_back(cq, 3);
    assert(cqueue_back(cq) == 3);
    assert(cqueue_front(cq) == 1);

    cqueue_add_back(cq, 4);
    assert(cqueue_back(cq) == 4);
    assert(cqueue_front(cq) == 1);

    int removed = cqueue_remove_front(cq);
    assert(removed == 1);
    assert(cqueue_back(cq) == 4);
    assert(cqueue_front(cq) == 2);

    cqueue_add_back(cq, 5);
    assert(cqueue_back(cq) == 5);
    assert(cqueue_front(cq) == 2);

    cqueue_add_back(cq, 6);
    assert(cqueue_back(cq) == 6);
    assert(cqueue_front(cq) == 2);

    removed = cqueue_remove_front(cq);
    assert(removed == 2);
    assert(cqueue_back(cq) == 6);
    assert(cqueue_front(cq) == 3);

    cqueue_add_back(cq, 7);
    assert(cqueue_back(cq) == 7);
    assert(cqueue_front(cq) == 3);

    cqueue_print(cq);
    cqueue_print(cq);
    cqueue_add_back(cq, 8);
    assert(cqueue_back(cq) == 8);
    assert(cqueue_front(cq) == 3);

    cqueue_print(cq);

    cqueue_destroy(cq);
    return 0;
}
