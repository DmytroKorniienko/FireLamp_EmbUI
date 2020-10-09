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
  setTimeOut(200); //Set serial communictaion time out 200ms
  LOG(println);
  LOG(println, F("DFRobot DFPlayer Mini Demo"));
  LOG(println, F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!begin(mp3player)) {  //Use softwareSerial to communicate with mp3.
    LOG(println, F("Unable to begin:"));
    LOG(println, F("1.Please recheck the connection!"));
    LOG(println, F("2.Please insert the SD card!"));
    ready = false;
    return;
  }
  ready = true;

  LOG(println, F("DFPlayer Mini online."));
  EQ(DFPLAYER_EQ_NORMAL);
  outputDevice(DFPLAYER_DEVICE_SD);
  periodicCall.attach_scheduled(1, std::bind(&MP3PLAYERDEVICE::handle, this));   // "ленивый" опрос - раз в 1 сек
  volume(5);  //Set volume value. From 0 to 30
  //outputSetting(true, 15); //output setting, enable the output and set the gain to 15
  //volume(15);  //Set volume value. From 0 to 30
  //LOG(println, readFileCounts()); //read all file counts in SD card
  //LOG(println, readCurrentFileNumber()); //read current play file number
  //LOG(println, readFileCountsInFolder(1)); //read fill counts in folder SD:/03
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
      break;
    case DFPlayerPlayFinished:
      LOG(print, F("Number:"));
      LOG(print, value);
      LOG(println, F(" Play Finished!"));
      if(cur_effnb>0)
        playEffect(cur_effnb); // начать повтороное воспроизведение в эффекте
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
          break;
        case Advertise:
          LOG(println, F("In Advertise"));
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
#ifdef LAMP_DEBUG  
  if (available()) {
    printSatusDetail(); //Print the detail message from DFPlayer to handle different errors and states.
  }
#endif
}

void MP3PLAYERDEVICE::playTime(int hours, int minutes)
{
  if(!isReady() || !isOn()) return; // || isInAdv()

  int currentState = readState();
  LOG(printf_P,PSTR("readState()=%d\n"), currentState);
  if(currentState == 513 || currentState == -1)
  {
    playAdvertise(hours);
    nextAdv = minutes+100;
    delayedCall.once_scheduled(1.75, std::bind(&MP3PLAYERDEVICE::playAdvertise, this, nextAdv)); // воспроизведение минут через 1.75 секунды после произношения часов
  } else {
    playLargeFolder(0x00, hours);
    nextAdv = minutes+100;
    delayedCall.once_scheduled(1.75, std::bind(&MP3PLAYERDEVICE::playFolder0, this, nextAdv)); // воспроизведение минут через 1.75 секунды после произношения часов
  }
}

void MP3PLAYERDEVICE::playFolder0(int filenb) {
  playLargeFolder(0x00, filenb);
}

void MP3PLAYERDEVICE::playAdvertise(int filenb) {
  advertise(filenb);
}

void MP3PLAYERDEVICE::playEffect(uint16_t effnb)
{
  stop();
  playFolder(3, effnb%256);
  cur_effnb = effnb;
}

void MP3PLAYERDEVICE::playName(uint16_t effnb)
{
  stop();
  playFolder(2, effnb%256);
}

void MP3PLAYERDEVICE::StartAlarmSound(ALARM_SOUND_TYPE val){
  volume(0);
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
      playFolder(random(5)+1,1);
      break;
    case ALARM_SOUND_TYPE::AT_RANDOMMP3 :
      playMp3Folder(random(mp3filescount)+1);
      break;
    default:
    break;
  }
}

#endif