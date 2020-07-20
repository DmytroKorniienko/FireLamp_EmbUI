#pragma once

typedef enum _remote_action {
    RA_UNKNOWN,
    RA_ON,
    RA_OFF,
    RA_DEMO,
    RA_DEMO_NEXT,
    RA_ALARM,
    RA_ALARM_OFF,
    RA_LAMP_CONFIG,
    RA_EFF_CONFIG,
    RA_EVENTS_CONFIG,
#ifdef AUX_PIN
    RA_AUX_ON,
    RA_AUX_OFF,
    RA_AUX_TOGLE,
#endif
#ifdef OTA
    RA_OTA,
#endif
    RA_REBOOT,
    RA_EFF_NEXT,
    RA_EFF_PREV,
    RA_EFF_RAND,
    RA_BRIGHT,
    RA_BRIGHT_NF,
    RA_SPEED,
    RA_SCALE,
    RA_EXTRA,
    RA_MIC,
    RA_EFFECT,
    RA_SEND_TEXT,
    RA_SEND_TIME,
    RA_SEND_IP,
    RA_WHITE_HI,
    RA_WHITE_LO,
    RA_WIFI_REC
} RA;

#define TOGLE_STATE(val, curr) (val == F("true"))? true : (val == F("false"))? false : !curr;

#define SETPARAM(key, call...) if (data->containsKey(key)) { \
    jee.var(key, (*data)[key]); \
    call; \
}

#define CALL_SETTER(key, val, call) { \
    obj[key] = val; \
    call(nullptr, &obj); \
    obj.clear(); \
}

#define CALL_INTF(key, val, call) { \
    obj[key] = val; \
    Interface *interf = jee.ws.count()? new Interface(&jee, &jee.ws, 1000) : nullptr; \
    call(interf, &obj); \
    if (interf) { \
        interf->json_frame_value(); \
        interf->value(key, val, false); \
        interf->json_frame_flush(); \
        delete interf; \
    } \
}

#define CALL_INTF_OBJ(call) { \
    Interface *interf = jee.ws.count()? new Interface(&jee, &jee.ws, 1000) : nullptr; \
    call(interf, &obj); \
    if (interf) { \
        interf->json_frame_value(); \
        for (JsonPair kv : obj) { \
            interf->value(kv.key().c_str(), kv.value(), false); \
        } \
        interf->json_frame_flush(); \
        delete interf; \
    } \
}

void section_main_frame(Interface *interf, JsonObject *data);
void section_effects_frame(Interface *interf, JsonObject *data);
void section_lamp_frame(Interface *interf, JsonObject *data);
void section_settings_frame(Interface *interf, JsonObject *data);
void pubCallback(Interface *interf);


void remote_action(RA action, ...);
void httpCallback(const String &param, const String &value);
#ifdef ESP_USE_BUTTON
void default_buttons();
#endif

void uploadProgress(size_t len, size_t total);
