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
#include "effects.h"

#ifdef MIC_EFFECTS
void bmicCalCallback()
{
    if(!myLamp.isMicOnOff())
        myLamp.sendStringToLamp(String(F("Включите микрофон")).c_str(), CRGB::Red);
    else if(!iGLOBAL.isMicCal){
        myLamp.sendStringToLamp(String(F("Калибровка микрофона")).c_str(), CRGB::Red);
        myLamp.setMicCalibration();
        iGLOBAL.isMicCal = true;
    } else if(myLamp.isMicCalibration()){
        myLamp.sendStringToLamp(String(F("... в процессе ...")).c_str(), CRGB::Red);
    } else {
        jee.var(F("micScale"), String(myLamp.getMicScale()));
        jee.var(F("micNoise"), String(myLamp.getMicNoise()));
        iGLOBAL.isMicCal = false;
    }
    jee._refresh = true;
}
#endif

void bEventsCallback()
{
    myLamp.setIsEventsHandled(!myLamp.IsEventsHandled());
    jee._refresh = true;
}

void bSetCloseCallback()
{
    iGLOBAL.isAddSetup = false;
    jee.var("isAddSetup", "false");
    jee._refresh = true;
}

void bDelEventCallback(bool);
void bAddEventCallback();

void bOwrEventCallback()
{
    bDelEventCallback(false);
    bAddEventCallback();
}

void event_worker(const EVENT *event) // обработка эвентов лампы
{
    LOG.printf_P(PSTR("%s - %s\n"), ((EVENT *)event)->getName().c_str(), myLamp.timeProcessor.getFormattedShortTime().c_str());

    String filename;
    String tmpStr = jee.param(F("txtColor"));
    tmpStr.replace(F("#"),F("0x"));
    CRGB::HTMLColorCode color = (CRGB::HTMLColorCode)strtol(tmpStr.c_str(),NULL,0);

    switch (event->event)
    {
    case EVENT_TYPE::ON :
        myLamp.setOnOff(true);
        jee.var(F("ONflag"), (myLamp.isLampOn()?F("true"):F("false")));
        break;
    case EVENT_TYPE::OFF :
        myLamp.disableEffectsUntilText();
        myLamp.setOffAfterText();
        //if(event->message) myLamp.sendStringToLamp(event->message,color);
        jee.var(F("ONflag"), (F("false")));
        //return;
        break;
    case EVENT_TYPE::ALARM :
        myLamp.startAlarm();
        //return; // если не нужен вывод текста - раскомментировать
        break;
    case EVENT_TYPE::DEMO_ON :
        if(myLamp.getMode()!=MODE_DEMO || !myLamp.isLampOn())
            myLamp.startDemoMode();
        break;
    case EVENT_TYPE::LAMP_CONFIG_LOAD :
        filename=String(F("/glb/"));
        filename.concat(event->message);
        if(event->message)
            jee.load(filename.c_str());
        break;
    case EVENT_TYPE::EFF_CONFIG_LOAD :
        filename=String(F("/cfg/"));
        filename.concat(event->message);
        if(event->message)
            myLamp.effects.loadConfig(filename.c_str());    
        break;
    case EVENT_TYPE::EVENTS_CONFIG_LOAD :
        filename=String(F("/evn/"));
        filename.concat(event->message);
        if(event->message)
            myLamp.events.loadConfig(filename.c_str());
        break;
    case EVENT_TYPE::SEND_TEXT :
        if(event->message==nullptr)
            break;
        if(!myLamp.isLampOn()){
            myLamp.disableEffectsUntilText(); // будем выводить текст, при выкюченной матрице
            myLamp.setOffAfterText();
            myLamp.setOnOff(true);
            myLamp.setBrightness(1,false,false); // выводить будем минимальной яркостью myLamp.getNormalizedLampBrightness()
            myLamp.sendStringToLamp(event->message,color);
        } else {
            myLamp.sendStringToLamp(event->message,color);
        }
        return;
    case EVENT_TYPE::PIN_STATE : {
            if(event->message==nullptr)
                break;
            //LOG.printf_P(PSTR("TEST: %s\n"),src);
            String tmpS(event->message);
            tmpS.replace(F("'"),F("\"")); // так делать не красиво, но шопаделаешь...
            DynamicJsonDocument doc(128);
            deserializeJson(doc,tmpS);
            JsonArray arr = doc.as<JsonArray>();
            for (size_t i=0; i<arr.size(); i++) {
                JsonObject item = arr[i];
                uint8_t pin = item[F("pin")].as<int>();
                String action = item[F("act")].as<String>();
                //LOG.printf_P(PSTR("text: %s, pin: %d - %s\n"), tmpS.c_str(), pin, action.c_str());
                pinMode(pin, OUTPUT);
                switch(action.c_str()[0]){
                    case 'H':
                        digitalWrite(pin, HIGH); // LOW
                        break;
                    case 'L':
                        digitalWrite(pin, LOW); // LOW
                        break;
                    case 'T':
                        digitalWrite(pin, !digitalRead(pin)); // inverse
                        break;
                    default:
                        break;
                }
                char tmpbuffer[32];
                sprintf_P(tmpbuffer, PSTR("Set PIN: %d to %s"), pin, action.c_str());
                myLamp.sendStringToLamp(tmpbuffer,color);
                return;
            }
        }
        break;
    default:
        break;
    }
    if(event->message) myLamp.sendStringToLamp(event->message,color);
    jee._refresh = true;
}

