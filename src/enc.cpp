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

(Цей файл є частиною FireLamp_JeeUI.

   FireLamp_JeeUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_JeeUI поширюється в надії, що вона буде корисною,
   але БЕЗ ВСЯКИХ ГАРАНТІЙ; навіть без неявної гарантії ТОВАРНОГО ВИГЛЯДУ
   або ПРИДАТНОСТІ ДЛЯ ВИЗНАЧЕНИХ ЦІЛЕЙ. Докладніше див. у Стандартній
   громадська ліцензія GNU.

   Ви повинні були отримати копію Стандартної громадської ліцензії GNU
   разом із цією програмою. Якщо це не так, див.
   <https://www.gnu.org/licenses/>.)
*/

#include "config.h"
#ifdef ENCODER
#include "enc.h"
#ifdef TM1637_CLOCK
#include "tm.h"
#endif
#include "interface.h"
#include "effects.h"
#include "ui.h"
#include "DS18B20.h"
#include "extra_tasks.h"

uint8_t currDynCtrl;        // текущий контрол, с которым работаем
uint8_t currAction;         // идент текущей операции: 0 - ничего, 1 - крутим яркость, 2 - меняем эффекты, 3 - меняем динамические контролы
uint16_t currEffNum;        // текущий номер эффекта
uint16_t anyValue;          // просто любое значение, которое крутим прямо сейчас, очищается в enc_loop
uint8_t loops;              // счетчик псевдотаймера
bool done;                  // true == все отложенные до enc_loop операции выполнены.
bool inSettings;            // флаг - мы в настройках эффекта
uint8_t speed, fade;

uint8_t txtDelay = 40U;
CRGB txtColor = CRGB::Orange;

Task encTask(1 * TASK_MILLISECOND, TASK_FOREVER, &callEncTick, &ts, true);

void callEncTick () {
  enc.tick();
}

void encLoop() {
  static uint16_t valRepiteChk = anyValue;
  noInterrupt();
  //enc.tick();

  if (inSettings) { // Время от времени выводим название контрола (в режиме "Настройки эффекта")
    resetTimers();
#ifdef TM1637_CLOCK
    tm1637.getSetDelay() = TM_TIME_DELAY;
#endif
    EVERY_N_SECONDS(10) {
      loops++;
      if (inSettings && loops == EXIT_TIMEOUT) {  // timeout выхода из режима "Настройки эффекта"
        exitSettings();
        return;
      }
      encSendString(myLamp.getEffControls()[currDynCtrl]->getName(), txtColor, false, txtDelay); 
    }
  }
#ifdef DS18B20
  else {
    EVERY_N_SECONDS(1) {  // раз в секунду проверяем возможность выводить температуру
      canDisplayTemp() = done;
    }
  }
#endif

/*
*     Оперативные изменения, яркость например, чтобы было видно что происходит
*/
  if (currAction != 2) {
    if (valRepiteChk != (currAction == 3 ? myLamp.getEffControls()[currDynCtrl]->getVal().toInt() : anyValue) && !done) {  // проверим менялось ли значение, чтобы не дергать почем зря 
      valRepiteChk = (currAction == 3 ? myLamp.getEffControls()[currDynCtrl]->getVal().toInt() : anyValue);
      switch (currAction)
      {
      case 1: // регулировка яркости
        remote_action(RA::RA_BRIGHT_NF, (String(FPSTR(TCONST_0015)) + "0").c_str(), String(anyValue).c_str(), NULL);
        done = true;
        break;
      case 3: // регулировка любого из динамических контролов в режиме "Настройки эффекта"
        remote_action(RA::RA_CONTROL, (String(FPSTR(TCONST_0015)) + String(myLamp.getEffControls()[currDynCtrl]->getId())).c_str(), myLamp.getEffControls()[currDynCtrl]->getVal().c_str(), NULL);
        done = true;
      break;
      default:
        break;
      } 
    }
  } else
/*
*     Отложенные операция смены эффекта
*/
  {
    EVERY_N_MILLIS(150) {
      static bool printed = false;
      if (valRepiteChk == currEffNum) {
        if (!printed) {
          encSendStringNumEff(currEffNum <= 255 ? String(currEffNum) : (String((byte)(currEffNum & 0xFF)) + "." + String((byte)(currEffNum >> 8) - 1U)), txtColor);
          printed = true;
        }
      } 
      else {
        valRepiteChk = currEffNum;
        printed = false;
      }
    }
    if (!done && digitalRead(SW)) { // если эффект еще не меняли и кнопка уже отпущена - переключаем эффект
      resetTimers();
      LOG(printf_P, PSTR("Enc: New effect number: %d\n"), currEffNum);
      myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), currEffNum);
      remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL);
      encSendString(String(FPSTR(TINTF_00A)) + ": " + (currEffNum <= 255 ? String(currEffNum) : (String((byte)(currEffNum & 0xFF)) + "." + String((byte)(currEffNum >> 8) - 1U))), txtColor, true, txtDelay);
      done = true;
      currAction = 0;
    }
  }
  interrupt();
}


