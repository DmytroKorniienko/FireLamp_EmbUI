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

MicroDS18B20 dallas(DS18B20_PIN);


void ds_setup() {
  analogWriteFreq(25000);
  pinMode(COOLER_PIN, OUTPUT);
  dallas.requestTemp();
  LOG(printf_P, PSTR("DS18b20 was initialized \n")); 
}

int16_t getTemp() {
  int currentTemp = dallas.getTemp(); // получить с далласа
  dallas.requestTemp();           // запросить измерение
  LOG(printf_P, PSTR("Sensor temperature  %d\U000000B0\n"), currentTemp);
  return currentTemp;
}

// преобразуем температуру в скорость
void tempToSpeed(int16_t& currentTemp) {
#if COOLER_PIN_TYPE
  uint16_t fanSpeed = 0;
#endif

    // преобразовать диапазон и ограничить значение
  #if COOLER_PIN_TYPE
      if (currentTemp >= (int)(TEMP_LOW - TEMP_LOW/10U)) {
        fanSpeed = constrain(map(currentTemp, TEMP_LOW - TEMP_LOW/10U, TEMP_HIGH, MIN_SPEED, MAX_SPEED), 0, MAX_SPEED);  
      } else {
        fanSpeed = 0;
      }
      analogWrite(COOLER_PIN, fanSpeed);
      LOG(printf_P, PSTR("New PWM value  - %d\n"), fanSpeed);
  #else
    if (currentTemp >= (int)TEMP_LOW && digitalRead(COOLER_PIN) != COOLER_PIN_LEVEL) {
      digitalWrite(COOLER_PIN, COOLER_PIN_LEVEL);
      LOG(printf_P, PSTR("Cooler fan activated \n"));
    }
    else if (currentTemp <= (int)(TEMP_LOW - TEMP_LOW/10U) && digitalRead(COOLER_PIN) != !COOLER_PIN_LEVEL) {
      digitalWrite(COOLER_PIN, !COOLER_PIN_LEVEL);
      LOG(printf_P, PSTR("Cooler fan desactivated \n"));
    }
  #endif
}


void ds_loop() { 
  int16_t curTemp = getTemp();

  tempToSpeed(curTemp);

  if (myLamp.isLampOn() && CURRENT_LIMIT_STEP == 0U) {
    static uint8_t delayCounter = 0;
    static uint16_t myCurrlimit = myLamp.getcurLimit();
    delayCounter++;
    if (delayCounter%COOLING_FAIL == 0 && curTemp > (int)TEMP_LOW) {
      myCurrlimit -= myCurrlimit/100*CURRENT_LIMIT_STEP;
      LOG(printf_P, PSTR("Cooling failure! Current Limit will be charged to: %d\n"), myCurrlimit);
      myLamp.setcurLimit(myCurrlimit);
      delayCounter = 0;
    }
  }
}

#endif
