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

#ifdef MP3PLAYER
#ifndef __MP3_PLAYER_H
#define __MP3_PLAYER_H
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

class MP3PLAYERDEVICE : protected DFRobotDFPlayerMini {
  private:
    union {
      struct {
        bool ready:1; // закончилась ли инициализация
        bool on:1; // включен ли...
        bool inAdv:1; // выводится сообщение времени
      };
      uint32_t flags;
    };
    int nextAdv=0; // следующее воспроизводимое сообщение (произношение минут после часов)
    SoftwareSerial mp3player;
    Ticker delayedCall;
    Ticker periodicCall;
    void printSatusDetail();
    void playAdvertise(int filenb);
  public:
    MP3PLAYERDEVICE(const uint8_t rxPin= MP3_RX_PIN, const uint8_t txPin=MP3_TX_PIN); // конструктор
    bool isReady() {return ready;}
    bool isInAdv() {return inAdv;};
    bool isOn() {return on && ready;}
    void setIsOn(bool val) {on = val; if(!on) stop();}
    void playTime(int hours, int minutes);
    void playEffect(uint16_t effnb);
    void setVolume(uint8_t vol) { volume(vol); }
    void handle();
};
#endif
#endif