void bEditEventCallback()
{
    EVENT *next = myLamp.events.getNextEvent(nullptr);
    int index = jee.param(F("evSelList")).toInt();
    int i = 1;
    if(!index) index=1;
    while (next!=nullptr)
    {
        //LOG.printf_P(PSTR("%d %d\n"), i, index);
        if(i==index) break;
        i++;
        next = myLamp.events.getNextEvent(next);
    }
    if(next==nullptr)
        return;

    jee.var(F("isEnabled"),(next->isEnabled?F("true"):F("false")));
    jee.var(F("d1"),(next->d1?F("true"):F("false")));
    jee.var(F("d2"),(next->d2?F("true"):F("false")));
    jee.var(F("d3"),(next->d3?F("true"):F("false")));
    jee.var(F("d4"),(next->d4?F("true"):F("false")));
    jee.var(F("d5"),(next->d5?F("true"):F("false")));
    jee.var(F("d6"),(next->d6?F("true"):F("false")));
    jee.var(F("d7"),(next->d7?F("true"):F("false")));
    jee.var(F("evList"),String(next->event));
    jee.var(F("repeat"),String(next->repeat));
    jee.var(F("stopat"),String(next->stopat));
    jee.var(F("msg"),String(next->message));
    jee.var(F("tmEvent"), next->getDateTime());
    iGLOBAL.isEdEvent = true;
    jee.var(F("isEdEvent"),F("true"));
    jee._refresh = true;
}

void bDelEventCallback(bool isRefresh)
{
    EVENT *next = myLamp.events.getNextEvent(nullptr);
    int index = jee.param(F("evSelList")).toInt();
    int i = 1;
    if(!index) index=1;
    while (next!=nullptr)
    {
        if(i==index) break;
        i++;
        next = myLamp.events.getNextEvent(next);
    }
    if(next!=nullptr)
        myLamp.events.delEvent(*next);
    myLamp.events.saveConfig();
    jee._refresh = isRefresh;
}

void bDelEventCallback()
{
    bDelEventCallback(true);
}

void bAddEventCallback()
{
    EVENT event;

    event.isEnabled=(jee.param(F("isEnabled"))==F("true"));
    event.d1=(jee.param(F("d1"))==F("true"));
    event.d2=(jee.param(F("d2"))==F("true"));
    event.d3=(jee.param(F("d3"))==F("true"));
    event.d4=(jee.param(F("d4"))==F("true"));
    event.d5=(jee.param(F("d5"))==F("true"));
    event.d6=(jee.param(F("d6"))==F("true"));
    event.d7=(jee.param(F("d7"))==F("true"));
    event.event=(EVENT_TYPE)jee.param(F("evList")).toInt();
    event.repeat=jee.param(F("repeat")).toInt();
    event.stopat=jee.param(F("stopat")).toInt();
    String tmEvent = jee.param(F("tmEvent"));
    time_t unixtime;
    tmElements_t tm;
    // Serial.println(tmEvent);
    // Serial.println(tmEvent.substring(0,4).c_str());
    tm.Year=atoi(tmEvent.substring(0,4).c_str())-1970;
    tm.Month=atoi(tmEvent.substring(5,7).c_str());
    tm.Day=atoi(tmEvent.substring(8,10).c_str());
    tm.Hour=atoi(tmEvent.substring(11,13).c_str());
    tm.Minute=atoi(tmEvent.substring(14,16).c_str());
    tm.Second=0;

    Serial.printf_P(PSTR("%d %d %d %d %d\n"), tm.Year, tm.Month, tm.Day, tm.Hour, tm.Minute);

    unixtime = makeTime(tm);
    event.unixtime = unixtime;
    String tmpMsg(jee.param(F("msg")));
    event.message = (char*)(tmpMsg.c_str());
    myLamp.events.addEvent(event);
    myLamp.events.saveConfig();
    iGLOBAL.isEdEvent = false;
    jee.var(F("isEdEvent"),F("false"));
    jee._refresh = true;
}

#ifdef OTA
void bOTACallback()
{
    myLamp.startOTA();
}
#endif

void bRefreshCallback()
{
    jee._refresh = true;
}

void bFDelCallback()
{
    // Удаление конфигурации из ФС
    String filename = String(F("/cfg/"))+jee.param(F("fileName"));
    if(filename.length()>4)
        if(SPIFFS.begin()){
            SPIFFS.remove(filename);
        }
    filename = String(F("/glb/"))+jee.param(F("fileName"));
    if(filename.length()>4)
        if(SPIFFS.begin()){
            SPIFFS.remove(filename);
        }
    filename = String(F("/evn/"))+jee.param(F("fileName"));
    if(filename.length()>4)
        if(SPIFFS.begin()){
            SPIFFS.remove(filename);
        }
    iGLOBAL.isAddSetup = false;
    jee.var(F("isAddSetup"), F("false"));
    jee.var(F("fileName"),F(""));
    jee._refresh = true;
}

void bFLoadCallback()
{
    // Загрузка сохраненной конфигурации эффектов вместо текущей
    String fn = jee.param(F("fileList"));
    myLamp.effects.loadConfig(fn.c_str());
    jee.var(F("fileName"),fn);
    jee._refresh = true;
}

void bFSaveCallback()
{
    // Сохранение текущей конфигурации эффектов в ФС
    String filename = String(F("/cfg/"))+jee.param(F("fileName"));
    if(filename.length()>4)
        if(SPIFFS.begin()){
            // if(!SPIFFS.exists(F("/cfg")))
            //     SPIFFS.mkdir(F("/cfg"));
            myLamp.effects.saveConfig(filename.c_str());
            filename = String(F("/glb/"))+jee.param(F("fileName"));
            jee.save(filename.c_str());
            filename = String(F("/evn/"))+jee.param(F("fileName"));
            myLamp.events.saveConfig(filename.c_str());
        }
    iGLOBAL.isAddSetup = false;
    jee.var(F("isAddSetup"), F("false"));
    jee._refresh = true;
}