// // Обработчик прерываний
//void IRAM_ATTR isrEnc() { 
//  noInterrupt();
//  enc.tick();  // отработка в прерывании
//  interrupt();
//}

// Функция запрещает прерывания от энкодера, на время других операций, чтобы не спамить контроллер
void interrupt() {
  //attachInterrupt(digitalPinToInterrupt(DT), isrEnc, FALLING/*CHANGE*/);   // прерывание на DT пине
  //attachInterrupt(digitalPinToInterrupt(CLK), isrEnc, FALLING);  // прерывание на CLK пине
  // attachInterrupt(digitalPinToInterrupt(SW), isrEnc, FALLING);   // прерывание на SW пине
}

// Функция восстанавливает прерывания энкодера
void noInterrupt() {
  //detachInterrupt(DT);
  // detachInterrupt(CLK);
  // detachInterrupt(SW);
}

// Функция обрабатывает повороты энкодера
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
      encSetDynCtrl(turnType == 1 ? -1 : -16);
    } 
    else 
      encSetBri(turnType == 1 ? -1 : -16);
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
      encSetDynCtrl(turnType == 5 ? 1 : 16);
    } 
    else 
      encSetBri(turnType == 5 ? 1 : 16);
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

// Функция обрабатывает клики по кнопке
void isClick() {
  noInterrupt();
  resetTimers();
  if (!inSettings) encDisplay(enc.clicks, String(F("CL.")));
  else {
    enc.clicks = 0;
    resetTimers();
    loops = 0;
    while (1) {
      currDynCtrl ++;
      if (currDynCtrl == myLamp.getEffControls().size()) {
        currDynCtrl = 1;
        break;
      }


      if (validControl(myLamp.getEffControls()[currDynCtrl]->getType())) break;
    }
    encDisplay(myLamp.getEffControls()[currDynCtrl]->getVal().toInt(), String(myLamp.getEffControls()[currDynCtrl]->getId()) + String(FPSTR(".")));
    encSendString(myLamp.getEffControls()[currDynCtrl]->getName(), txtColor, true, txtDelay);  
  }
  interrupt();
}

// Функция проверяет может ли контрол быть использоваан (проверка на скрытость, на скрытость по микрофону и т.п.)
bool validControl(const CONTROL_TYPE ctrlCaseType) {
  bool isOk = false;
#ifdef MIC_EFFECTS
  bool isMicOn = myLamp.isMicOnOff();
  if (myLamp.getEffControls()[myLamp.getEffControls().size()-1]->getName().startsWith(FPSTR(TINTF_020)))
    isMicOn = isMicOn && myLamp.getEffControls()[myLamp.getEffControls().size()-1]->getVal().toInt();
#endif

  switch (ctrlCaseType & 0x0F) {
    case CONTROL_TYPE::RANGE:     // мы меняем только ползунки или чекбоксы
    case CONTROL_TYPE::CHECKBOX:
      isOk = true;
      break;
    default:
      isOk = false;
      break;
    }

    switch (ctrlCaseType >> 4) 
    {
    case CONTROL_CASE::HIDE: // Если спрятанный контрол, возвращаем ложь.
      return false;
      break;
#ifdef MIC_EFFECTS
    case CONTROL_CASE::ISMICON: // проверка "спрятан по микрофону"
      if (!isMicOn && (!myLamp.isMicOnOff() || !(myLamp.getEffControls()[currDynCtrl]->getId() == 7 && myLamp.getEffControls()[currDynCtrl]->getName().startsWith(FPSTR(TINTF_020)) == 1)))
        isOk = false;
      break;
#endif
    default:
      break;
    }
    
  return isOk;
}

