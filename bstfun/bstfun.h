#include <stdbool.h>

// each node in bst
struct bstnode;

// wrapper structure for a binary search tree
struct bst;

// Create a newly allocated empty binary search tree.
struct bst* bst_create(void);

// Produce the size of a binary seach tree
int bst_size(struct bst* t);

// Destroy a binary search tree.
void bst_destroy(struct bst* t);

// Insert a key into the binary search tree, if the key already exist,
// the function should produce false, otherwise the tree is be mutated
// and true is produced. the resulting tree should still be a binary
// search tree.
// Must be O(logn)
bool bst_insert(struct bst* b, int key);

// Given a sorted array of integers with size <len>, produce a
// newly allocated balanced binary search tree using integers from array.
// O(n)
struct bst* sorted_array_to_bst(int *a, int len);

// Given a binary search tree, produce a newly allocated integer arrays
// that is sorted in increasing order using all the elements from bst.
// O(n)
int *bst_to_sorted_array(struct bst* t);

// Produce the size of the given bst.
// O(n)
int bst_size(struct bst* t);

// Sort the array of integers by using an binary search tree.
// O(nlogn)
void bst_sort_array(int* nums, int len);

// Produce a newly allocated integer array, the array should not contain
// any duplicate integer and new size should be set using newlen.
// O(nlogn)
int* bst_remove_dup(int* nums, int len, int* newlen);

// Produce the sum of all the integers that are greater threshold.
// O(n)
int bst_sum_greater(struct bst* t, int threshold);

// Given a binary search tree. mutate height, nodes, leaves so that
// those pointers points to the height of the binary search tree, the total
// number of nodes in the binary search tree, and total number of leaves in
// the binary search tree.
// O(n)
void bst_stats(struct bst* t, int* height, int* nodes, int* leaves);
