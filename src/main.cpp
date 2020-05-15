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
#include "main.h"
#ifdef USE_FTP
#include "ftpServer.h"
#endif

// глобальные переменные для работы с ними в программе
SHARED_MEM GSHMEM; // глобальная общая память эффектов
INTRFACE_GLOBALS iGLOBAL; // объект глобальных переменных интерфейса
jeeui2 jee; // Создаем объект класса для работы с JeeUI2 фреймворком
LAMP myLamp;
Ticker _isrHelper;       // планировщик Смены эффектов в ДЕМО


void setup() {
    Serial.begin(115200);

    jee.udp(String(jee.mc)); // Ответ на UDP запрс. в качестве аргуиена - переменная, содержащая id по умолчанию
#if defined(ESP8266) && defined(LED_BUILTIN_AUX) && !defined(__DISABLE_BUTTON0)
    jee.led(LED_BUILTIN_AUX, false); // назначаем пин на светодиод, который нам будет говорит о состоянии устройства. (быстро мигает - пытается подключиться к точке доступа, просто горит (или не горит) - подключен к точке доступа, мигает нормально - запущена своя точка доступа)
#elif defined(__DISABLE_BUTTON0)
    jee.led(LED_BUILTIN, false); // Если матрица находится на этом же пине, то будет ее моргание!
#endif
    jee.ap(20000); // если в течении 20 секунд не удастся подключиться к Точке доступа - запускаем свою (параметр "wifi" сменится с AP на STA)

    myLamp.effects.loadConfig();
    myLamp.events.loadConfig();
    myLamp.updateParm(updateParm);

    jee.ui(interface); // обратный вызов - интерфейс
    jee.update(update); // обратный вызов - вызывается при введении данных в веб интерфейс, нужна для сравнения значений пременных с параметрами
    jee.httpCallback(httpCallback);
#ifdef LAMP_DEBUG
    jee.begin(true); // Инициализируем JeeUI2 фреймворк. Параметр bool определяет, показывать ли логи работы JeeUI2 (дебаг)
#else
    jee.begin(false); // Инициализируем JeeUI2 фреймворк. Параметр bool определяет, показывать ли логи работы JeeUI2 (дебаг)
#endif
#ifdef USE_FTP
    ftp_setup(); // запуск ftp-сервера
#endif
    create_parameters(); // создаем дефолтные параметры, отсутствующие в текущем загруженном конфиге
    update(); // этой функцией получаем значения параметров в переменные (обратный вызов UI)
    updateParm(); // вызвать обновление параметров UI (синхронизация с конфигом эффектов и кнопкой)
    if(myLamp.timeProcessor.getIsSyncOnline()){
      myLamp.refreshTimeManual(); // принудительное обновление времени
    }
    if(myLamp.timeProcessor.isDirtyTime())
      myLamp.setIsEventsHandled(false);
    myLamp.events.setEventCallback(event_worker);

    jee.mqtt(jee.param(F("m_host")), jee.param(F("m_port")).toInt(), jee.param(F("m_user")), jee.param(F("m_pass")), mqttCallback, true); // false - никакой автоподписки!!!
#ifdef ESP_USE_BUTTON
    attachInterrupt(digitalPinToInterrupt(BTN_PIN), buttonpinisr, BUTTON_PRESS_TRANSITION);  // цепляем прерывание на кнопку
#endif
}

void loop() {
    if(!jee.isLoading()){
      jee.handle(); // цикл, необходимый фреймворку
      
      // TODO: Проконтроллировать и по возможности максимально уменьшить создание объектов на стеке
      myLamp.handle(); // цикл, обработка лампы
      jeebuttonshandle();

      sendData(); // цикл отправки данных по MQTT
#ifdef USE_FTP
      ftp_loop(); // цикл обработки событий фтп-сервера
#endif
    }
}

void mqttCallback(const String &topic, const String &payload){ // функция вызывается, когда приходят данные MQTT
  LOG.printf_P(PSTR("Message [%s - %s]\n"), topic.c_str() , payload.c_str());
  //jee._refresh = true;
}

void sendData(){
  static unsigned long i;
  static unsigned int in;

  if(i + (in * 1000) > millis() || iGLOBAL.mqtt_int == 0) return; // если не пришло время, или интервал = 0 - выходим из функции
  i = millis();
  in = iGLOBAL.mqtt_int;
  // всё, что ниже будет выполняться через интервалы


#ifdef ESP8266
  LOG.printf_P(PSTR("MQTT send data, MEM: %d, HF: %d, Time: %s\n"), ESP.getFreeHeap(), ESP.getHeapFragmentation(), myLamp.timeProcessor.getFormattedShortTime().c_str());
#else
  LOG.printf_P(PSTR("MQTT send data, MEM: %d, Time: %s\n"), ESP.getFreeHeap(), myLamp.timeProcessor.getFormattedShortTime().c_str());
#endif
  //jee.publish(F("jee/set/BTN_bRefresh"),F("*"));
}

/*
 *  Button pin interrupt handler
 */
ICACHE_RAM_ATTR void buttonpinisr(){
  detachInterrupt(BTN_PIN);
  _isrHelper.once_ms(0, buttonhelper, pinTransition);   // вместо флага используем тикер :)
  iGLOBAL.pinTransition = !iGLOBAL.pinTransition;
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), buttonpinisr, iGLOBAL.pinTransition ? BUTTON_PRESS_TRANSITION : BUTTON_RELEASE_TRANSITION);  // меням прерывание
}

/*
 * Используем обертку и тикер ибо:
 * 1) TaskScheduler нельзя положить в ICACHE
 * 2) Тикер не может дернуть нестатический метод класса
 */
void buttonhelper(bool state){
  myLamp.buttonPress(state);
}