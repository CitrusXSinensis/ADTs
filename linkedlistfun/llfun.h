#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

struct node;

// Remove the ith node from the linked list.
// O(n)
void remove_ith(struct node** head, int i);

// Remove the node with maximum value from the linked list.
// You should only go through the list ONCE.
// O(n)
void remove_maximum(struct node** head);

// Remove all the nodes that does not meet <pred>
// O(n)
void filter(struct node** head, bool (*pred)(int));

// Produce true if the given linked list is sorted based on
// given compare function <compare_func>
// O(n)
bool is_sorted(struct node* head, bool(*compare_func)(int, int));

// Split the linked list into two parts
// left should store the address of the first half's head
// right should store the address of the second half's head
// O(n)
void split(struct node* head, struct node** left, struct node** right);
