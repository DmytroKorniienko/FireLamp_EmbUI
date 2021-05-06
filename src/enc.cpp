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

uint8_t lastAction; // id тложенной операции, котору нужно выпонить в enc_loop
uint16_t effNum;
uint16_t anyValue; // просто любое значение, которое крутить прямо сейчас, очищается в enc_loop
bool done; // true == все отложенные до enc_loop операции выполнены.
uint16_t loops; // счетчик псевдотаймера






//Task encTask(100 * TASK_MILLISECOND, TASK_FOREVER, &callEncTick, &ts, true);

void callEncTick () {
  enc.tick();
}

void encLoop() {
  static uint16_t valRepiteChk = anyValue;
  enc.tick();
  noInter();
  loops++;  // счетчик псевдотаймера
/*
*     Оперативные изменения, яркость например, чтобы было видно что происходит
*/
  if (loops%10 and !done) {
    myLamp.demoTimer(T_RESET);
    myLamp.effects.autoSaveConfig();
    switch (lastAction)
    {
    case 1: // регулировка яркости
      myLamp.setBrightness(anyValue);
      break;

    default:
      break;
    } 
  }
/*
*     Отложенные операции, к примеру отправка яркости в УИ, запись новых значений в ФС, etc.
*/
  if (loops%3000 == 0 and !done) { // пока псевдотаймер. Тут кровь из носу нужен четкий таск. Вообще все это в другую процедуру и в таск.
    myLamp.demoTimer(T_RESET);
    myLamp.effects.autoSaveConfig();
    
    if (valRepiteChk != anyValue) {  // проверим менялось ли значение, чтобы не дергать почем зря отложенную операцию
      valRepiteChk = anyValue;
    } else {
      LOG(printf_P, PSTR("Enc: New setting appled.\n"));

      switch (lastAction)
      {
      case 1: // регулировка яркости
        remote_action(RA::RA_BRIGHT_NF, String(anyValue).c_str(), NULL);
        break;
      case 2: // переключение эффектов
        LOG(printf_P, PSTR("Enc: Effect number: %d\n"), anyValue);
        myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), anyValue);
        done = true;
        remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL);
      break;
      default:
        break;
      } 
      lastAction = 0;
      //anyValue = 0;
      done = true;
    } 
  } 
  
  inter();
}


void IRAM_ATTR isrEnc() {
  noInter();
  enc.tick();  // отработка в прерывании
  inter();
}

void inter() {
  attachInterrupt(digitalPinToInterrupt(DT), isrEnc, CHANGE);   // прерывание на DT пине
  attachInterrupt(digitalPinToInterrupt(CLK), isrEnc, CHANGE);  // прерывание на CLK пине
  attachInterrupt(digitalPinToInterrupt(SW), isrEnc, FALLING);   // прерывание на SW пине
}

void noInter() {
  detachInterrupt(DT);
  detachInterrupt(CLK);
  detachInterrupt(SW);
}

void isTurn() {
  if (!myLamp.isLampOn()) return;
  noInter();
  myLamp.demoTimer(T_RESET);
  myLamp.effects.autoSaveConfig();
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
    encSetBri(-1);
    break;
  case 2:  // Влево быстро
    encSetBri(-10);
    break;
  case 3: // нажатый влево 
    encSetEffect(-1);
    break;
  case 4: // влево нажатый и быстро
    encSetEffect(-5);
    break;
  case 5: // Вправо    
    encSetBri(1);
    break;
  case 6:  // Вправо быстро
    encSetBri(10);
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
  inter();
}

void isClick() {
  myLamp.demoTimer(T_RESET);
  myLamp.effects.autoSaveConfig();
  display(enc.clicks, FPSTR("CL."));
}

void isHolded() {
  LOG(printf_P, PSTR("Enc: Pressed and holded\n"));
  display(FPSTR("Ctrl"));
  if (!myLamp.isLampOn()) {
    remote_action(RA::RA_WHITE_LO, "0", NULL); // для энкодера я хочу сделать запуск белой лампы с яркостью из конфига, а не фиксированной
  }
  for (uint8_t i = 0; i < myLamp.getEffControls().size(); i++) {
    uint8_t id =    myLamp.getEffControls()[i]->getId();
    uint8_t type =  myLamp.getEffControls()[i]->getType();
    uint8_t min  =  myLamp.getEffControls()[i]->getMin().toInt();
    uint8_t max  =  myLamp.getEffControls()[i]->getMax().toInt();
    uint8_t value = myLamp.getEffControls()[i]->getVal().toInt();
    String name = myLamp.getEffControls()[i]->getName();
    LOG(printf_P, PSTR("Enc: Controls: id, type, min, max, value, name: %d, %d, %d, %d, %d, %s\n"), id, type, min, max, value, name.c_str());

    
  }
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
  noInter();
	if (myLamp.isAlarm()) {
		// нажатие во время будильника
		myLamp.stopAlarm();
		return;
	}
  
  switch (enc.clicks)
  {
  case 1: // Включение\выключение лампы
  bool onOff;
    if (myLamp.isLampOn()) {
      onOff = false;
      remote_action(RA::RA_OFF, NULL);
    } else {
      onOff = true;
      remote_action(RA::RA_ON, NULL);
    }
#ifdef TM1637_CLOCK
    tm1637.display(String(onOff ? F("On") : F("Off")), true, false, onOff ? 2 : 1);  // Выводим 
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
  inter();
}


void enc_setup() {
  lastAction = 0; // id операции, котору нужно выпонить в enc_loop
  effNum = 0;
  anyValue = 0; // просто любое значение, которое крутить прямо сейчас, очищается в enc_loop
  done = true; // true == все отложенные до enc_loop операции выполнены.
  loops = 0;
  inter(); // включаем прерывания энкодера и кнопки
  //enc.counter = 100;      // изменение счётчика
  enc.attach(TURN_HANDLER, isTurn);
  enc.attach(CLICK_HANDLER, isClick);
  enc.attach(HOLDED_HANDLER, isHolded);
 // enc.attach(STEP_HANDLER, myStep);
  enc.attach(CLICKS_HANDLER, myClicks);
  //enc.attachClicks(6, isClick6);

}



void encSetBri(int val) {
  loops = 0; // сбросим псевдотаймер

  if (done or lastAction !=1) { 
    anyValue = myLamp.getLampBrightness();
    done = false;
  }
  lastAction = 1;
  anyValue = constrain(anyValue+val, 1, 255);
#ifdef VERTGAUGE
  myLamp.GaugeShow(anyValue, 255, 10);
#endif
  display(anyValue, FPSTR("b."));
}

void encSetEffect(int val) {
  loops = 0; // сбросим псевдотаймер

  if (done or lastAction !=2) { // если сеттер отраболтал или предыдущий мод не отвечает текущему, перечитаем значение, и взведем сеттер
    anyValue = myLamp.effects.getSelected();
    done = false;
  }
  lastAction = 2;

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

  display(myLamp.effects.realEffNumdByList(anyValue), "");
}

void display(int16_t value, String type, uint8_t bri) {
#ifdef TM1637_CLOCK
  getSetDelay() = TM_TIME_DELAY;
  tm1637.setBrightness(bri);
  tm1637.display(value, true, false, value >= 100 ? 1 : (value >= 10 ? 2 : 3) );  
  tm1637.display(type);
#endif
}

void display(String str, uint8_t bri) {
#ifdef TM1637_CLOCK
  getSetDelay() = TM_TIME_DELAY;
  tm1637.setBrightness(bri);
  tm1637.display("");
  tm1637.display(str);
#endif
}

#endif

