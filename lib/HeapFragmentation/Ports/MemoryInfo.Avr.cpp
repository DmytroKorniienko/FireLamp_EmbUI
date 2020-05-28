// C++ for Arduino
// What is heap fragmentation?
// https://cpp4arduino.com/

// This source file captures the platform dependent code.
// This version was tested with the AVR Core version 1.6.22

// This code is freely inspired from https://github.com/McNeight/MemoryFree

#include <stddef.h>

// This heap allocator defines this structure to keep track of free blocks.
struct block_t {
  size_t sz;
  struct block_t *nx;
};

// NOTE. The following extern variables are defined in malloc.c in avr-stdlib

// A pointer to the first block
extern struct block_t *__flp;

// A pointer to the end of the heap, initialized at first malloc()
extern char *__brkval;

// A pointer to the beginning of the heap
extern char *__malloc_heap_start;

static size_t getBlockSize(struct block_t *block) {
  return block->sz + 2;
}

static size_t getUnusedBytes() {
  char foo;
  if (__brkval) {
    return size_t(&foo - __brkval);
  } else {
    return size_t(&foo - __malloc_heap_start);
  }
}

size_t getTotalAvailableMemory() {
  size_t sum = getUnusedBytes();
  for (struct block_t *block = __flp; block; block = block->nx) {
    sum += getBlockSize(block);
  }
  return sum;
}

size_t getLargestAvailableBlock() {
  size_t largest = getUnusedBytes();
  for (struct block_t *block = __flp; block; block = block->nx) {
    size_t size = getBlockSize(block);
    if (size > largest) {
      largest = size;
    }
  }
  return largest;
}
