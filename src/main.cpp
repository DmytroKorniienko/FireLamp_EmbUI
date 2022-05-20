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
#elif defined(LED_BUILTIN)
    embui.led(LED_BUILTIN, false); // Если матрица находится на этом же пине, то будет ее моргание!
#endif

#if defined(LED_BUILTIN) && defined (DISABLE_LED_BUILTIN)
#ifdef ESP8266
    digitalWrite(LED_BUILTIN, HIGH); // "душим" светодиод nodeMCU
#elif defined(LED_BUILTIN)
    digitalWrite(LED_BUILTIN, LOW); // "душим" светодиод nodeMCU32
#endif
#endif

    // EmbUI
    embui.begin(); // Инициализируем EmbUI фреймворк - загружаем конфиг, запускаем WiFi и все зависимые от него службы
#ifdef EMBUI_USE_MQTT
    //embui.mqtt(embui.param(F("m_pref")), embui.param(F("m_host")), embui.param(F("m_port")).toInt(), embui.param(F("m_user")), embui.param(F("m_pass")), mqttCallback, true); // false - никакой автоподписки!!!
    //embui.mqtt(mqttCallback, true);
    embui.mqtt(mqttCallback, mqttConnect, true);
#endif
    myLamp.effects.setEffSortType((SORT_TYPE)embui.param(FPSTR(TCONST_0050)).toInt()); // сортировка должна быть определена до заполнения
    myLamp.effects.initDefault(); // если вызывать из конструктора, то не забыть о том, что нужно инициализировать Serial.begin(115200); иначе ничего не увидеть!
    myLamp.events.loadConfig(); // << -- SDK3.0+ помилка пов'язана з unsigned long long на деяких esp32
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

    LOG(println, F("setup() done"));
}   // End setup()


