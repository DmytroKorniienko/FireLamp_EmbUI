/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_EmbUI.

    FireLamp_EmbUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_EmbUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_EmbUI.  If not, see <https://www.gnu.org/licenses/>.

(Цей файл є частиною FireLamp_EmbUI.

   FireLamp_EmbUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_EmbUI поширюється в надії, що вона буде корисною,
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

#ifndef EXIT_TIMEOUT
  #define EXIT_TIMEOUT (3U)
#endif

#ifndef ENC_STRING_EFFNUM_DELAY
  #define ENC_STRING_EFFNUM_DELAY (17)
#endif

Encoder enc;

// Функція копіює вміст одного файлу в інший (немає стандартної в LittleFS)
void copyPastFile(String FileFrom, String FileTo) {
  if (LittleFS.exists(FileFrom))  // видаляємо копію, якщо існує
    LittleFS.remove(FileTo);

  File f1 = LittleFS.open(FileFrom, "r");    // відкриваємо джерело
  File f2 = LittleFS.open(FileTo, "w");    // створюємо кінцевий файл
  if (!f2 || !f1)
    return;

  char b;
  while (f1.available() > 0) { // побайтово копіюємо файл-джерело в кінцевий файл
    f1.readBytes(&b, 1);
    f2.write(b);
  }
  //  закриваємо файли
  f2.close();
  f1.close();
}

// Функція затирає системний конфіг та заміняє його дефолтним
void resetLamp() { // Функція відновлює дефолтні налаштування лампи (основний конфіг, WiFi)
    WiFi.disconnect(true); // закриваємо з'єднання, та забуваємо налаштування WiFi
    if (LittleFS.exists(F("/backup/glb/default.json"))) {
      copyPastFile(F("/backup/glb/default.json"), F("/backup/glb/config.json")); // створюємо копію дефолтних налаштувань в ту ж папку
    }
    LittleFS.remove(F("/config.json"));  // видаляємо файли конфігурації, як оригінал, так і копію
    LittleFS.remove(F("/config_bkp.json"));
    if (LittleFS.exists(F("/backup/glb/default.json"))) {
      LittleFS.rename(F("/backup/glb/config.json"), F("/config.json"));   // переносимо новий конфіг в корінь
    }
    delay(1000);
    ESP.restart();  // Приміняємо зміни, перезагрузивши лампу
}

// Поместить в общий setup()
void Encoder::init() {
  currAction = WAIT; // id операции, котору нужно выпонить в enc_loop
  currEffNum = 0;
  anyValue = 0; // просто любое значение, которое крутить прямо сейчас, очищается в enc_loop
  done = true; // true == все отложенные до enc_loop операции выполнены.
  loops = 0;
  inSettings = false;
  currDynCtrl = 1;
  interruptEn(); // включаем прерывания энкодера и кнопки
  //enc.counter = 100;      // изменение счётчика
}

void Encoder::handle() {
  static uint16_t valRepiteChk = anyValue;
  interruptOff();
  enc.tick();
  interruptEn();

  EncState state = (EncState)getState();
  resetState();

  switch (state)
  {
  case CLICK:
    click();
    break;

  case HOLD:
    hold();
    break;

  case RIGHT:
  case RIGHT_HOLD:
  case LEFT:
  case LEFT_HOLD:
    turn(state);
    break;

  default:
    break;
  }

  if (hasClicks())
    myClicks();

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
      sendString(myLamp.getEffControls()[currDynCtrl]->getName(), txtColor, false, txtDelay);
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
  if (SET_EFFECT != currAction) {
    if (valRepiteChk != (SET_CONTROL == currAction ? myLamp.getEffControls()[currDynCtrl]->getVal().toInt() : anyValue) && !done) {  // проверим менялось ли значение, чтобы не дергать почем зря
      valRepiteChk = (SET_CONTROL == currAction ? myLamp.getEffControls()[currDynCtrl]->getVal().toInt() : anyValue);
      switch (currAction)
      {
      case SET_BRIGHT: // регулировка яркости
        remote_action(RA::RA_BRIGHT_NF, (String(FPSTR(TCONST_0015)) + "0").c_str(), String(anyValue).c_str(), NULL);
        done = true;
        break;
      case SET_CONTROL: // регулировка любого из динамических контролов в режиме "Настройки эффекта"
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
          sendStringNumEff(currEffNum <= 255 ? String(currEffNum) : (String((byte)(currEffNum & 0xFF)) + "." + String((byte)(currEffNum >> 8) - 1U)), txtColor);
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
      sendString(String(FPSTR(TINTF_00A)) + ": " + (currEffNum <= 255 ? String(currEffNum) : (String((byte)(currEffNum & 0xFF)) + "." + String((byte)(currEffNum >> 8) - 1U))), txtColor, true, txtDelay);
      done = true;
      currAction = WAIT;
    }
  }
}


// Обработчик прерываний
void IRAM_ATTR Encoder::isrEnc() {
 enc.tick();  // отработка в прерывании
}

// Функция восстанавливает прерывания энкодера
void Encoder::interruptEn() {
  attachInterrupt(digitalPinToInterrupt(DT), isrEnc, CHANGE);   // прерывание на DT пине
  attachInterrupt(digitalPinToInterrupt(CLK), isrEnc, CHANGE);  // прерывание на CLK пине
  attachInterrupt(digitalPinToInterrupt(SW), isrEnc, FALLING);  // прерывание на SW пине
}

// Функция запрещает прерывания от энкодера, на время других операций, чтобы не спамить контроллер
void Encoder::interruptOff() {
  detachInterrupt(DT);
  detachInterrupt(CLK);
  detachInterrupt(SW);
}


// Функция обрабатывает повороты энкодера
void Encoder::turn(EncState turnType) {
  if (!myLamp.isLampOn()) return;
  resetTimers();
  bool fast = isFast();

  switch (turnType)
  {
  case LEFT:
    if (inSettings) {
      setDynCtrl(fast == false ? -1 : -16);
    }
    else
      setBri(fast == false ? -1 : -16);
    break;
  case LEFT_HOLD:
    setEffect(fast == false ? -1 : -5);
    break;
  case RIGHT:
    if (inSettings) {
      setDynCtrl(fast == false ? 1 : 16);
    }
    else
      setBri(fast == false ? 1 : 16);
    break;
  case RIGHT_HOLD: // вправо нажатый
    setEffect(fast == false ? 1 : 5);
    break;
  case NONE:
  default:
    break;
  }

  LOG(printf_P, PSTR("Enc: Turn type: %s%s\n"),
                getStateName((EncState)turnType),
                (fast ? PSTR("_FAST") : PSTR("")));
}

// Функция обрабатывает клики по кнопке
void Encoder::click() {
  resetTimers();
  if (!inSettings) display(enc.clicks, String(F("CL.")));
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
    display(myLamp.getEffControls()[currDynCtrl]->getVal().toInt(), String(myLamp.getEffControls()[currDynCtrl]->getId()) + String(FPSTR(".")));
    sendString(myLamp.getEffControls()[currDynCtrl]->getName(), txtColor, true, txtDelay);
  }
}

// Функция проверяет может ли контрол быть использоваан (проверка на скрытость, на скрытость по микрофону и т.п.)
bool Encoder::validControl(const CONTROL_TYPE ctrlCaseType) {
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
void Encoder::hold() {
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
    sendString(String(FPSTR(TINTF_01A)), CRGB::Green, true, txtDelay);
    display(myLamp.getEffControls()[currDynCtrl]->getVal().toInt(), String(currDynCtrl) + String(F(".")));
    sendString(myLamp.getEffControls()[currDynCtrl]->getName(), txtColor, false, txtDelay);
  } else {
      exitSettings();
  }
}

// Функция выхода из режима "Настройки эффекта", восстанавливает состояния до, форсирует запись конфига эффекта
void Encoder::exitSettings() {
  if (!inSettings) return;
  currDynCtrl = 1;
  done = true;
  loops = 0;
  currAction = WAIT;
  anyValue = 0;
  inSettings = false;
  display(String(F("done")));
  sendString(String(FPSTR(TINTF_00B)), CRGB::Red, true, txtDelay);
  myLamp.effects.autoSaveConfig(true);
#ifdef DS18B20
  canDisplayTemp() = true;
#endif
  LOG(printf_P, PSTR("Enc: exit Settings\n"));
}

// Функция обрабатывает клики по кнопке
void Encoder::myClicks() {
  resetTimers();

	if (myLamp.isAlarm()) {
		// нажатие во время будильника
    clicks = 0;
		ALARMTASK::stopAlarm();
		return;
	}

  switch (clicks)
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

  done = true;
}


// Функция регулировки яркости в обычном режиме
void Encoder::setBri(int val) {
  resetTimers();

  if (done || currAction != SET_BRIGHT) {
    anyValue = myLamp.getLampBrightness();
    done = false;
  }
  if (currAction == SET_EFFECT) {
    currAction = SET_BRIGHT; // сменим мод, но на вылет. Крутить яркостью будем в следующем цикле. Так уменьшим количество ошибок юзера, когда при отпускании энкодера он проворачивает его.
    return;
  }
  currAction = SET_BRIGHT;
  anyValue = constrain(anyValue + val, 1, 255);
  if (myLamp.getGaugeType()!=GAUGETYPE::GT_NONE){
      GAUGE::GaugeShow(anyValue, 255, 0, getGaugeColor());
  }
  display(anyValue, String(F("b.")));
  LOG(printf_P, PSTR("Enc: setBri Value %d\n"), anyValue);
}

// Функция смены эффекта зажатым энкодером
void Encoder::setEffect(int val) {
  resetTimers();
  if (inSettings) { // если в режиме "Настройки эффекта" выходим из него
    exitSettings();
    return;
  }

  if (done || SET_EFFECT != currAction) { // если сеттер отработал или предыдущий мод не отвечает текущему, перечитаем значение, и взведем сеттер
    anyValue = myLamp.effects.effIndexByList(myLamp.effects.getCurrent());
    done = false;
    display(anyValue, "");
  }

  currAction = SET_EFFECT;

  anyValue = anyValue + val;

  while (1)  // в цикле проверим может быть текущий накрученный контролл выбранным
  {
    if (myLamp.effects.effCanBeSelected(anyValue)) break;

    if (val > 0) { // если курутили вперед по списку - скипим в том же направлении, если назад - в обратном
      anyValue++;
      if(anyValue >= myLamp.effects.getModeAmount()) // если ничего не нашли, - снова начинаем сначала
        anyValue = 0;
    }
    else {
      anyValue--;
      if (0U == anyValue) // если ничего не нашли, - снова начинаем с конца
        anyValue = myLamp.effects.getModeAmount()-1;
    }
  }
  currEffNum = myLamp.effects.realEffNumdByList(anyValue);
  display(currEffNum <= 255 ? String(currEffNum) : (String((byte)(currEffNum & 0xFF)) + "." + String((byte)(currEffNum >> 8) - 1U)));
}

// Функция настройки динамического контрола в режиме "Настройки эффекта"
void Encoder::setDynCtrl(int val) {
  resetTimers();
  loops = 0;

  if (done || currAction != SET_CONTROL) {
    done = false;
  }
  currAction = SET_CONTROL;
  // тут магия, некоторые чекбоксы у нас особенные, типа локальный "Микрофон".
  // Придется проверять что это - ползунок или чекбокс и по разному подходить к процессу внесения нового значения. Бля...
  if ((myLamp.getEffControls()[currDynCtrl]->getType() & 0x0F) == 0) // если ползунок
    myLamp.getEffControls()[currDynCtrl]->setVal(String(myLamp.getEffControls()[currDynCtrl]->getVal().toInt() + val));
  else // если чекбокс
    myLamp.getEffControls()[currDynCtrl]->setVal(String(constrain(myLamp.getEffControls()[currDynCtrl]->getVal().toInt() + val, 0, 1)));

  if ((myLamp.getEffControls()[currDynCtrl]->getType() & 0x0F) == 2) sendString(myLamp.getEffControls()[currDynCtrl]->getName() + String(myLamp.getEffControls()[currDynCtrl]->getVal().toInt() ? F(": ON") : F(": OFF")), txtColor, true, txtDelay);
  else if (myLamp.getGaugeType()!=GAUGETYPE::GT_NONE){
      GAUGE::GaugeShow(myLamp.getEffControls()[currDynCtrl]->getVal().toInt(), myLamp.getEffControls()[currDynCtrl]->getMax().toInt(), 0, getGaugeColor());
  }
  display(myLamp.getEffControls()[currDynCtrl]->getVal().toInt(), String(myLamp.getEffControls()[currDynCtrl]->getId()) + String(F(".")));
  LOG(printf_P, PSTR("Enc: dynCtrl: %d Value %d\n"), myLamp.getEffControls()[currDynCtrl]->getId(), myLamp.getEffControls()[currDynCtrl]->getVal().toInt());
}


void Encoder::display(uint16_t value, String type) {
#ifdef TM1637_CLOCK
  tm1637.getSetDelay() = TM_TIME_DELAY;
  tm1637.display(value, true, false, value >= 100 ? 1 : (value >= 10 ? 2 : 3) );
  tm1637.display(type);
#endif
}


void Encoder::display(String str) {
#ifdef TM1637_CLOCK
  tm1637.getSetDelay() = TM_TIME_DELAY;
  tm1637.clearScreen();
  tm1637.display(str);
#endif
}

// Ресетим таймера автосохранения конфигов и Демо на время "баловства" с энкодером
void Encoder::resetTimers() {
  myLamp.demoTimer(T_RESET);
  myLamp.effects.autoSaveConfig();
  embui.autosave();
}

// Функция выводит информацию, с помощью бегущей строки
void Encoder::sendString(String str, CRGB color, bool force, uint8_t delay) {
  fade = myLamp.getBFade();
  myLamp.setBFade(FADETOBLACKVALUE);
  myLamp.setTextMovingSpeed(delay);
  speed = myLamp.getTextMovingSpeed();
  myLamp.sendStringToLamp(str.c_str(), color, false, force);
  myLamp.setBFade(fade);
  myLamp.setTextMovingSpeed(speed);

}

void Encoder::sendStringNumEff(String str, CRGB color) {
  fade = myLamp.getBFade();
  myLamp.setBFade(FADETOBLACKVALUE);
  myLamp.setTextMovingSpeed(ENC_STRING_EFFNUM_DELAY);
  speed = myLamp.getTextMovingSpeed();
  String tmp = " " + str + " ";
  myLamp.sendStringToLampDirect(tmp.c_str(), color, false, true);
  myLamp.setBFade(fade);
  myLamp.setTextMovingSpeed(speed);
}


void Encoder::toggleDemo() {
  if (myLamp.getMode() == LAMPMODE::MODE_DEMO) {
    remote_action(RA::RA_DEMO, "0", NULL);
    sendString(String(F("Demo OFF")), txtColor, true, txtDelay);
  }
  else
    remote_action(RA::RA_DEMO, "1", NULL);
}

void Encoder::toggleGBright() {
  remote_action(RA::RA_GLOBAL_BRIGHT, myLamp.IsGlobalBrightness() ? "0" : String(myLamp.getGlobalBrightness()).c_str(), NULL);
  sendString(String(FPSTR(TINTF_00C)) + String(myLamp.IsGlobalBrightness() ? F(": ON") : F(": OFF")), txtColor, true, txtDelay);
}

void Encoder::toggleMic() {
#ifdef MIC_EFFECTS
  remote_action(RA::RA_MICONOFF, myLamp.isMicOnOff() ? "0" : "1", NULL);
  sendString(String(FPSTR(TINTF_021)) + String(myLamp.isMicOnOff() ? F(": ON") : F(": OFF")), txtColor, true, txtDelay);
#endif
}

void Encoder::toggleAUX() {
#ifdef AUX_PIN
  remote_action(RA::RA_AUX_TOGLE, NULL);
  sendString(String(FPSTR(TCONST_000E)) + String(digitalRead(AUX_PIN) == AUX_LEVEL ? F(": ON") : F(": OFF")), txtColor, true, txtDelay);
#endif
}

void Encoder::sendTime() {
  remote_action(RA::RA_SEND_TIME, NULL);
}

void Encoder::sendIP() {
  remote_action(RA::RA_SEND_IP, NULL);
  #ifdef TM1637_CLOCK
  tm1637.setIpShow();
  #endif
}

#if LAMP_DEBUG == 1
const char* Encoder::getStateName(EncState state) {

  switch (state)
  {
  case NONE:
    return PSTR("NONE");
  case RIGHT:
    return PSTR("RIGHT");
  case LEFT:
    return PSTR("LEFT");
  case RIGHT_HOLD:
    return PSTR("RIGHT_HOLD");
  case LEFT_HOLD:
    return PSTR("LEFT_HOLD");
  case CLICK:
    return PSTR("CLICK");
  case HOLD:
    return PSTR("HOLD");
  case STEP:
    return PSTR("STEP");
  default:
    return PSTR("DEFAULT");
  }
}
#endif

#endif
