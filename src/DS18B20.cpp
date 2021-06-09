/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_JeeUI.

    FireLamp_JeeUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_JeeUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_JeeUI.  If not, see <https://www.gnu.org/licenses/>.

  (Этот файл — часть FireLamp_JeeUI.

   FireLamp_JeeUI - свободная программа: вы можете перераспространять ее и/или
   изменять ее на условиях Стандартной общественной лицензии GNU в том виде,
   в каком она была опубликована Фондом свободного программного обеспечения;
   либо версии 3 лицензии, либо (по вашему выбору) любой более поздней
   версии.

   FireLamp_JeeUI распространяется в надежде, что она будет полезной,
   но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА
   или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной
   общественной лицензии GNU.

   Вы должны были получить копию Стандартной общественной лицензии GNU
   вместе с этой программой. Если это не так, см.
   <https://www.gnu.org/licenses/>.)
*/

#include "config.h"
#ifdef DS18B20
#include "DS18B20.h"
#ifdef TM1637_CLOCK
#include "tm.h"
#endif
#ifdef ENCODR
#include "enc.h"
#endif


MicroDS18B20 dallas(DS18B20_PIN);

bool canBeDisplayed;

bool& canDisplayTemp() {return canBeDisplayed;};

void ds_setup() {
#if CURRENT_LIMIT_STEP > 0 && CURRENT_LIMIT == 0
  #undef CURRENT_LIMIT
  #define CURRENT_LIMIT (NUM_LEDS * 60U)
#endif
#if COOLER_PIN >= 0
  pinMode(COOLER_PIN, OUTPUT);
#endif
  dallas.requestTemp();
  LOG(printf_P, PSTR("DS18b20 was initialized.\n")); 
  canBeDisplayed = true;
}

int16_t getTemp() {
#if defined(ENCODER) && SW == DS18B20_PIN
  detachInterrupt(SW);
#endif
  int currentTemp = dallas.getTemp(); // получить с далласа
  dallas.requestTemp();           // запросить измерение
  LOG(printf_P, PSTR("DS18B20: Temperature  %d\U000000B0C\n"), currentTemp);
#if defined(ENCODER) && SW == DS18B20_PIN
  attachInterrupt(digitalPinToInterrupt(SW), isrEnc, FALLING);
#endif
  return currentTemp;
}


// преобразуем температуру в скорость
void tempToSpeed(int16_t& currentTemp) {
#if COOLER_PIN >= 0
#if COOLER_PIN_TYPE
  uint16_t fanSpeed = 0;
#endif

    // преобразовать диапазон и ограничить значение
  #if COOLER_PIN_TYPE
      if (currentTemp >= (int)TEMP_MIN) {
        fanSpeed = constrain(map(currentTemp, TEMP_MIN, TEMP_MAX, MIN_SPEED, MAX_SPEED), 0, MAX_SPEED);  
      } else {
        fanSpeed = 0;
      }
      analogWrite(COOLER_PIN, fanSpeed);
      LOG(printf_P, PSTR("DS18B20: New fan PWM value: %d\n"), fanSpeed);
  #else
    if (currentTemp >= (int)(TEMP_DEST + TEMP_DEST/10U) && digitalRead(COOLER_PIN) != COOLER_PIN_LEVEL) {
      digitalWrite(COOLER_PIN, COOLER_PIN_LEVEL);
      LOG(printf_P, PSTR("DS18B20: Fan activated! \n"));
    }
    else if (currentTemp <= (int)(TEMP_DEST - TEMP_DEST/10U) && digitalRead(COOLER_PIN) != !COOLER_PIN_LEVEL) {
      digitalWrite(COOLER_PIN, !COOLER_PIN_LEVEL);
      LOG(printf_P, PSTR("DS18B20: Fan desactivated. \n"));
    }
  #endif
  #endif
}


void ds_loop() { 
#if defined(ENCODER) && SW == DS18B20_PIN
  if (!digitalRead(SW)) return; // если кнопка энкодера нажата, пропускаем этот loop
#endif
  int16_t curTemp = getTemp();
  if (canDisplayTemp()) ds_display(curTemp);

  tempToSpeed(curTemp);

  if (myLamp.isLampOn() && CURRENT_LIMIT_STEP > 0U) {
    static uint8_t delayCounter = 0;
    delayCounter++;
#if COOLER_PIN_TYPE
    if (delayCounter%COOLING_FAIL == 0 && curTemp > (int)TEMP_MAX) { // троттлинг по максимальной температуре (ШИМ)
#else
    if (delayCounter%COOLING_FAIL == 0 && curTemp > (int)(TEMP_DEST+TEMP_DEST/10U)) {  // троттлинг по желаемой рабочей + 10% (Дискретный)
#endif
      uint16_t myCurrlimit = myLamp.getcurLimit();
      myCurrlimit -= myCurrlimit/100*CURRENT_LIMIT_STEP;
      myLamp.setcurLimit(myCurrlimit);
      FastLED.setMaxPowerInVoltsAndMilliamps(5, myCurrlimit);
      delayCounter = 0;
      LOG(printf_P, PSTR("DS18B20: Cooling failure! New Current Limit is (mA): %d\n"), myLamp.getcurLimit());
    }
  }
}

void ds_display(int16_t value) { 
#ifdef TM1637_CLOCK
  tm1637.getSetDelay() = TM_TIME_DELAY;
  String tmp = String(value) + ((value < -9 || value > 99) ? "" : String(F("%")));    // "%" - для отображения "о" вверху, "*" - для отображения "с" вверху
  tm1637.display(tmp, true, false, (value < -9 || value > 99) ? 0 : (value < 10 ? 2 : 1));
#endif
}

#endif
