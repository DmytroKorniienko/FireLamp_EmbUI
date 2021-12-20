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
        bool mp3mode:1; // режим mp3 плеера
        bool effectmode:1; // режим проигрывания эффектов
        bool alarm:1; // сейчас будильник
        bool isplayname:1; // проигрывается имя
      };
      uint32_t flags;
    };
    Task *tPeriodic = nullptr; // периодический опрос плеера
    uint8_t cur_volume = 1;
    uint16_t mp3filescount = 255; // кол-во файлов в каталоге MP3
    uint16_t nextAdv=0; // следующее воспроизводимое сообщение (произношение минут после часов)
    uint16_t cur_effnb=0; // текущий эффект
    uint16_t prev_effnb=0; // предыдущий эффект
    ALARM_SOUND_TYPE tAlarm;
    SoftwareSerial mp3player;
    String soundfile; // хранилище пути/имени
    unsigned long restartTimeout = millis(); // таймаут воспроизведения имени эффекта
    void printSatusDetail();
    void playAdvertise(int filenb);
    void playFolder0(int filenb);
    void restartSound();
  public:
    MP3PLAYERDEVICE(const uint8_t rxPin= MP3_RX_PIN, const uint8_t txPin=MP3_TX_PIN); // конструктор
    uint16_t getCurPlayingNb() {return prev_effnb;} // вернуть предыдущий для смещения
    void setupplayer(uint16_t effnb, const String &_soundfile) {soundfile = _soundfile; cur_effnb=effnb;};
    bool isReady() {return ready;}
    bool isAlarm() {return alarm;}
    bool isOn() {return on && ready;}
    bool isMP3Mode() {return mp3mode;}
    void setIsOn(bool val, bool forcePlay=true) {
      on = val;
      if(!on)
        stop();
      else if(forcePlay && (effectmode || mp3mode))
        playEffect(cur_effnb, soundfile);

      if(tPeriodic)
        tPeriodic->cancel();
      
      if(on){
        tPeriodic = new Task(1.21 * TASK_SECOND, TASK_FOREVER, std::bind(&MP3PLAYERDEVICE::handle,this), &ts, false, nullptr, [this](){TASK_RECYCLE; tPeriodic = nullptr;}); // "ленивый" опрос - раз в 1.21 сек (стараюсь избежать пересеченией с произнесением времени)
        tPeriodic->enableDelayed();
      }
    }
    void playTime(int hours, int minutes, TIME_SOUND_TYPE tst);
    void playEffect(uint16_t effnb, const String &_soundfile, bool delayed=false);
    void playName(uint16_t effnb);
    void setVolume(uint8_t vol) { cur_volume=vol; if(ready) {volume(vol);} LOG(printf_P, PSTR("DFplayer: Set volume: %d\n"), cur_volume); }
    uint8_t getVolume() { return cur_volume; }
    void setTempVolume(uint8_t vol) { volume(vol); LOG(printf_P, PSTR("DFplayer: Set temp volume: %d\n"), vol); }
    void setMP3count(uint16_t cnt) {mp3filescount = cnt;} // кол-во файлов в папке MP3
    uint16_t getMP3count() {return mp3filescount;}
    void setEqType(uint8_t val) { EQ(val); }
    void setPlayMP3(bool flag) {mp3mode = flag;}
    void setPlayEffect(bool flag) {effectmode = flag;}
    void setAlarm(bool flag) {alarm = flag; stop();}
    void StartAlarmSoundAtVol(ALARM_SOUND_TYPE val, uint8_t vol);
    void ReStartAlarmSound(ALARM_SOUND_TYPE val);
    void RestoreVolume() { setVolume(cur_volume); }
    void setCurEffect(uint16_t effnb) { prev_effnb=cur_effnb; cur_effnb = effnb%256; }
    void handle();
};
#endif
#endif
