#include "desll.h"
#include "cs136-trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// a double ended structure use linked list
struct dllnode {
  int item;
  struct dllnode *prev;
  struct dllnode *next;
};

// DO NOT MODIFY THIS STRUCTURE
struct destack {
  struct dllnode *top;
  struct dllnode *bot;
};

// destack_create() creates a new empty double ended stack
// effects: allocates memory (client must call destack_destroy)
// time: O(1)
struct destack *destack_create(void) {
  struct destack *s = malloc(sizeof(struct destack)); 
  s->top = NULL; 
  s->bot = NULL; 
  return s;

}

// destack_is_empty(s) determines if the double ended stack s is empty
// time: O(1)
bool destack_is_empty(const struct destack *s) {
  assert(s);
  return (s->top == NULL && s->bot == NULL);
}

// destack_top(s) returns the top item in the double ended stack s
// requires: s is not empty
// time: O(1)
int destack_top(const struct destack *s) {
  assert(s);
  assert(!destack_is_empty(s));
  return s->top->item;
}

// destack_bot(s) returns the bottom item in the double ended stack s
// requires: s is not empty
// time: O(1)
int destack_bot(const struct destack *s) {
  assert(s);
  assert(!destack_is_empty(s));
  return s->bot->item;
}

// destack_pop_top(s) returns and removes the top item from the double ended stack s
// requires: s is not empty
// effects: modifies s
// time: O(1)
int destack_pop_top(struct destack *s) {
  assert(s);
  assert(!destack_is_empty(s));
  int retval = s->top->item;
  struct dllnode *old_top = s->top;
  s->top = s->top->next;
  free(old_top);
  if(s->top == NULL) { 
    s->bot = NULL; 
  } else {
    s->top->prev = NULL;
  }
  return retval;
}

// destack_pop_bot(s) returns and removes the bottom item from the double ended stack s
// requires: s is not empty
// effects: modifies s
// time: O(1)
int destack_pop_bot(struct destack *s) {
  assert(s);
  assert(!destack_is_empty(s));
  int retval = s->bot->item; 
  struct dllnode *old_bot = s->bot;
  s->bot = s->bot->prev;
  free(old_bot);
  if (s->bot == NULL) { 
    s->top = NULL; 
  } else {
    s->bot->next = NULL;
  }
  return retval;
}

// new_node(pprev, i, pnext) creates a new double ended node
// effects: allocates memory (client must call free the meomry)
// time: O(1)
struct dllnode *new_node(struct dllnode *pprev, int i, struct dllnode *pnext) {
  struct dllnode *node = malloc(sizeof(struct dllnode)); 
  node->prev = pprev; 
  node->item = i; 
  node->next = pnext; 
  return node; 
}

// destack_push_top(item, s) adds item to the top of the double ended stack s
// effects: modifies s
// time: O(1)
void destack_push_top(int item, struct destack *s) {
  assert(s);
  struct dllnode *node = new_node(NULL,item,s->top); 
  if (s->bot == NULL) { 
    s->bot = node; 
  } else {
    s->top->prev = node;
  } 
  s->top = node;
}

// destack_push_bot(item, s) adds item to the bottom of the double ended stack s
// effects: modifies s
// time: O(1)
void destack_push_bot(int item, struct destack *s){
  assert(s);
  struct dllnode *node = new_node(s->bot,item, NULL); 
  if (s->top == NULL) { 
    s->top = node; 
  } else {
    s->bot->next = node;
  } 
  s->bot = node;
}

// destack_destroy(s) frees all memory for the double ended stack s
// requires: s must be from a previous malloc
// effects: s is no longer valid
// time: O(n)
void destack_destroy(struct destack *s) {
  assert(s);
  while(!destack_is_empty(s)) {
    destack_pop_top(s);
  }
  free(s);
}

// destack_print(s) prints the contents of the double ended stack s
// effects: prints to output
// time: O(n)
void destack_print(const struct destack *s) {
  assert(s);
  struct dllnode *temp = s->bot;
  
  printf("Destack:\n");
  printf("Bot -> ");
  while (temp) {
    printf("%d -> ", temp->item);
    temp = temp->next;
  }
  printf("NULL\n");
  
  temp = s->top;
  printf("Top -> ");
  while (temp) {
    printf("%d -> ", temp->item);
    temp = temp->prev;
  }
  printf("NULL\n");
}
