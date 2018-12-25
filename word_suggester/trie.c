#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "trie.h"

// trie_create() creates a new empty trie
// effects: allocates memory (must call trie_destroy)
struct trie *trie_create(void) {
  struct trie *t = malloc(sizeof(struct trie));
  t->root = NULL;
  return t;
}

// new_node() return a new trienode
// effects: allocates memory (must call trie_destroy)
// time: O(1)
struct trienode *new_node() {
  struct trienode *new = malloc(sizeof(struct trienode));
  new->word_end = false;
  for (int i = 0; i < 26; ++i) {
    new->children[i] = NULL;
  }
  return new;
}

// trie_add(s, t) adds the word s to trie t
// effects: modifies t
// time:    O(n), n is the length of s
void trie_add(const char *s, struct trie *t) {
  assert(s);
  assert(t);
  if (t->root == NULL) t->root = new_node();
  
  if (!strcmp(s, "")) {
    t->root->word_end = true;
    return;
  }
  
  struct trienode *node = t->root;
  const char *p = s;
  while(1) {
    if(*p == 0) {
      node->word_end = true;
      return;
    }
    
    if(node->children[*p - 'a'] == NULL) {
      node->children[*p - 'a'] = new_node();
    }
    
    node = node->children[*p - 'a'];
    ++p;
  }
}

// free_node(node) free all memory of node
// effects: node is no longer valid
// time:    O(n), n is the number of characters in all of the words in node
void free_node(struct trienode *node) {
  if (node) {
    for (int i = 0; i < 26; ++i) {
      free_node(node->children[i]);
    }
    free(node);
    node = NULL;
  }
}

// has_child(node) return if the node has at least 1 child
// time:    O(1)
bool has_child(struct trienode *node) {
  for (int i = 0; i < 26; ++i) {   
    if (node->children[i] != NULL) return true;
  }
  return false;
}


// trie_remove_h(rn,s,level,len) removes the word s from node
//   and removes any trienodes that are no longer needed
// effects: modifies node
// time:    O(n), where n is the length of s
bool remove_nodes(const char* s, struct trienode* node, int s_len, int dept) {
  
  if(dept == s_len && node->word_end) {
    node->word_end = false;
    if(has_child(node)) return false;
    return true;
  }
  
  if(remove_nodes(s, node->children[s[dept] - 'a'], s_len, dept + 1)) {
    free(node->children[s[dept] - 'a']);
    node->children[s[dept] - 'a'] = NULL;
    if((!node->word_end) && !has_child(node)) return true;
  }
  
  return false;
}

// trie_remove(s, t) removes the word s from trie t (if it exists)
//   and removes any trienodes that are no longer needed
// effects: may modify t
// time:    O(n), n is the length of s
void trie_remove(const char *s, struct trie *t) {
  assert(s);
  assert(t);
  if(!trie_lookup(s,t)) return;
  
  if(!strcmp(s, "")) {
    if(!has_child(t->root)) free(t->root);
    t->root->word_end = false;
    return;
  }
  
  int s_len = strlen(s);
  remove_nodes(s, t->root,s_len, 0);
}

// trie_lookup(s, t) determines if t contains the word s
// time: O(n), n is the length of s
bool trie_lookup(const char *s, const struct trie *t) {
  assert(s);
  assert(t);
  if (t->root == NULL) return false;
  if (!strcmp(s, "")) return t->root->word_end;
  struct trienode *node = t->root;
  const char *p = s;
  
  while(*p) {
    if(node->children[*p - 'a'] == NULL) return false;
    node = node->children[*p - 'a'];
    ++p;
  }
  
  return node->word_end;
}

// trie_destroy(t) frees all memory for t
// effects: t is no longer valid
// time:    O(n), n is the number of characters in all of the words in t
void trie_destroy(struct trie *t) {
  assert(t);
  free_node(t->root);
  free(t);
}

