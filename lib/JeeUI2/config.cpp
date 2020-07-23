#include "JeeUI2.h"

void jeeui2::save(const char *_cfg, bool force){
    if ((isNeedSave || force) && LittleFS.begin()) {
        File configFile;
        if (_cfg == nullptr) {
            LOG(println, F("Save default main config file"));
            configFile = LittleFS.open(F("/config.json"), "w"); // PSTR("w") использовать нельзя, будет исключение!
        } else {
            LOG(printf_P, PSTR("Save %s main config file\n"), _cfg);
            configFile = LittleFS.open(_cfg, "w"); // PSTR("w") использовать нельзя, будет исключение!
        }

        String cfg_str;
        serializeJson(cfg, cfg_str);
        configFile.print(cfg_str);
        configFile.flush();
        configFile.close();

        cfg.garbageCollect();
        isNeedSave = false;
    }
    delay(10); // задержка после записи
}

void jeeui2::autosave(){
    if (isNeedSave && millis() > astimer + asave){
        save();
        LOG(println, F("AutoSave"));
        astimer = millis();
    }
}

void jeeui2::load(const char *_cfg){
    if (LittleFS.begin()) {
        File configFile;
        if (_cfg == nullptr) {
            LOG(println, F("Load default main config file"));
            configFile = LittleFS.open(F("/config.json"), "r"); // PSTR("r") использовать нельзя, будет исключение!
        } else {
            LOG(printf_P, PSTR("Load %s main config file\n"), _cfg);
            configFile = LittleFS.open(_cfg, "r"); // PSTR("w") использовать нельзя, будет исключение!
        }

        String cfg_str = configFile.readString();
        configFile.close();
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
    }
}
