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
#include "interface.h"
#include "effects.h"
#include "ui.h"

#ifdef AUX_PIN
void AUX_toggle(bool key)
{
    if (key)
    {
        digitalWrite(AUX_PIN, AUX_LEVEL);
        jee.var(F("AUX"), (F("true")));
    }
    else
    {
        digitalWrite(AUX_PIN, !AUX_LEVEL);
        jee.var(F("AUX"), (F("false")));
    }
}
#endif

void pubCallback(Interface *interf){
    interf->json_frame_value();
    interf->value(F("pTime"), myLamp.timeProcessor.getFormattedShortTime(), true);
    interf->value(F("pMem"), String(ESP.getFreeHeap()), true);
    interf->json_frame_flush();
}

void block_menu(Interface *interf, JsonObject *data){
    // создаем меню
    interf->json_section_menu();

    interf->option(F("effects"), F("Эффекты"));
    interf->option(F("lamp"), F("Лампа"));
    interf->option(F("settings"), F("Настройки"));

    interf->json_section_end();
}

static EFFECT *confEff = nullptr;
void block_effects_config_param(Interface *interf, JsonObject *data){
    if (!interf || !confEff) return;

    interf->json_section_begin(F("set_effect_prm"));
    interf->checkbox(F("eff_sel"), confEff->canBeSelected? F("true") : F("false"), F("В списке выбора"), false);
    interf->checkbox(F("eff_fav"), confEff->isFavorite? F("true") : F("false"), F("В списке демо"), false);
    interf->text(F("param"), confEff->param, F("Доп. параметры"));

    interf->button_submit(F("set_effect_prm"), F("Сохранить"), F("gray"));
    interf->json_section_end();
}

void show_effects_config_param(Interface *interf, JsonObject *data){
    interf->json_frame_interface();
    block_effects_config_param(interf, data);
    interf->json_frame_flush();
}

void set_effects_config_param(Interface *interf, JsonObject *data){
    if (!interf || !confEff || !data) return;

    if (data->containsKey(F("param"))) {
       myLamp.effects.updateParam(confEff, (*data)[F("param")]);
    }
    confEff->canBeSelected = ((*data)[F("eff_sel")] == F("true"));
    confEff->isFavorite = ((*data)[F("eff_fav")] == F("true"));

    // myLamp.effects.saveConfig();
}

void block_effects_config(Interface *interf, JsonObject *data){
    if (!interf) return;

    interf->json_section_main(F("effects_config"), F("Конфигурирование"));

    EFFECT enEff; enEff.setNone();
    confEff = myLamp.effects.enumNextEffect(&enEff);

    interf->select(F("effListConf"), String((int)enEff.eff_nb), F("Эффект"), true);
    while ((enEff = *myLamp.effects.enumNextEffect(&enEff)).eff_nb != EFF_NONE) {
        interf->option(String((int)enEff.eff_nb), FPSTR(enEff.eff_name));
    }
    interf->json_section_end();

    block_effects_config_param(interf, nullptr);

    interf->spacer();
    interf->button(F("effects"), F("Выход"));
    interf->json_section_end();
}

void show_effects_config(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_effects_config(interf, data);
    interf->json_frame_flush();
}

void set_effects_config_list(Interface *interf, JsonObject *data){
    if (!interf || !data) return;
    EFF_ENUM num = (EFF_ENUM)(*data)[F("effListConf")];
    confEff = myLamp.effects.getEffectBy(num);
    show_effects_config_param(interf, data);
}

void block_effects_param(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_begin(F("effects_param"));
    if (myLamp.IsGlobalBrightness() || myLamp.getMode() == MODE_DEMO) {
        interf->range(F("bright"), myLamp.getNormalizedLampBrightness(), 1, 255, 1, F("Глобальная яркость"), true);
    } else {
        interf->range(F("bright"), myLamp.getNormalizedLampBrightness(), 1, 255, 1, F("Яркость"), true);
    }
    interf->range(F("speed"), myLamp.effects.getSpeedS(), 1, 255, 1, F("Скорость"), true);
    interf->range(F("scale"), myLamp.effects.getScaleS(), 1, 255, 1, F("Масштаб"), true);

    String v = myLamp.effects.getValue(myLamp.effects.getSelected()->param, F("R"));
    if (!v.isEmpty()) {
        interf->range(FPSTR(extraR), (int)v.toInt(), 1, 255, 1, F("Доп. регулятор"), true);
    }
    interf->json_section_end();
}

void show_effects_param(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_effects_param(interf, data);
    interf->json_frame_flush();
}

void set_effects_list(Interface *interf, JsonObject *data){
    if (!data) return;
    EFF_ENUM num = (EFF_ENUM)(*data)[F("effList")];
    EFF_ENUM curr = myLamp.effects.getSelected()->eff_nb;
    EFFECT *curEff = myLamp.effects.getEffectBy(num);
    if (!curEff) return;

    LOG(printf_P, PSTR("EFF LIST n:%d, o:%d, on:%d, md:%d\n"), curEff->eff_nb, curr, myLamp.isLampOn(), myLamp.getMode());
    if (curEff->eff_nb != curr) {
        if (!myLamp.isLampOn()) {
            myLamp.effects.moveBy(curEff->eff_nb); // переходим на выбранный эффект для начальной инициализации
        } else {
            myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), curEff->eff_nb);
        }

        jee.var(F("effList"), (*data)[F("effList")]);
    }

    show_effects_param(interf, data);
}

void set_effects_bright(Interface *interf, JsonObject *data){
    if (!data) return;

    myLamp.setLampBrightness((*data)[F("bright")]);
    myLamp.setBrightness(myLamp.getNormalizedLampBrightness(), !((*data)[F("nofade")]));
    if (myLamp.IsGlobalBrightness()) {
        jee.var("GlobBRI", (*data)[F("bright")]);
    }
    LOG(printf_P, PSTR("Новое значение яркости: %d\n"), myLamp.getNormalizedLampBrightness());

    myLamp.demoTimer(T_RESET);
}

void set_effects_speed(Interface *interf, JsonObject *data){
    if (!data) return;

    myLamp.effects.setSpeedS((*data)[F("speed")]);
    LOG(printf_P, PSTR("Новое значение скорости: %d\n"), myLamp.effects.getSpeedS());

    myLamp.demoTimer(T_RESET);
}