void bTxtSendCallback()
{
    String tmpStr = jee.param(F("txtColor"));
    tmpStr.replace(F("#"),F("0x"));
    //LOG.printf("%s %d\n", tmpStr.c_str(), strtol(tmpStr.c_str(),NULL,0));
    myLamp.sendStringToLamp(jee.param(F("msg")).c_str(), (CRGB::HTMLColorCode)strtol(tmpStr.c_str(),NULL,0)); // вывести текст на лампу
}

void bTmSubmCallback()
{
#ifdef LAMP_DEBUG
    LOG.println(F("bTmSubmCallback pressed"));
#endif
    myLamp.timeProcessor.setTimezone(jee.param(F("timezone")).c_str());
    myLamp.timeProcessor.setTime(jee.param(F("time")).c_str());

    if(myLamp.timeProcessor.getIsSyncOnline()){
        myLamp.refreshTimeManual(); // принудительное обновление времени
    }
    iGLOBAL.isTmSetup = false;
    jee.var(F("isTmSetup"), F("false"));
    myLamp.sendStringToLamp(myLamp.timeProcessor.getFormattedShortTime().c_str(), CRGB::Green); // вывести время на лампу
    jee._refresh = true;
}

void bMQTTformCallback()
{
    jee.save();
    ESP.restart();
}

void bDemoCallback()
{
    if(myLamp.getMode()!=LAMPMODE::MODE_DEMO)
        myLamp.startDemoMode();
    else
        myLamp.startNormalMode();

    jee._refresh = true;
}

void jeebuttonshandle()
{
    static unsigned long timer;

    //публикация изменяющихся значений
    if (timer + 5*1000 > millis())
        return;
    timer = millis();
    jee.var(F("pTime"),myLamp.timeProcessor.getFormattedShortTime(), true); // обновить опубликованное значение
}

void create_parameters(){
#ifdef LAMP_DEBUG
    LOG.println(F("Создание дефолтных параметров"));
#endif
    // создаем дефолтные параметры для нашего проекта
    jee.var_create(F("wifi"), F("STA")); // режим работы WiFi по умолчанию ("STA" или "AP")  (параметр в энергонезависимой памяти)
    jee.var_create(F("ssid"), F("")); // имя точки доступа к которой подключаемся (параметр в энергонезависимой памяти)
    jee.var_create(F("pass"), F("")); // пароль точки доступа к которой подключаемся (параметр в энергонезависимой памяти)

    jee.var_create(F("m_host"), F("")); // Дефолтные настройки для MQTT
    jee.var_create(F("m_port"), F("1883"));
    jee.var_create(F("m_user"), F(""));
    jee.var_create(F("m_pass"), F(""));

    // параметры подключения к MQTT
    jee.var_create(F("mqtt_int"), F("30")); // интервал отправки данных по MQTT в секундах (параметр в энергонезависимой памяти)
    
    jee.var_create(F("effList"), F("1"));
    jee.var_create(F("isSetup"), F("false"));

    jee.var_create(F("bright"), F("127"));
    jee.var_create(F("speed"), F("127"));
    jee.var_create(F("scale"), F("127"));
    jee.var_create(F("canBeSelected"), F("true"));
    jee.var_create(F("isFavorite"), F("true"));

    jee.var_create(F("ONflag"), F("true"));
    jee.var_create(F("MIRR_H"), F("false"));
    jee.var_create(F("MIRR_V"), F("false"));
    jee.var_create(F("msg"), F(""));
    jee.var_create(F("txtColor"), F("#ffffff"));
    jee.var_create(F("txtSpeed"), F("100"));
    jee.var_create(F("txtOf"), F("0"));
    jee.var_create(F("perTime"), F("1"));

    jee.var_create(F("GlobBRI"), F("127"));

    jee.var_create(F("time"), F("00:00"));
    jee.var_create(F("timezone"), F(""));
    jee.var_create(F("tm_offs"), F("0"));
    jee.var_create(F("isTmSetup"), F("false"));
    jee.var_create(F("isTmSync"), F("true"));

    jee.var_create(F("isGLBbr"),F("false"));

    jee.var_create(F("isAddSetup"), F("false"));
    jee.var_create(F("fileName"), F("cfg1"));

    jee.var_create(F("ny_period"), F("0"));
    jee.var_create(F("ny_unix"), F("1609459200"));

    jee.var_create(F("addSList"),F("1"));
    jee.var_create(F("isEdEvent"),F("false"));
    jee.var_create(F("isEnabled"),F("true"));
    jee.var_create(F("tmEvent"),F(""));
    jee.var_create(F("repeat"),F("0"));
    jee.var_create(F("stopat"),F("0"));
    jee.var_create(F("d1"),F("false"));
    jee.var_create(F("d2"),F("false"));
    jee.var_create(F("d3"),F("false"));
    jee.var_create(F("d4"),F("false"));
    jee.var_create(F("d5"),F("false"));
    jee.var_create(F("d6"),F("false"));
    jee.var_create(F("d7"),F("false"));
    jee.var_create(F("evSelList"),F("1"));
    jee.var_create(F("evList"),F("1"));
    jee.var_create(F("fileList"),F("1"));

# ifdef MIC_EFFECTS
    jee.var_create(F("micScale"),F("1.28"));
    jee.var_create(F("micNoise"),F("0.00"));
    jee.var_create(F("micnRdcLvl"),F("0"));
    jee.var_create(F("isMicON"),F("true"));
#endif
    jee.var_create(F("param"),F(""));
    jee.var_create(F("extraR"),F("127"));
    jee.var_create(F("isFaderON"),(FADE==true?F("true"):F("false")));

#ifdef ESP_USE_BUTTON
    jee.var_create(F("isBtnOn"), F("true"));
#endif
    //-----------------------------------------------

    jee.btn_create(F("bTmSubm"), bTmSubmCallback);
    jee.btn_create(F("bMQTTform"), bMQTTformCallback); // MQTT form button
    jee.btn_create(F("bDemo"), bDemoCallback);
    jee.btn_create(F("bTxtSend"), bTxtSendCallback);
    jee.btn_create(F("bFLoad"), bFLoadCallback);
    jee.btn_create(F("bFSave"), bFSaveCallback);
    jee.btn_create(F("bFDel"), bFDelCallback);
    jee.btn_create(F("bRefresh"), bRefreshCallback);
#ifdef OTA
    jee.btn_create(F("bOTA"), bOTACallback);
#endif
    jee.btn_create(F("bAddEvent"), bAddEventCallback);
    jee.btn_create(F("bDelEvent"), bDelEventCallback);
    jee.btn_create(F("bEditEvent"), bEditEventCallback);
    jee.btn_create(F("bOwrEvent"), bOwrEventCallback);
    jee.btn_create(F("bSetClose"), bSetCloseCallback);
    jee.btn_create(F("bEvents"), bEventsCallback);
# ifdef MIC_EFFECTS
    jee.btn_create(F("bmicCal"), bmicCalCallback);
#endif
}

