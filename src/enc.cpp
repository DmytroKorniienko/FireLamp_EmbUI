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
#ifdef ENCODER
#include "enc.h"
#include "tm.h"
#include "interface.h"
#include "effects.h"
#include "ui.h"
#include "DS18B20.h"

uint8_t controlsAmount;     // колличество контролов у эффекта
uint8_t currDynCtrl;        // текущий контрол, с которым работаем
uint8_t currAction;         // идент тложенной операции, котору нужно выпонить в enc_loop
uint16_t currEffNum;            // текущий номер эффекта
uint16_t anyValue;          // просто любое значение, которое крутим прямо сейчас, очищается в enc_loop
uint32_t loops;             // счетчик псевдотаймера
bool done;                  // true == все отложенные до enc_loop операции выполнены.
bool inSettings;            // флаг - мы в настройках эффекта

#ifndef EXIT_TIMEOUT
#define EXIT_TIMEOUT 3U
#endif

struct control {
    uint8_t id = 0; 
    uint8_t type = 2;  
    uint8_t min  = 1; 
    uint8_t max  = 255; 
    uint8_t value = 127;
    String name = "";
};


std::vector<control> encDynCtrl;



//Task encTask(100 * TASK_MILLISECOND, TASK_FOREVER, &callEncTick, &ts, true);

uint8_t getCurrDynCtrl() {
  return currDynCtrl;
}

void callEncTick () {
  enc.tick();
}

void encLoop() {
  static uint16_t valRepiteChk = anyValue;
  noInterrupt();
  enc.tick();
  loops++;  // счетчик псевдотаймера
  if (inSettings and loops%100 == 0) {
    resetTimers();
#ifdef TM1637_CLOCK
    getSetDelay() = TM_TIME_DELAY;
#endif
  }

  if (inSettings and loops == EXIT_TIMEOUT * 65000) {
    inSettings =  false;
    exitSettings();
  }
  if (inSettings and loops%30000 == 0) {
    myLamp.sendStringToLamp(encDynCtrl[currDynCtrl].name.c_str(), CRGB::Orange, true); 
  }
/*
*     Оперативные изменения, яркость например, чтобы было видно что происходит
*/
  if (valRepiteChk != (currAction == 3 ? encDynCtrl[currDynCtrl].value : anyValue) and !done) {  // проверим менялось ли значение, чтобы не дергать почем зря 
    valRepiteChk = (currAction == 3 ? encDynCtrl[currDynCtrl].value : anyValue);
    switch (currAction)
    {
    case 1: // регулировка яркости
      myLamp.setBrightness(anyValue);
      break;
    case 3:
      remote_action(RA::RA_DYNCTRL, String(encDynCtrl[currDynCtrl].value).c_str(), nullptr);
      done = true;
    break;

    default:
      break;
    } 
  }

/*
*     Отложенные операции, к примеру отправка яркости в УИ, запись новых значений в ФС, etc.
*/
  if (loops%3000 == 0 and !done) { // пока псевдотаймер. Тут кровь из носу нужен четкий таск. Вообще все это в другую процедуру и в таск.
    resetTimers();
    
    if (valRepiteChk != anyValue) {  // проверим менялось ли значение, чтобы не дергать почем зря отложенную операцию
      valRepiteChk = anyValue;
    } else {
      LOG(printf_P, PSTR("Enc: New setting appled.\n"));

      switch (currAction)
      {
      case 1: // регулировка яркости
        remote_action(RA::RA_BRIGHT_NF, String(anyValue).c_str(), NULL);
        break;
      case 2: // переключение эффектов
        LOG(printf_P, PSTR("Enc: Effect number: %d\n"), currEffNum);
        myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), currEffNum);
        done = true;
        remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL);
      break;
      default:
        break;
      } 
      currAction = 0;
      //anyValue = 0;
      done = true;
    } 
  } 
  
  interrupt();
}


void IRAM_ATTR isrEnc() {
  noInterrupt();
  enc.tick();  // отработка в прерывании
  interrupt();
}

