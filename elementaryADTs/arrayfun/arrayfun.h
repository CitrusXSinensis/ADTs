#include <stdbool.h>
#include <string.h>

// Produce total number of occurences of <target> in
// string <str>
int count_char(char* str, char target);

// Produce total number of occurences of <target> in
// int array <lon> with size <len>
int count_int(int* lon, int len, int target);

// Produce a newly allocated string in heap that is an
// concatenation of string <str1> and string <str2>.
char* my_strcat(char* str1, char* str2);

// Produce a newly allocated string in heap that is an
// concatenation of all the strings in the array <los> with
// size <len>
char* connect_all(char** los, int len);

// Produce a newly allocated string in heap that contains
// any characters from string <str> that is not white space
// characters should have the original order.
char* remove_white(char* str);

// Remove those item that does not met the predicate <pred>
// and you may need to reallocate the given array so it only use
// enough memory to hold all the kept items.
// Pre: arr is a pointer to an integer array in heap
int filter(int **arr, int len, bool (*pred) (int));

// Produces true if given string <str> is a palindrome
bool is_palindrome(char* str);

// Put everything from <arr1> and <arr2> into <destination>
// integers in destination must be sorted in increasing order
// Pre: arr1 is sorted in increasing order
//      arr2 is sorted in increasing order
//      length of destination is at least len1 + len2
void merge_array(int* destination, int* arr1, int* arr2, int len1, int len2);

// Produce the largest item in <arr> with given length len.
// the <compare_func> produce true if first argument is smaller than second
// false otherise
void* find_max(void** arr, int len, bool(*compare_func)(void*, void*));
