#include "JeeUI2.h"

void jeeui2::save(const char *_cfg)
{
    if(SPIFFS.begin()){
        File configFile;
        if(_cfg == nullptr)
            configFile = SPIFFS.open(F("/config.json"), "w"); // PSTR("w") использовать нельзя, будет исключение!
        else
            configFile = SPIFFS.open(_cfg, "w"); // PSTR("w") использовать нельзя, будет исключение!

        // serializeJson(cfg,configFile);
        // configFile.flush();
        // configFile.close();

        String cfg_str;
        serializeJson(cfg, cfg_str);
        deserializeJson(cfg, cfg_str);
        configFile.print(cfg_str);
        configFile.flush();
        configFile.close();

        if(dbg)Serial.println(F("Save Config"));
    }
}

void jeeui2::autosave(){
    
    if (isConfSaved) return;
    if (!isConfSaved && astimer + asave < millis()){
        save();
        if(dbg)Serial.println(F("AutoSave"));
        astimer = millis();
        isConfSaved = true; // сохранились
        //sv = false;
        //upd();
        //mqtt_update();
    } 
}

void jeeui2::pre_autosave(){
    if (!sv) return;
    if (sv && astimer + 1000 < millis()){
        upd();
        mqtt_update();
        sv = false;
        isConfSaved = false;
        astimer = millis(); // обновляем счетчик после последнего изменения UI
    } 
}

void jeeui2::update(void (*updateFunction) ())
{
    upd = updateFunction;
}

void jeeui2::as(){
    sv = true;
    astimer = millis();
}

void jeeui2::load(const char *_cfg)
{
    if(SPIFFS.begin()){
        File configFile;
        if(_cfg == nullptr)
            configFile = SPIFFS.open(F("/config.json"), "r"); // PSTR("r") использовать нельзя, будет исключение!
        else
            configFile = SPIFFS.open(_cfg, "w"); // PSTR("w") использовать нельзя, будет исключение!

        String cfg_str = configFile.readString();
        if (cfg_str == F("")){
            if(dbg)Serial.println(F("Failed to open config file"));
            save();
            return;
        }
        DeserializationError error = deserializeJson(cfg, cfg_str);
        if (error) {
            if(dbg)Serial.print(F("JSON config deserializeJson error: "));
            if(dbg)Serial.println(error.code());
            return;
        }
        if(dbg)Serial.println(F("JSON config loaded"));
        configFile.close();
        sv = false;
    }
}