// trev_end(node) returns a list of pointers to trienode which are all of
//   the nodes in t with node->end_word is true
// effects: allocate memory (must free memory after use)
// time: O(n), n is the number of characters in all of the words in node
void trev_end(struct trienode *node, int *len) {
  if(node) {
    if (node->word_end == true) *len += 1;
    
    if (!has_child(node)) return;
    for (int i = 0; i < 26; ++i) {
      if(node->children[i] != NULL) {
        trev_end(node->children[i], len);
      }
    }
    
  }
  return;
}

// a store struct
struct store {
  int *a_word;
  int len;
  int maxlen;
};

// insert_char(word, i) insert i into word at index word->len
// effects: may allocate memory (need to call free)
//          mutate word
// time     O(1)
void insert_char(struct store *word, int i) {
  assert(word);
  
  if (word->len == word->maxlen) {
    word->maxlen *= 2;
    word->a_word = realloc(word->a_word, sizeof(int) * word->maxlen);
  }
  
  word->a_word[word->len] = i;
  word->len += 1;
}

// addstring(count, aos, word) add a string into
// effects: allocate memory (need to call free)
//          mutate aos[*count]
// time     O(n), n is the length of word->a_word
void addstring(int *count, char **aos, struct store *word) {
  assert(aos);
  assert(word);
  aos[*count] = malloc(sizeof(char) * (word->len + 1));
  
  for(int i= 0; i < word->len; ++i) {
    char c = word->a_word[i] + 'a';
    aos[*count][i] = c;
  }
  
  aos[*count][word->len] = 0;
  *count += 1;
}

// my_string(node, word, aos, count) returns pointers of the string in node
// effects: allocates memory (caller must free all strings and the array)
// time:    O(n^2), n is the number of characters in all of the words in node
void my_string(struct trienode *node, struct store *word,
               char **aos, int *count) {
  assert(node);
  if (node->word_end) {
    addstring(count, aos, word);
  }
  
  for (int i = 0; i < 26; ++i) {
    
    if(node->children[i] != NULL) {
      int *store_back_a = malloc(sizeof(int) * word->maxlen);
      int store_len = word->len;
      int store_maxlen = word->maxlen;
      for(int j = 0; j < word->maxlen; ++j) {
        store_back_a[j] = word->a_word[j];
      }
      
      insert_char(word, i);
      my_string(node->children[i], word, aos, count);
      free(word->a_word);
      
      word->a_word = store_back_a;
      word->len = store_len;
      word->maxlen = store_maxlen;
    }
  }
}

// trie_to_aos(t) generates a new array containing all of the words
//   in trie t in alphabetical order, each word is a new string
// notes:    returns NULL if t is empty
//           use trie_count(t) to determine the length of the array
// effects:  allocates memory (caller must free all strings and the array)
// time:     O(n^2), n is the number of characters in all of the words in t
char **trie_to_aos(const struct trie *t) {
  assert(t);
  if (t->root == NULL) return NULL;
  struct store *word = malloc(sizeof(struct store));
  
  word->len = 0;
  word->maxlen = 1;
  word->a_word = malloc(sizeof(int) * word->maxlen);
  
  int len = trie_count(t);
  char **aos = malloc(sizeof(char *) * len);
  int count = 0;
  my_string(t->root, word, aos , &count);
  
  free(word->a_word);
  free(word);
  return aos;
}
  
// trie_count(t) determines how many words are in t
// time: O(n), n is the number of characters in all of the words in t
int trie_count(const struct trie *t) {
  assert(t);
  if (t->root == NULL) return 0;
  int *len = malloc(sizeof(int));
  *len = 0;
  trev_end(t->root, len);
  int count = *len;
  free(len);
  return count;
}

// trie_print(t) prints each word in trie t in alphabetical order,
//   with each word on a newline
// effects: displays output (if t is not empty)
// time:    O(n^2), n is the number of characters in all of the words in t
void trie_print(const struct trie *t) {
  assert(t);
  if (t->root == NULL) return;
  char **strings = trie_to_aos(t);
  int len = trie_count(t);
  
  for (int i = 0; i < len; ++i) {
    printf("%s\n", strings[i]); 
  }
  
  for (int i = 0; i < len; ++i) {
    free(strings[i]); 
  }
  
  free(strings);
}
