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

//#define __IDPREFIX F("JeeUI2-")
#include <Arduino.h>
#include "JeeUI2.h"
#include "config.h"
#include "lamp.h"
#include "buttons.h"
#include "main.h"
#ifdef USE_FTP
#include "ftpServer.h"
#endif

// глобальные переменные для работы с ними в программе
SHARED_MEM GSHMEM; // глобальная общая память эффектов
jeeui2 jee; // Создаем объект класса для работы с JeeUI2 фреймворком
LAMP myLamp;
Ticker _isrHelper;       // планировщик для обработки прерываний
#ifdef ESP_USE_BUTTON
Buttons myButtons;
#endif

void setup() {
    Serial.begin(115200);
#ifdef AUX_PIN
	pinMode(AUX_PIN, OUTPUT);
#endif
    jee.udp(); // Ответ на UDP запрс. в качестве аргумента - переменная, содержащая macid (по умолчанию)

#if defined(ESP8266) && defined(LED_BUILTIN_AUX) && !defined(__DISABLE_BUTTON0)
    jee.led(LED_BUILTIN_AUX, false); // назначаем пин на светодиод, который нам будет говорит о состоянии устройства. (быстро мигает - пытается подключиться к точке доступа, просто горит (или не горит) - подключен к точке доступа, мигает нормально - запущена своя точка доступа)
#elif defined(__DISABLE_BUTTON0)
    jee.led(LED_BUILTIN, false); // Если матрица находится на этом же пине, то будет ее моргание!
#endif

    jee.init();
    create_parameters(); // создаем дефолтные параметры, отсутствующие в текущем загруженном конфиге

    myLamp.effects.loadConfig();
    myLamp.events.loadConfig();

#ifdef ESP_USE_BUTTON
    if (!myButtons.loadConfig()) {
      default_buttons();
      myButtons.saveConfig();
    }
#endif

    jee.begin(); // Инициализируем JeeUI2 фреймворк.

#ifdef USE_FTP
    ftp_setup(); // запуск ftp-сервера
#endif

    sync_parameters();
    jee.mqtt(jee.param(F("m_host")), jee.param(F("m_port")).toInt(), jee.param(F("m_user")), jee.param(F("m_pass")), mqttCallback, true); // false - никакой автоподписки!!!

#ifdef ESP_USE_BUTTON
    attachInterrupt(digitalPinToInterrupt(BTN_PIN), buttonpinisr, BUTTON_PRESS_TRANSITION);  // цепляем прерывание на кнопку
#endif

    // восстанавливаем настройки времени
    myLamp.timeProcessor.tzsetup((jee.param(F("TZSET")).c_str()));
    myLamp.timeProcessor.setcustomntp((jee.param(F("userntp")).c_str()));
    myLamp.events.setEventCallback(event_worker);
    myLamp.timeProcessor.attach_callback(std::bind(&LAMP::setIsEventsHandled, &myLamp, true));
}

void loop() {
    jee.handle(); // цикл, необходимый фреймворку

    // TODO: Проконтроллировать и по возможности максимально уменьшить создание объектов на стеке
    myLamp.handle(); // цикл, обработка лампы

    // по-моему эта функция уже давно ничего по мкутт не отправляет
    //sendData(); // цикл отправки данных по MQTT
#ifdef USE_FTP
    ftp_loop(); // цикл обработки событий фтп-сервера
#endif
}

void mqttCallback(const String &topic, const String &payload){ // функция вызывается, когда приходят данные MQTT
  LOG(printf_P, PSTR("Message [%s - %s]\n"), topic.c_str() , payload.c_str());
}

// нужно подчистить эту функцию, печатать инфо можно более аккуратным способом
void sendData(){
  static unsigned long i;
  static unsigned int in;

  if(i + (in * 1000) > millis() || myLamp.getmqtt_int() == 0) return; // если не пришло время, или интервал = 0 - выходим из функции
  i = millis();
  in = myLamp.getmqtt_int();
  // всё, что ниже будет выполняться через интервалы


#ifdef ESP8266
  LOG(printf_P, PSTR("MQTT send data, MEM: %d, HF: %d, Time: %s\n"), ESP.getFreeHeap(), ESP.getHeapFragmentation(), myLamp.timeProcessor.getFormattedShortTime().c_str());
#else
  LOG(printf_P, PSTR("MQTT send data, MEM: %d, Time: %s\n"), ESP.getFreeHeap(), myLamp.timeProcessor.getFormattedShortTime().c_str());
#endif

}

#ifdef ESP_USE_BUTTON
/*
 * Используем обертку и тикер ибо:
 * 1) убираем функции с ICACHE из класса лампы
 * 2) Тикер не может дернуть нестатический метод класса
 */
void buttonhelper(bool state){
  myButtons.buttonPress(state);
}

/*
 *  Button pin interrupt handler
 */
ICACHE_RAM_ATTR void buttonpinisr(){
  detachInterrupt(BTN_PIN);
  _isrHelper.once_ms(0, buttonhelper, myButtons.getpinTransition());   // вместо флага используем тикер :)
  myButtons.setpinTransition(!myButtons.getpinTransition());
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), buttonpinisr, myButtons.getpinTransition() ? BUTTON_PRESS_TRANSITION : BUTTON_RELEASE_TRANSITION);  // меням прерывание
}
#endif