void interrupt() {
  attachInterrupt(digitalPinToInterrupt(DT), isrEnc, CHANGE);   // прерывание на DT пине
  attachInterrupt(digitalPinToInterrupt(CLK), isrEnc, CHANGE);  // прерывание на CLK пине
  attachInterrupt(digitalPinToInterrupt(SW), isrEnc, FALLING);   // прерывание на SW пине
}

void noInterrupt() {
  detachInterrupt(DT);
  detachInterrupt(CLK);
  detachInterrupt(SW);
}

void isTurn() {
  if (!myLamp.isLampOn()) return;
  noInterrupt();
  resetTimers();
  uint8_t turnType = 0;

  // тут опрос эвентов с энкодера Right, Left, etc.
    if (enc.isLeft()) {
        if (enc.isFast()) {turnType = 2; }  // Fast left
        else {turnType = 1; } // Left
    } else if (enc.isLeftH()) {
        if (enc.isFast()) {turnType = 4; }  // Fast left hold
        else {turnType = 3; }  // Hold left
    } else 
    if (enc.isRight()) {
        if (enc.isFast()) {turnType = 6; }  // Fast right
        else {turnType = 5; }  // Right
    } else if (enc.isRightH()) {
        if (enc.isFast()) {turnType = 8; }  // Fast right hold
        else {turnType = 7; } // Hold right
    }

  switch (turnType)
  {
  case 1: // Влево 
  case 2: // Влево быстро
    if (inSettings) {
      encSetDynCtrl(turnType == 1 ? -1 : -10);
    } 
    else 
      encSetBri(turnType == 1 ? -1 : -10);
    break;
  case 3: // нажатый влево 
    encSetEffect(-1);
    break;
  case 4: // влево нажатый и быстро
    encSetEffect(-5);
    break;
  case 5: // Вправо  
  case 6:  // Вправо быстро  
    if (inSettings) {
      encSetDynCtrl(turnType == 5 ? 1 : 10);
    } 
    else 
      encSetBri(turnType == 5 ? 1 : 10);
    break;
  case 7: // вправо нажатый 
    encSetEffect(1);
    break;
  case 8: // вправо нажатый и быстро
    encSetEffect(5);
    break;
  
  default:
    break;
  }
  LOG(printf_P, PSTR("Enc: Turn type: %d\n"), turnType);
  interrupt();
}

void isClick() {
  noInterrupt();
  resetTimers();
  if (!inSettings) encDisplay(enc.clicks, String(FPSTR("CL.")));
  else {
    enc.clicks = 0;
    while (1) {
      currDynCtrl ++;
      if (encDynCtrl[currDynCtrl].type == 0 or encDynCtrl[currDynCtrl].type == 2) 
        break;
      if (currDynCtrl > controlsAmount-1) {
        currDynCtrl = 0;
        break;
      }
    }
    encDisplay(currDynCtrl, String(FPSTR("Ctr.")));
    myLamp.sendStringToLamp(encDynCtrl[currDynCtrl].name.c_str(), CRGB::Orange, true);  
  }
  interrupt();
}

void isHolded() {
  noInterrupt();
  LOG(printf_P, PSTR("Enc: Pressed and holded\n"));
  if (!myLamp.isLampOn()) {
    remote_action(RA::RA_WHITE_LO, "0", NULL); // для энкодера я хочу сделать запуск белой лампы с яркостью из конфига, а не фиксированной
    return;
  }
  
  inSettings = !inSettings; // вошли\вышли
  if (inSettings) {
    encDisplay(String(FPSTR("Sets")));
    resetTimers();
    loops = 0;
#ifdef DS18B20
    canDisplayTemp() = false;
#endif
    myLamp.sendStringToLamp(String(FPSTR(TINTF_002)).c_str(), CRGB::Orange, true);
    controlsAmount = myLamp.getEffControls().size();
    encDynCtrl.resize(controlsAmount);
    for (uint8_t i = 0; i < myLamp.getEffControls().size(); i++) { // вычитаем
      encDynCtrl[i].id =    myLamp.getEffControls()[i]->getId();
      encDynCtrl[i].type =  myLamp.getEffControls()[i]->getType();
      encDynCtrl[i].min  =  myLamp.getEffControls()[i]->getMin().toInt();
      encDynCtrl[i].max  =  myLamp.getEffControls()[i]->getMax().toInt();
      encDynCtrl[i].value = myLamp.getEffControls()[i]->getVal().toInt();
      encDynCtrl[i].name = myLamp.getEffControls()[i]->getName();
      LOG(printf_P, PSTR("Enc: in Settings: Controls: id, type, min, max, value, name: %d, %d, %d, %d, %d, %s\n"), encDynCtrl[i].id, encDynCtrl[i].type, encDynCtrl[i].min, encDynCtrl[i].max, encDynCtrl[i].value, encDynCtrl[i].name.c_str());
    }
  } else {
      exitSettings();
  }
  interrupt();
}