void set_effects_scale(Interface *interf, JsonObject *data){
    if (!data) return;

    myLamp.effects.setScaleS((*data)[F("scale")]);
    LOG(printf_P, PSTR("Новое значение масштаба: %d\n"), myLamp.effects.getScaleS());

    myLamp.demoTimer(T_RESET);
}

void set_effects_extra(Interface *interf, JsonObject *data){
    if (!data) return;

    const char *extra = (*data)[FPSTR(extraR)].as<String>().c_str();
    myLamp.effects.setValue(myLamp.effects.getSelected()->param, F("R"), extra);
    LOG(printf_P, PSTR("Новое значение R: %s\n"), extra);

    myLamp.demoTimer(T_RESET);
}

void block_main_flags(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_line(F("flags"));
    interf->checkbox(F("ONflag"), myLamp.isLampOn()? F("true") : F("false"), F("Включить"), true);
    interf->checkbox(F("Demo"), (myLamp.getMode() == MODE_DEMO)? F("true") : F("false"), F("Демо"), true);
    interf->checkbox(F("GBR"), myLamp.IsGlobalBrightness()? F("true") : F("false"), F("Гл.Яркость"), true);
    interf->checkbox(F("Events"), myLamp.IsEventsHandled()? F("true") : F("false"), F("События"), true);
#ifdef MIC_EFFECTS
    interf->checkbox(F("Mic"), F("Микр."), true);
#else
    interf->hidden("nil");
#endif
#ifdef AUX_PIN
    interf->checkbox(F("AUX"), F("AUX"), true);
#else
    interf->hidden("nil");
#endif
    interf->json_section_end();
}

void show_main_flags(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_main_flags(interf, data);
    interf->spacer();
    interf->button(F("effects"), F("Выход"));
    interf->json_frame_flush();
}

void block_effects_main(Interface *interf, JsonObject *data){
    // Страница "Управление эффектами"
    if (!interf) return;
    interf->json_section_main(F("effects"), F("Эффекты"));

    interf->json_section_line(F("flags"));
    interf->checkbox(F("ONflag"), myLamp.isLampOn()? F("true") : F("false"), F("Включить"), true);
    interf->button(F("show_flags"), F("Режимы"));
    interf->json_section_end();

    EFFECT enEff; enEff.setNone();
    interf->select(F("effList"), F("Эффект"), true);
    while ((enEff = *myLamp.effects.enumNextEffect(&enEff)).eff_nb != EFF_NONE) {
        if (enEff.canBeSelected) {
            interf->option(String((int)enEff.eff_nb), FPSTR(enEff.eff_name));
        }
    }
    interf->json_section_end();

    block_effects_param(interf, data);

    interf->button(F("effects_config"), F("Конфигурирование"));

    interf->json_section_end();
}

void set_onflag(Interface *interf, JsonObject *data){
    if (!data) return;
    bool newpower = (*data)[F("ONflag")] == F("true");
    if (newpower != myLamp.isLampOn()) {
        if (newpower) {
            // включаем через switcheffect, т.к. простого isOn недостаточно чтобы запустить фейдер и поменять яркость (при необходимости)
            myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), myLamp.effects.getEn());
        } else {
            myLamp.changePower(newpower);
        }
    }
#ifdef RESTORE_STATE
    jee.var(F("ONflag"), (*data)[F("ONflag")]);
#endif
}

void set_demoflag(Interface *interf, JsonObject *data){
    if (!data) return;
    // Специально не сохраняем, считаю что демо при старте не должно запускаться
    bool newdemo = (*data)[F("Demo")] == F("true");
    switch (myLamp.getMode()) {
        case MODE_NORMAL:
            if (newdemo) myLamp.startDemoMode(jee.param(F("DTimer")).toInt()); break;
        case MODE_DEMO:
            if (!newdemo) myLamp.startNormalMode(); break;
        default:;
    }
#ifdef RESTORE_STATE
    jee.var(F("Demo"), (*data)[F("Demo")]);
#endif
}

#ifdef OTA
void set_otaflag(Interface *interf, JsonObject *data){
    myLamp.startOTA();

    interf->json_frame_interface();
    interf->json_section_content();
    interf->button(F("OTA"), F("Обновление по ОТА-PIO"), F("grey"));
    interf->json_section_end();
    interf->json_frame_flush();
}
#endif

#ifdef AUX_PIN
void set_auxflag(Interface *interf, JsonObject *data){
    if (!data) return;
    if (((*data)[F("AUX")] == F("true")) != (digitalRead(AUX_PIN) == AUX_LEVEL ? true : false)) {
        AUX_toggle(!(digitalRead(AUX_PIN) == AUX_LEVEL ? true : false));
    }
}
#endif

void set_gbrflag(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(F("GBR"), myLamp.setIsGlobalBrightness((*data)[F("GBR")] == F("true")));
    if (myLamp.isLampOn()) {
        myLamp.setBrightness(myLamp.getNormalizedLampBrightness());
    }
}

void block_lamp_config(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_hidden(F("lamp_config"), F("Конфигурации"));

    interf->json_section_begin(F("edit_lamp_config"));
    String cfg(F("Конфигурации")); cfg+=" ("; cfg+=jee.param(F("fileName")); cfg+=")";

    interf->select(F("fileName"), cfg);
    if(LittleFS.begin()){
#ifdef ESP32
        File root = LittleFS.open("/cfg");
        File file = root.openNextFile();
#else
        Dir dir = LittleFS.openDir(F("/cfg"));
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
            //LOG(println, fn);
            interf->option(fn, fn);
#ifdef ESP32
            file = root.openNextFile();
        }
#endif
        }
    }
    interf->json_section_end();

    interf->json_section_line();
    interf->button_submit_value(F("edit_lamp_config"), F("load"), F("Загрузить"), F("green"));
    interf->button_submit_value(F("edit_lamp_config"), F("save"), F("Сохранить"));
    interf->button_submit_value(F("edit_lamp_config"), F("del"), F("Удалить"), F("red"));
    interf->json_section_end();

    interf->json_section_end();

    interf->spacer();
    interf->json_section_begin(F("add_lamp_config"));
    interf->text(F("fileName"), F("Конфигурация"));
    interf->button_submit(F("add_lamp_config"), F("Создать"));
    interf->json_section_end();

    interf->json_section_end();
}