void interface(){ // функция в которой мф формируем веб интерфейс
#ifdef MIC_EFFECTS
    myLamp.setMicAnalyseDivider(0); // отключить микрофон на время прорисовки интерфейса
#endif
    if(!jee.isLoading()){
#ifdef LAMP_DEBUG
        LOG.println(F("Внимание: Создание интерфейса! Такие вызовы должны быть минимизированы."));
#endif
        jee.app(F(("Огненная лампа"))); // название программы (отображается в веб интерфейсе)

        // создаем меню
        jee.menu(F("Эффекты"));
        if(!iGLOBAL.isSetup){
            jee.menu(F("Лампа"));
            jee.menu(F("Настройки"));
        }
        // создаем контент для каждого пункта меню

        jee.page(); // разделитель между страницами
        // Страница "Управление эффектами"

        EFFECT enEff; enEff.setNone();
        jee.checkbox(F("ONflag"),F("Включение&nbspлампы"));
        jee.uiPush();       // не сбрасывать буфер перед page() после option(), это портит джейсон
        if(!iGLOBAL.isAddSetup){
            do {
                enEff = *myLamp.effects.enumNextEffect(&enEff);
                if(enEff.eff_nb!=EFF_NONE && (enEff.canBeSelected || iGLOBAL.isSetup)){
                    jee.option(String((int)enEff.eff_nb), FPSTR(enEff.eff_name));
                }
            } while((enEff.eff_nb!=EFF_NONE));
            jee.select(F("effList"), F("Эффект"));
        } else {
            jee.option(jee.param(F("effList")), F("Список эффектов отключен, выйдите из режима настройки!"));
            jee.select(F("effList"), F("Эффект"));
            jee.button(F("bSetClose"), F("gray"), F("Выйти из настроек"));
        }
        jee.uiPush();
        jee.range(F("bright"),1,255,1,F("Яркость"));
        jee.range(F("speed"),1,255,1,F("Скорость"));
        jee.range(F("scale"),1,255,1,F("Масштаб"));
        String v=myLamp.effects.getCurrent()->getValue(myLamp.effects.getCurrent()->param,F("R"));
        //LOG.printf_P(PSTR("\nJsonObject: %s\n"),v.c_str());
        if(!v.isEmpty())
            jee.range(F("extraR"),1,255,1,F("Доп. регулятор"));

        //jee.button(F("btn1"),F("gray"),F("<"), 1);
        if(myLamp.getMode()==MODE_DEMO)
            jee.button(F("bDemo"),F("green"),F("DEMO -> OFF"));
        else
            jee.button(F("bDemo"),F("gray"),F("DEMO -> ON"));
        //jee.button(F("btn3"),F("gray"),F(">"), 3);
        jee.checkbox(F("isSetup"),F("Конфигурирование"));
        if(iGLOBAL.isSetup){
            jee.checkbox(F("canBeSelected"),F("В&nbspсписке&nbspвыбора"));
            jee.checkbox(F("isFavorite"),F("В&nbspсписке&nbspдемо"));  
            jee.text(F("param"),F("Доп. параметры"));
            jee.page(); // разделитель между страницами
            jee.uiPush();
        }
        if(!iGLOBAL.isSetup){
            jee.page(); // разделитель между страницами
            jee.uiPush();
            //Страница "Управление лампой"
            if(iGLOBAL.isTmSetup){
                jee.time(F("time"),F("Время"));
                jee.number(F("tm_offs"), F("Смещение времени в секундах для NTP"));
                jee.text(F("timezone"),F("Часовой пояс (http://worldtimeapi.org/api/timezone/)"));
                jee.checkbox(F("isTmSync"),F("Включить&nbspсинхронизацию"));
                jee.button(F("bTmSubm"),F("gray"),F("Сохранить"));
            } else {
                jee.pub(F("pTime"),F("Текущее время на ESP"),F("--:--"));
                jee.var(F("pTime"),myLamp.timeProcessor.getFormattedShortTime()); // обновить опубликованное значение
                jee.text(F("msg"),F("Текст для вывода на матрицу"));
                jee.color(F("txtColor"), F("Цвет сообщения"));
                jee.button(F("bTxtSend"),F("gray"),F("Отправить"));
            }
            jee.checkbox(F("isTmSetup"),F("Настройка&nbspвремени"));

            jee.page(); // разделитель между страницами
            jee.uiPush();
            // Страница "Настройки соединения"
            // if(!jee.connected || jee.param(F("wifi"))==F("AP")){
            //     jee.formWifi(); // форма настроек Wi-Fi
            //     jee.formMqtt(); // форма настроек MQTT
            // }
            if(!jee.connected){
                iGLOBAL.isAddSetup = true;
                iGLOBAL.addSList = 4;
            }
            jee.checkbox(F("isAddSetup"),F("Расширенные&nbspнастройки"));
            if(iGLOBAL.isAddSetup){
                jee.option(F("1"), F("Конфигурации"));
                jee.option(F("2"), F("Время/Текст"));
                jee.option(F("3"), F("События"));
                jee.option(F("4"), F("Wifi & MQTT"));
# ifdef MIC_EFFECTS
                jee.option(F("8"), F("Микрофон"));
#endif
                jee.option(F("9"), F("Другое"));
                jee.select(F("addSList"), F("Группа настроек"));
                jee.uiPush();
                switch (iGLOBAL.addSList)
                {
                case 1:
                    jee.text(F("fileName"),F("Конфигурация"));
                    jee.button(F("bFSave"),F("green"),F("Записать в ФС"));
                    jee.button(F("bFDel"),F("red"),F("Удалить из ФС"));
                    break;
                case 2:
                    jee.number(F("ny_period"), F("Период вывода новогоднего поздравления в минутах (0 - не выводить)"));
                    jee.number(F("ny_unix"), F("UNIX дата/время нового года"));
                    jee.range(F("txtSpeed"),10,100,10,F("Задержка прокрутки текста"));
                    jee.range(F("txtOf"),-1,10,1,F("Смещение вывода текста"));
                    jee.option(String(PERIODICTIME::PT_NOT_SHOW), F("Не выводить"));
                    jee.option(String(PERIODICTIME::PT_EVERY_60), F("Каждый час"));
                    jee.option(String(PERIODICTIME::PT_EVERY_30), F("Каждые полчаса"));
                    jee.option(String(PERIODICTIME::PT_EVERY_15), F("Каждые 15 минут"));
                    jee.option(String(PERIODICTIME::PT_EVERY_10), F("Каждые 10 минут"));
                    jee.option(String(PERIODICTIME::PT_EVERY_5), F("Каждые 5 минут"));
                    jee.option(String(PERIODICTIME::PT_EVERY_1), F("Каждую минуту"));
                    jee.select(F("perTime"), F("Периодический вывод времени"));
                    break;       
                case 3:
                    jee.checkbox(F("isEdEvent"),F("Новое&nbspсобытие"));
                    if(jee.param(F("isEdEvent"))==F("true")){ // редактируем параметры событий
                        jee.option(String(EVENT_TYPE::ON), F("Включить лампу"));
                        jee.option(String(EVENT_TYPE::OFF), F("Выключить лампу"));
                        jee.option(String(EVENT_TYPE::DEMO_ON), F("Включить DEMO"));
                        jee.option(String(EVENT_TYPE::ALARM), F("Будильник"));
                        jee.option(String(EVENT_TYPE::LAMP_CONFIG_LOAD), F("Загрузка конф. лампы"));
                        jee.option(String(EVENT_TYPE::EFF_CONFIG_LOAD), F("Загрузка конф. эффектов"));
                        jee.option(String(EVENT_TYPE::EVENTS_CONFIG_LOAD), F("Загрузка конф. событий"));
                        jee.option(String(EVENT_TYPE::SEND_TEXT), F("Вывести текст"));
                        jee.option(String(EVENT_TYPE::PIN_STATE), F("Состояние пина"));
                        jee.select(F("evList"), F("Тип события"));
                        jee.checkbox(F("isEnabled"),F("Разрешено"));
                        jee.datetime(F("tmEvent"),F("Дата/время события"));
                        jee.number(F("repeat"),F("Повтор, мин"));
                        jee.number(F("stopat"),F("Остановить через, мин"));
                        jee.text(F("msg"),F("Параметр (текст)"));
                        jee.checkbox(F("d1"),F("Понедельник"));
                        jee.checkbox(F("d2"),F("Вторник"));
                        jee.checkbox(F("d3"),F("Среда"));
                        jee.checkbox(F("d4"),F("Четверг"));
                        jee.checkbox(F("d5"),F("Пятница"));
                        jee.checkbox(F("d6"),F("Суббота"));
                        jee.checkbox(F("d7"),F("Воскресенье"));
                        jee.button(F("bOwrEvent"),F("grey"),F("Обновить событие"));
                        jee.button(F("bAddEvent"),F("green"),F("Добавить событие"));
                    } else {
                        EVENT *next = myLamp.events.getNextEvent(nullptr);
                        int i = 1;
                        while (next!=nullptr)
                        {
                            jee.option(String(i), next->getName());
                            i++;
                            next = myLamp.events.getNextEvent(next);
                        }
                        jee.select(F("evSelList"), F("Событие"));
                        jee.button(F("bEditEvent"),F("green"),F("Редактировать событие"));
                        jee.button(F("bDelEvent"),F("red"),F("Удалить событие"));
                    }
                    break;      
                case 4:
                    jee.formWifi(); // форма настроек Wi-Fi
                    jee.formMqtt(); // форма настроек MQTT            
                    break;       
                case 5:
                    break;
# ifdef MIC_EFFECTS
                case 8:
                    if(!iGLOBAL.isMicCal){
                        jee.number(F("micScale"), F("Коэф. коррекции нуля"));
                        jee.number(F("micNoise"), F("Уровень шума, ед"));
                        jee.range(F("micnRdcLvl"), 0,4,1, F("Шумодав"));
                        jee.button(F("bmicCal"),F("red"), F("Калибровка микрофона"));
                    }
                    else {
                        jee.button(F("bmicCal"),F("grey"),F("Обновить"));
                    }
                    break;
#endif
                case 9:
                    jee.number(F("mqtt_int"), F("Интервал mqtt сек."));
                    jee.checkbox(F("isGLBbr"),F("Глобальная&nbspяркость"));
                    jee.checkbox(F("MIRR_H"),F("Отзеркаливание&nbspH"));
                    jee.checkbox(F("MIRR_V"),F("Отзеркаливание&nbspV"));
                    jee.checkbox(F("isFaderON"),F("Плавное&nbspпереключение&nbspэффектов"));
#ifdef ESP_USE_BUTTON
                    jee.checkbox(F("isBtnOn"), F("Кнопка&nbspактивна"));
#endif
#ifdef OTA
                    jee.button(F("bOTA"),(myLamp.getMode()==MODE_OTA?F("grey"):F("blue")),F("Обновление по ОТА-PIO"));   
#endif
                    break;      
                default:
                    break;
                }
            } else {
                if(SPIFFS.begin()){
                    File root = SPIFFS.open("/cfg");
                    File file = root.openNextFile();
                    String fn;

                    while (file) {
                        fn=file.name();
                        if(!file.isDirectory()){

                            fn.replace(F("/cfg/"),F(""));
                            //LOG.println(fn);
                            jee.option(fn, fn);
                            file = root.openNextFile();
                        }
                    }
                }
                String cfg(F("Конфигурации")); cfg+=" ("; cfg+=jee.param(F("fileList")); cfg+=")";
                jee.select(F("fileList"), cfg);

                jee.button(F("bFLoad"),F("gray"),F("Считать с ФС"));
                if(myLamp.IsEventsHandled())
                    jee.button(F("bEvents"),F("red"),F("EVENTS -> OFF"));
                else
                    jee.button(F("bEvents"),F("green"),F("EVENTS -> ON"));
# ifdef MIC_EFFECTS
                jee.checkbox(F("isMicON"), F("Микрофон"));
#endif
            }
            jee.page(); // разделитель между страницами
            jee.uiPush();
        }
    } else {
#ifdef LAMP_DEBUG
        LOG.println(F("Внимание: Загрузка минимального интерфейса, т.к. обнаружен вызов index.htm"));
#endif
        jee.app(F(("Огненная лампа"))); // название программы (отображается в веб интерфейсе)

        // создаем меню
        jee.menu(F("Эффекты"));

        jee.page(); // разделитель между страницами
        jee.button(F("bRefresh"),F("gray"),F("Обновить интерфейс"));
        jee.page(); // разделитель между страницами
    }
#ifdef MIC_EFFECTS
    myLamp.setMicAnalyseDivider(1); // восстановить делитель, при любой активности (поскольку эффекты могут его перенастраивать под себя)
#endif
}

