#include "desarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// destack structure
struct destack {
  int top;
  int bot;
  int len;
  int maxlen;
  int *stack;
};

// destack_create() creates a new empty double ended stack
// effects: allocates memory (client must call destack_destroy)
// time: O(1)
struct destack *destack_create(void) {
  struct destack *s = malloc(sizeof(struct destack));
  s->top = 0;
  s->bot = 0;
  s->len = 0;
  s->maxlen = 1;
  s->stack = malloc(s->maxlen * sizeof(int));
  return s;
}

// destack_is_empty(s) determines if the double ended stack s is empty
// time: O(1)
bool destack_is_empty(const struct destack *s) {
  assert(s);
  return s->len == 0;
}

// destack_top(s) returns the top item in the double ended stack s
// requires: s is not empty
// time: O(1)
int destack_top(const struct destack *s) {
  assert(s);
  assert(s->len);
  return s->stack[s->top];
}

// destack_bot(s) returns the bottom item in the double ended stack s
// requires: s is not empty
// time: O(1)
int destack_bot(const struct destack *s) {
  assert(s);
  assert(s->len);
  return s->stack[s->bot];
}

// destack_pop_top(s) returns and removes the top item from the double ended stack s
// requires: s is not empty
// effects: modifies s
// time: O(1) [amortized]
int destack_pop_top(struct destack *s) {
  assert(s);
  assert(s->len);
  int pop_num = s->stack[s->top];
  s->len -= 1;
  s->top -= 1;
  if ((s->len * 5) < s->maxlen && s->len != 0) {
    s->maxlen = s->len * 3;
    int *a = malloc(s->maxlen * sizeof(int));
    for (int i = s->len; i < 2 * s->len; ++i) {
      a[i] = s->stack[i + s->bot - s->len];
    }
    free(s->stack);
    s->stack = realloc(a, s->maxlen * sizeof(int));
    s->bot = s->len;
    s->top = 2*s->len - 1;
  } else if ((s->len * 5) < s->maxlen && s->len == 0) {
    s->top = 0;
    s->bot = 0;
    s->maxlen = 1;
    s->stack = realloc(s->stack, s->maxlen * sizeof(int));
  }
  return pop_num;
}

// destack_pop_bot(s) returns and removes the bottom item from the double ended stack s
// requires: s is not empty
// effects: modifies s
// time: O(1) [amortized]
int destack_pop_bot(struct destack *s) {
  assert(s);
  assert(s->len);
  int pop_num = s->stack[s->bot];
  s->len -= 1;
  s->bot += 1;
  if ((s->len * 5) < s->maxlen && s->len != 0) {
    s->maxlen = s->len * 3;
    int *a = malloc(s->maxlen * sizeof(int));
    for (int i = s->len; i < 2 * s->len; ++i) {
      a[i] = s->stack[i + s->bot - s->len];
    }
    free(s->stack);
    s->stack = realloc(a, s->maxlen * sizeof(int));
    s->bot = s->len;
    s->top = 2*s->len - 1;
  } else if ((s->len * 5) < s->maxlen && s->len == 0) {
    s->top = 0;
    s->bot = 0;
    s->maxlen = 1;
    s->stack = realloc(s->stack, s->maxlen * sizeof(int));
  }
  return pop_num;
}

// destack_push_top(item, s) adds item to the top of the double ended stack s
// effects: modifies s
// time: O(1) [amortized]
void destack_push_top(int item, struct destack *s) {
  assert(s);
  if (s->len == 0) {
    s->len += 1;
    s->stack[s->top] = item;
  } else {
    if (s->top == s->maxlen - 1) {
    s->maxlen = s->len * 3;
    int *a = malloc(s->maxlen * sizeof(int));
    for (int i = s->len; i < 2 * s->len; ++i) {
      a[i] = s->stack[i + s->bot - s->len];
    }
    free(s->stack);
    s->stack = realloc(a, s->maxlen * sizeof(int));
    s->bot = s->len;
    s->top = 2*s->len - 1;
  }
    s->top += 1;
    s->stack[s->top] = item;
    s->len += 1;
  }
}

// destack_push_bot(item, s) adds item to the bottom of the double ended stack s
// effects: modifies s
// time: O(1) [amortized]
void destack_push_bot(int item, struct destack *s) {
  assert(s);
  if (s->len == 0) {
    s->len += 1;
    s->stack[s->bot] = item;
  } else {
    if (s->bot == 0) {
    s->maxlen = s->len * 3;
    int *a = malloc(s->maxlen * sizeof(int));
    for (int i = s->len; i < 2 * s->len; ++i) {
      a[i] = s->stack[i + s->bot - s->len];
    }
    free(s->stack);
    s->stack = realloc(a, s->maxlen * sizeof(int));
    s->bot = s->len;
    s->top = 2*s->len - 1;
  }
    s->bot -= 1;
    s->stack[s->bot] = item;
    s->len += 1;
  }
}

// destack_destroy(s) frees all memory for the double ended stack s
// requires: s must be from a previous malloc
// effects: s is no longer valid
// time: O(1)
void destack_destroy(struct destack *s) {
  assert(s);
  free(s->stack);
  free(s);
}


// destack_print(s) prints all th elements in destack
// effects: produces output
// time: O(n),  n is the number of elements in destack
void destack_print(const struct destack *s) {
  assert(s);
  printf("Destack: [%d...%d], %d of %d\n", s->bot, s->top, s->len, s->maxlen);
  if (!s->len) {
    printf("[empty]\n");
    return;
  }
  for (int i = s->top; i >= s->bot; --i) {
    printf("%d\n", s->stack[i]);
  }
}