void exitSettings() {
  encDynCtrl.resize(0);
  controlsAmount = 0;
  currDynCtrl = 0;
  done = true;
  loops = 0;
  currAction = 0;
  anyValue = 0;
  encDisplay(String(FPSTR("done")));
  myLamp.sendStringToLamp(String(FPSTR(TINTF_00B)).c_str(), CRGB::Orange, true);
  myLamp.effects.autoSaveConfig(true);
#ifdef DS18B20
  canDisplayTemp() = true;
#endif
  LOG(printf_P, PSTR("Enc: exit Settings\n"));
}

/*

Из выключенного состояния
    1 касание - включить на последнем эффекте
    2 касания - включить в режиме ДЕМО
    долгое удержание - включить в режиме "белая лампа" на минимальную яркость (ночник)
    касание, удержание - включить в режиме "белая лампа" на максимальную яркость

Во включенном состоянии
    1 касание - выключить лампу
    4 касания + 4 касания - запуск сервиса ОТА
    5 касаний - вывод IP на лампу
    6 касаний - вывод текущего времени бегущей строкой
    удержание - регулировка яркости
    1 касание, удержание - регулировка "скорости" эффекта
    2 касания, удержание - регулировка "масштаба" эффекта
*/


void myClicks() {
  noInterrupt();
	if (myLamp.isAlarm()) {
		// нажатие во время будильника
    enc.clicks = 0;
		myLamp.stopAlarm();
		return;
	}
  
  switch (enc.clicks)
  {
  case 1: // Включение\выключение лампы
    if (myLamp.isLampOn()) {
      remote_action(RA::RA_OFF, NULL);
    } else {
      remote_action(RA::RA_ON, NULL);
    }
#ifdef TM1637_CLOCK
    tmDisplay(String(myLamp.isLampOn() ? F("On") : F("Off")), true, false, onOff ? 2 : 1);  // Выводим 
#endif
    break;
  case 2:  // Вкл\выкл Демо
    if (myLamp.getMode() == MODE_DEMO) {
      remote_action(RA::RA_DEMO, "0", NULL); 
      myLamp.sendStringToLamp(String(PSTR("- Demo OFF -")).c_str(), CRGB::Green);
    }
    else 
      remote_action(RA::RA_DEMO, "1", NULL);
    break;
  case 3:
    remote_action(RA::RA_SEND_TIME, NULL);
    break;
  case 4:
    remote_action(RA::RA_SEND_IP, NULL);
    break;
  case 5:
    remote_action(RA::RA_OTA, NULL);
    break;
  default:
    LOG(printf_P, PSTR("Enc: Click: %d\n"), enc.clicks);
    break;
  }
  interrupt();
}