// ??
//void setEffectParams(EFFECT *curEff);

void update(){ // функция выполняется после ввода данных в веб интерфейсе. получение параметров из веб интерфейса в переменные
#ifdef LAMP_DEBUG
    LOG.println(F("In update..."));
#endif
    // получаем данные в переменную в ОЗУ для дальнейшей работы
    bool isRefresh = jee._refresh;
    EFFECT *curEff = myLamp.effects.getEffectBy((EFF_ENUM)jee.param(F("effList")).toInt()); // если эффект поменялся, то строкой ниже - переход на него, если не менялся - то там же и останемся
    if(iGLOBAL.prevEffect==nullptr)
        myLamp.effects.moveBy(curEff->eff_nb); // переходим на выбранный эффект для начальной инициализации
    myLamp.restartDemoTimer(); // при любом изменении UI сбрасываем таймер ДЕМО режима и начинаем отсчет снова

    iGLOBAL.mqtt_int = jee.param(F("mqtt_int")).toInt();
    bool isGlobalBrightness = jee.param(F("isGLBbr"))==F("true");
    myLamp.setIsGlobalBrightness(isGlobalBrightness);

    if(iGLOBAL.isEdEvent!=(jee.param(F("isEdEvent"))==F("true"))){
        iGLOBAL.isEdEvent = !iGLOBAL.isEdEvent;
        isRefresh = true;
    }
    
    if(iGLOBAL.isTmSetup!=(jee.param(F("isTmSetup"))==F("true"))){
        iGLOBAL.isTmSetup = !iGLOBAL.isTmSetup;
        isRefresh = true;
    }

    if(iGLOBAL.isAddSetup!=(jee.param(F("isAddSetup"))==F("true"))){
        iGLOBAL.isAddSetup = !iGLOBAL.isAddSetup;
        isRefresh = true;
    }

    if((jee.param(F("isSetup"))==F("true"))!=iGLOBAL.isSetup){
        iGLOBAL.isSetup = !iGLOBAL.isSetup;
        if(iGLOBAL.prevEffect!=nullptr)
            isRefresh = true;
    }
 
    uint8_t cur_addSList = jee.param(F("addSList")).toInt();
    if(iGLOBAL.addSList!=cur_addSList){
        iGLOBAL.addSList = cur_addSList;
        isRefresh = true;
    }

    if(curEff->eff_nb!=EFF_NONE){ // для служебного "пустого" эффекта EFF_NONE вообще ничего не делаем
        //LOG.printf_P(PSTR("curEff: %p iGLOBAL.prevEffect: %p\n"), curEff, iGLOBAL.prevEffect);
        if(curEff!=iGLOBAL.prevEffect && iGLOBAL.prevEffect!=nullptr){ // Если эффект поменялся или требуется обновление UI, при этом не первый вход в процедуру после перезагрузки
            myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), curEff->eff_nb);
            isRefresh = true; // рефрешим UI если поменялся эффект, иначе все ползунки будут неправильными
        } else { // эффект не менялся, либо обновление UI не требуется, либо первый вход - обновляем текущий эффект значениями из UI
            curEff->isFavorite = (jee.param(F("isFavorite"))==F("true"));
            curEff->canBeSelected = (jee.param(F("canBeSelected"))==F("true"));
            myLamp.setLampBrightness(jee.param(F("bright")).toInt());
            if(myLamp.isLampOn()) // только если включена, поскольку этот вызов при перезагрузке зажжет лампу, даже если она выключена в конфиге
                myLamp.setBrightness(jee.param(F("bright")).toInt(), myLamp.getFaderFlag());    // два вызова выглядят коряво, но встраивать setBrightness в setLampBrightness нельзя, т.к. это корежит фэйдер и отложенную смену эфектов, можно попробовать наоборот сделать setBrightness будет менять яркость в конфиге эффекта
            curEff->speed = jee.param(F("speed")).toInt();
            curEff->scale = jee.param(F("scale")).toInt();

            //LOG.printf_P(PSTR("curEff->param=%p\n"),curEff->param);
            // Если руками правили строковый параметр - то обновляем его в эффекте, а дальше синхронизируем (нужно для возможности очистки)
            if(curEff->param==nullptr || strcmp_P(curEff->param, (jee.param(F("param"))).c_str())){ // различаются
                if(curEff->param==nullptr)
                    curEff->updateParam(("")); // для вновь добавленного эффекта сделаем очистку, а не копирование с предыдущего эффекта
                else
                    curEff->updateParam((jee.param(F("param"))).c_str());
            }
            String var = myLamp.effects.getCurrent()->getValue(myLamp.effects.getCurrent()->param, F("R"));
            if(!var.isEmpty()){
                myLamp.effects.getCurrent()->setValue(myLamp.effects.getCurrent()->param, F("R"), (jee.param(F("extraR"))).c_str());
                jee.var(F("param"),String(FPSTR(curEff->param)));
            }
            if(strcmp_P((jee.param(F("param"))).c_str(), curEff->param)){ // различаются  || (curEff->param==nullptr && (jee.param(F("param"))).length()!=0)
                curEff->updateParam((jee.param(F("param"))).c_str());
            }

            myLamp.setLoading(true); // перерисовать эффект

            if(iGLOBAL.prevEffect!=nullptr){
                if(!myLamp.effects.autoSaveConfig()){ // отложенная запись, не чаще чем однократно в 30 секунд 
                    myLamp.ConfigSaveSetup(60*1000); //через минуту сработает еще попытка записи и так до успеха
                }
            }
        }
    }

    iGLOBAL.prevEffect = curEff;

    myLamp.setTextMovingSpeed(jee.param(F("txtSpeed")).toInt());
    myLamp.setTextOffset(jee.param(F("txtOf")).toInt());
    myLamp.setPeriodicTimePrint((PERIODICTIME)jee.param(F("perTime")).toInt());

    myLamp.setMIRR_H(jee.param(F("MIRR_H"))==F("true"));
    myLamp.setMIRR_V(jee.param(F("MIRR_V"))==F("true"));
    myLamp.setOnOff(jee.param(F("ONflag"))==F("true"));
    myLamp.setFaderFlag(jee.param(F("isFaderON"))==F("true"));

