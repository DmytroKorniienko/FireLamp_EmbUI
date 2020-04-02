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

static EFFECT *prevEffect = nullptr;
static bool isSetup = false;
static bool isTmSetup = false;
static bool isAddSetup = false;
static uint8_t addSList = 1;

void btnOTACallback()
{
    myLamp.startOTA();
}


void btnRefreshCallback()
{
    jee._refresh = true;
}

void btnFDelCallback()
{
    // Удаление конфигурации из ФС
    String filename = String(F("/cfg/"))+jee.param(F("fileName"));
    if(filename.length()>4)
        if(SPIFFS.begin()){
            SPIFFS.remove(filename);
        }
    isAddSetup = false;
    jee.var(F("isAddSetup"), F("false"));
    jee.var(F("fileName"),F(""));
    jee._refresh = true;
}

void btnFLoadCallback()
{
    // Загрузка сохраненной конфигурации эффектов вместо текущей
    String fn = jee.param(F("fileList"));
    myLamp.effects.loadConfig(fn.c_str());
    jee.var(F("fileName"),fn);
    jee._refresh = true;
}

void btnFSaveCallback()
{
    // Сохранение текущей конфигурации эффектов в ФС
    String filename = String(F("/cfg/"))+jee.param(F("fileName"));
    if(filename.length()>4)
        if(SPIFFS.begin()){
            // if(!SPIFFS.exists(F("/cfg")))
            //     SPIFFS.mkdir(F("/cfg"));
            myLamp.effects.saveConfig(filename.c_str());
        }
    isAddSetup = false;
    jee.var(F("isAddSetup"), F("false"));
    jee._refresh = true;
}

void btnTxtSendCallback()
{
    String tmpStr = jee.param(F("txtColor"));
    tmpStr.replace(F("#"),F("0x"));
    //LOG.printf("%s %d\n", tmpStr.c_str(), strtol(tmpStr.c_str(),NULL,0));
    myLamp.sendStringToLamp(jee.param(F("msg")).c_str(), (CRGB::HTMLColorCode)strtol(tmpStr.c_str(),NULL,0)); // вывести текст на лампу
}

void btnTmSubmCallback()
{
#ifdef LAMP_DEBUG
    LOG.println(F("btnTmSubmCallback pressed"));
#endif
    myLamp.timeProcessor.setTimezone(jee.param(F("timezone")).c_str());
    myLamp.timeProcessor.setTime(jee.param(F("time")).c_str());

    if(myLamp.timeProcessor.getIsSyncOnline()){
        myLamp.refreshTimeManual(); // принудительное обновление времени
    }
    isTmSetup = false;
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
    if(myLamp.getMode()!=MODE_DEMO)
        myLamp.startDemoMode();
    else
        myLamp.startNormalMode();

    // // Отладка
    // myLamp.sendStringToLamp(WiFi.localIP().toString().c_str(), CRGB::Blue);
    // myLamp.sendStringToLamp(WiFi.localIP().toString().c_str(), CRGB::Green);
    // myLamp.sendStringToLamp(WiFi.localIP().toString().c_str(), CRGB::Red);

    jee._refresh = true;
}

void jeebuttonshandle()
{
    static unsigned long timer;
    jee.btnCallback(F("btnTmSubm"), btnTmSubmCallback);
    jee.btnCallback(F("bMQTTform"), bMQTTformCallback); // MQTT form button
    jee.btnCallback(F("bDemo"), bDemoCallback);
    jee.btnCallback(F("btnTxtSend"), btnTxtSendCallback);
    jee.btnCallback(F("btnFLoad"), btnFLoadCallback);
    jee.btnCallback(F("btnFSave"), btnFSaveCallback);
    jee.btnCallback(F("btnFDel"), btnFDelCallback);
    jee.btnCallback(F("btnRefresh"), btnRefreshCallback);
    jee.btnCallback(F("btnOTA"), btnOTACallback);

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

}