void show_lamp_config(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_lamp_config(interf, data);
    interf->json_frame_flush();
}

void edit_lamp_config(Interface *interf, JsonObject *data){
    // Удаление конфигурации из ФС
    if (!data || !data->containsKey(F("fileName"))) return;
    String name = (*data)[F("fileName")];
    String act = (*data)[F("edit_lamp_config")];

    if (act == "del") {
        String filename = String(F("/glb/")) + name;
        if (LittleFS.begin()) LittleFS.remove(filename);

        filename = String(F("/cfg/")) + name;
        if (LittleFS.begin()) LittleFS.remove(filename);

        filename = String(F("/evn/")) + name;
        if (LittleFS.begin()) LittleFS.remove(filename);

    } else
    if (act == "load") {
        String filename = String(F("/glb/")) + name;
        jee.load(filename.c_str());

        filename = String(F("/cfg/")) + name;
        myLamp.effects.loadConfig(filename.c_str());

        filename = String(F("/evn/")) + name;
        myLamp.events.loadConfig(filename.c_str());

        jee.var(F("fileName"), name);
    } else {
        String filename = String(F("/glb/")) + name;
        jee.save(filename.c_str(), true);

        filename = String(F("/cfg/")) + name;
        myLamp.effects.saveConfig(filename.c_str());

        filename = String(F("/evn/")) + name;
        myLamp.events.saveConfig(filename.c_str());
    }

    show_lamp_config(interf, data);
}

void block_lamp_textsend(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_begin(F("textsend"));

    interf->spacer(F("Вывести на лампе"));
    interf->text(F("msg"), F("Текст"));
    interf->color(F("txtColor"), F("Цвет сообщения"));
    interf->button_submit(F("textsend"), F("Отправить"), F("gray"));

    interf->json_section_end();
}

void set_lamp_textsend(Interface *interf, JsonObject *data){
    if (!data) return;
    String tmpStr = (*data)[F("txtColor")];
    jee.var(F("txtColor"), tmpStr);
    jee.var(F("msg"), (*data)[F("msg")]);

    tmpStr.replace(F("#"), F("0x"));
    myLamp.sendStringToLamp((*data)[F("msg")], (CRGB::HTMLColorCode)strtol(tmpStr.c_str(), NULL, 0));
}

void block_lamp(Interface *interf, JsonObject *data){
    //Страница "Управление лампой"
    if (!interf) return;
    interf->json_section_main(F("lamp"), F("Лампа"));

    block_lamp_textsend(interf, data);
    block_lamp_config(interf, data);

    interf->json_section_end();
}

#ifdef MIC_EFFECTS
void block_settings_mic(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(F("set_mic"), F("Микрофон"));

    interf->checkbox(F("Mic"), F("Микрофон"), true);
    if (!iGLOBAL.isMicCal) {
        interf->number(F("micScale"), F("Коэф. коррекции нуля"));
        interf->number(F("micNoise"), F("Уровень шума, ед"));
        interf->range(F("micnRdcLvl"), 0, 4, 1, F("Шумодав"));
    }
    interf->button_submit(F("set_mic"), F("Сохранить"), F("grey"));
    interf->spacer();
    interf->button(F("mic_cal"), F("Калибровка микрофона"), iGLOBAL.isMicCal? F("grey") : F("red"));

    interf->spacer();
    interf->button(F("settings"), F("Выход"));

    interf->json_section_end();
}

void show_settings_mic(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_mic(interf, data);
    interf->json_frame_flush();
}

void set_settings_mic(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(F("micScale"), myLamp.setMicScale((*data)[F("micScale")].as<float>()));
    SETPARAM(F("micNoise"), myLamp.setMicNoise((*data)[F("micNoise")].as<float>()));
    SETPARAM(F("micnRdcLvl"), myLamp.setMicNoiseRdcLevel((MIC_NOISE_REDUCE_LEVEL)(*data)[F("micNoise")].as<long>()));
}

void set_micflag(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(F("Mic"), myLamp.setMicOnOff((*data)[F("Mic")] == F("true")));
}

void set_settings_mic_calib(Interface *interf, JsonObject *data){
    if (!myLamp.isMicOnOff()) {
        myLamp.sendStringToLamp(String(F("Включите микрофон")).c_str(), CRGB::Red);
    } else
    if(!iGLOBAL.isMicCal) {
        myLamp.sendStringToLamp(String(F("Калибровка микрофона")).c_str(), CRGB::Red);
        myLamp.setMicCalibration();
        iGLOBAL.isMicCal = true;
    } else
    if (myLamp.isMicCalibration()) {
        myLamp.sendStringToLamp(String(F("... в процессе ...")).c_str(), CRGB::Red);
    } else {
        jee.var(F("micScale"), String(myLamp.getMicScale()));
        jee.var(F("micNoise"), String(myLamp.getMicNoise()));
        iGLOBAL.isMicCal = false;
    }
}
#endif

void block_settings_wifi(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(F("settings_wifi"), F("WiFi"));
    // форма настроек Wi-Fi
    interf->json_section_hidden(F("set_wifi"), F("WiFi"));
    interf->text(F("ap_ssid"), F("AP/mDNS"));
    interf->text(F("ssid"), F("SSID"));
    interf->password(F("pass"), F("Password"));
    interf->button_submit(F("set_wifi"), F("Connect"), F("gray"));
    interf->json_section_end();

    // форма настроек MQTT
    interf->json_section_hidden(F("set_mqtt"), F("MQTT"));
    interf->text(F("m_host"), F("MQTT host"));
    interf->number(F("m_port"), F("MQTT port"));
    interf->text(F("m_user"), F("User"));
    interf->text(F("m_pass"), F("Password"));
    interf->number(F("mqtt_int"), F("Интервал mqtt сек."));
    interf->button_submit(F("set_mqtt"), F("Connect"), F("gray"));
    interf->json_section_end();

    interf->spacer();
    interf->button(F("settings"), F("Выход"));

    interf->json_section_end();
}

void show_settings_wifi(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_wifi(interf, data);
    interf->json_frame_flush();
}

void set_settings_wifi(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(F("ap_ssid"), jee.param(F("ap_ssid")).c_str());
    SETPARAM(F("ssid"), jee.param(F("ssid")).c_str());
    SETPARAM(F("pass"), jee.param(F("pass")).c_str());

    jee.var(F("wifi"), F("STA"));
    jee.save();
    ESP.restart();
}

