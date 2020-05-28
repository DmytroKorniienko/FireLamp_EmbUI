// C++ for Arduino
// What is heap fragmentation?
// https://cpp4arduino.com/

// This program uses several String instances of random size.
// Over time, the Strings produces heap fragmentation.
// The program prints the value of the heap fragmentation.
// You can use the Serial Plotter to see the curve.

#include "MemoryInfo.h"

// Try to change these values and observe the evolution of the fragmentation.
// As you'll see, fragmentation disappears if SMALLEST_STRING == LARGEST_STRING,
// i.e. if the size of the strings is constant.
const size_t NUMBER_OF_STRINGS = 20;
const size_t MAX_MEM_USAGE = getTotalAvailableMemory() * 3 / 4; // 75% of RAM
const size_t LARGEST_STRING = MAX_MEM_USAGE / NUMBER_OF_STRINGS;
const size_t SMALLEST_STRING = LARGEST_STRING / 5; // varies from 1x to 5x

// The collection of string.
// They allocate and release memory from the heap. We could have called malloc()
// and free() manually but using String is simpler.
String strings[NUMBER_OF_STRINGS];

// At program startup, initialize the serial port.
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    continue;
  }
}

// At each iteration, get new values for each string and print the value.
void loop() {
  for (String &s : strings) {
    // Replace each string with a new random one
    s = generateRandomString();
  }

  Serial.print(getTotalAvailableMemory());
  Serial.print(' ');
  Serial.print(getLargestAvailableBlock());
  Serial.print(' ');
  Serial.print(getFragmentation());
  Serial.println();
}

// Generates a string whose length is picked randomly between SMALLEST_STRING
// and LARGEST_STRING
String generateRandomString() {
  String result;
  int len = random(SMALLEST_STRING, LARGEST_STRING);
  while (len--) {
    result += '?';
  }
  return result;
}