void loop() {
    embui.handle(); // цикл, необходимый фреймворку
    // TODO: Проконтроллировать и по возможности максимально уменьшить создание объектов на стеке
    myLamp.handle(); // цикл, обработка лампы

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

//------------------------------------------

#ifdef EMBUI_USE_MQTT
// реализация autodiscovery
String ha_autodiscovery()
{
    LOG(println,F("MQTT: Autodiscovery"));
    DynamicJsonDocument hass_discover(1024);
    String name = embui.param(FPSTR(P_hostname));
    String unique_id = embui.mc;

    hass_discover[F("~")] = embui.id(FPSTR(TCONST_00E7));     // embui.param(FPSTR(P_m_pref)) + F("/embui/")
    hass_discover[F("name")] = name;                // name
    hass_discover[F("uniq_id")] = unique_id;        // String(ESP.getChipId(), HEX); // unique_id

    hass_discover[F("avty_t")] = F("~pub/online");  // availability_topic
    hass_discover[F("pl_avail")] = F("1");          // payload_available
    hass_discover[F("pl_not_avail")] = F("0");      // payload_not_available

    hass_discover[F("cmd_t")] = F("~set/on");       // command_topic
    hass_discover[F("stat_t")] = F("~pub/on");      // state_topic
    hass_discover[F("pl_on")] = F("1");             // payload_on
    hass_discover[F("pl_off")] = F("0");            // payload_off

    hass_discover[F("json_attr_t")] = F("~pub/state"); // json_attributes_topic

    hass_discover[F("rgb_cmd_t")] = "~set/RGB";        // rgb_command_topic
    hass_discover[F("rgb_stat_t")] = "~pub/RGB";       // rgb_state_topic

    hass_discover[F("bri_cmd_t")] = F("~set/g_bright");     // brightness_command_topic
    hass_discover[F("bri_stat_t")] = F("~pub/dynCtrl0");    // brightness_state_topic
    hass_discover[F("bri_scl")] = 255;

    JsonArray data = hass_discover.createNestedArray(F("effect_list"));
    data.add(FPSTR(TCONST_00E8));
    data.add(FPSTR(TCONST_00E9));
    data.add(FPSTR(TCONST_00EA));
    data.add(FPSTR(TCONST_00EB));
    data.add(FPSTR(TCONST_00EC));
    data.add(FPSTR(TCONST_00ED));

    hass_discover[F("fx_cmd_t")] = F("~set/modecmd");                              // effect_command_topic
    hass_discover[F("fx_stat_t")] = F("~pub/state");                               // effect_state_topic
    hass_discover[F("fx_val_tpl")] = F("{{ value_json.Mode }}");                   // effect_value_template effect_template

    //---------------------


    hass_discover[F("clr_temp_cmd_t")] = F("~set/COLORTEMP");     // speed as color temperature
    hass_discover[F("clr_temp_stat_t")] = F("~pub/COLORTEMP");    // speed as color temperature
    hass_discover[F("min_mireds")] = 1;
    hass_discover[F("max_mireds")] = 255;

    // JsonObject devobj = hass_discover.createNestedObject(F("dev"));
    // //JsonArray devids = devobj.createNestedArray(F("ids"));
    // //devids.add()
    // devobj[F("name")] = F("Firelamp");
    // devobj[F("mdl")] = F("Firelamp");
    // devobj[F("sw")] = F("2.7.0");
    // devobj[F("mf")] = F("kDn");
    

    // "\"dev\":{"
    //   "\"ids\":[\"%s\"],"                                //clientId
    //   "\"name\":\"%s\","                              //host
    //   "\"mdl\":\"%s\","                                 //host
    //   "\"sw\":\"%s\","                                  //version
    //   "\"mf\":\"lg\""
    // "}"


    // hass_discover[F("whit_val_cmd_t")] = F("~set/scale");     // scale as white level (Яркость белого)
    // hass_discover[F("whit_val_stat_t")] = F("~pub/scale");    // scale as white level
    // hass_discover[F("whit_val_scl")] = 255;

    // hass_discover[F("xy_cmd_t")] = F("~set/speed");     // scale as white level (Яркость белого)
    // hass_discover[F("xy_stat_t")] = F("~pub/speed");    // scale as white level
    //hass_discover[F("whit_val_scl")] = 255; // 'xy_val_tpl':          'xy_value_template',

    String hass_discover_str;
    serializeJson(hass_discover, hass_discover_str);
    hass_discover.clear();

    embui.publishto(String(F("homeassistant/light/")) + name + F("/config"), hass_discover_str, true);
    return hass_discover_str;
}

extern void mqtt_dummy_connect();
void mqttConnect(){ 
    mqtt_dummy_connect();
    ha_autodiscovery();
}

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

// Periodic MQTT publishing
void sendData(){
    // Здесь отсылаем текущий статус лампы и признак, что она живая (keepalive)
    DynamicJsonDocument obj(512);
    //JsonObject obj = doc.to<JsonObject>();
    obj[FPSTR(TCONST_00DD)] = myLamp.getModeDesc();
    obj[FPSTR(TCONST_00EE)] = myLamp.effects.getEffectName();
    obj[FPSTR(TCONST_00DE)] = String(embui.timeProcessor.getFormattedShortTime());
    obj[FPSTR(TCONST_00DF)] = String(myLamp.getLampState().freeHeap);
    obj[FPSTR(TCONST_00E0)] = String(embui.getUptime());
    obj[FPSTR(TCONST_00E1)] = String(myLamp.getLampState().rssi);
    obj[FPSTR(TCONST_00E2)] = WiFi.localIP().toString();
    obj[FPSTR(TCONST_00E3)] = WiFi.macAddress();
    obj[FPSTR(TCONST_00E4)] = String(F("http://"))+WiFi.localIP().toString();
    obj[FPSTR(TCONST_00E5)] = embui.getEmbUIver();
    obj[FPSTR(TCONST_00E6)] = embui.id(FPSTR(TCONST_00E7));     // embui.param(FPSTR(P_m_pref)) + F("/embui/")
    String sendtopic=FPSTR(TCONST_008B);
    sendtopic+=FPSTR(TCONST_00AD);
    String out;
    serializeJson(obj, out);
    LOG(println, F("send MQTT Data :"));
    LOG(println, out);
    embui.publish(sendtopic, out, true); // отправляем обратно в MQTT в топик embui/pub/
}
#endif

