#include "pool.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct used_info {
  int start;
  int end;
  struct used_info* next;
};

struct pool {
  char *blocks;
  int len;
  int active_len;
  struct used_info *used_info;
};

// TIME: n is the number of "active" allocations, which is the number
//   of successful allocs that have not been freed

// pool_create() creates a new pool of size characters
// effects: allocates memory (caller must call pool_destroy)
// time: O(1)
struct pool *pool_create(int size) {
  assert(size >0);
  struct pool *p = malloc(sizeof(struct pool));
  p->blocks = malloc(sizeof(char) * size);
  p->len = size;
  p->active_len = 0;
  p->used_info = NULL;
  return p;
}

// pool_destroy(p) destroys the pool p if there are no active 
//   allocations returns true if successful or false if there are 
//   active allocations
// effects:  the memory at p is invalid (freed) if successful
// time: O(1)
bool pool_destroy(struct pool *p) {
  assert(p);
  if (p->active_len == 0) {
    free(p->blocks);
    free(p->used_info);
    free(p);
    return true;
  }
  return false;
}

// new_info(start, end, next) createsa new node of used_info with 
//   start, end and next
// effect:// effects: allocates memory (caller must call free)
// time: O(1)
struct used_info *new_info(int start, int end, struct used_info *next){
  struct used_info *info = malloc(sizeof(struct used_info));
  info->start = start;
  info->end = end;
  info->next = next;
  return info;
}

// pool_alloc(p, size) returns a pointer to an uninitialized char array 
//   of size from within pool p, or NULL if no block of size is 
//   available
// effects: modifies p if successful
// time: O(n)
char *pool_alloc(struct pool *p, int size) {
  assert(p);
  assert(size >0);
  if (size > (p->len - p->active_len)) return NULL;
  struct used_info* info = p->used_info;
  if (info == NULL || size < info->start) {
    p->active_len += size;
    info = new_info(0, size - 1, p->used_info);
    p->used_info = info;
    return &(p->blocks[0]);
  }
  while(info->next) {
    if((info->next->start - info->end) > size) {
      p->active_len += size;
      struct used_info* old = info;
      info = new_info(old->end + 1, old->end + size, old->next);
      old->next = info;
      return &(p->blocks[info->start]);
    }
    info = info->next;
  }
  if (p->len - info->end > size) {
    p->active_len += size;
    struct used_info* old = info;
    info = new_info(old->end + 1, old->end + size, NULL);
    old->next = info;
    return &(p->blocks[info->start]);
  }
  return NULL;
}

// pool_free(p, addr) makes the active allocation at addr available in 
//   the pool.  returns true if successful (addr corresponds to an 
//   active allocation from a previous call to pool_alloc or 
//   pool_realloc) or false otherwise
// effects: modifies p if successful
// time: O(n)
bool pool_free(struct pool *p, char *addr) {
  assert(p);
  assert(addr);
  int pos = addr - p->blocks;
  if(p->len <= pos) return false;
  struct used_info* info = p->used_info;
  struct used_info* prev = NULL;
  while(info) {
    if(info->start <= pos && pos <= info->end) {
      if(prev == NULL) {
        p->used_info = info->next;
      } else {
        prev->next = info->next;
      }
      p->active_len -= info->end - info->start + 1;
      free(info);
      return true;
    }
    prev = info;
    info = info->next;
  }
  return false;
}

// pool_realloc(p, addr, size) changes the size of the active 
//   allocation at addr and returns the new address for the allocation.
//   returns NULL if addr does not correspond to an active allocation 
//   or the pool can not be resized (in which case the original 
//   allocation does not change)
// effects: modifies p if successful
// time: O(n) + O(k) where k is min(size, m) and 
//       m is the size of the original allocation
char *pool_realloc(struct pool *p, char *addr, int size) {
  assert(p);
  assert(addr);
  assert(size >0);
  int pos = addr - p->blocks;
  if(p-> len <= pos || pos < 0) return NULL;
  struct used_info* info = p->used_info;
  struct used_info* prev = NULL;
  while(info) {
    if(info->start <= pos && pos <= info->end) {
      if ((size <= info->end - info->start + 1) || 
          (info->next && info->next->start - info->start >= size) ||
          (!info->next && p->len - info->start >= size)) {
        p->active_len += (size - (info->end - info->start + 1));
        info->end = info->start + size - 1;
        return &(p->blocks[info->start]);
      }
      struct used_info* info2 = p->used_info;
      if (info2->start >= size) {
       pool_free(p, addr);
       return pool_alloc(p, size);
      }
      while (info2->next) {
        if((info2->next->start - info2->end) > size) {
          pool_free(p, addr);
          return pool_alloc(p, size);
        }
        info2 = info2->next;
      }
      if ((p->len - info2->end) > size) {
       pool_free(p, addr);
       return pool_alloc(p, size);
      }
    }
    prev = info;
    info = info->next;
  }
  return NULL;
}

// pool_print_active(p) prints out a description of the active 
//   allocations in pool p using the following format:
//   "active: index1 [size1], index2 [size2], ..., indexN [sizeN]\n" or
//   "active: none\n" if there are no active allocations where the 
//   index of an allocation is relative to the start of the pool
// effects: displays a message
// time: O(n)
void pool_print_active(struct pool *p) {
  assert(p);
  struct used_info* info = p->used_info;
  if (p->active_len == 0) {
    printf("active: none\n");
    return;
  }
  printf("active: ");
  while(info->next) {
    printf("%d [%d], ", info->start, info->end - info->start + 1);
    info = info->next;
  }
  printf("%d [%d]\n", info->start, info->end - info->start + 1);
}

// pool_print_available(p) prints out a description of the available 
//   continuous blocks of memory still available in pool p:
//   "available: index1 [size1], index2 [size2], ..., indexM [sizeM]\n" or
//   "available: none\n" if all of the pool has been allocated
// NOTE: It is impossible for two blocks of available memory to be adjacent.
//       If two blocks are adjacent, they should be merged to be a single,
//       larger block. In other words: index_K+1 > index_K + size_K
// effects: displays a message
// time: O(n)
void pool_print_available(struct pool *p) {
  assert(p);
  printf("available: ");
  if (p->len == p->active_len) {
    printf("none\n");
    return;
  }
  struct used_info* info = p->used_info;
  if(info == NULL) {
    printf("%d [%d]\n",0, p->len);
    return;
  }
  int ava_num = 0;
  if(info->start != 0) {
    printf("%d [%d]",0, info->start - 1);
    ava_num += info->start;
    if (ava_num + p->active_len < p->len) printf(", ");
  }
  while(info->next) {
    if(info->end !=info->next->start - 1) {
      printf("%d [%d]",info->end + 1, 
             info->next->start - info->end - 1);
      ava_num += info->next->start - info->end - 1;
      if (ava_num + p->active_len < p->len) printf(", ");
    }
    info = info->next;
  }
  if (info->end != p->len - 1) {
    printf("%d [%d]",info->end + 1, p->len - info->end - 1);
  }
  printf("\n");
}
