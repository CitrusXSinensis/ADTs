#include <stdlib.h>
#include <assert.h>
#include "dictionary.h"

struct bstnode {
  void *item;                // key
  void *value;               // additional value
  struct bstnode *left;
  struct bstnode *right;
  int count;                 // *** new augmentation
};

struct dictionary {
  struct bstnode *root;
  // function pointers
  DictKeyCompare key_compare;
  FreeFunction free_key;
  FreeFunction free_val;
  PrintKeyVal print_keyval;  
};

const int PREORDER = -1;
const int INORDER = 0;
const int POSTORDER = 1;

/////////////////////////////

// dict_create(cmp_function, free_key_function, free_val_function,
//             print_function) creates a new dictionary
//   (see above function typedefs for parameter descriptions)
// effects: allocates memory (client must call dict_destroy)
Dictionary dict_create(DictKeyCompare cmp_function,
                       FreeFunction free_key_function,
                       FreeFunction free_val_function,
                       PrintKeyVal print_function) {
  Dictionary d = malloc(sizeof(struct dictionary));
  d->root = NULL;
  d->key_compare = cmp_function;
  d->free_key = free_key_function;
  d->free_val = free_val_function;
  d->print_keyval = print_function;
  return d;
}

// new_leaf(key, val) creates a new bstnode structure
// effects: allocates memory (client must call dict_destroy)
struct bstnode *new_leaf(void *key, void *val) {
  struct bstnode *leaf = malloc(sizeof(struct bstnode));
  leaf->item = key;
  leaf->value = val;
  leaf->left = NULL;
  leaf->right = NULL;
  leaf->count = 1;
  return leaf;
}
               
// insert_bstnode(key, val, node, d) inserts key/val pair into d
//   if key already exists then it frees both the old key and old value
// effects: modifies d, may free old key/value
// time:    O(h)
void insert_bstnode(void *key, void *val,
                    struct bstnode *node, Dictionary d) {
  if (!d->key_compare(key, node->item)) {
    d->free_val(node->value);
    d->free_key(node->item);
    node->value = val;
    node->item = key;
    struct bstnode *node = d->root;
    
    while (node) {
      if (!d->key_compare(key, node->item)) {
        return;
      }
      node->count -= 1;
      if (d->key_compare(key, node->item) < 0) {
        node = node->left;
      } else {
        node = node->right;
      }
    }
    
  } else if (d->key_compare(key, node->item) < 0) {
    node->count += 1;
    
    if (node->left) { 
      insert_bstnode(key, val, node->left, d); 
    } else {
      node->left = new_leaf(key, val); 
    }
    
  } else if (node->right) {
    node->count += 1;
    insert_bstnode(key, val, node->right, d); 
  } else {
    node->count += 1;
    node->right = new_leaf(key, val); 
  }
}
  
// dict_insert(key, val, d) inserts key/val pair into d
//   if key already exists then it frees both the old key and old value
// effects: modifies d, may free old key/value
// time:    O(h)
void dict_insert(void *key, void *val, Dictionary d) {
  assert(key);
  assert(val);
  assert(d);
  if (d->root) { 
    insert_bstnode(key, val, d->root, d); 
  } else { 
    d->root = new_leaf(key, val); 
  }
}

// dict_lookup(key, d) finds the value corresponding to the key in d
//   or returns NULL if key does not exist
// time:    O(h)
void *dict_lookup(void *key, Dictionary d) {
  assert(key);
  assert(d);
  struct bstnode *node = d->root;
  while (node) {
  
    if (!d->key_compare(key, node->item)) {
      return node->value;
    }
    
    if (d->key_compare(key, node->item) < 0) {
      node = node->left;
    } else {
      node = node->right;
    }
  } 
  return NULL;
}

// remove_bstnode(key, node, d) removes (and frees) the key and value in d
//   if the key does not exist, the dictionary does not change
// effects: modifies d, frees key/value
// time:    O(h)
struct bstnode *remove_bstnode(void *key, struct bstnode *node, Dictionary d) {
  if (node == NULL) return NULL;
  
  // finding node
  if (d->key_compare(key, node->item) < 0) {
    node->count -= 1;
    node->left = remove_bstnode(key, node->left, d); 
  } 
  
