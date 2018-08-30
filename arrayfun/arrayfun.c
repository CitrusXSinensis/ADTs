#include "arrayfun.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// Produce total number of occurences of <target> in
// string <str>
int count_char(char* str, char target) {
  int i = 0;
  int count = 0;
  while(str[i]) {
    if (str[i] == target) ++count;
    ++i;
  }
  return count;
}

int count_int(int* lon, int len, int target) {
  int count = 0;
  for (int i = 0; i < len; ++i) {
    if (lon[i] == target) ++count;
  }
  return count;
}

char* my_strcat(char* str1, char* str2) {
  int len1 = strlen(str1);
  int len2 = strlen(str2);
  char *s = malloc(sizeof(char) * (len1 + len2 + 1));
  strcpy(s, str1);
  strcpy(s + len1, str2);
  return s;
}

char* remove_white(char* str) {
  int len = strlen(str);
  int count = 0;
  char* s = malloc(sizeof(char) *(1 + len));
  for (int i = 0; i < len; ++i) {
    if (str[i] != ' ') {
      s[count] = str[i];
      ++count;
    }
  }
  s[count] = 0;
  s = realloc(s, sizeof(char) * (1 + count));
  return s;
}

char* connect_all(char** los, int len) {
  char* s = malloc(sizeof(char) * (1 + strlen(los[0])));
  strcpy(s, los[0]);
  int pos = strlen(los[0]);
  for (int i = 1; i <len; ++i) {
    s = realloc(s, (pos + strlen(los[i])) * sizeof(char));
    strcat(s, los[i]);
    pos += strlen(los[i]);
  }
  return s;
}

int filter(int **arr, int len, bool (*pred) (int)) {
  int count = 0;
  int *arr1 = *arr;
  for (int i = 0; i < len; ++i) {
    if (pred(arr1[i])) {
      arr1[count] = arr1[i];
      ++count;
    }
  }
  *arr = realloc(arr1, count * sizeof (int));
  return count;
}

bool is_palindrome(char* str) {
  int len = strlen(str);
  for (int i = 0; i < len / 2; ++i) {
    if (str[i] != str[len - 1 - i]) return false;
  }
  return true;
}

void merge_array(int* destination, int* arr1, int* arr2, int len1, int len2) {
  int pos1 = 0;
  int pos2 = 0;
  for (int i = 0; i < len1 + len2; ++i) {
    if (pos1 == len1 || (pos2 < len2 && arr2[pos2] < arr1[pos1])) {
      destination[pos1 + pos2] = arr2[pos2];
      ++pos2;
    } else {
      destination[pos1 + pos2] = arr1[pos1];
      ++pos1;
    }
  }
}

void* find_max(void** arr, int len, bool(*compare_func)(void*, void*)) {
  void *temp = arr[0];
  for (int i = 0; i < len; ++i) {
    if (compare_func(temp, arr[i])) temp = arr[i];
  }
  return temp;
}

void* find_max2(void* arr, int len, int msize, bool(*compare_func)(void*, void*)) {
  void* temp = arr;
  for(int i = 0; i<len; ++i) {
    if(compare_func(temp, arr + i * msize)) temp = arr + i * msize;
  }
  return temp;
}

/* for testing filter/find_max */

bool is_even(int n) {
  return n % 2 == 0;
}

bool is_greater_4(int n) {
  return n > 4;
}

bool is_smaller(void* n, void* m) {
  int* a = (int*) n;
  int* b = (int*) m;
  return *a < *b;
}

bool is_greater(void* n, void* m) {
  int* a = (int*) n;
  int* b = (int*) m;
  return *a > *b;
}

/* tests */

