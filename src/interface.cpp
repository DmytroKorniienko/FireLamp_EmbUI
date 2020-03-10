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

void parameters(){
    // создаем параметры для нашего проекта
    jee.var(F("wifi"), F("STA")); // режим работы WiFi по умолчанию ("STA" или "AP")  (параметр в энергонезависимой памяти)
    jee.var(F("ssid"), F("")); // имя точки доступа к которой подключаемся (параметр в энергонезависимой памяти)
    jee.var(F("pass"), F("")); // пароль точки доступа к которой подключаемся (параметр в энергонезависимой памяти)

    // параметры подключения к MQTT
    jee.var(F("mqtt_int"), F("30")); // интервал отправки данных по MQTT в секундах (параметр в энергонезависимой памяти)
    jee.var(F("effList"), F("1"));
    jee.var(F("isSetup"), F("false"));

}

void interface(){ // функция в которой мф формируем веб интерфейс
    jee.app(F(("Огненная лампа"))); // название программы (отображается в веб интерфейсе)

    // создаем меню
    jee.menu(F("Управление эффектами"));
    jee.menu(F("Управление лампой"));
    jee.menu(F("Настройки соединения"));
    // создаем контент для каждого пункта меню

    jee.page(); // разделитель между страницами
    // Страница "Управление эффектами"

    EFFECT enEff; enEff.setNone();
    jee.checkbox(F("isSetup"),F("Setup"));

    do {
        enEff = myLamp.effects.enumNextEffect(enEff);
        if(enEff.eff_nb!=EFF_NONE && (enEff.canBeSelected || jee.param(F("isSetup"))==F("true"))){
            jee.option(String((int)enEff.eff_nb),enEff.eff_name);
        }
    } while((enEff.eff_nb!=EFF_NONE));
    jee.select(F("effList"), F("Эффект"));

    jee.range(F("bright"),1,255,1,F("Яркость"));
    jee.range(F("speed"),1,255,1,F("Скорость"));
    jee.range(F("scale"),1,255,1,F("Масштаб"));

    if(jee.param(F("isSetup"))==F("true")){
        jee.checkbox(F("canBeSelected"),F("В&nbspсписке&nbspвыбора"));
        jee.checkbox(F("isFavorite"),F("В&nbspсписке&nbspдемо"));  
    }

    jee.page(); // разделитель между страницами
    //Страница "Управление лампой"
    jee.checkbox(F("ONflag"),F("Включение&nbspлампы"));
    jee.checkbox(F("MIRR_H"),F("Инверсия&nbspH"));
    jee.checkbox(F("MIRR_V"),F("Инверсия&nbspV"));

    jee.page(); // разделитель между страницами
    // Страница "Настройки соединения"
    if(!jee.connected || jee.param(F("wifi"))==F("AP")){
        jee.formWifi(); // форма настроек Wi-Fi
        jee.formMqtt(); // форма настроек MQTT
    }
    jee.number(F("mqtt_int"), F("Интервал mqtt сек."));
    //jee.pub(F("test"),F("Тут заголовок и все такое"),F("---"),F(" единиц"));
    //jee.var(F("test"),String(777)); // обновить опубликованное значение
    
    jee.page(); // разделитель между страницами
}

void update(){ // функция выполняется после ввода данных в веб интерфейсе. получение параметров из веб интерфейса в переменные
    // получаем данные в переменную в ОЗУ для дальнейшей работы
    bool isRefresh = false;
    static EFFECT *prevEffect = nullptr;
    static bool ispSetup = false;
    EFFECT *curEff = myLamp.effects.getEffectBy((EFF_ENUM)jee.param(F("effList")).toInt());
    mqtt_int = jee.param(F("mqtt_int")).toInt();

    if((jee.param(F("isSetup"))==F("true"))!=ispSetup){
        ispSetup = !ispSetup;
        if(prevEffect!=nullptr)
            isRefresh = true;
    }

    myLamp.effects.moveBy(curEff->eff_nb);
    myLamp.restartDemoTimer();

    if(curEff->eff_nb!=EFF_NONE){
        if((curEff!=prevEffect && prevEffect!=nullptr) || isRefresh){
            jee.var(F("isFavorite"), (curEff->isFavorite?F("true"):F("false")));
            jee.var(F("canBeSelected"), (curEff->canBeSelected?F("true"):F("false")));
            //jee.var(F("bright"),String(curEff->brightness));
            jee.var(F("bright"),String(myLamp.getLampBrightness()));
            jee.var(F("speed"),String(curEff->speed));
            jee.var(F("scale"),String(curEff->scale));
            jee.var(F("ONflag"), (myLamp.isLampOn()?F("true"):F("false")));

            //LOG.printf_P("%s , передали %d %d\n",curEff->eff_name,curEff->isFavorite,curEff->canBeSelected);
            isRefresh = true;
            myLamp.startFader(true);
        } else {
            curEff->isFavorite = (jee.param(F("isFavorite"))==F("true"));
            curEff->canBeSelected = (jee.param(F("canBeSelected"))==F("true"));
            //curEff->brightness = jee.param(F("bright")).toInt();
            myLamp.setLampBrightness(jee.param(F("bright")).toInt());
            curEff->speed = jee.param(F("speed")).toInt();
            curEff->scale = jee.param(F("scale")).toInt();
            
            myLamp.setLoading(true); // перерисовать эффект
            //LOG.printf_P("%s , получили %d %d\n",curEff->eff_name,curEff->isFavorite,curEff->canBeSelected);

            if(prevEffect!=nullptr){
                if(!myLamp.effects.autoSaveConfig()){ // отложенная запись, не чаще чем однократно в 30 секунд 
                    myLamp.ConfigSaveSetup(60*1000); //через минуту сработает еще попытка записи и так до успеха
                }
            }
        }
    }
#ifdef LAMP_DEBUG
    LOG.println(F("In update..."));
#endif
    prevEffect = curEff;

    myLamp.setMIRR_H(jee.param(F("MIRR_H"))==F("true"));
    myLamp.setMIRR_V(jee.param(F("MIRR_V"))==F("true"));
    myLamp.setOnOff(jee.param(F("ONflag"))==F("true"));
    //jee.param(F("effList"))=String(0);
    //jee.var(F("test"),String(777));

    jee._refresh = isRefresh; // устанавливать в самом конце!
}

void updateParm() // передача параметров в UI после нажатия сенсорной или мех. кнопки
{
#ifdef LAMP_DEBUG
    LOG.println(F("Обновляем параметры после нажатия кнопки..."));
#endif
    EFFECT *curEff = myLamp.effects.getCurrent();
    jee.var(F("bright"),String(myLamp.getLampBrightness()));
    jee.var(F("speed"),String(curEff->speed));
    jee.var(F("scale"),String(curEff->scale));
    jee.var(F("effList"),String(curEff->eff_nb));
    jee.var(F("ONflag"), (myLamp.isLampOn()?F("true"):F("false")));
    jee._refresh = true;
}