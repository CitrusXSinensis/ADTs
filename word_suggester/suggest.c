#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "suggest.h"

// str_delete(s, index, len) return a new pointer of string which delete the 
//   char of s at index
// requires: s is a valid non-empty string in lowercase
//           len >index >= 0
// effects:  allocate memory (client must call free)
// time: O(n), where n is the len
char *str_delete(const char *s, int index, int len) {
  assert(s);
  assert(index >= 0);
  assert(index < len);
  char *word = malloc(sizeof(char) * (len));
  const char *p = s;
  for(int i = 0; i < len - 1; ++i) {
    if (i == index) {
      ++p;
      word[i] = *p;
      ++p;
    }
    else {
      word[i] = *p;
      ++p;
    }
  }
  word[len - 1] = 0;
  return word;
}

// str_insert(s, index, c, len) return a new pointer of string which insert the 
//   char c at index of the new string
// requires: s is a valid non-empty string in lowercase
//           len + 1 >index >= 0
//           'a' <= c <= 'z'
// effects:  allocate memory (client must call free)
// time: O(n), where n is the len
char *str_insert(const char *s, int index, char c, int len) {
  assert(s);
  assert(index >= 0);
  assert(index < len + 1);
  assert(c >= 'a');
  assert(c <= 'z');
  char *word = malloc(sizeof(char) * (len + 2));
  const char *p = s;
  for(int i = 0; i < len + 1; ++i) {
    if (i == index) {
      word[i] = c;
    }
    else {
      word[i] = *p;
      ++p;
    }
  }
  word[len + 1] = 0;
  return word;
}

// str_sub(s, index, c, len) return a new pointer of string which substitute 
//   char c with the original char at index of s
// requires: s is a valid non-empty string in lowercase
//           len >index >= 0
//           'a' <= c <= 'z'
// effects:  allocate memory (client must call free)
// time: O(n), where n is the len
char *str_sub(const char *s, int index, char c, int len) {
  assert(s);
  assert(index >= 0);
  assert(index < len);
  assert(c >= 'a');
  assert(c <= 'z');
  char *word = malloc(sizeof(char) * (1 + len));
  const char *p = s;
  for(int i = 0; i < len; ++i) {
    if (i == index) {
      word[i] = c;
      ++p;
    }
    else {
      word[i] = *p;
      ++p;
    }
  }
  word[len] = 0;
  return word;
}

// str_swap(s, index, c, len) return a new pointer of string which swap 
//   the position of two char at index and index + 1
// requires: s is a valid non-empty string in lowercase
//           len - 1 >index >= 0
//           'a' <= c <= 'z'
// effects:  allocate memory (client must call free)
// time: O(n), where n is the len
char *str_swap(const char *s, int index, int len) {
  assert(s);
  assert(index >= 0);
  assert(index < len - 1);
  char *word = malloc(sizeof(char) * (1 + len));
  const char *p = s;
  for(int i = 0; i < len; ++i) {
    if (i == index) {
      char temp = *p;
      ++p;
      word[i] = *p;
      ++i;
      word[i] = temp;
      ++p;
    }
    else {
      word[i] = *p;
      ++p;
    }
  }
  word[len] = 0;
  return word;
}


// suggest(word, is_word) returns a new wordlist containing
//   correctly spelled words (according to is_word) that have
//   an edit distance of one from word (see assignment)
//   or returns NULL if there are no suggested words
// note:     does not suggest "" or word itself
// requires: word is a valid non-empty string in lowercase
// effects:  may create a new wordlist (client calls wordlist_destroy)
// time: O(n * g(m)), where n is the length of s, and g(m) is the efficiency of
//   is_word(s)
struct wordlist *suggest(const char *s,
                         bool (*is_word)(const char *)) {
  assert(s);
  assert(is_word);
  struct wordlist *wl = wordlist_create();
  int len = strlen(s);
  assert(len > 0);
  
  for(int i = 0; i < len; ++i) {
    char *s_word = str_delete(s, i, len);
    if(is_word(s_word) && strcmp("", s_word) && strcmp(s, s_word)) {
      wordlist_add(s_word, wl);
    }
    free(s_word);
  }
  
  for(int i = 0; i < len + 1; ++i) {
    for (char c = 'a'; c <= 'z'; ++c) {
      char *s_word = str_insert(s, i, c, len);
      if(is_word(s_word) && strcmp("", s_word) && strcmp(s, s_word)) {
      wordlist_add(s_word, wl);
      }
      free(s_word);
    }
  }
  
  for(int i = 0; i < len; ++i) {
    for (char c = 'a'; c <= 'z'; ++c) {
      char *s_word = str_sub(s, i, c, len);
      if(is_word(s_word) && strcmp("", s_word) && strcmp(s, s_word)) {
      wordlist_add(s_word, wl);
      }
      free(s_word);
    }
  }
  
  for(int i = 0; i < len - 1; ++i) {
    char *s_word = str_swap(s, i, len);
    if(is_word(s_word) && strcmp("", s_word) && strcmp(s, s_word)) {
      wordlist_add(s_word, wl);
    }
    free(s_word);
  }
  

  if (wordlist_count(wl) == 0) 
  {wordlist_destroy(wl);
   return NULL;
  }
  return wl;
}
