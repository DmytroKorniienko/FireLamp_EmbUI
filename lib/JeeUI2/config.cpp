#include "JeeUI2.h"

void jeeui2::save(const char *_cfg) {
    if (isNeedSave && SPIFFS.begin()) {
        File configFile;
        if (_cfg == nullptr) {
            configFile = SPIFFS.open(F("/config.json"), "w"); // PSTR("w") использовать нельзя, будет исключение!
        } else {
            configFile = SPIFFS.open(_cfg, "w"); // PSTR("w") использовать нельзя, будет исключение!
        }

        String cfg_str;
        serializeJson(cfg, cfg_str);
        configFile.print(cfg_str);
        configFile.flush();
        configFile.close();
        cfg.garbageCollect();
        isNeedSave = false;
        LOG(println, F("Save Config"));
    }
}

void jeeui2::autosave(){
    if (isNeedSave && millis() > astimer + asave){
        save();
        LOG(println, F("AutoSave"));
        astimer = millis();
    }
}

void jeeui2::load(const char *_cfg){
    if (SPIFFS.begin()) {
        File configFile;
        if (_cfg == nullptr) {
            configFile = SPIFFS.open(F("/config.json"), "r"); // PSTR("r") использовать нельзя, будет исключение!
        } else {
            configFile = SPIFFS.open(_cfg, "w"); // PSTR("w") использовать нельзя, будет исключение!
        }

        String cfg_str = configFile.readString();
        if (cfg_str == F("")){
            LOG(println, F("Failed to open config file"));
            save();
            return;
        }
        DeserializationError error = deserializeJson(cfg, cfg_str);
        if (error) {
            LOG(print, F("JSON config deserializeJson error: "));
            LOG(println, error.code());
            return;
        }
        LOG(println, F("JSON config loaded"));
        configFile.close();
    }
}
