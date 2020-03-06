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

#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <string.h>
#include <FastLed.h>
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined ESP32
#include <WiFi.h>
#include "SPIFFS.h"
#endif

typedef enum _EFF_ENUM {
EFF_NONE = (0U),                              // Специальный служебный эффект, не комментировать и индекс не менять константу!
EFF_SPARKLES,                                 // Конфетти
EFF_FIRE,                                     // Огонь
EFF_EVERYTHINGFALL,                           // Водо/огне/лава/радуга/хренопад
EFF_RAINBOW_2D,                               // Радуга универсальная
EFF_COLORS,                                   // Смена цвета
EFF_MADNESS,                                  // Безумие 3D
EFF_CLOUDS,                                   // Облака 3D
EFF_LAVA,                                     // Лава 3D
EFF_PLASMA,                                   // Плазма 3D
EFF_RAINBOW,                                  // Радуга 3D
EFF_RAINBOW_STRIPE,                           // Павлин 3D
EFF_ZEBRA,                                    // Зебра 3D
EFF_FOREST,                                   // Лес 3D
EFF_OCEAN,                                    // Океан 3D
EFF_SNOW,                                     // Снегопад
EFF_SNOWSTORMSTARFALL,                        // Метель + Звездопад
EFF_MATRIX,                                   // Матрица
EFF_LIGHTERS,                                 // Светлячки
EFF_LIGHTER_TRACES,                           // Светлячки со шлейфом
EFF_PAINTBALL,                                // Пейнтбол
EFF_CUBE,                                     // Блуждающий кубик
EFF_PULSE,                                    // Пульс 
EFF_WHITE_COLOR                               // Белый свет
} EFF_ENUM;

void sparklesRoutine(CRGB*,const char*);
void fireRoutine(CRGB*,const char*);
void whiteColorStripeRoutine(CRGB*, const char*);
void fire2012WithPalette(CRGB*, const char*);
void pulseRoutine(CRGB*, const char*);
void rainbowDiagonalRoutine(CRGB*, const char*);
void colorsRoutine(CRGB*, const char*);
void matrixRoutine(CRGB*, const char*);
void snowRoutine(CRGB*, const char*);
void snowStormStarfallRoutine(CRGB*, const char*);
void lightersRoutine(CRGB*, const char*);
void ballsRoutine(CRGB*, const char*);
void lightBallsRoutine(CRGB*, const char*);

//-------------------------------------------------

typedef struct _EFFECT {
    EFF_ENUM eff_nb;
    const char *eff_name;
    byte brightness;
    byte speed;
    byte scale;
    bool canBeSelected;
    bool isFavorite;
    void (*func)(CRGB*,const char*);
    char *param;
    void setNone(){ eff_nb=EFF_NONE; eff_name=nullptr; brightness=127; speed=127; scale=127; canBeSelected=false; isFavorite=false; func=nullptr; param=nullptr; }
} EFFECT;

static EFFECT _EFFECTS_ARR[] = {
    {EFF_NONE, nullptr, 127, 127, 127, false, false, nullptr, nullptr},
    {EFF_SPARKLES, "Светлячки", 127, 127, 127, true, true, sparklesRoutine, nullptr},
    {EFF_FIRE, "Огненная лампа", 127, 127, 127, true, true, fireRoutine, nullptr},
    {EFF_EVERYTHINGFALL, "Эффектопад", 127, 127, 127, true, true, fire2012WithPalette, nullptr},
    {EFF_RAINBOW_2D, "Радуга 2D", 127, 127, 127, true, true, rainbowDiagonalRoutine, nullptr},
    {EFF_COLORS, "Цвета", 127, 127, 127, true, true, colorsRoutine, nullptr},
    {EFF_PULSE, "Пульс", 127, 127, 127, true, true, pulseRoutine, nullptr},
    {EFF_MATRIX, "Матрица", 127, 127, 127, true, true, matrixRoutine, nullptr},
    {EFF_SNOW, "Снегопад", 127, 127, 127, true, true, snowRoutine, nullptr},    
    {EFF_SNOWSTORMSTARFALL, "Метель + Звездопад", 127, 127, 127, true, true, snowStormStarfallRoutine, nullptr},
    {EFF_LIGHTERS, "Светлячки", 127, 127, 127, true, true, lightersRoutine, nullptr},
    {EFF_LIGHTER_TRACES, "Светлячки со шлейфом", 127, 127, 127, true, true, ballsRoutine, nullptr},
    {EFF_PAINTBALL, "Пейнтбол", 127, 127, 127, true, true, lightBallsRoutine, nullptr},
    {EFF_WHITE_COLOR, "Белая лампа", 127, 127, 127, true, true, whiteColorStripeRoutine, nullptr}
};