void interface(){ // функция в которой мф формируем веб интерфейс
    if(!jee.isLoading()){
#ifdef LAMP_DEBUG
        LOG.println(F("Внимание: Создание интерфейса! Такие вызовы должны быть минимизированы."));
#endif
        jee.app(F(("Огненная лампа"))); // название программы (отображается в веб интерфейсе)

        // создаем меню
        jee.menu(F("Эффекты"));
        jee.menu(F("Лампа"));
        jee.menu(F("Настройки"));
        // создаем контент для каждого пункта меню

        jee.page(); // разделитель между страницами
        // Страница "Управление эффектами"

        EFFECT enEff; enEff.setNone();
        jee.checkbox(F("ONflag"),F("Включение&nbspлампы"));
        //char nameBuffer[64]; // Exception (3) при обращении к PROGMEM, поэтому ход конем - копируем во временный буффер
        do {
            enEff = *myLamp.effects.enumNextEffect(&enEff);
            if(enEff.eff_nb!=EFF_NONE && (enEff.canBeSelected || isSetup)){
                //strncpy_P(nameBuffer, enEff.eff_name, sizeof(nameBuffer)-1);
                //jee.option(String((int)enEff.eff_nb), nameBuffer);
                jee.option(String((int)enEff.eff_nb), FPSTR(enEff.eff_name));
            }
        } while((enEff.eff_nb!=EFF_NONE));
        jee.select(F("effList"), F("Эффект"));

        jee.range(F("bright"),1,255,1,F("Яркость"));
        jee.range(F("speed"),1,255,1,F("Скорость"));
        jee.range(F("scale"),1,255,1,F("Масштаб"));

        //jee.button(F("btn1"),F("gray"),F("<"), 1);
        if(myLamp.getMode()==MODE_DEMO)
            jee.button(F("bDemo"),F("green"),F("DEMO -> OFF"));
        else
            jee.button(F("bDemo"),F("gray"),F("DEMO -> ON"));
        //jee.button(F("btn3"),F("gray"),F(">"), 3);

        if(isSetup){
            jee.checkbox(F("canBeSelected"),F("В&nbspсписке&nbspвыбора"));
            jee.checkbox(F("isFavorite"),F("В&nbspсписке&nbspдемо"));  
        }
        jee.checkbox(F("isSetup"),F("Конфигурирование"));

        jee.page(); // разделитель между страницами
        //Страница "Управление лампой"
        if(isTmSetup){
            jee.time(F("time"),F("Время"));
            jee.number(F("tm_offs"), F("Смещение времени в секундах для NTP"));
            jee.text(F("timezone"),F("Часовой пояс (http://worldtimeapi.org/api/timezone/)"));
            jee.checkbox(F("isTmSync"),F("Включить&nbspсинхронизацию"));
            jee.button(F("btnTmSubm"),F("gray"),F("Сохранить"));
        } else {
            jee.pub(F("pTime"),F("Текущее время на ESP"),F("--:--"));
            jee.var(F("pTime"),myLamp.timeProcessor.getFormattedShortTime()); // обновить опубликованное значение
            jee.text(F("msg"),F("Текст для вывода на матрицу"));
            jee.color(F("txtColor"), F("Цвет сообщения"));
            jee.button(F("btnTxtSend"),F("gray"),F("Отправить"));
        }
        jee.checkbox(F("isTmSetup"),F("Настройка&nbspвремени"));

        jee.page(); // разделитель между страницами
        // Страница "Настройки соединения"
        if(!jee.connected || jee.param(F("wifi"))==F("AP")){
            jee.formWifi(); // форма настроек Wi-Fi
            jee.formMqtt(); // форма настроек MQTT
        }
        jee.checkbox(F("isAddSetup"),F("Расширенные&nbspнастройки"));
        if(isAddSetup){
            jee.option(F("1"), F("Конфигурации"));
            jee.option(F("2"), F("Время/Текст"));
            jee.option(F("3"), F("Другое"));
            jee.select(F("addSList"), F("Группа настроек"));
            switch (addSList)
            {
            case 1:
                jee.text(F("fileName"),F("Конфигурация"));
                jee.button(F("btnFSave"),F("green"),F("Записать в ФС"));
                jee.button(F("btnFDel"),F("red"),F("Удалить из ФС"));
                break;
            case 2:
                jee.number(F("ny_period"), F("Период вывода новогоднего поздравления в минутах (0 - не выводить)"));
                jee.number(F("ny_unix"), F("UNIX дата/время нового года"));
                jee.range(F("txtSpeed"),30,150,10,F("Задержка прокрутки текста"));
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
                jee.number(F("mqtt_int"), F("Интервал mqtt сек."));
                jee.checkbox(F("isGLBbr"),F("Глобальная&nbspяркость"));
                jee.checkbox(F("MIRR_H"),F("Отзеркаливание&nbspH"));
                jee.checkbox(F("MIRR_V"),F("Отзеркаливание&nbspV")); 
                jee.button(F("btnOTA"),(myLamp.getMode()==MODE_OTA?F("grey"):F("blue")),F("Обновление по ОТА-PIO"));   
                break;       
            case 4:
                break;       
            case 5:
                break;       
            default:
                break;
            }
        } else {
            if(SPIFFS.begin()){
#ifdef ESP32
                File root = SPIFFS.open("/cfg");
                File file = root.openNextFile();
#else
                Dir dir = SPIFFS.openDir(F("/cfg"));
#endif
                String fn;
#ifdef ESP32
                while (file) {
                    fn=file.name();
                    if(!file.isDirectory()){
#else                    
                while (dir.next()) {
                    fn=dir.fileName();
#endif

                        fn.replace(F("/cfg/"),F(""));
                        //LOG.println(fn);
                        jee.option(fn, fn);
#ifdef ESP32
                        file = root.openNextFile();
                    }
#endif
                }
            }
            String cfg(F("Конфигурации")); cfg+=" ("; cfg+=jee.param(F("fileList")); cfg+=")";
            jee.select(F("fileList"), cfg);

            jee.button(F("btnFLoad"),F("gray"),F("Считать с ФС"));
        }
        jee.page(); // разделитель между страницами
    } else {
#ifdef LAMP_DEBUG
        LOG.println(F("Внимание: Загрузка минимального интерфейса, т.к. обнаружен вызов index.htm"));
#endif
        jee.app(F(("Огненная лампа"))); // название программы (отображается в веб интерфейсе)

        // создаем меню
        jee.menu(F("Эффекты"));

        jee.page(); // разделитель между страницами
        jee.button(F("btnRefresh"),F("gray"),F("Обновить интерфейс"));
        jee.page(); // разделитель между страницами
    }
}

void update(){ // функция выполняется после ввода данных в веб интерфейсе. получение параметров из веб интерфейса в переменные
#ifdef LAMP_DEBUG
    LOG.println(F("In update..."));
#endif
    // получаем данные в переменную в ОЗУ для дальнейшей работы
    bool isRefresh = jee._refresh;
    EFFECT *curEff = myLamp.effects.getEffectBy((EFF_ENUM)jee.param(F("effList")).toInt());
    mqtt_int = jee.param(F("mqtt_int")).toInt();
    bool isGlobalBrightness = jee.param(F("isGLBbr"))==F("true");
    myLamp.setIsGlobalBrightness(isGlobalBrightness);

    if(isTmSetup!=(jee.param(F("isTmSetup"))==F("true"))){
        isTmSetup = !isTmSetup;
        isRefresh = true;
    }

    if(isAddSetup!=(jee.param(F("isAddSetup"))==F("true"))){
        isAddSetup = !isAddSetup;
        isRefresh = true;
    }

    if((jee.param(F("isSetup"))==F("true"))!=isSetup){
        isSetup = !isSetup;
        if(prevEffect!=nullptr)
            isRefresh = true;
    }

    myLamp.effects.moveBy(curEff->eff_nb);
    myLamp.restartDemoTimer();

    if(curEff->eff_nb!=EFF_NONE){
        if((curEff!=prevEffect  || isRefresh) && prevEffect!=nullptr){
            jee.var(F("isFavorite"), (curEff->isFavorite?F("true"):F("false")));
            jee.var(F("canBeSelected"), (curEff->canBeSelected?F("true"):F("false")));
            jee.var(F("bright"),String(myLamp.getLampBrightness()));
            jee.var(F("speed"),String(curEff->speed));
            jee.var(F("scale"),String(curEff->scale));
            jee.var(F("ONflag"), (myLamp.isLampOn()?F("true"):F("false")));

            isRefresh = true;
            myLamp.startFader(true);
        } else {
            curEff->isFavorite = (jee.param(F("isFavorite"))==F("true"));
            curEff->canBeSelected = (jee.param(F("canBeSelected"))==F("true"));
            myLamp.setLampBrightness(jee.param(F("bright")).toInt());
            curEff->speed = jee.param(F("speed")).toInt();
            curEff->scale = jee.param(F("scale")).toInt();
            
            myLamp.setLoading(true); // перерисовать эффект

            if(prevEffect!=nullptr){
                if(!myLamp.effects.autoSaveConfig()){ // отложенная запись, не чаще чем однократно в 30 секунд 
                    myLamp.ConfigSaveSetup(60*1000); //через минуту сработает еще попытка записи и так до успеха
                }
            }
        }
    }

    prevEffect = curEff;

    uint8_t cur_addSList = jee.param(F("addSList")).toInt();
    if(addSList!=cur_addSList){
        addSList = cur_addSList;
        isRefresh = true;
    }

    myLamp.setTextMovingSpeed(jee.param(F("txtSpeed")).toInt());
    myLamp.setTextOffset(jee.param(F("txtOf")).toInt());
    myLamp.setPeriodicTimePrint((PERIODICTIME)jee.param(F("perTime")).toInt());

    myLamp.setMIRR_H(jee.param(F("MIRR_H"))==F("true"));
    myLamp.setMIRR_V(jee.param(F("MIRR_V"))==F("true"));
    myLamp.setOnOff(jee.param(F("ONflag"))==F("true"));
    myLamp.timeProcessor.SetOffset(jee.param(F("tm_offs")).toInt());
    myLamp.setNYUnixTime(jee.param(F("ny_unix")).toInt());
    myLamp.setNYMessageTimer(jee.param(F("ny_period")).toInt());


    if(myLamp.getMode() == MODE_DEMO || isGlobalBrightness)
        jee.var(F("GlobBRI"), String(myLamp.getLampBrightness()));
    myLamp.timeProcessor.setIsSyncOnline(jee.param(F("isTmSync"))==F("true"));
    //jee.param(F("effList"))=String(0);
    jee.var(F("pTime"),myLamp.timeProcessor.getFormattedShortTime()); // обновить опубликованное значение

    jee.setDelayedSave(30000); // отложенное сохранение конфига, раз в 30 секунд относительно последнего изменения
    jee._refresh = isRefresh; // устанавливать в самом конце!
}

void updateParm() // передача параметров в UI после нажатия сенсорной или мех. кнопки
{
#ifdef LAMP_DEBUG
    LOG.println(F("Обновляем параметры после нажатия кнопки..."));
#endif
    EFFECT *curEff = myLamp.effects.getCurrent();

    if(myLamp.getMode() == MODE_DEMO || myLamp.IsGlobalBrightness())
        jee.var(F("GlobBRI"), String(myLamp.getLampBrightness()));
    else
        myLamp.setGlobalBrightness(jee.param(F("GlobBRI")).toInt());

    jee.var(F("bright"),String(myLamp.getLampBrightness()));
    jee.var(F("speed"),String(curEff->speed));
    jee.var(F("scale"),String(curEff->scale));
    jee.var(F("effList"),String(curEff->eff_nb));
    jee.var(F("ONflag"), (myLamp.isLampOn()?F("true"):F("false")));

    myLamp.setLoading(); // обновить эффект
    prevEffect = curEff; // обновить указатель на предыдущий эффект
    if(myLamp.getMode()!=MODE_DEMO)
        jee.save(); // Cохранить конфиг
    jee._refresh = true;
}