void set_settings_mqtt(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(F("m_host"), strncpy(jee.m_host, jee.param(F("m_host")).c_str(), sizeof(jee.m_host)-1));
    SETPARAM(F("m_user"), strncpy(jee.m_user, jee.param(F("m_user")).c_str(), sizeof(jee.m_user)-1));
    SETPARAM(F("m_pass"), strncpy(jee.m_pass, jee.param(F("m_pass")).c_str(), sizeof(jee.m_pass)-1));
    SETPARAM(F("m_port"), jee.m_port = jee.param(F("m_port")).toInt());
    SETPARAM(F("mqtt_int"), iGLOBAL.mqtt_int = (*data)[F("mqtt_int")]);
    //m_pref

    jee.save();
    ESP.restart();
}

void block_settings_other(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(F("set_other"), F("Разные"));

    interf->checkbox(F("MIRR_H"), F("Отзеркаливание H"));
    interf->checkbox(F("MIRR_V"), F("Отзеркаливание V"));
    interf->checkbox(F("isFaderON"), F("Плавное переключение эффектов"));
    interf->checkbox(F("DRand"), F("Случайный эффект в Демо"));
    interf->range(F("DTimer"), 30, 250, 10, F("Время в секундах для смены режима"));
#ifdef ESP_USE_BUTTON
    interf->checkbox(F("isBtnOn"), F("Кнопка активна"));
#endif

    interf->spacer(F("Вывод текста"));
    interf->range(F("txtSpeed"), 10, 100, 10, F("Задержка прокрутки текста"));
    interf->range(F("txtOf"), -1, 10, 1, F("Смещение вывода текста"));

    interf->select(F("perTime"), F("Периодический вывод времени"));
    interf->option(String(PERIODICTIME::PT_NOT_SHOW), F("Не выводить"));
    interf->option(String(PERIODICTIME::PT_EVERY_60), F("Каждый час"));
    interf->option(String(PERIODICTIME::PT_EVERY_30), F("Каждые полчаса"));
    interf->option(String(PERIODICTIME::PT_EVERY_15), F("Каждые 15 минут"));
    interf->option(String(PERIODICTIME::PT_EVERY_10), F("Каждые 10 минут"));
    interf->option(String(PERIODICTIME::PT_EVERY_5), F("Каждые 5 минут"));
    interf->option(String(PERIODICTIME::PT_EVERY_1), F("Каждую минуту"));
    interf->json_section_end();

    interf->spacer(F("Новогоднее поздравление"));
    interf->number(F("ny_period"), F("Период вывода в минутах (0 - не выводить)"));
    interf->number(F("ny_unix"), F("UNIX дата/время нового года"));

    interf->button_submit(F("set_other"), F("Сохранить"), F("grey"));

    interf->spacer();
    interf->button(F("settings"), F("Выход"));

    interf->json_section_end();
}

void show_settings_other(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_other(interf, data);
    interf->json_frame_flush();
}

void set_settings_other(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(F("MIRR_H"), myLamp.setMIRR_H((*data)[F("MIRR_H")] == F("true")));
    SETPARAM(F("MIRR_V"), myLamp.setMIRR_V((*data)[F("MIRR_V")] == F("true")));
    SETPARAM(F("isFaderON"), myLamp.setFaderFlag((*data)[F("isFaderON")] == F("true")));
    SETPARAM(F("DRand"), jee.param(F("DRand")).c_str());
    SETPARAM(F("DTimer"), ({if (myLamp.getMode() == MODE_DEMO){ myLamp.demoTimer(T_DISABLE); myLamp.demoTimer(T_ENABLE, jee.param(F("DTimer")).toInt()); }}));

#ifdef ESP_USE_BUTTON
    SETPARAM(F("isBtnOn"), myLamp.setButtonOn((*data)[F("isBtnOn")] == F("true")));
#endif

    SETPARAM(F("txtSpeed"), myLamp.setTextMovingSpeed((*data)[F("txtSpeed")]));
    SETPARAM(F("txtOf"), myLamp.setTextOffset((*data)[F("txtOf")]));
    SETPARAM(F("perTime"), myLamp.setPeriodicTimePrint((PERIODICTIME)(*data)[F("perTime")].as<long>()));
    SETPARAM(F("ny_period"), myLamp.setNYMessageTimer((*data)[F("ny_period")]));
    SETPARAM(F("ny_unix"), myLamp.setNYUnixTime((*data)[F("ny_unix")]));
}

void block_settings_time(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(F("set_time"), F("Настройка времени"));

    interf->time(F("time"), F("Время"));
    interf->number(F("tm_offs"), F("Смещение времени в секундах для NTP"));
    interf->text(F("timezone"), F("Часовой пояс (http://worldtimeapi.org/api/timezone/)"));
    interf->checkbox(F("isTmSync"), F("Включить синхронизацию"));
    interf->button_submit(F("set_time"), F("Сохранить"), F("gray"));

    interf->spacer();
    interf->button(F("settings"), F("Выход"));

    interf->json_section_end();
}

void show_settings_time(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_time(interf, data);
    interf->json_frame_flush();
}

void set_settings_time(Interface *interf, JsonObject *data){
    if (!data) return;
/*
    // пока решаем чего хотим в этом месте
    SETPARAM(F("tm_offs"), myLamp.timeProcessor.setOffset((*data)[F("tm_offs")]));
    SETPARAM(F("timezone"), myLamp.timeProcessor.setTimezone((*data)[F("timezone")]));
    SETPARAM(F("time"), myLamp.timeProcessor.setTime((*data)[F("time")]));
    SETPARAM(F("isTmSync"), myLamp.timeProcessor.setIsSyncOnline((*data)[F("isTmSync")] == F("true")));

    if (myLamp.timeProcessor.getIsSyncOnline()) {
        myLamp.refreshTimeManual(); // принудительное обновление времени
    }
*/
    myLamp.sendStringToLamp(myLamp.timeProcessor.getFormattedShortTime().c_str(), CRGB::Green);
}

