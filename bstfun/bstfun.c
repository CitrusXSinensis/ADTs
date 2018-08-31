#include "bstfun.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

struct bstnode {
  int item;
  struct bstnode* left;
  struct bstnode* right;
};

struct bst {
  struct bstnode* root;
};

/* forward declaration */
int bst_size(struct bst* t);

struct bst* bst_create(void) {
  struct bst* b = malloc(sizeof(struct bst));
  b->root = NULL;
  return b;
}

void free_bst(struct bstnode* node) {
  if(node == NULL) {
    return;
  }
  free_bst(node->left);
  free_bst(node->right);
  free(node);
}

void bst_destroy(struct bst* t)
{
  free_bst(t->root);
  free(t);
}

bool bst_insert(struct bst* b, int key) {
  struct bstnode *new = malloc(sizeof (struct bstnode));
  new->item = key;
  new->left = NULL;
  new->right = NULL;
  if (b->root == NULL) {
    b->root  = new;
    return true;
  }
  struct bstnode *node = b->root;
  while(node) {
    if (key == node->item) {
      free(new);
      return false;
    } else if (key < node->item) {
      if (node->left == NULL) {
        node->left = new;
        return true;
      } else {
        node = node->left;
      }
    } else {
      if (node->right == NULL) {
        node->right = new;
        return true;
      } else {
        node = node->right;
      }
    }
  }
  node = new;
  return true;
}

void bst_to_arr_h(int* a, struct bstnode* node, int* index) {
  if (node) {
    bst_to_arr_h(a, node->left, index);
    a[*index] = node->item;
    (*index) += 1;
    bst_to_arr_h(a, node->right, index);
  }
}

int *bst_to_sorted_array(struct bst* t) {
  int index = 0;
  int *arr = malloc(sizeof(int) * bst_size(t));
  bst_to_arr_h(arr, t->root, &index);
  return arr;
}

// helper function for print out the bst horizontally
void bst_print(struct bstnode* node, int indent) {
  if(node == NULL) {
    return;
  }
  bst_print(node->right, indent + 2);
  for(int i = 0; i < indent; i++) {
    printf(" ");
  }
  printf("%d\n", node->item);
  bst_print(node->left, indent + 2);
}

int bst_size_helper(struct bstnode* curr) {
  if(!curr) {
    return 0;
  }
  return 1 + bst_size_helper(curr->left)
           + bst_size_helper(curr->right);
}

int bst_size(struct bst* t) {
  return bst_size_helper(t->root);
}

void bst_sort_array(int* nums, int len) {
  struct bst *b = bst_create();
  for (int i = 0; i <len; ++i) {
    bst_insert(b, nums[i]);
  }
  int *a;
  a = bst_to_sorted_array(b);
  for (int i = 0; i <len; ++i) {
    nums[i] = a[i];
  }
  bst_destroy(b);
  free(a);
}

int* bst_remove_dup(int* nums, int len, int* newlen) {
  struct bst *b = bst_create();
  *newlen = len;
  for (int i = 0; i < len; ++i) {
    if(!bst_insert(b, nums[i])) {
      *newlen -= 1;
    }
  }
  int *new = bst_to_sorted_array(b);
  free_bst(b->root);
  free(b);
  return new;
}

void sum_h(struct bstnode* node, int num, int *sum) {
  if (node) {
    if (node->item > num) *sum += node->item;
    sum_h(node->left, num, sum);
    sum_h(node->right, num, sum);
  }
}

int bst_sum_greater(struct bst* t, int threshold) {
  int sum = 0;
  sum_h(t->root, threshold, &sum);
  return sum;
}

void bst_stats_h(struct bstnode* node, int* height, int* nodes, int* leaves, int h) {
  if (node) {
    *nodes += 1;
    if (node->left == NULL && node->right == NULL) {
      (*leaves) += 1;
      if (h > *height) {
        *height = h;
      }
      return;
    }
    bst_stats_h(node->left, height, nodes, leaves, h + 1);
    bst_stats_h(node->right, height, nodes, leaves, h + 1);
  }
}

void bst_stats(struct bst* t, int* height, int* nodes, int* leaves) {
  *height = 1;
  *nodes = 0;
  *leaves = 0;
  if(t->root == NULL) {
    *height = 0;
    return;
  }
  bst_stats_h(t->root, height, nodes, leaves, 1);
}

int main(void) {
  struct bst* bt = bst_create();;
  assert(bst_size(bt) == 0);
  bst_insert(bt, 5);
  assert(bst_size(bt) == 1);
  bst_insert(bt, 8);
  bst_insert(bt, 7);
  bst_insert(bt, 9);
  assert(bst_size(bt) == 4);
  bst_insert(bt, 1);
  bst_insert(bt, 2);
  bst_insert(bt, 0);

  printf("This is da tree\n");
  bst_print(bt->root, 0);

  assert(bst_size(bt) == 7);
  printf("bst_size passed!\n");

  int arr[] = {3, 7, 1, 6, 4, 5, 2, 8};
  bst_sort_array(arr, 8);
  assert(arr[0] == 1);
  assert(arr[1] == 2);
  assert(arr[2] == 3);
  assert(arr[3] == 4);
  assert(arr[4] == 5);
  assert(arr[5] == 6);
  assert(arr[6] == 7);
  assert(arr[7] == 8);

  printf("bst_sort_array passed!\n");

  int arr2[] = {1, 1, 1, 1};
  int newlen = 0;
  int* newarr = bst_remove_dup(arr2, 4, &newlen);
  assert(newlen == 1);
  assert(newarr[0] == 1);
  free(newarr);

  int arr3[] = {1, 2, 1, 3, 2, 1, 3, 2, 1};
  newarr = bst_remove_dup(arr3, 9, &newlen);
  assert(newlen == 3);
  free(newarr);

  printf("bst_remove_dup passed!\n");

  assert(bst_sum_greater(bt, 0) == 32);
  printf("%d\n", bst_sum_greater(bt, 2));
  assert(bst_sum_greater(bt, 1) == 31);
  assert(bst_sum_greater(bt, 2) == 29);
  assert(bst_sum_greater(bt, 5) == 24);
  assert(bst_sum_greater(bt, 7) == 17);
  assert(bst_sum_greater(bt, 8) == 9);
  assert(bst_sum_greater(bt, 9) == 0);

  printf("bst_sum_greater passed!\n");

  int height;
  int nodes;
  int leaves;

  bst_stats(bt, &height, &nodes, &leaves);
  assert(height == 3);
  assert(nodes == 7);
  assert(leaves == 4);

  bst_insert(bt, 15);
  bst_stats(bt, &height, &nodes, &leaves);
  assert(height == 4);
  assert(nodes == 8);
  assert(leaves == 4);

  bst_insert(bt, 3);
  bst_stats(bt, &height, &nodes, &leaves);
  assert(height == 4);
  assert(nodes == 9);
  assert(leaves == 4);

  printf("bst_stats passed\n");

  printf("Congraz, you mastered BINARY TREE\n");
  bst_destroy(bt);
}