class EffectWorker {
private:
    const int MODE_AMOUNT = sizeof(_EFFECTS_ARR)/sizeof(EFFECT);     // количество режимов
    const uint8_t maxDim = ((WIDTH>HEIGHT)?WIDTH:HEIGHT);
    EFFECT* effects = _EFFECTS_ARR;
    EFF_ENUM curEff = EFF_NONE;
    unsigned int arrIdx = 0;
    unsigned int storedIdx = 0; // предыдущий эффект
    EffectWorker(const EffectWorker&);  // noncopyable
    EffectWorker& operator=(const EffectWorker&);  // noncopyable
public:
    EffectWorker() {
        //loadConfig();       
    }

    ~EffectWorker() {}

    void loadConfig() {
        if(SPIFFS.begin()){
            File configFile = SPIFFS.open(F("/eff_config.json"), "r"); // PSTR("r") использовать нельзя, будет исключение!
            String cfg_str = configFile.readString();

            if (cfg_str == F("")){
#ifdef LAMP_DEBUG
                LOG.println(F("Failed to open effects config file"));
#endif
                saveConfig();
                return;
            }

#ifdef LAMP_DEBUG
                LOG.println(F("\nStart desialization of effects\n\n"));
#endif

            DynamicJsonDocument doc(4096);
            DeserializationError error = deserializeJson(doc, cfg_str);
            if (error) {
#ifdef LAMP_DEBUG
                LOG.print(F("deserializeJson error: "));
                LOG.println(error.code());
#endif
                return;
            }

            JsonArray arr = doc.as<JsonArray>();
            EFFECT *eff;
            for (size_t i=0; i<arr.size(); i++) {
                JsonObject item = arr[i];

                EFF_ENUM nb = (EFF_ENUM)(item[F("nb")].as<int>());
                eff = getEffectBy(nb);
                eff->brightness = item[F("br")].as<int>();
                eff->speed = item[F("sp")].as<int>();
                eff->scale = item[F("sc")].as<int>();
                eff->isFavorite = (bool)(item[F("isF")].as<int>());
                eff->canBeSelected = (bool)(item[F("cbS")].as<int>());
#ifdef LAMP_DEBUG
                LOG.printf_P(PSTR("(%d - %d - %d - %d - %d - %d)\n"), nb, eff->brightness, eff->speed, eff->scale, eff->isFavorite, eff->canBeSelected);
#endif
            }
            // JsonArray::iterator it;
            // for (it=arr.begin(); it!=arr.end(); ++it) {
            //     const JsonObject& elem = *it;
            // }
#ifdef LAMP_DEBUG
            LOG.println(F("Effects config loaded"));
#endif
        }
    }

    void saveConfig() {
        if(SPIFFS.begin()){
            File configFile = SPIFFS.open(F("/eff_config.json"), "w"); // PSTR("w") использовать нельзя, будет исключение!
            EFFECT *cur_eff;

            configFile.print("[");
            for(int i=0; i<MODE_AMOUNT; i++){
                cur_eff = &(effects[i]);
                configFile.printf_P(PSTR("%s{\"nb\":%d,\"br\":%d,\"sp\":%d,\"sc\":%d,\"isF\":%d,\"cbS\":%d,\"prm\":\"%s\"}"),
                    (i?F(","):F("")), cur_eff->eff_nb, cur_eff->brightness, cur_eff->speed, cur_eff->scale, (int)cur_eff->isFavorite, (int)cur_eff->canBeSelected,
                    ((cur_eff->param!=nullptr)?cur_eff->param:""));
#ifdef LAMP_DEBUG
                LOG.printf_P(PSTR("%s{\"nb\":%d,\"br\":%d,\"sp\":%d,\"sc\":%d,\"isF\":%d,\"cbS\":%d,\"prm\":\"%s\"}"),
                    (i?F(","):F("")), cur_eff->eff_nb, cur_eff->brightness, cur_eff->speed, cur_eff->scale, (int)cur_eff->isFavorite, (int)cur_eff->canBeSelected,
                    ((cur_eff->param!=nullptr)?cur_eff->param:""));
#endif
            }     
            configFile.print("]");
            configFile.flush();
            configFile.close();
            LOG.println(F("\nSave effects config"));
        }
    }