void block_settings_update(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_hidden(F("update"), F("Обновление"));
#ifdef OTA
    interf->spacer(F("Обновление по ОТА-PIO"));
    if (myLamp.getMode() != MODE_OTA) {
        interf->button(F("OTA"), F("Начать"), F("blue"));
    } else {
        interf->button(F("OTA"), F("Включено"), F("grey"));
    }
#endif
    interf->spacer(F("Загрузка прошивки"));
    interf->file(F("update"), F("update"), F("Upload"));
    interf->json_section_end();
}

void block_settings_event(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(F("show_event"), F("События"));

    interf->checkbox(F("Events"), myLamp.IsEventsHandled()? F("true") : F("false"), F("События"), true);

    int num = 1;
    EVENT *next = nullptr;
    while ((next = myLamp.events.getNextEvent(next)) != nullptr) {
        String name = "evconf" + String(num);
        interf->json_section_begin(name, next->getName(), false, false, true);
        interf->button_submit_value(name, F("edit"), F("Редактировать"), F("green"));
        interf->button_submit_value(name, F("del"), F("Удалить"), F("red"));
        interf->hidden(F("event"), String(num));
        interf->json_section_end();
        ++num;
    }

    interf->button(F("event_conf"), F("Добавить событие"));

    interf->spacer();
    interf->button(F("settings"), F("Выход"));

    interf->json_section_end();
}

void show_settings_event(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_event(interf, data);
    interf->json_frame_flush();
}

void set_eventflag(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(F("Events"), myLamp.setIsEventsHandled((*data)[F("Events")] == F("true")));
}

void set_event_conf(Interface *interf, JsonObject *data){
    EVENT event;
    String act;
    if (!data) return;

    if (data->containsKey(F("event"))) {
        EVENT *curr = nullptr;
        int i = 1, num = (*data)[F("event")];
        while ((curr = myLamp.events.getNextEvent(curr)) && i != num) ++i;
        if (!curr) return;
        myLamp.events.delEvent(*curr);
    }

    if (data->containsKey(F("enabled"))) {
        event.isEnabled = ((*data)[F("enabled")] == F("true"));
    } else {
        event.isEnabled = true;
    }

    event.d1 = ((*data)[F("d1")] == F("true"));
    event.d2 = ((*data)[F("d2")] == F("true"));
    event.d3 = ((*data)[F("d3")] == F("true"));
    event.d4 = ((*data)[F("d4")] == F("true"));
    event.d5 = ((*data)[F("d5")] == F("true"));
    event.d6 = ((*data)[F("d6")] == F("true"));
    event.d7 = ((*data)[F("d7")] == F("true"));
    event.event = (EVENT_TYPE)(*data)[F("evList")].as<long>();
    event.repeat = (*data)[F("repeat")];
    event.stopat = (*data)[F("stopat")];
    String tmEvent = (*data)[F("tmEvent")];

    struct tm t;
    tm *tm=&t;
    Serial.println( tmEvent);   //debug
    Serial.println( tmEvent.substring(0,4).c_str());
    tm->tm_year=tmEvent.substring(0,4).toInt()-TM_BASE_YEAR;
    tm->tm_mon=tmEvent.substring(5,7).toInt();
    tm->tm_mday=tmEvent.substring(8,10).toInt();
    tm->tm_hour=tmEvent.substring(11,13).toInt();
    tm->tm_min=tmEvent.substring(14,16).toInt();
    tm->tm_sec=0;

    LOG(printf_P, PSTR("Event at %d %d %d %d %d\n"), tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min);

    event.unixtime = mktime(tm);;
    String tmpMsg(jee.param(F("msg")));
    event.message = (char*)(tmpMsg.c_str());

    myLamp.events.addEvent(event);
    myLamp.events.saveConfig();
    show_settings_event(interf, data);
}

void show_event_conf(Interface *interf, JsonObject *data){
    EVENT event;
    String act;
    bool edit = false;
    int i = 1, num = 0;
    if (!interf) return;

    if (data->containsKey(F("event"))) {
        EVENT *curr = nullptr;
        num = (*data)[F("event")];
        while ((curr = myLamp.events.getNextEvent(curr)) && i != num) ++i;
        if (!curr) return;
        act = (*data)["evconf" + String(num)].as<String>();
        event = *curr;
        edit = true;
    }

    if (act == F("del")) {
        myLamp.events.delEvent(event);
        myLamp.events.saveConfig();
        show_settings_event(interf, data);
        return;
    } else
    if (data->containsKey(F("save"))) {
        set_event_conf(interf, data);
        return;
    }


    interf->json_frame_interface();

    if (edit) {
        interf->json_section_main(F("set_event"), F("Редактировать событие"));
        interf->constant(F("event"), String(num), event.getName());
        interf->checkbox(F("enabled"), (event.isEnabled? F("true") : F("false")), F("Активно"), false);
    } else {
        interf->json_section_main(F("set_event"), F("Добавить событие"));
    }

    interf->select(F("evList"), String(event.event), F("Тип события"), false);
    interf->option(String(EVENT_TYPE::ON), F("Включить лампу"));
    interf->option(String(EVENT_TYPE::OFF), F("Выключить лампу"));
    interf->option(String(EVENT_TYPE::DEMO_ON), F("Включить DEMO"));
    interf->option(String(EVENT_TYPE::ALARM), F("Будильник"));
    interf->option(String(EVENT_TYPE::LAMP_CONFIG_LOAD), F("Загрузка конф. лампы"));
    interf->option(String(EVENT_TYPE::EFF_CONFIG_LOAD), F("Загрузка конф. эффектов"));
    interf->option(String(EVENT_TYPE::EVENTS_CONFIG_LOAD), F("Загрузка конф. событий"));
    interf->option(String(EVENT_TYPE::SEND_TEXT), F("Вывести текст"));
    interf->option(String(EVENT_TYPE::PIN_STATE), F("Состояние пина"));
#ifdef AUX_PIN
    interf->option(String(EVENT_TYPE::AUX_ON), F("Включить AUX"));
    interf->option(String(EVENT_TYPE::AUX_OFF), F("Выключить AUX"));
    interf->option(String(EVENT_TYPE::AUX_TOGGLE), F("Переключить AUX"));
#endif
    interf->json_section_end();

    interf->datetime(F("tmEvent"), event.getDateTime(), F("Дата/время события"));
    interf->number(F("repeat"), event.repeat, F("Повтор, мин"));
    interf->number(F("stopat"), event.stopat, F("Остановить через, мин"));
    interf->text(F("msg"), String(event.message), F("Параметр (текст)"));

    interf->json_section_hidden(F("repeat"), F("Повтор"));
    interf->checkbox(F("d1"), (event.d1? F("true") : F("false")), F("Понедельник"), false);
    interf->checkbox(F("d2"), (event.d1? F("true") : F("false")), F("Вторник"), false);
    interf->checkbox(F("d3"), (event.d1? F("true") : F("false")), F("Среда"), false);
    interf->checkbox(F("d4"), (event.d1? F("true") : F("false")), F("Четверг"), false);
    interf->checkbox(F("d5"), (event.d1? F("true") : F("false")), F("Пятница"), false);
    interf->checkbox(F("d6"), (event.d1? F("true") : F("false")), F("Суббота"), false);
    interf->checkbox(F("d7"), (event.d1? F("true") : F("false")), F("Воскресенье"), false);
    interf->json_section_end();

    if (edit) {
        interf->hidden(F("save"), F("true"));
        interf->button_submit(F("set_event"), F("Обновить событие"));
    } else {
        interf->button_submit(F("set_event"), F("Добавить событие"), F("green"));
    }

    interf->spacer();
    interf->button(F("show_event"), F("Выход"));

    interf->json_section_end();
    interf->json_frame_flush();
}

