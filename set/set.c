#include "mergesort.h"
#include "set.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct set {
  int len;
  int maxlen;
  int *stack;
};

// set_create() returns a new empty set
// effects: allocates memory (caller must call set_destroy)
// time: O(1)
struct set *set_create(void) {
  struct set *s = malloc(sizeof(struct set));
  s->len = 0;
  s->maxlen = 1;
  s->stack = malloc(sizeof(int) * s->maxlen);
  return s;
}

// set_destroy(s) frees all dynamically allocated memory 
// effects: the memory at s is invalid (freed)
// time: O(1)
void set_destroy(struct set *s) {
  assert(s);
  free(s->stack);
  free(s);
}

// set_size(s) returns the number of elements in s
// time: O(1)
int set_size(const struct set *s) {
  assert(s);
  return s->len;
}

// set_member(s, i) determines if i is in set s
// time: O(log n) where n is set_size(s)
bool set_member(const struct set *s, int i) {
  assert(s);
  int low = 0; 
  int mid = 0; 
  int high = s->len - 1; 
  while (low <= high) {
    mid = (low + high) / 2; 
    if (s->stack[mid] == i) { 
      return 1; 
    } else if (s->stack[mid] < i) { 
      low = mid + 1; 
    } else { 
      high = mid - 1; 
    }
  }
  return 0;
}

// set_add(s, i) adds i to the set s, if it does not already contain i
// effects: s may be modified
// time: O(n) where n is set_size(s)
void set_add(struct set *s, int i) {
  assert(s);
  if (set_member(s, i)) return;
  int j = 0;
  while(j < s->len) {
    if (s->stack[j] < i) ++j;
    else {
      if (s->maxlen == s->len) {
        s->maxlen *= 2; 
        s->stack = realloc(s->stack, s->maxlen * sizeof(int));
      }
      int *a = malloc(sizeof(int) * (s->len + 1));
      for(int k = 0; k < j; ++k) {
        a[k] = s->stack[k];
      }
      a[j] = i;
      for(int l = j + 1; l < s->len + 1; ++l) {
        a[l] = s->stack[l - 1];
      }
      free(s->stack);
      s->stack = realloc(a, s->maxlen * sizeof(int));
      s->len += 1;
      return;
    }
  }
  if (s->maxlen == s->len) {
    s->maxlen *= 2; 
    s->stack = realloc(s->stack, s->maxlen * sizeof(int));
  }
  s->stack[s->len] = i;
  s->len += 1;
}

// set_remove(s, i) removes i from s.  if i is not in s, s isn't changed
// effects: s may be modified
// time: O(n) where n is set_size(s)
void set_remove(struct set *s, int i) {
  assert(s);
  if (!set_member(s, i)) return;
  for (int j = 0; j < s->len; ++j) {
    if (s->stack[j] == i) {
      int *a = malloc(sizeof(int) * s->maxlen);
      for (int k = 0; k < j; ++k) {
        a[k] = s->stack[k];
      }
      for (int l = j; l < s->len - 1; ++l) {
        a[l] = s->stack[l + 1];
      }
      free(s->stack);
      s->stack = realloc(a, sizeof(int) * s->maxlen);
      s->len -= 1;
    }
  }
}

// set_union(s1, s2) returns a new set that is the union of s1 and s2
// effects: allocates memory for the new set (caller must call set_destroy)
// time: O(n) where n is set_size(s1) + set_size(s2)
struct set *set_union(const struct set *s1, const struct set *s2) {
  assert(s1);
  assert(s2);
  struct set *union1 = set_create();
  int pos1 = 0;
  int pos2 = 0;
  int index = 0;
  free(union1->stack);
  union1->stack = malloc(sizeof(int) * (s1->len + s2->len));
  for (int i = 0; i < s1->len + s2->len; ++i) {
    if (pos1 == s1->len ||
        (pos2 < s2->len && s2->stack[pos2] < s1->stack[pos1])) {
      if ((i == 0) || (union1->stack[index - 1] != s2->stack[pos2])) {
        union1->stack[index] = s2->stack[pos2];
        ++index;
        ++pos2;
      } else {
        ++pos2;
      }
    } else {
      if ((i == 0) || (union1->stack[index - 1] != s1->stack[pos1])) {
        union1->stack[index] = s1->stack[pos1];
        ++index;
        ++pos1;
      } else {
        ++pos1;
      }
    }
  }
  
  union1->stack = realloc(union1->stack, index * sizeof(int));
  union1->len = index;
  union1->maxlen = index;
  return union1;
}

// set_intersect(s1, s2) returns a new set that is the intersection of s1 and s2
// effects: allocates memory for the new set (caller must call set_destroy)
// time: O(n) where n is set_size(s1) + set_size(s2)
struct set *set_intersect(const struct set *s1, const struct set *s2) {
  assert(s1);
  assert(s2);
  struct set *union1 = set_create();
  int pos1 = 0;
  int pos2 = 0;
  int index = 0;
  free(union1->stack);
  union1->stack = malloc(sizeof(int) * s2->len);
  while (1) {
    if(pos1 == s1->len || pos2 == s2->len) break;
    if (s1->stack[pos1] < s2->stack[pos2]) {
      ++pos1;
    } else if (s2->stack[pos2] < s1->stack[pos1]) {
      ++pos2;
    } else {
      union1->stack[index] = s1->stack[pos1];
      ++index;
      ++pos1;
      ++pos2;
    }
  }
  union1->stack = realloc(union1->stack, index * sizeof(int));
  union1->len = index;
  union1->maxlen = index;
  return union1;
}

// array_to_set(a, len) returns a new set that is all the unique elements of a
// requires: len > 0
// effects: allocates memory for the new set (caller must call set_destroy)
// time: O(n log n) where n is set_size(s)
struct set *array_to_set(const int a[], int len) {
  assert(a);
  assert(len > 0);
  int *b = malloc(sizeof(int) * len);
  for (int j = 0; j < len; ++j) {
    b[j] = a[j];
  }
  merge_sort(b, len);
  struct set *union1 = set_create();
  free(union1->stack);
  union1->stack = malloc(sizeof(int) * len);
  union1->stack[0] = b[0];
  int index = 1;
  for(int i = 1; i < len; ++i) {
    if(b[i] != b[i - 1]) {
      union1->stack[index] = b[i];
      index++;
    }
  }
  free(b);
  union1->stack = realloc(union1->stack, index * sizeof(int));
  union1->len = index;
  union1->maxlen = index;
  return union1;
}

// set_to_array(s) returns a new array that is the elements of s
// in ascending order, or NULL if s is empty
// effects: may allocate memory for a new array (caller must call free)
// time: O(n)
int *set_to_array(const struct set *s) {
  assert(s);
  if (s->len == 0) return NULL;
  else {
    int *a = malloc(sizeof(int) * s->len);
    for (int i = 0; i < s->len; ++i) {
      a[i] = s->stack[i];
    }
    return a;
  }
}

// set_print(s) prints the elements of set s in ascending order
// using the format: 
// "[element_smallest,element_next,...,element_largest]\n"
// or "[empty]\n"
// effects: prints to output
// time: O(n)
void set_print(const struct set *s) {
  assert(s);
  if (s->len == 0) printf("[empty]\n");
  else {
    printf("[");
    for (int i = 0; i < s->len - 1; ++i) {
      printf("%d,", s->stack[i]);
    }
    printf("%d]\n", s->stack[s->len - 1]);
  }
}