#ifdef ESP_USE_BUTTON
    myLamp.setButtonOn(jee.param(F("isBtnOn"))==F("true"));
#endif

    myLamp.timeProcessor.SetOffset(jee.param(F("tm_offs")).toInt());
    myLamp.setNYUnixTime(jee.param(F("ny_unix")).toInt());
    myLamp.setNYMessageTimer(jee.param(F("ny_period")).toInt());

# ifdef MIC_EFFECTS
    myLamp.setMicScale(jee.param(F("micScale")).toFloat());
    myLamp.setMicNoise(jee.param(F("micNoise")).toFloat());
    myLamp.setMicNoiseRdcLevel((MIC_NOISE_REDUCE_LEVEL)jee.param(F("micnRdcLvl")).toInt());
    myLamp.setMicOnOff(jee.param(F("isMicON"))==F("true"));
#endif


    if(myLamp.getMode() == MODE_DEMO || isGlobalBrightness)
        jee.var(F("GlobBRI"), String(myLamp.getLampBrightness()));
    myLamp.timeProcessor.setIsSyncOnline(jee.param(F("isTmSync"))==F("true"));
    //jee.param(F("effList"))=String(0);
    jee.var(F("pTime"),myLamp.timeProcessor.getFormattedShortTime()); // обновить опубликованное значение

    jee.setDelayedSave(30000); // отложенное сохранение конфига, раз в 30 секунд относительно последнего изменения