void section_effects_frame(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface(F(("Огненная лампа")));
    block_effects_main(interf, data);
    interf->json_frame_flush();
}

void section_lamp_frame(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface(F(("Огненная лампа")));
    block_lamp(interf, data);
    interf->json_frame_flush();
}

void section_settings_frame(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface(F(("Огненная лампа")));

    interf->json_section_main(F("settings"), F("Настройки"));

    interf->button(F("show_time"), F("Настройка времени"));

    interf->button(F("show_other"), F("Разные"));

    interf->button(F("show_wifi"), F("WiFi & MQTT"));
#ifdef MIC_EFFECTS
    interf->button(F("show_mic"), F("Микрофон"));
#endif

    interf->button(F("show_event"), F("События"));

    interf->spacer();
    block_settings_update(interf, data);

    interf->json_section_end();
    interf->json_frame_flush();
}

void section_main_frame(Interface *interf, JsonObject *data){
    if (!interf) return;
    if(!jee.connected && !iGLOBAL.isAPMODE){
        // только для первого раза форсируем выбор вкладки настройки WiFi, дальше этого не делаем
        show_settings_wifi(interf, data);
        return;
    }

#ifdef MIC_EFFECTS
    myLamp.setMicAnalyseDivider(0); // отключить микрофон на время прорисовки интерфейса
#endif

    LOG(println, F("Внимание: Создание интерфейса! Такие вызовы должны быть минимизированы."));
    interf->json_frame_interface(F(("Огненная лампа")));

    block_menu(interf, data);
    block_effects_main(interf, data);

    interf->json_frame_flush();

#ifdef MIC_EFFECTS
    myLamp.setMicAnalyseDivider(1); // восстановить делитель, при любой активности (поскольку эффекты могут его перенастраивать под себя)
#endif
}


void create_parameters(){
    LOG(println, F("Создание дефолтных параметров"));
    // создаем дефолтные параметры для нашего проекта
    jee.var_create(F("wifi"), F("STA")); // режим работы WiFi по умолчанию ("STA" или "AP")  (параметр в энергонезависимой памяти)
    jee.var_create(F("ssid"), F("")); // имя точки доступа к которой подключаемся (параметр в энергонезависимой памяти)
    jee.var_create(F("pass"), F("")); // пароль точки доступа к которой подключаемся (параметр в энергонезависимой памяти)

    // параметры подключения к MQTT
    jee.var_create(F("m_host"), F("")); // Дефолтные настройки для MQTT
    jee.var_create(F("m_port"), F("1883"));
    jee.var_create(F("m_user"), F(""));
    jee.var_create(F("m_pass"), F(""));
    jee.var_create(F("m_pref"), F(""));
    jee.var_create(F("mqtt_int"), F("30")); // интервал отправки данных по MQTT в секундах (параметр в энергонезависимой памяти)

    jee.var_create(F("effList"), F("1"));

    jee.var_create(F("fileName"),F("cfg1"));

    jee.var_create(F("MIRR_H"), F("false"));
    jee.var_create(F("MIRR_V"), F("false"));
#ifdef ESP_USE_BUTTON
    jee.var_create(F("isBtnOn"), F("true"));
#endif
#ifdef AUX_PIN
    jee.var_create(F("AUX"), F("false"));
#endif
    jee.var_create(F("msg"), F(""));
    jee.var_create(F("txtColor"), F("#ffffff"));
    jee.var_create(F("txtSpeed"), F("100"));
    jee.var_create(F("txtOf"), F("0"));
    jee.var_create(F("perTime"), F("1"));

    jee.var_create(F("GBR"), F("false"));
    jee.var_create(F("GlobBRI"), F("127"));

    jee.var_create(F("isTmSync"), F("true"));
    jee.var_create(F("time"), F("00:00"));
    jee.var_create(F("timezone"), F(""));
    jee.var_create(F("tm_offs"), F("0"));

    jee.var_create(F("ny_period"), F("0"));
    jee.var_create(F("ny_unix"), F("1609459200"));

#ifdef MIC_EFFECTS
    jee.var_create(F("micScale"),F("1.28"));
    jee.var_create(F("micNoise"),F("0.00"));
    jee.var_create(F("micnRdcLvl"),F("0"));
    jee.var_create(F("Mic"), F("true"));
#endif

#ifdef RESTORE_STATE
    jee.var_create(F("ONflag"), F("false"));
    jee.var_create(F("Demo"), F("false"));
#endif

    jee.var_create(F("Events"), F("false"));

    jee.var_create(F("isFaderON"), (FADE == true? F("true") : F("false")));
    jee.var_create(F("DRand"), (RANDOM_DEMO == true? F("true") : F("false")));
    jee.var_create(F("DTimer"), String(DEMO_TIMEOUT).c_str());


    // далее идут обработчики параметров

    jee.section_handle_add(F("main"), section_main_frame);
    jee.section_handle_add(F("show_flags"), show_main_flags);

    jee.section_handle_add(F("effects"), section_effects_frame);
    jee.section_handle_add(F("effects_param"), show_effects_param);
    jee.section_handle_add(F("effList"), set_effects_list);
    jee.section_handle_add(F("bright"), set_effects_bright);
    jee.section_handle_add(F("speed"), set_effects_speed);
    jee.section_handle_add(F("scale"), set_effects_scale);
    jee.section_handle_add(FPSTR(extraR), set_effects_extra);

    jee.section_handle_add(F("effects_config"), show_effects_config);
    jee.section_handle_add(F("effListConf"), set_effects_config_list);
    jee.section_handle_add(F("set_effect_prm"), set_effects_config_param);

    jee.section_handle_add(F("ONflag"), set_onflag);
    jee.section_handle_add(F("Demo"), set_demoflag);
    jee.section_handle_add(F("GBR"), set_gbrflag);
#ifdef OTA
    jee.section_handle_add(F("OTA"), set_otaflag);
#endif
#ifdef AUX_PIN
    jee.section_handle_add(F("AUX"), set_auxflag);
#endif

    jee.section_handle_add(F("lamp"), section_lamp_frame);
    jee.section_handle_add(F("textsend"), set_lamp_textsend);
    jee.section_handle_add(F("add_lamp_config"), edit_lamp_config);
    jee.section_handle_add(F("edit_lamp_config"), edit_lamp_config);

    jee.section_handle_add(F("settings"), section_settings_frame);
    jee.section_handle_add(F("show_wifi"), show_settings_wifi);
    jee.section_handle_add(F("set_wifi"), set_settings_wifi);
    jee.section_handle_add(F("set_mqtt"), set_settings_mqtt);
    jee.section_handle_add(F("show_other"), show_settings_other);
    jee.section_handle_add(F("set_other"), set_settings_other);
    jee.section_handle_add(F("show_time"), show_settings_time);
    jee.section_handle_add(F("set_time"), set_settings_time);
#ifdef MIC_EFFECTS
    jee.section_handle_add(F("show_mic"), show_settings_mic);
    jee.section_handle_add(F("set_mic"), set_settings_mic);
    jee.section_handle_add(F("Mic"), set_micflag);
    jee.section_handle_add(F("mic_cal"), set_settings_mic_calib);
#endif

    jee.section_handle_add(F("show_event"), show_settings_event);
    jee.section_handle_add(F("event_conf"), show_event_conf);
    jee.section_handle_add(F("evconf*"), show_event_conf);
    jee.section_handle_add(F("set_event"), set_event_conf);
    jee.section_handle_add(F("Events"), set_eventflag);

}

