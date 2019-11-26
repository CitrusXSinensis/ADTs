#include <assert.h>
#include <stdlib.h>
#include "set.h"

// a simple test client

int main(void) {
  int a[] = {3, 2, 1, 1, 2, 3};
  struct set *set_a = array_to_set(a, 6);
  set_print(set_a);
  set_add(set_a, 2);
  assert(set_size(set_a) == 3);
  assert(set_member(set_a, 2));
  assert(!set_member(set_a, 0));
  set_remove(set_a, 2);
  set_print(set_a);
  assert(!set_member(set_a, 2));
  struct set *set_b = set_create();
  set_print(set_b);
  set_add(set_b, 7);
  set_add(set_b, 5);
  set_add(set_b, 3);
  set_print(set_b);
  struct set *set_f = set_create();
  struct set *set_g = set_intersect(set_f, set_b);
  set_print(set_g);
  struct set *set_c = set_union(set_a, set_b);
  set_print(set_c);
  struct set *set_d = set_intersect(set_b, set_c);
  set_print(set_d);
  int *e = set_to_array(set_d);
  assert(e[0] == 3);
  assert(e[1] == 5);
  assert(e[2] == 7);
  struct set *set_test1 = set_create();
  set_add(set_test1, -10);
  set_add(set_test1, -9);
  set_add(set_test1, -8);
  set_add(set_test1, -7);
  set_add(set_test1, -6);
  set_add(set_test1, -5);
  set_add(set_test1, -4);
  set_add(set_test1, -3);
  struct set *set_test2 = set_create();
  set_add(set_test2, -3);
  set_add(set_test2, -7);
  set_add(set_test2, -5);
  set_add(set_test2, -9);
  set_add(set_test2, -4);
  struct set *set_test3 = set_union(set_test1, set_test2);
  set_print(set_test3);
  set_destroy(set_a);
  set_destroy(set_b);
  set_destroy(set_c);
  set_destroy(set_d);
  set_destroy(set_f);
  set_destroy(set_g);
  set_destroy(set_test1);
  set_destroy(set_test2);
  set_destroy(set_test3);
  free(e);
}
