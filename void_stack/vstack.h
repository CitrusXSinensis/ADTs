struct vstack;

// FreeFunction(obj) frees all memory associated with obj
typedef void (*DestroyFunction) (void* obj);

// PrintKeyVal(value) prints out the value
typedef void (*PrintValueFunction) (void* value);

// Creates a new stack.
// effects: allocates memory (client must call stack_destroy)
struct vstack* stack_create(DestroyFunction df,
                         PrintValueFunction pf);

// Destroy the stack and all the items, all the items memory
// will also be freed
// effects: memory will be freed, stk will be invalid after use
void stack_destroy(struct vstack* stk);

// Push a new item into the stack
// the item must be allocated in heap
// effects: modifies stk's content
void stack_push(struct vstack* stk, void* item);

// Produce the last item pushed into the stk
void* stack_peek(struct vstack* stk);

// Remove and produce the last item pushed into the stack
// Caller must free the produced value
// effects: modifies stk's content
void* stack_pop(struct vstack* stk);

// Produce the size of the stack.
int stack_size(struct vstack* stk);