    bool autoSaveConfig() {
        static unsigned long i;
        if(i + (30 * 1000) > millis()){  // если не пришло время - выходим из функции и сбрасываем счетчик (ожидаем бездействия в 30 секунд относительно последней записи)
            i = millis();
            return false;
        }
        saveConfig();
        i = millis();
        return true; // сохранились
    }

    void setBrightness(byte val) {effects[arrIdx].brightness = val;}
    void setSpeed(byte val) {effects[arrIdx].speed = val;}
    void setScale(byte val) {effects[arrIdx].scale = val;}
    byte getBrightness() { return effects[arrIdx].brightness; }
    byte getSpeed() { return effects[arrIdx].speed; }
    byte getScale() { return effects[arrIdx].scale; }
    byte getModeAmount() {return MODE_AMOUNT;}
    const char *getName() {return effects[arrIdx].eff_name;}
    const EFF_ENUM getEn() {return effects[arrIdx].eff_nb;}
    
    void moveNext() // следующий эффект, кроме canBeSelected==false
    {
        int i;

        for(i=arrIdx+1; i<MODE_AMOUNT; i++){
            if(effects[i].canBeSelected){
                arrIdx = i;
                curEff = effects[i].eff_nb;
                return;
            }
        }
        i = 1;
        for(; i<MODE_AMOUNT; i++){
            if(effects[i].canBeSelected){
                arrIdx = i;
                curEff = effects[i].eff_nb;
                return;
            }
        }
    }

    void movePrev() // предыдущий эффект, кроме canBeSelected==false
    { 
        int i;

        for(i=arrIdx-1; i>0; i--){
            if(effects[i].canBeSelected){
                arrIdx = i;
                curEff = effects[i].eff_nb;
                return;
            }
        }
        i = MODE_AMOUNT-1;
        for(; i>=0; i--){
            if(effects[i].canBeSelected){
                arrIdx = i;
                curEff = effects[i].eff_nb;
                return;
            }
        }
    }

    void moveBy(EFF_ENUM select){ // перейти по перечислению
        for(int i=MODE_AMOUNT-1; i>=0; i--){
            if(effects[i].eff_nb == select){
                arrIdx = i;
                curEff = effects[i].eff_nb;
            }
        }
    }

    void moveBy(byte cnt){ // перейти на количество шагов, к ближайшему большему (для DEMO)
        while(cnt){
            for(int i=arrIdx; i<MODE_AMOUNT; i++){
                if(cnt>0)
                    cnt--;
                if(effects[i].isFavorite && cnt==0){
                    arrIdx = i;
                    curEff = effects[i].eff_nb;
                    return;
                }
            }
            if(arrIdx == 0){ return; }
            else {
                arrIdx = 0;
                curEff = effects[0].eff_nb;
            }
        }
    }

    EFFECT enumNextEffect(EFFECT current){
        for(int i=0; i<MODE_AMOUNT; i++){
            if(effects[i].eff_nb == current.eff_nb){
                if((i+1)!=MODE_AMOUNT)
                    return effects[i+1];
                else
                    return effects[0]; // NONE
            }
        }
        return effects[0]; // NONE
    }

    EFFECT *getEffectBy(EFF_ENUM select){
        for(int i=MODE_AMOUNT-1; i>=0; i--){
            if(effects[i].eff_nb == select){
                return &(effects[i]);
            }
        }
        return &(effects[0]); // NONE
    }

    EFFECT *getCurrent(){ // вернуть текущий
        return &(effects[arrIdx]);
    }
};