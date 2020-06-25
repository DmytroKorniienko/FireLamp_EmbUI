#pragma once

typedef enum _remote_action {
    RA_UNKNOWN,
    RA_ON,
    RA_OFF,
    RA_DEMO,
    RA_ALARM,
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
    RA_EFFECT,
    RA_SEND_TEXT,
    RA_SEND_TIME,
    RA_SEND_IP,
    RA_WHITE_HI,
    RA_WHITE_LO,
} RA;

#define SETPARAM(key, call) if (data->containsKey(F(key))) { \
    call; \
    jee.var(F(key), (*data)[F(key)]); \
}

#define CALLSETTER(key, val, call) { \
    StaticJsonDocument<128> doc; \
    doc[key] = val; \
    JsonObject obj = doc.as<JsonObject>(); \
    call(nullptr, &obj); \
}

void remote_action(RA action, const char *value);
void httpCallback(const String &param, const String &value);