// Функция обрабатывает состояние "кнопка нажата и удержана"
void isHolded() {
  noInterrupt();
  LOG(printf_P, PSTR("Enc: Pressed and holded\n"));
  if (!myLamp.isLampOn()) {
    remote_action(RA::RA_WHITE_LO, "0", NULL); // для энкодера я хочу сделать запуск белой лампы с яркостью из конфига, а не фиксированной
    return;
  }
  
  if (!inSettings) {
    inSettings = true;
    resetTimers();
    loops = 0;
#ifdef DS18B20
    canDisplayTemp() = false; // в режиме "Настройки Эффекта" запрещаем выводить температуру, в функции exitSettings() снова разрешим
    currEffNum = myLamp.effects.getCurrent();
    LOG(printf_P, PSTR("Enc: Effect number: %d controls amount %d\n"), currEffNum, myLamp.getEffControls().size());
#endif
    encSendString(String(FPSTR(TINTF_01A)), CRGB::Green, true, txtDelay);
    encDisplay(myLamp.getEffControls()[currDynCtrl]->getVal().toInt(), String(currDynCtrl) + String(F(".")));
    encSendString(myLamp.getEffControls()[currDynCtrl]->getName(), txtColor, false, txtDelay);
  } else {
      exitSettings();
  }
  interrupt();
}

// Функция выхода из режима "Настройки эффекта", восстанавливает состояния до, форсирует запись конфига эффекта
void exitSettings() {
  if (!inSettings) return;
  noInterrupt();
  currDynCtrl = 1;
  done = true;
  loops = 0;
  currAction = 0;
  anyValue = 0;
  inSettings = false;
  encDisplay(String(F("done")));
  encSendString(String(FPSTR(TINTF_00B)), CRGB::Red, true, txtDelay);
  myLamp.effects.autoSaveConfig(true);
#ifdef DS18B20
  canDisplayTemp() = true;
#endif
  LOG(printf_P, PSTR("Enc: exit Settings\n"));
  interrupt();
}

// Функция обрабатывает клики по кнопке
void myClicks() {
  noInterrupt();
  resetTimers();
	if (myLamp.isAlarm()) {
		// нажатие во время будильника
    enc.clicks = 0;
		ALARMTASK::stopAlarm();
		return;
	}
  
  switch (enc.clicks)
  {
  case 1: // Включение\выключение лампы
    if (myLamp.isLampOn()) {
      myLamp.startNormalMode(); // З енкодеру перемикаємо в нормальний режим
      remote_action(RA::RA_OFF, NULL);
#ifdef TM1637_CLOCK
      tm1637.getSetDelay() = 1;
      tm1637.display(String(F("Off")), true, false, 1);  // Выводим 
#endif
    } else {
      remote_action(RA::RA_ON, NULL);
#ifdef TM1637_CLOCK
      tm1637.getSetDelay() = 1;
      tm1637.display(String(F("On")), true, false, 2);  // Выводим 
#endif
    }
    break;
#ifdef ENC_DEMO_CLICK
  case ENC_DEMO_CLICK:  // Вкл\выкл Демо
    toggleDemo();
    break;
#endif
#ifdef ENC_GBRI_CLICK
  case ENC_GBRI_CLICK:
    toggleGBright();
    break;
#endif
#if defined(MIC_EFFECTS) && defined(ENC_MIC_CLICK)
  case ENC_MIC_CLICK:
    toggleMic();
    break;
#endif
#ifdef ENC_IP_CLICK
  case ENC_IP_CLICK:
    sendIP();
    break;
#endif
#ifdef ENC_TIME_CLICK
  case ENC_TIME_CLICK:
    sendTime();
    break;
#endif
#if defined(AUX_PIN) && defined(ENC_AUX_CLICK)
  case ENC_AUX_CLICK:
    toggleAUX();
    break;
#endif
#if defined(OTA) && defined(ENC_OTA_CLICK)
  case ENC_OTA_CLICK:
    remote_action(RA::RA_OTA, NULL);
    break;
#endif
  default:
    LOG(printf_P, PSTR("Enc: Click: %d\n"), enc.clicks);
    break;
  }
  interrupt();
}

