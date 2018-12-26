
#include <stdbool.h>

// spellcheck interface

// Clients are responsible to build own dictionary for different usages
// For common use, a modified version of a list generated at: 
// http://app.aspell.net/create   could be a good choice

// spellcheck(word) determines if word is spelled correctly
// requires: word is a valid string of only lowercase letters
// time:     O(n), where n is the length of word
bool spellcheck(const char *word);