  else if (d->key_compare(key, node->item) > 0) {
    node->count -= 1;
    node->right = remove_bstnode(key, node->right, d); 
  } 
  
  // has at most 1 child
  else if (node->left == NULL) {
    struct bstnode *new_root = node->right;
    d->free_key(node->item);
    d->free_val(node->value); 
    free(node); 
    return new_root; 
  } 
  else if (node->right == NULL) {
    struct bstnode *new_root = node->left; 
    d->free_key(node->item);
    d->free_val(node->value); 
    free(node); 
    return new_root; 
    
  // has 2 children
  } else {
    struct bstnode *next = node->right; 
    struct bstnode *parent_of_next = NULL; 
    while (next->left) {
      next->count -= 1;
      parent_of_next = next; 
      next = next->left; 
    }
    free(node->value); 
    node->item = next->item; 
    node->value = next->value; 
    node->count -= 1;
    if (parent_of_next) { 
      parent_of_next->left = next->right; 
    } else {
      node->right = next->right; 
    } free(next); 
  } 
  return node;
} 

// dict_remove(key, d) removes (and frees) the key and value in d
//   if the key does not exist, the dictionary does not change
// effects: modifies d, frees key/value
// time:    O(h)
void dict_remove(void *key, Dictionary d) {
  assert(key);
  assert(d);
  d->root = remove_bstnode(key, d->root, d);
}

// print_helper(order, node) prints the dictionary in the given order
// notes: reveals the underlying BST structure
// requires: order is one of (PREORDER, INORDER, POSTORDER)
// effects:  displays output
// time:     O(n)
void print_helper(int order, struct bstnode *node, Dictionary d) {
  if (order == -1) {
    if(node == NULL) return;
    d->print_keyval(node->item, node->value);
    print_helper(order, node->left, d);
    print_helper(order, node->right, d);
  } else if (order == 0) {
    if(node == NULL) return;
    print_helper(order, node->left, d);
    d->print_keyval(node->item, node->value);
    print_helper(order, node->right, d);
  } else {
    if(node == NULL) return;
    print_helper(order, node->left, d);
    print_helper(order, node->right, d);
    d->print_keyval(node->item, node->value);
  }
}

// dict_print(order, d) prints the dictionary in the given order
// notes: reveals the underlying BST structure
//        if the dictionary is empty, prints nothing
// requires: order is one of (PREORDER, INORDER, POSTORDER)
// effects:  displays output
// time:     O(n)
void dict_print(int order, Dictionary d) {
  assert(d);
  assert(order == 0 || order == 1 || order == -1);
  if (d->root == NULL) return;
  print_helper(order, d->root, d);
}

// select_node(k, node) finds the key with index k in d from node
// requires: 0 <= k < node->count
// time:     O(h)
void *select_node(int k, struct bstnode *node) {
  int left_count = 0;
  if (node->left) left_count = node->left->count; 
  if (k < left_count) return select_node(k, node->left); 
  if (k == left_count) return node->item; 
  return select_node(k - left_count - 1, node->right); 
}

// dict_select(k, d) finds the key with index k in d
// requires: 0 <= k < dict_count(d)
// time:     O(h)
void *dict_select(int k, Dictionary d) {
  assert(k >= 0);
  assert(k < dict_count(d));
  return select_node(k, d->root);
}

// dict_count(d) returns the number of keys in d
// time: O(1)
int dict_count(Dictionary d) {
  assert(d);
  if (d->root == NULL) return 0;
  return d->root->count;
}


// free_bstnode(d) frees all memory of node and its children
// effects: node and its children are not valid, all keys and values are freed
// time:    O(n)
void free_bstnode(struct bstnode *node, Dictionary d) { 
  if (node) { 
    free_bstnode(node->left, d);
    free_bstnode(node->right, d);
    d->free_key(node->item);
    d->free_val(node->value);
    free(node);
  } 
}

// dict_destroy(d) frees all memory for d
// effects: d is no longer valid, all keys and values are freed
// time:    O(n)
void dict_destroy(Dictionary d) {
  free_bstnode(d->root, d); 
  free(d);
}