// Поместить в общий setup()
void enc_setup() {
  currAction = 0; // id операции, котору нужно выпонить в enc_loop
  currEffNum = 0;
  anyValue = 0; // просто любое значение, которое крутить прямо сейчас, очищается в enc_loop
  done = true; // true == все отложенные до enc_loop операции выполнены.
  loops = 0;
  inSettings = false;
  currDynCtrl = 1;
  interrupt(); // включаем прерывания энкодера и кнопки
  //enc.counter = 100;      // изменение счётчика
  enc.attach(TURN_HANDLER, isTurn);
  enc.attach(CLICK_HANDLER, isClick);
  enc.attach(HOLDED_HANDLER, isHolded);
 // enc.attach(STEP_HANDLER, myStep);
  enc.attach(CLICKS_HANDLER, myClicks);
  //enc.attachClicks(6, isClick6);

}

// Функция регулировки яркости в обычном режиме
void encSetBri(int val) {
  resetTimers();

  if (done or currAction !=1) { 
    anyValue = myLamp.getLampBrightness();
    done = false;
  }
  if (currAction == 2) {
    currAction = 1; // сменим мод, но на вылет. Крутить яркостью будем в следующем цикле. Так уменьшим количество ошибок юзера, когда при отпускании энкодера он проворачивает его.
    return;
  }
  currAction = 1;
  anyValue = constrain(anyValue + val, 1, 255);
  if (myLamp.getGaugeType()!=GAUGETYPE::GT_NONE){
      GAUGE::GaugeShow(anyValue, 255);
  }
  encDisplay(anyValue, String(F("b.")));
  LOG(printf_P, PSTR("Enc: setBri Value %d\n"), anyValue);
}

// Функция смены эффекта зажатым энкодером
void encSetEffect(int val) {
  noInterrupt();
  resetTimers();
  if (inSettings) { // если в режиме "Настройки эффекта" выходим из него
    exitSettings();
    return;
  }

  if (done or currAction !=2) { // если сеттер отработал или предыдущий мод не отвечает текущему, перечитаем значение, и взведем сеттер
    anyValue = myLamp.effects.effIndexByList(myLamp.effects.getCurrent());
    done = false;
    encDisplay(anyValue, "");
  }

  currAction = 2;

  anyValue = anyValue + val;
  
  while (1)  // в цикле проверим может быть текущий накрученный выбранным
  {
    if (myLamp.effects.effCanBeSelected(anyValue)) break;

    if (val > 0) { // если курутили вперед по списку - скипим в том же направлении, если назад - в обратном
      anyValue++; 
      if(anyValue >= myLamp.effects.getModeAmount()) // если ничего не нашли, - снова начинаем сначала
        anyValue = 0;
    }
    else {
      anyValue--;
      if (anyValue == 0) // если ничего не нашли, - снова начинаем с конца
        anyValue = myLamp.effects.getModeAmount()-1;
    }
  }
  currEffNum = myLamp.effects.realEffNumdByList(anyValue);
  encDisplay(currEffNum <= 255 ? String(currEffNum) : (String((byte)(currEffNum & 0xFF)) + "." + String((byte)(currEffNum >> 8) - 1U)));  
  interrupt();
}

// Функция настройки динамического контрола в режиме "Настройки эффекта"
void encSetDynCtrl(int val) {
  noInterrupt();
  resetTimers();
  loops = 0;
  
  if (done or currAction !=3) { 
    done = false;
  }
  currAction = 3;
  // тут магия, некоторые чекбоксы у нас особенные, типа локальный "Микрофон". 
  // Придется проверять что это - ползунок или чекбокс и по разному подходить к процессу внесения нового значения. Бля...
  if ((myLamp.getEffControls()[currDynCtrl]->getType() & 0x0F) == 0) // если ползунок
    myLamp.getEffControls()[currDynCtrl]->setVal(String(myLamp.getEffControls()[currDynCtrl]->getVal().toInt() + val));
  else // если чекбокс
    myLamp.getEffControls()[currDynCtrl]->setVal(String(constrain(myLamp.getEffControls()[currDynCtrl]->getVal().toInt() + val, 0, 1)));
  
  if ((myLamp.getEffControls()[currDynCtrl]->getType() & 0x0F) == 2) encSendString(myLamp.getEffControls()[currDynCtrl]->getName() + String(myLamp.getEffControls()[currDynCtrl]->getVal().toInt() ? F(": ON") : F(": OFF")), txtColor, true, txtDelay); 
  else if (myLamp.getGaugeType()!=GAUGETYPE::GT_NONE){
      GAUGE::GaugeShow(myLamp.getEffControls()[currDynCtrl]->getVal().toInt(), myLamp.getEffControls()[currDynCtrl]->getMax().toInt());
  }
  encDisplay(myLamp.getEffControls()[currDynCtrl]->getVal().toInt(), String(myLamp.getEffControls()[currDynCtrl]->getId()) + String(F(".")));
  LOG(printf_P, PSTR("Enc: dynCtrl: %d Value %d\n"), myLamp.getEffControls()[currDynCtrl]->getId(), myLamp.getEffControls()[currDynCtrl]->getVal().toInt());
  interrupt();
}