void sync_parameters(){
    DynamicJsonDocument doc(512);
    JsonObject obj = doc.to<JsonObject>();

    CALLSETTER(F("effList"), jee.param(F("effList")), set_effects_list);
    CALLSETTER(F("Events"), jee.param(F("Events")), set_eventflag);
    CALLSETTER(F("GBR"), jee.param(F("GBR")), set_gbrflag);

#ifdef RESTORE_STATE
    CALLSETTER(F("ONflag"), jee.param(F("ONflag")), set_onflag);
    CALLSETTER(F("Demo"), jee.param(F("Demo")), set_demoflag);
#endif

#ifdef AUX_PIN
    CALLSETTER(F("AUX"), jee.param(F("AUX")), set_auxflag);
#endif

#ifdef MIC_EFFECTS
    CALLSETTER(F("Mic"), jee.param(F("Mic")), set_micflag);

    obj[F("micScale")] = jee.param(F("micScale"));
    obj[F("micNoise")] = jee.param(F("micNoise"));
    obj[F("micnRdcLvl")] = jee.param(F("micnRdcLvl"));
    set_settings_mic(nullptr, &obj);
    obj.clear();
#endif

#ifdef ESP_USE_BUTTON
    obj[F("isBtnOn")] = jee.param(F("isBtnOn"));
#endif

    obj[F("isFaderON")] = jee.param(F("isFaderON"));
    obj[F("MIRR_H")] = jee.param(F("MIRR_H"));
    obj[F("MIRR_V")] = jee.param(F("MIRR_V"));
    obj[F("txtSpeed")] = jee.param(F("txtSpeed"));
    obj[F("txtOf")] = jee.param(F("txtOf"));
    obj[F("perTime")] = jee.param(F("perTime"));
    obj[F("ny_period")] = jee.param(F("ny_period"));
    obj[F("ny_unix")] = jee.param(F("ny_unix"));
    set_settings_other(nullptr, &obj);
    obj.clear();

    if (myLamp.IsGlobalBrightness()) {
        CALLSETTER(F("bright"), jee.param(F("GlobBRI")), set_effects_bright);
    }
}

