#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

// a simple assertion-based client for the trie module

int main(void) {
  struct trie *t = trie_create();
  assert(t);
  assert(t->root == NULL);
  assert(trie_count(t) == 0);
  trie_add("a", t);
  assert(trie_lookup("a", t));  
  assert(trie_count(t) == 1);
  assert(t->root->children[0]);
  assert(t->root->children[0]->word_end);
  trie_print(t); // should just print out "a\n"
  char **aos = trie_to_aos(t);
  assert(aos);
  assert(!strcmp("a", aos[0]));
  free(aos[0]);
  free(aos);
  assert(!trie_lookup("b", t));
  trie_add("b", t);
  assert(t->root->children[1]);
  assert(t->root->children[1]->word_end);
  assert(trie_lookup("b", t));
  assert(trie_count(t) == 2);
  trie_remove("b", t);
  assert(t->root->children[1] == NULL);
  assert(!trie_lookup("b", t));
  trie_add("a", t);
  trie_add("away", t);
  trie_add("air", t);
  trie_add("ads", t);
  trie_add("as", t);
  trie_add("an", t);
  trie_add("car", t);
  trie_add("call", t);
  trie_add("camera", t);
  trie_add("banana", t);
  trie_add("homo", t);
  trie_add("home", t);
  trie_add("zoo", t);
  trie_add("", t);
  assert(trie_count(t) == 14);
  trie_print(t);
  assert(trie_lookup("", t));
  trie_remove("home", t);
  trie_remove("homo", t);
  trie_remove("", t);
  assert(trie_count(t) == 11);
  assert(t->root->children[7] == NULL);
  assert(!trie_lookup("", t));
  assert(!trie_lookup("b", t));
  assert(!trie_lookup("home", t));
  assert(!trie_lookup("homo", t));
  trie_print(t);
  trie_destroy(t);  
}
