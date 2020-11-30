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
#include "mp3player.h"
#include "main.h"
#ifdef MP3PLAYER
MP3PLAYERDEVICE::MP3PLAYERDEVICE(const uint8_t rxPin, const uint8_t txPin) : mp3player(rxPin, txPin) // RX, TX
{
  on = false;
  ready = false;

  mp3player.begin(9600);
  setTimeOut(MP3_SERIAL_TIMEOUT); //Set serial communictaion time out ~300ms
  LOG(println);
  LOG(println, F("DFRobot DFPlayer Mini"));
  LOG(println, F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  delay(DFPALYER_START_DELAY);
  uint8_t retry_cnt=0;
  while(!begin(mp3player) && retry_cnt++<=5){ //Use softwareSerial to communicate with mp3.
    LOG(printf_P, PSTR("DFPlayer: Unable to begin: %d...\n"), retry_cnt);
    delay(MP3_SERIAL_TIMEOUT);
  }
  if (retry_cnt>=5 && !begin(mp3player)) {
    LOG(println, F("1.Please recheck the connection!"));
    LOG(println, F("2.Please insert the SD card!"));
    ready = false;
    return;
  }
  ready = true;

  LOG(println, F("DFPlayer Mini online."));
  outputDevice(DFPLAYER_DEVICE_SD);
  periodicCall.attach_scheduled(1.21, std::bind(&MP3PLAYERDEVICE::handle, this)); // "ленивый" опрос - раз в 1.21 сек (стараюсь избежать пересеченией с произнесением времени)
  volume(5);  // start volume
}

void MP3PLAYERDEVICE::restartSound()
{
  isplayname = false;
  int currentState = readState();
  LOG(printf_P,PSTR("readState()=%d, mp3mode=%d\n"), currentState, mp3mode);
  if(currentState == 512 || currentState == -1){
    delayedCall.once(0.2,std::bind([this](){
      if(isOn()){
        if(!mp3mode){
          if(cur_effnb>0)
            playEffect(cur_effnb, soundfile); // начать повтороное воспроизведение в эффекте
        } else {
          cur_effnb++;
          if(cur_effnb>mp3filescount)
            cur_effnb=1;
          playMp3Folder(cur_effnb);
        }
      }
    }));
  }
}

void MP3PLAYERDEVICE::printSatusDetail(){
  uint8_t type = readType();
  int value = read();

  switch (type) {
    case TimeOut:
      LOG(println, F("Time Out!"));
      break;
    case WrongStack:
      LOG(println, F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      LOG(println, F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      LOG(println, F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      LOG(println, F("Card Online!"));
      setVolume(cur_volume); // в случае перетыкания карты или сборса плеера - восстановим громкость
      break;
    case DFPlayerPlayFinished: {
        LOG(print, F("Number:"));
        LOG(print, value);
        LOG(println, F(" Play Finished!"));
        if(restartTimeout+5000<millis()){ // c момента инициализации таймаута прошло более 5 секунд (нужно чтобы не прерывало вывод времени в режиме без звука)
          restartSound();
        }
      }
      break;
    case DFPlayerError:
      LOG(print, F("DFPlayerError:"));
      switch (value) {
        case Busy:
          LOG(println, F("Card not found"));
          break;
        case Sleeping:
          LOG(println, F("Sleeping"));
          break;
        case SerialWrongStack:
          LOG(println, F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          LOG(println, F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          LOG(println, F("File Index Out of Bound"));
          break;
        case FileMismatch:
          LOG(println, F("Cannot Find File"));
          if(isplayname) // только для случая когда нет файла с именем эффекта, если нет самой озвучки эффекта, то не рестартуем
            restartSound();
          break;
        case Advertise:
          LOG(println, F("In Advertise"));
          // возникла ошибка с минутами, попробуем еще раз
          if(restartTimeout+10000<millis()){ // c момента инициализации таймаута прошло более 10 секунд, избавляюсь от зацикливания попыток
            restartTimeout=millis();
            restartSound(); // тут будет отложенный запуск через 0.2 секунды
            delay(300);
            delayedCall.once_scheduled(2.5, std::bind(&MP3PLAYERDEVICE::playAdvertise, this, nextAdv)); // повторное воспроизведение минут через 1.5 секунды
          }
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

void MP3PLAYERDEVICE::handle()
{
  if (available()) { // эта часть не только пишет ошибки, но также отлавливает изменение состояний!!!
    printSatusDetail(); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void MP3PLAYERDEVICE::playTime(int hours, int minutes, TIME_SOUND_TYPE tst)
{
  if(!isReady()) return;

  int currentState = readState();
  LOG(printf_P,PSTR("readState()=%d\n"), currentState);
  if(tst==TIME_SOUND_TYPE::TS_VER1){
    if(currentState == 513 || currentState == -1)
    {
      playAdvertise(3000+hours);
      nextAdv = minutes+3100;
      delayedCall.once_scheduled(2.25, std::bind(&MP3PLAYERDEVICE::playAdvertise, this, nextAdv)); // воспроизведение минут через 2.25 секунды после произношения часов
    } else {
      playLargeFolder(0x00, 3000+hours);
      nextAdv = minutes+3100;
      delayedCall.once_scheduled(2.25, std::bind(&MP3PLAYERDEVICE::playFolder0, this, nextAdv)); // воспроизведение минут через 2.25 секунды после произношения часов
      restartTimeout = millis();
    }
  } else if(tst==TIME_SOUND_TYPE::TS_VER2){
    if(currentState == 513 || currentState == -1)
    {
      playAdvertise(hours*100+minutes);
    } else {
      playLargeFolder(0x00, hours*100+minutes);
    }
  }
}

void MP3PLAYERDEVICE::playFolder0(int filenb) {
  playLargeFolder(0x00, filenb);
}

void MP3PLAYERDEVICE::playAdvertise(int filenb) {
  LOG(printf_P, PSTR("Advertise filenb: %d\n"), filenb);
  advertise(filenb);
}

void MP3PLAYERDEVICE::playEffect(uint16_t effnb, const String &_soundfile, bool delayed)
{
  isplayname = false;
  soundfile = _soundfile;
  int folder = _soundfile.substring(1,_soundfile.lastIndexOf('\\')-1).toInt();
  int filenb = _soundfile.substring(_soundfile.lastIndexOf('\\')+1).toInt();
  LOG(printf_P, PSTR("soundfile:%s, folder:%d, filenb:%d, effnb:%d\n"), soundfile.c_str(), folder, filenb, effnb%256);
  if(!mp3mode){
    if(!filenb){
      cur_effnb = effnb%256;
      if(!delayed)
        playFolder(3, cur_effnb);
      prev_effnb = effnb%256;
    } else if(!folder){
      //mp3
      if(!delayed)
        playMp3Folder(filenb);
    } else {
      //folder#
      if(!delayed)
        playFolder(folder, filenb);
    }
  } else {
    int shift=effnb%256-prev_effnb%256;
    prev_effnb = effnb%256;
    cur_effnb = ((int32_t)cur_effnb + shift)%256;
    if(cur_effnb>mp3filescount)
      cur_effnb%=mp3filescount;
    else if(cur_effnb==0)
      cur_effnb=1;
    if(!delayed)
      playMp3Folder(cur_effnb);
  }
}

void MP3PLAYERDEVICE::playName(uint16_t effnb)
{
  isplayname = true;
  LOG(printf_P, PSTR("playName, effnb:%d\n"), effnb%256);
  playFolder(2, effnb%256);
}

void MP3PLAYERDEVICE::StartAlarmSound(ALARM_SOUND_TYPE val){
  volume(0);
  delay(100);
  switch(val){
    case ALARM_SOUND_TYPE::AT_FIRST :
      playFolder(1,1);
      break;
    case ALARM_SOUND_TYPE::AT_SECOND :
      playFolder(1,2);
      break;
    case ALARM_SOUND_TYPE::AT_THIRD :
      playFolder(1,3);
      break;
    case ALARM_SOUND_TYPE::AT_FOURTH :
      playFolder(1,4);
      break;
    case ALARM_SOUND_TYPE::AT_FIFTH :
      playFolder(1,5);
      break;
    case ALARM_SOUND_TYPE::AT_RANDOM :
      playFolder(1,random(5)+1);
      break;
    case ALARM_SOUND_TYPE::AT_RANDOMMP3 :
      playMp3Folder(random(mp3filescount)+1);
      break;
    default:
    break;
  }
}

#endif