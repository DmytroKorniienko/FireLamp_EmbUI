// C++ for Arduino
// What is heap fragmentation?
// https://cpp4arduino.com/

#include "MemoryInfo.h"

#if defined(ARDUINO_ARCH_AVR)
#include "Ports/MemoryInfo.Avr.cpp"
#elif defined(ARDUINO_ARCH_ESP8266)
#include "Ports/MemoryInfo.Esp8266.cpp"
#else
#error Your microcontroller architecture is not supported
#endif