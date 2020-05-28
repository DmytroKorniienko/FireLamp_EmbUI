// C++ for Arduino
// What is heap fragmentation?
// https://cpp4arduino.com/

// This source file captures the platform dependent code.
// This version was tested with ESP8266 core for Arduino version 2.4.2

#include <umm_malloc/umm_malloc.h>

const size_t block_size = 8;

size_t getTotalAvailableMemory() {
  umm_info(0, 0);
  return ummHeapInfo.freeBlocks * block_size;
}

size_t getLargestAvailableBlock() {
  umm_info(0, 0);
  return ummHeapInfo.maxFreeContiguousBlocks * block_size;
}