#ifdef MIC_EFFECTS
    myLamp.setMicAnalyseDivider(1); // восстановить делитель, при любой активности (поскольку эффекты могут его перенастраивать под себя)
#endif

    jee._refresh = isRefresh; // устанавливать в самом конце!
}

void setEffectParams(EFFECT *curEff)
{
    if(curEff==nullptr){
        LOG.println(F("!!! Обнаружена передача нулевого указалетя эффекта !!!")); // ловим подлый баг :)
        return;
    }
    
    jee.var(F("isFavorite"), (curEff->isFavorite?F("true"):F("false")));
    jee.var(F("canBeSelected"), (curEff->canBeSelected?F("true"):F("false")));
    jee.var(F("bright"),String(myLamp.getLampBrightness()));
    jee.var(F("speed"),String(curEff->speed));
    jee.var(F("scale"),String(curEff->scale));
    //LOG.print(F("param: ")); LOG.println(FPSTR(curEff->param));

    if(curEff->param!=nullptr){
        size_t slen = strlen_P(curEff->param)+1;
        char buffer[slen]; buffer[0]='\0';
        strncpy_P(buffer, curEff->param, slen-1); // Обход Exeption 3, это шаманство из-за корявого использования указателя, он одновременно может быть и на PROGMEM, и на RAM
        jee.var(F("param"), buffer);     // но надо будет подумать о более красивом решении
    } else {
        jee.var(F("param"), F(""));     // но надо будет подумать о более красивом решении
    }
    jee.var(F("ONflag"), (myLamp.isLampOn()?F("true"):F("false")));

    jee.var(F("effList"),String(curEff->eff_nb));

    if(myLamp.getMode() == MODE_DEMO || myLamp.IsGlobalBrightness())
        jee.var(F("GlobBRI"), String(myLamp.getLampBrightness()));
    else
        myLamp.setGlobalBrightness(jee.param(F("GlobBRI")).toInt());
    myLamp.setLoading(); // обновить эффект
    iGLOBAL.prevEffect = curEff; // обновить указатель на предыдущий эффект

    if(myLamp.getMode()==LAMPMODE::MODE_DEMO)
        jee._refresh = true; // форсировать перерисовку интерфейсов клиентов
}

