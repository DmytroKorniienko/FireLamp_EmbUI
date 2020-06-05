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
        //fcallback_update();
        //mqtt_update();
    }
}

void jeeui2::pre_autosave(){
    if (!sv) return;
    if (sv && astimer + 1000 < millis()){
        fcallback_update();
        mqtt_update();
        sv = false;
        isConfSaved = false;
        astimer = millis(); // обновляем счетчик после последнего изменения UI
    }
}

jeeui2::updateCallback jeeui2::updateCallbackHndl(){
    return fcallback_update;
}

void jeeui2::updateCallbackHndl(updateCallback func){
    fcallback_update = func;
}

jeeui2::httpCallback jeeui2::httpCallbackHndl(){
    return fcallback_http;
}

void jeeui2::httpCallbackHndl(httpCallback func){
    fcallback_http = func;
}

jeeui2::uiCallback jeeui2::uiCallbackHndl(){
    return fcallback_ui;
}

void jeeui2::uiCallbackHndl(uiCallback func){
    fcallback_ui = func;
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