void remote_action(RA action, const char *value){
    LOG(printf_P, PSTR("RA: %d (%s)\n"), action, String(value).c_str());
    StaticJsonDocument<128> doc;
    JsonObject obj = doc.to<JsonObject>();
    switch (action) {
        case RA::RA_ON:
            CALLINTERF(F("ONflag"), true, set_onflag);
            break;
        case RA::RA_OFF:
            CALLINTERF(F("ONflag"), false, set_onflag);
            break;
        case RA::RA_DEMO:
            CALLINTERF(F("Demo"), true, set_demoflag);
            break;
        case RA::RA_DEMO_NEXT:
            if (jee.param(F("DRand")) == F("true")) {
                myLamp.switcheffect(SW_RND, myLamp.getFaderFlag());
            } else {
                myLamp.switcheffect(SW_NEXT_DEMO, myLamp.getFaderFlag());
            }
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()->eff_nb).c_str());
        case RA::RA_EFFECT: {
            CALLINTERF(F("effList"), value, set_effects_list, ({
                interf->json_frame_value();
                interf->value(F("effList"), String(myLamp.effects.getSelected()->eff_nb));
                interf->json_frame_flush();
            }));
            break;
        }
        case RA::RA_BRIGHT_NF:
            obj[F("nofade")] = true;
        case RA::RA_BRIGHT:
            CALLINTERF(F("bright"), value, set_effects_bright);
            break;
        case RA::RA_SPEED:
            CALLINTERF(F("speed"), value, set_effects_speed);
            break;
        case RA::RA_SCALE:
            CALLINTERF(F("scale"), value, set_effects_scale);
            break;
        case RA::RA_EXTRA:
            CALLINTERF(FPSTR(extraR), value, set_effects_extra);
            break;
        case RA::RA_EFF_NEXT:
            myLamp.switcheffect(SW_NEXT, myLamp.getFaderFlag());
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()->eff_nb).c_str());
        case RA::RA_EFF_PREV:
            myLamp.switcheffect(SW_PREV, myLamp.getFaderFlag());
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()->eff_nb).c_str());
        case RA::RA_EFF_RAND:
            myLamp.switcheffect(SW_RND, myLamp.getFaderFlag());
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()->eff_nb).c_str());
        case RA::RA_WHITE_HI:
            myLamp.switcheffect(SW_WHITE_HI);
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()->eff_nb).c_str());
        case RA::RA_WHITE_LO:
            myLamp.switcheffect(SW_WHITE_LO);
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()->eff_nb).c_str());
        case RA::RA_ALARM:
            myLamp.startAlarm();
            break;
        case RA::RA_REBOOT:
            ESP.restart(); // так лучше :)
            break;
        case RA::RA_LAMP_CONFIG:
            if (value && *value) {
                String filename = String(F("/glb/"));
                filename.concat(value);
                jee.load(filename.c_str());
            }
            break;
        case RA::RA_EFF_CONFIG:
            if (value && *value) {
                String filename = String(F("/cfg/"));
                filename.concat(value);
                myLamp.effects.loadConfig(filename.c_str());
            }
            break;
        case RA::RA_EVENTS_CONFIG:
            if (value && *value) {
                String filename = String(F("/evn/"));
                filename.concat(value);
                myLamp.events.loadConfig(filename.c_str());
            }
            break;
        case RA::RA_SEND_TEXT: {
            String tmpStr = jee.param(F("txtColor"));
            tmpStr.replace(F("#"),F("0x"));
            CRGB::HTMLColorCode color = (CRGB::HTMLColorCode)strtol(tmpStr.c_str(), NULL, 0);

            myLamp.sendString(value, color);
            break;
        }
        case RA::RA_SEND_IP:
            myLamp.sendString(WiFi.localIP().toString().c_str(), CRGB::White);
            break;
        case RA::RA_SEND_TIME:
            myLamp.sendString(myLamp.timeProcessor.getFormattedShortTime().c_str(), CRGB::Green);
            break;
#ifdef OTA
        case RA::RA_OTA:
            myLamp.startOTA();
            break;
#endif
#ifdef AUX_PIN
        case RA::RA_AUX_ON:
            obj[F("AUX")] = true;
            set_auxflag(nullptr, &obj);
            break;
        case RA::RA_AUX_OFF:
            obj[F("AUX")] = false;
            set_auxflag(nullptr, &obj);
            break;
        case RA::RA_AUX_TOGLE:
            AUX_toggle(!digitalRead(AUX_PIN));
            break;
#endif
        default:;
    }
}

void httpCallback(const String &param, const String &value){
    RA action = RA_UNKNOWN;
    LOG(printf_P, PSTR("HTTP: %s - %s\n"), param.c_str(), value.c_str());

    if (param == F("on")) action = RA_ON;
    else if (param == F("off")) action = RA_OFF;
    else if(param == F("demo")) action = RA_DEMO;
    else if (param == F("msg")) action = RA_SEND_TEXT;
    else if (param == F("bright"))  action = RA_BRIGHT;
    else if (param == F("speed")) action = RA_SPEED;
    else if (param == F("scale")) action = RA_SCALE;
    else if (param == F("effect")) action = RA_EFFECT;
    else if (param == F("move_next")) action = RA_EFF_NEXT;
    else if (param == F("move_prev")) action = RA_EFF_PREV;
    else if (param == F("move_rnd")) action = RA_EFF_RAND;
    else if (param == F("reboot")) action = RA_REBOOT;
#ifdef OTA
    else if (param == F("OTA")) action = RA_OTA;
#endif
#ifdef AUX_PIN
    else if (param == F("aux_on")) action = RA_AUX_ON;
    else if (param == F("aux_off"))  action = RA_AUX_OFF;
    else if (param == F("aux_toggle"))  action = RA_AUX_TOGLE;
#endif
    remote_action(action, value.c_str());
    jee.publish(String(F("jee/pub/")) + param,value,false); // отправляем обратно в MQTT в топик jee/pub/
}

// обработка эвентов лампы
void event_worker(const EVENT *event){
    RA action = RA_UNKNOWN;
    LOG(printf_P, PSTR("%s - %s\n"), ((EVENT *)event)->getName().c_str(), myLamp.timeProcessor.getFormattedShortTime().c_str());

    switch (event->event) {
    case EVENT_TYPE::ON: action = RA_ON; break;
    case EVENT_TYPE::OFF: action = RA_OFF; break;
    case EVENT_TYPE::DEMO_ON: action = RA_DEMO; break;
    case EVENT_TYPE::ALARM: action = RA_ALARM; break;
    case EVENT_TYPE::LAMP_CONFIG_LOAD: action = RA_LAMP_CONFIG; break;
    case EVENT_TYPE::EFF_CONFIG_LOAD:  action = RA_EFF_CONFIG; break;
    case EVENT_TYPE::EVENTS_CONFIG_LOAD: action = RA_EVENTS_CONFIG; break;
    case EVENT_TYPE::SEND_TEXT:  action = RA_SEND_TEXT; break;
#ifdef AUX_PIN
    case EVENT_TYPE::AUX_ON: action = RA_AUX_ON; break;
    case EVENT_TYPE::AUX_OFF: action = RA_AUX_OFF; break;
    case EVENT_TYPE::AUX_TOGGLE: action = RA_AUX_TOGLE; break;
#endif
    case EVENT_TYPE::PIN_STATE: {
        if (event->message == nullptr) break;

        String tmpS(event->message);
        tmpS.replace(F("'"),F("\"")); // так делать не красиво, но шопаделаешь...
        StaticJsonDocument<256> doc;
        deserializeJson(doc, tmpS);
        JsonArray arr = doc.as<JsonArray>();
        for (size_t i = 0; i < arr.size(); i++) {
            JsonObject item = arr[i];
            uint8_t pin = item[F("pin")].as<int>();
            String action = item[F("act")].as<String>();
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
        }
        break;
    }
    default:;
    }

    remote_action(action, event->message);
}
