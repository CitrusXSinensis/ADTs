// A Queue ADT that has a maximum capcity. and this Queue
// only allow add item at the back and remove item from front.

struct cqueue;
typedef struct cqueue* CQueue;

// NOTE: All of the following function REQUIRE:
//       pointers to a cqueue are valid (NOT NULL)


// Make the queue dynamic sized, but only grow the size if
// size == max_len
// max_capacity in cqueue_create function serves as initial capacity

// cqueue_create() creates a new cqueue maximum size integers
// effects: allocates memory (caller must call cqueue_destroy)
// time: O(1)
CQueue cqueue_create(int max_capacity);

// cqueue_destroy() destroys the queue q
// effects: the memory at q is invalid(freed) after use
// time: O(1)
void cqueue_destroy(CQueue q);

// cqueue_add_front(q, item) adds item to the back of the Queue
// effects: the content of the q will be mutated
// time: O(1)
void cqueue_add_back(CQueue q, int item);

// cqueue_front(q) will produce the integer at the front of the Queue
// time: O(1)
int cqueue_front(CQueue q);

// cqueue_back(q) will produce the integer at the back of the Queue
// time: O(1)
int cqueue_back(CQueue q);

// cqueue_remove_front(q) will produce the integer at the front of the queue
// effects: the integer at the front of th queue will be deleted
// time: O(1)
int cqueue_remove_front(CQueue q);

// cqueue_size(q) will produce the total numbers of integer stored in queue
// time: O(1)
int cqueue_size(CQueue q);
