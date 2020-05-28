What is heap fragmentation?
===========================

An Arduino sketch that demonstrate the increase of heap fragmentation.

[Read the articule on cpp4arduino.com](https://cpp4arduino.com/2018/11/06/what-is-heap-fragmentation.html)

### Supported platforms

Because it inspects the content of the heap, this program highly depends on
the "core" of the board.

Only the following cores are supported:

* [The official Arduino AVR core](https://github.com/arduino/ArduinoCore-avr) (bundled with the IDE):
    - Adafruit Circuit Playground
    - Arduino BT
    - Arduino Duemilanove or Diecimila
    - Arduino Esplora
    - Arduino Ethernet
    - Arduino Fio
    - Arduino Gemma
    - Arduino Industrial 101
    - Arduino Leonardo
    - Arduino Leonardo ETH
    - Arduino Mega ADK
    - Arduino Mini
    - Arduino Nano
    - Arduino NG or older
    - Arduino Pro or Pro Mini
    - Arduino Robot Control
    - Arduino Robot Motor
    - Arduino Uno WiFi
    - Arduino Yún
    - Arduino Yún Mini
    - Arduino/Genuino Mega or Mega 2560
    - Arduino/Genuino Micro
    - Arduino/Genuino Uno
    - LilyPad Arduino
    - LilyPad Arduino USB
    - Linino One
* [The ESP8266 core for Arduino](https://github.com/esp8266/Arduino):
    - 4D Systems gen4 IoD Range
    - Adafruit Feather HUZZAH ESP8266
    - Amperka WiFi Slot
    - Arduino
    - Digistump Oak
    - ESPDuino (ESP-13 Module)
    - ESPino (ESP-12 Module)
    - ESPresso Lite 1.0
    - ESPresso Lite 2.0
    - Generic ESP8266 Module
    - Generic ESP8285 Module
    - Invent One
    - LOLIN(WEMOS) D1 mini Lite
    - LOLIN(WEMOS) D1 mini Pro
    - LOLIN(WEMOS) D1 R2 & mini
    - NodeMCU 0.9 (ESP-12 Module)
    - NodeMCU 1.0 (ESP-12E Module)
    - Olimex MOD-WIFI-ESP8266(-DEV)
    - Phoenix 1.0
    - Phoenix 2.0
    - Seeed Wio Link
    - SparkFun ESP8266 Thing
    - SparkFun ESP8266 Thing Dev
    - SweetPea ESP-210
    - ThaiEasyElec's ESPino
    - WeMos D1 R1
    - WiFiduino
    - WifInfo
    - XinaBox CW01

Adding support for other core requires adding a new variant in
[the folder "Ports"](Ports/) and to include the file in
[MemoryInfo.cpp](MemoryInfo.cpp).