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
#include "main.h"

// глобальные переменные для работы с ними в программе
LAMP myLamp;
#ifdef ESP_USE_BUTTON
Buttons *myButtons;
#endif

#ifdef MP3PLAYER
MP3PLAYERDEVICE *mp3 = nullptr;
#endif

void setup() {
    //Serial.begin(115200);
    Serial.begin(460800);

#ifdef PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED
    {
        HeapSelectIram ephemeral;
        LOG(printf_P, PSTR("\n\nIRAM ESP.getFreeHeap:  %u\n"), ESP.getFreeHeap());
    }
    {
        HeapSelectDram ephemeral;
        LOG(printf_P, PSTR("DRAM ESP.getFreeHeap:  %u\n"), ESP.getFreeHeap());
    }
#else
    LOG(printf_P, PSTR("\n\nsetup: free heap  : %d\n"), ESP.getFreeHeap());
#endif

#ifdef ESP32
    LOG(printf_P, PSTR("setup: free PSRAM  : %d\n"), ESP.getFreePsram()); // 4194252
#endif

#ifdef AUX_PIN
	pinMode(AUX_PIN, OUTPUT);
#endif

#ifdef EMBUI_USE_UDP
    embui.udp(); // Ответ на UDP запрс. в качестве аргумента - переменная, содержащая macid (по умолчанию)
#endif

#if defined(ESP8266) && defined(LED_BUILTIN_AUX)
    embui.led(LED_BUILTIN_AUX, false); // назначаем пин на светодиод, который нам будет говорит о состоянии устройства. (быстро мигает - пытается подключиться к точке доступа, просто горит (или не горит) - подключен к точке доступа, мигает нормально - запущена своя точка доступа)
#else
    embui.led(LED_BUILTIN, false); // Если матрица находится на этом же пине, то будет ее моргание!
#endif

    // EmbUI
    embui.begin(); // Инициализируем EmbUI фреймворк - загружаем конфиг, запускаем WiFi и все зависимые от него службы
#ifdef EMBUI_USE_MQTT
    //embui.mqtt(embui.param(F("m_pref")), embui.param(F("m_host")), embui.param(F("m_port")).toInt(), embui.param(F("m_user")), embui.param(F("m_pass")), mqttCallback, true); // false - никакой автоподписки!!!
    embui.mqtt(mqttCallback, true);
#endif
    myLamp.effects.setEffSortType((SORT_TYPE)embui.param(FPSTR(TCONST_0050)).toInt()); // сортировка должна быть определена до заполнения
    myLamp.effects.initDefault(); // если вызывать из конструктора, то не забыть о том, что нужно инициализировать Serial.begin(115200); иначе ничего не увидеть!
    myLamp.events.loadConfig(); // << -- SDK3.0 будет падение, разобраться позже
#ifdef RTC
    rtc.init();
#endif

#ifdef DS18B20
    ds_setup();
#endif

#ifdef SHOWSYSCONFIG
    myLamp.lamp_init(embui.param(FPSTR(TCONST_0098)).toInt());
#else
    myLamp.lamp_init(CURRENT_LIMIT);
#endif
#ifdef USE_FTP
    ftp_setup(); // запуск ftp-сервера
#endif

#ifdef ESP_USE_BUTTON
#ifdef SHOWSYSCONFIG
    myLamp.setbPin(embui.param(FPSTR(TCONST_0097)).toInt());
    myButtons = new Buttons(myLamp.getbPin(), PULL_MODE, NORM_OPEN);
#else
    myButtons = new Buttons(BTN_PIN, PULL_MODE, NORM_OPEN);
#endif
    if (!myButtons->loadConfig()) {
      default_buttons();
      myButtons->saveConfig();
    }
#endif

    myLamp.events.setEventCallback(event_worker);

#ifdef MP3PLAYER
#ifdef SHOWSYSCONFIG
    int rxpin = embui.param(FPSTR(TCONST_009B)).isEmpty() ? MP3_RX_PIN : embui.param(FPSTR(TCONST_009B)).toInt();
    int txpin = embui.param(FPSTR(TCONST_009C)).isEmpty() ? MP3_TX_PIN : embui.param(FPSTR(TCONST_009C)).toInt();
    mp3 = new MP3PLAYERDEVICE(rxpin, txpin); //rxpin, txpin
#else
    mp3 = new MP3PLAYERDEVICE(MP3_RX_PIN, MP3_TX_PIN); //rxpin, txpin
#endif
#endif

#ifdef ESP8266
  embui.server.addHandler(new SPIFFSEditor(F("esp8266"),F("esp8266"), LittleFS));
#else
  embui.server.addHandler(new SPIFFSEditor(LittleFS, F("esp32"), F("esp32")));
#endif

  sync_parameters();        // падение есп32 не воспоизводится, kDn

  //embui.setPubInterval(5);   // change periodic WebUI publish interval from EMBUI_PUB_PERIOD to 5

#ifdef TM1637_CLOCK
  tm1637.tm_setup();
#endif 

#ifdef ENCODER
  enc_setup();
#endif

#if defined LED_BUILTIN && defined DISABLE_LED_BUILTIN
#ifdef ESP8266
    digitalWrite(LED_BUILTIN, HIGH); // "душим" светодиод nodeMCU
#else
    digitalWrite(LED_BUILTIN, LOW); // "душим" светодиод nodeMCU32
#endif
#endif

    LOG(println, F("setup() done"));
}   // End setup()