int main() {
  /* tests for count_char */
  assert(count_char("This is a pretty long string you know?", 's') == 3);
  assert(count_char("eagle", 'e') == 2);
  assert(count_char("eagle", 'a') == 1);
  assert(count_char("eagle", 'z') == 0);
  printf("count_char passed\n");

  /* tests for count_int */

  int arr[] = {1, 2, 3, 1, 2, 3, 1};
  assert(count_int(arr, 7, 1) == 3);
  assert(count_int(arr, 7, 2) == 2);
  assert(count_int(arr, 7, 3) == 2);
  printf("count_int passed\n");

  /* tests for my_strcat */

  char* srv = my_strcat("a", "b");
  assert(strcmp(srv, "ab") == 0);
  free(srv);

  srv = my_strcat("", "b");
  assert(strcmp(srv, "b") == 0);
  free(srv);

  srv = my_strcat("a", "");
  assert(strcmp(srv, "a") == 0);
  free(srv);

  srv = my_strcat("eagle", "sogood");
  assert(strcmp(srv, "eaglesogood") == 0);
  free(srv);

  printf("my_strcat passed\n");

  /* tests for remove_white */

  srv = remove_white("nowhite");
  assert(strcmp(srv, "nowhite") == 0);
  free(srv);

  srv = remove_white("n o w h     i te");
  assert(strcmp(srv, "nowhite") == 0);
  free(srv);

  printf("remove_white passed\n");

  /* tests for connect_all */

  char* ss[] = {"e", "agle", "ru", "le"};
  srv = connect_all(ss, 4);
  assert(strcmp(srv, "eaglerule") == 0);
  free(srv);

  printf("connect_all passed\n");

  /* tests for filter */

  int* ints = (int*)malloc(sizeof(int) * 10);
  for(int i = 0; i < 10; i++) {
    ints[i] = i;
  }
  int nlen = filter(&ints, 10, is_even);
  assert(nlen == 5);
  assert(ints[0] == 0);
  assert(ints[1] == 2);
  assert(ints[2] == 4);
  assert(ints[3] == 6);
  assert(ints[4] == 8);

  nlen = filter(&ints, 5, is_greater_4);
  assert(nlen == 2);
  assert(ints[0] == 6);
  assert(ints[1] == 8);
  free(ints);

  printf("filter passed\n");

  /* tests for is_plalindrome */

  assert(is_palindrome("aba") == true);
  assert(is_palindrome("abba") == true);
  assert(is_palindrome("abcba") == true);
  assert(is_palindrome("a") == true);
  assert(is_palindrome("aBA") == false);
  assert(is_palindrome("abc") == false);
  assert(is_palindrome("abbbaa") == false);

  printf("is_plalindrome passed\n");

  /* tests for merge_array */

  int narr1[] = {2, 4, 6, 9};
  int narr2[] = {1, 3, 5};
  int* dest = malloc(sizeof(int) * 7);
  merge_array(dest, narr1, narr2, 4, 3);
  assert(dest[0] == 1);
  assert(dest[1] == 2);
  assert(dest[2] == 3);
  assert(dest[3] == 4);
  assert(dest[4] == 5);
  assert(dest[5] == 6);
  assert(dest[6] == 9);
  merge_array(dest, narr2, narr1, 3, 4);
  assert(dest[0] == 1);
  assert(dest[1] == 2);
  assert(dest[2] == 3);
  assert(dest[3] == 4);
  assert(dest[4] == 5);
  assert(dest[5] == 6);
  assert(dest[6] == 9);
  free(dest);

  printf("merge_array passed\n");

  /* tests for find_max */
  int n1 = 1;
  int n2 = 2;
  int n3 = 3;
  void** varr = malloc(sizeof(void*) * 3);
  varr[0] = &n1;
  varr[1] = &n2;
  varr[2] = &n3;
  assert(*((int*)find_max(varr, 3, is_greater)) == 1);
  assert(*((int*)find_max(varr, 3, is_smaller)) == 3);
  free(varr);
  printf("find_max passed\nyou are the BOSS!\n");

  int* finalarr = malloc(sizeof(int) * 5);
  finalarr[0] = 4;
  finalarr[1] = 8;
  finalarr[2] = 1;
  finalarr[3] = 6;
  finalarr[4] = 7;
  int* found = find_max2(finalarr, 5, sizeof(int), is_greater);
  assert(*found == 1);
  found = find_max2(finalarr, 5, sizeof(int), is_smaller);
  assert(*found == 8);
  free(finalarr);
}