void updateParm() // передача параметров в UI после нажатия сенсорной или мех. кнопки
{
#ifdef LAMP_DEBUG
    LOG.println(F("Обновляем параметры после нажатия кнопки..."));
#endif
    EFFECT *curEff = myLamp.effects.getCurrent();
    setEffectParams(curEff);

    if(myLamp.getMode()!=MODE_DEMO)
        jee.save(); // Cохранить конфиг
    jee._refresh = true; // форсировать перерисовку интерфейсов клиентов
}

void httpCallback(const char *param, const char *value)
{
    EFFECT *curEff = myLamp.effects.getCurrent();
    
    LOG.printf_P("HTTP: %s - %s\n", param, value);
    if(!strcmp_P(param,PSTR("on"))){
        myLamp.setOnOff(true);
        jee.var(F("ONflag"), (myLamp.isLampOn()?F("true"):F("false")));
    } else if(!strcmp_P(param,PSTR("off"))){
        myLamp.setOnOff(false);
        jee.var(F("ONflag"), (myLamp.isLampOn()?F("true"):F("false")));
    } else if(!strcmp_P(param,PSTR("demo"))){
        myLamp.startDemoMode();
    } else if(!strcmp_P(param,PSTR("msg"))){
        myLamp.sendStringToLamp(value,CRGB::Green);
    } else if(!strcmp_P(param,PSTR("bright"))){
        if(atoi(value)>0){
            if(myLamp.getMode() == MODE_DEMO || myLamp.IsGlobalBrightness()){
                jee.var(F("GlobBRI"), value);
                //myLamp.setGlobalBrightness(atoi(value));
            } else {
                jee.var(F("bright"), value);
            }
            myLamp.setLampBrightness(atoi(value));
            myLamp.fadelight(myLamp.getNormalizedLampBrightness());
            //myLamp.setLoading(true); // перерисовать эффект
        }
    } else if(!strcmp_P(param,PSTR("speed"))){
        if(atoi(value)>0){
            jee.var(F("speed"), value);
            curEff->speed = atoi(value);
            myLamp.setLoading(true); // перерисовать эффект
        }
    } else if(!strcmp_P(param,PSTR("scale"))){
        if(atoi(value)>0){
            jee.var(F("scale"), value);
            curEff->scale = atoi(value);
            myLamp.setLoading(true); // перерисовать эффект
        }    
    } else if(!strcmp_P(param,PSTR("effect"))){
        if(atoi(value)>0){
            jee.var(F("effList"), value);
            myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), (EFF_ENUM)atoi(value));
        }
    } else if(!strcmp_P(param,PSTR("move_next"))){
        myLamp.switcheffect(SW_NEXT);
    } else if(!strcmp_P(param,PSTR("move_prev"))){
        myLamp.switcheffect(SW_PREV);
    } else if(!strcmp_P(param,PSTR("move_rnd"))){
        myLamp.switcheffect(SW_RND);
    } else if(!strcmp_P(param,PSTR("reboot"))){
        ESP.restart(); // так лучше :)
    } else if(!strcmp_P(param,PSTR("OTA"))){
        myLamp.startOTA();
    }
    jee._refresh = true;
}