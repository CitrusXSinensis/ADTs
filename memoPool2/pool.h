#ifndef POOL_H_
#define POOL_H_
#include <string>

struct Chunk {
    size_t length; // Length of this chunk.
    int *mem; // Array of length integers.
};
    
struct Pool {
    size_t capacity; // Capacity is the size of the chunks array.
    size_t size; // length is the number of elements in the chunks array.
    Chunk *chunks; // Array of chunks.
};

// Reads size_t's from cin, populating the pool with chunks of those
// sizes (you must allocate memory for each chunk). Once a non-integer
// input is read, the offending SINGLE character should be removed from
// the stream (using cin.ignore()) and cin's failbit should be cleared.
// The pool's capacity and size must match those as outlined in the 
// specification.
Pool readPool();

// Prints out the lengths of each chunk in the pool one by one.
// Seperated by spaces. There should be a space after each length 
// (including the last length), but not before the first.
void printPool(std::ostream &, const Pool &);

// Adds a new chunk to the end of the pool of size len. Creates a new
// chunk (allocating the memory for that chunk), and stores it after the
// last element in the chunks array. If there is not enough space in the 
// array, a new array with twice the memory is created, the old arrays 
// contents are copied over, and the chunk is then added after the last
// element. The old array must be freed!
void addChunk(Pool &, size_t);

// Finds the first chunk in chunks that is at LEAST of size len. 
// If such a chunk is found, it is returned by value,
// and that chunk is removed from the array (shifting all elements
// after that chunk over to the left).
// If no such chunk exists returns a chunk with length 0.
Chunk findFreeMem(Pool &, size_t);


// free's all heap allocated memory stored in the pool pointed to by p.
// Does not delete p itself as you do not know if the user stored
// that pool itself on the heap or not!
void freePool(Pool *p);


#endif