void encDisplay(uint16_t value, String type) {
#ifdef TM1637_CLOCK
  tm1637.getSetDelay() = TM_TIME_DELAY;
  tm1637.display(value, true, false, value >= 100 ? 1 : (value >= 10 ? 2 : 3) );  
  tm1637.display(type);
#endif
}

void encDisplay(float value) {
#ifdef TM1637_CLOCK
  tm1637.getSetDelay() = TM_TIME_DELAY;
  tm1637.clearScreen();
  tm1637.display(value, false, true); //, true, false, value >= 100 ? 1 : (value >= 10 ? 2 : 3) );  
#endif
}

void encDisplay(String str) {
#ifdef TM1637_CLOCK
  tm1637.getSetDelay() = TM_TIME_DELAY;
  tm1637.clearScreen();
  tm1637.display(str);
#endif
}

// Ресетим таймера автосохранения конфигов и Демо на время "баловства" с энкодером 
void resetTimers() {
  myLamp.demoTimer(T_RESET);
  myLamp.effects.autoSaveConfig();
  embui.autosave();
}

// Функция выводит информацию, с помощью бегущей строки
void encSendString(String str, CRGB color, bool force, uint8_t delay) {
  fade = myLamp.getBFade();
  myLamp.setBFade(FADETOBLACKVALUE);
  myLamp.setTextMovingSpeed(delay);
  speed = myLamp.getTextMovingSpeed();
  myLamp.sendStringToLamp(str.c_str(), color, false, force);
  myLamp.setBFade(fade);
  myLamp.setTextMovingSpeed(speed);
  
}

void encSendStringNumEff(String str, CRGB color) {
  fade = myLamp.getBFade();
  myLamp.setBFade(FADETOBLACKVALUE);
  myLamp.setTextMovingSpeed(ENC_STRING_EFFNUM_DELAY);
  speed = myLamp.getTextMovingSpeed();
  String tmp = " " + str + " ";
  myLamp.sendStringToLampDirect(tmp.c_str(), color, false, true);
  myLamp.setBFade(fade);
  myLamp.setTextMovingSpeed(speed);
}


void toggleDemo() {
  if (myLamp.getMode() == LAMPMODE::MODE_DEMO) {
    remote_action(RA::RA_DEMO, "0", NULL); 
    encSendString(String(F("Demo OFF")), txtColor, true, txtDelay);
  }
  else 
    remote_action(RA::RA_DEMO, "1", NULL);
}

void toggleGBright() {
  remote_action(RA::RA_GLOBAL_BRIGHT, myLamp.IsGlobalBrightness() ? "0" : "1", NULL);
  encSendString(String(FPSTR(TINTF_00C)) + String(myLamp.IsGlobalBrightness() ? F(": ON") : F(": OFF")), txtColor, true, txtDelay);
}

void toggleMic() {
#ifdef MIC_EFFECTS
  remote_action(RA::RA_MICONOFF, myLamp.isMicOnOff() ? "0" : "1", NULL);
  encSendString(String(FPSTR(TINTF_021)) + String(myLamp.isMicOnOff() ? F(": ON") : F(": OFF")), txtColor, true, txtDelay);
#endif
}

void toggleAUX() {
#ifdef AUX_PIN
  remote_action(RA::RA_AUX_TOGLE, NULL);
  encSendString(String(FPSTR(TCONST_000E)) + String(digitalRead(AUX_PIN) == AUX_LEVEL ? F(": ON") : F(": OFF")), txtColor, true, txtDelay);
#endif
}

void sendTime() {
  remote_action(RA::RA_SEND_TIME, NULL);
}

void sendIP() {
  remote_action(RA::RA_SEND_IP, NULL);
  #ifdef TM1637_CLOCK
  tm1637.setIpShow();
  #endif
}


uint8_t getEncTxtDelay(){ return txtDelay;}
void setEncTxtDelay(const uint8_t speed){ txtDelay = speed;}
CRGB getEncTxtColor(){ return txtColor;}
void setEncTxtColor(const CRGB color){ txtColor = color;}

#endif