void loop() {
    embui.handle(); // цикл, необходимый фреймворку
    // TODO: Проконтроллировать и по возможности максимально уменьшить создание объектов на стеке
    myLamp.handle(); // цикл, обработка лампы

#ifdef USE_FTP
    ftp_loop(); // цикл обработки событий фтп-сервера
#endif

#ifdef ENCODER
    encLoop(); // цикл обработки событий энкодера. Эта функция будет отправлять в УИ изменения, только тогда, когда подошло время ее loop
#endif

#ifdef RTC
    rtc.updateRtcTime();
#endif

#ifdef TM1637_CLOCK
    EVERY_N_SECONDS(1) {
        tm1637.tm_loop();
    }
#endif
#ifdef DS18B20
    EVERY_N_MILLIS(1000*DS18B_READ_DELAY + 25) {
        ds_loop();
    }
#endif
#ifdef USE_STREAMING
    if (ledStream)
        ledStream->handle();
#endif
}

#ifdef EMBUI_USE_MQTT
ICACHE_FLASH_ATTR void mqttCallback(const String &topic, const String &payload){ // функция вызывается, когда приходят данные MQTT
  LOG(printf_P, PSTR("Message [%s - %s]\n"), topic.c_str() , payload.c_str());
  if(topic.startsWith(FPSTR(TCONST_00AC))){
    String sendtopic=topic;
    sendtopic.replace(FPSTR(TCONST_00AC), "");
    if(sendtopic==FPSTR(TCONST_00AE)){
        sendtopic=String(FPSTR(TCONST_008B))+sendtopic;
        String effcfg = myLamp.effects.getfseffconfig(myLamp.effects.getCurrent());
        embui.publish(sendtopic, effcfg, true); // отправляем обратно в MQTT в топик embui/pub/
    } else if(sendtopic==FPSTR(TCONST_00AD)){
        sendData();
    }
  }
}
#endif

// Periodic MQTT publishing
void sendData(){
    // Здесь отсылаем текущий статус лампы и признак, что она живая (keepalive)
    LOG(println, F("send MQTT Data :"));
    DynamicJsonDocument obj(256);
    //JsonObject obj = doc.to<JsonObject>();
    obj[FPSTR(TCONST_0001)] = String(embui.timeProcessor.getFormattedShortTime());
    obj[FPSTR(TCONST_0002)] = String(myLamp.getLampState().freeHeap);
    obj[FPSTR(TCONST_008F)] = String(embui.getUptime());
    obj[FPSTR(TCONST_00CE)] = String(myLamp.getLampState().rssi);
    String sendtopic=FPSTR(TCONST_008B);
    sendtopic+=FPSTR(TCONST_00AD);
    String out;
    serializeJson(obj, out);
    LOG(println, out);
#ifdef EMBUI_USE_MQTT
    embui.publish(sendtopic, out, true); // отправляем обратно в MQTT в топик embui/pub/
#endif
}
