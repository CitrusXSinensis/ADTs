#include <stdlib.h>
#include <assert.h>
#include "trie.h"
#include "wordlist.h"

struct wordlist {
  struct trie *t;
  int count;
};

// wordlist_create() creates a new empty wordlist
// effects: allocates memory (must call wordlist_destroy)
struct wordlist *wordlist_create(void) {
  struct wordlist *wl = malloc(sizeof(struct wordlist));
  wl->t = trie_create();
  wl->count = 0;
  return wl;
}

// wordlist_add(s, wl) adds the word s to wordlist wl
// effects: modifies wl
// time:    O(n), n is the length of s
void wordlist_add(const char *s, struct wordlist *wl) {
  assert(wl);
  trie_add(s, wl->t);
  wl->count += 1;
}

// wordlist_remove(s, wl) removes the word s from wordlist wl (if it exists)
// effects: may modify wl
// time:    O(n), n is the length of s
void wordlist_remove(const char *s, struct wordlist *wl) {
  assert(wl);
  assert(s);
  trie_remove(s, wl->t);
  wl->count -= 1;
}

// wordlist_lookup(s, wl) determines if wl contains the word s
// time: O(n), n is the length of s
bool wordlist_lookup(const char *s, const struct wordlist *wl) {
  assert(wl);
  assert(s);
  return trie_lookup(s, wl->t);
}

// wordlist_destroy(wl) frees all memory for wl
// effects: wl is no longer valid
// time:    O(n), n is the number of characters in all of the words in wl
void wordlist_destroy(struct wordlist *wl) {
  assert(wl);
  trie_destroy(wl->t);
  free(wl);
}

// wordlist_print(wl) prints each word in wl in alphabetical order,
//   with each word on a newline
// effects: displays output (if wl is not empty)
// time:    O(n^2), n is the number of characters in all of the words in wl
void wordlist_print(const struct wordlist *wl) {
  assert(wl);
  trie_print(wl->t);
}

// wordlist_count(wl) determines how many words are in wl
// time: O(1)
int wordlist_count(const struct wordlist *wl) {
  assert(wl);
  return wl->count;
}
