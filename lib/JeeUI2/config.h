#ifndef config_h
#define config_h

#ifdef ESP8266
#include <FS.h>
#else
#include "SPIFFS.h"
#endif

#endif