void enc_setup() {
  currAction = 0; // id операции, котору нужно выпонить в enc_loop
  currEffNum = 0;
  anyValue = 0; // просто любое значение, которое крутить прямо сейчас, очищается в enc_loop
  done = true; // true == все отложенные до enc_loop операции выполнены.
  loops = 0;
  inSettings = false;
  currDynCtrl = 0;
  controlsAmount = 0;
  interrupt(); // включаем прерывания энкодера и кнопки
  //enc.counter = 100;      // изменение счётчика
  enc.attach(TURN_HANDLER, isTurn);
  enc.attach(CLICK_HANDLER, isClick);
  enc.attach(HOLDED_HANDLER, isHolded);
 // enc.attach(STEP_HANDLER, myStep);
  enc.attach(CLICKS_HANDLER, myClicks);
  //enc.attachClicks(6, isClick6);

}



void encSetBri(int val) {
  resetTimers();
  loops = 0;

  if (done or currAction !=1) { 
    anyValue = myLamp.getLampBrightness();
    done = false;
  }
  currAction = 1;
  anyValue = constrain(anyValue + val, 1, 255);
#ifdef VERTGAUGE
  myLamp.GaugeShow(anyValue, 255, 10);
#endif
  encDisplay(anyValue, String(FPSTR("b.")));
}

void encSetEffect(int val) {
  noInterrupt();
  resetTimers();
  loops = 0;

  if (done or currAction !=2) { // если сеттер отраболтал или предыдущий мод не отвечает текущему, перечитаем значение, и взведем сеттер
    anyValue = myLamp.effects.getSelected();
    done = false;
  }
  currAction = 2;

  anyValue = anyValue+val;
  
  while (!myLamp.effects.effCanBeSelected(anyValue))  // в цикле проверим может быть текущий накрученный выбранным
  {
    if (val > 0) { // если курутили вперед по списку - скипим в том же направлении, если назад - в обратном
      anyValue++; 
      if(anyValue == myLamp.effects.getModeAmount()) // если ничего не нашли, - снова начинаем сначала
        anyValue = 0;
    }
    else {
      anyValue--;
      if (anyValue == 0) // если ничего не нашли, - снова начинаем с конца
        anyValue = myLamp.effects.getModeAmount()-1;
    }
  }
  currEffNum = myLamp.effects.realEffNumdByList(anyValue);

  encDisplay(currEffNum, "");
  interrupt();
}

void encSetDynCtrl(int val) {
  noInterrupt();
  resetTimers();
  loops = 0;
  
  if (done or currAction !=3) { 
    done = false;
  }
  currAction = 3;
  encDynCtrl[currDynCtrl].value = constrain(encDynCtrl[currDynCtrl].value + val, encDynCtrl[currDynCtrl].min, encDynCtrl[currDynCtrl].max);
  if (encDynCtrl[currDynCtrl].type == 2) myLamp.sendStringToLamp((encDynCtrl[currDynCtrl].name + String(encDynCtrl[currDynCtrl].value ? FPSTR(": On") : FPSTR(": Off"))).c_str(), CRGB::Orange, true); //myLamp.showWarning(CRGB::Orange,1000,100,3,true, encDynCtrl[currDynCtrl].value ? "On" : "Off");
#ifdef VERTGAUGE
  else myLamp.GaugeShow(encDynCtrl[currDynCtrl].value, encDynCtrl[currDynCtrl].max, 10);
#endif
  encDisplay(encDynCtrl[currDynCtrl].value, String(currDynCtrl) + String(FPSTR(".")));
  interrupt();
}

void encDisplay(uint16_t value, String type) {
#ifdef TM1637_CLOCK
  getSetDelay() = TM_TIME_DELAY;
  tmDisplay(value, true, false, value >= 100 ? 1 : (value >= 10 ? 2 : 3) );  
  tmDisplay(type);
#endif
}

void encDisplay(String str) {
#ifdef TM1637_CLOCK
  getSetDelay() = TM_TIME_DELAY;
  tmDisplay(str);
#endif
}

void resetTimers() {
  myLamp.demoTimer(T_RESET);
  myLamp.effects.autoSaveConfig();
  embui.autosave();
  //loops = 0;
}

#endif

/* 

myLamp.showWarning(CRGB::Orange,5000,500);
String msg=F("жжж");
myLamp.showWarning(col,dur,per,type,true,msg.isEmpty()?(const char *)nullptr:msg.c_str());

*/