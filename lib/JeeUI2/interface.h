#pragma once

typedef enum _remote_action {
    RA_UNKNOWN,
    RA_ON,
    RA_OFF,
    RA_DEMO,
    RA_DEMO_NEXT,
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
    RA_EXTRA,
    RA_EFFECT,
    RA_SEND_TEXT,
    RA_SEND_TIME,
    RA_SEND_IP,
    RA_WHITE_HI,
    RA_WHITE_LO
} RA;

#define SETPARAM(key, call...) if (data->containsKey(key)) { \
    jee.var(key, (*data)[key]); \
    call; \
}

#define CALLSETTER(key, val, call) { \
    obj[key] = val; \
    call(nullptr, &obj); \
    obj.clear(); \
}

#define CALLINTERF(key, val, call, prm...) { \
    obj[key] = val; \
    Interface *interf = jee.ws.count()? new Interface(&jee, &jee.ws, 1000) : nullptr; \
    call(interf, &obj); \
    prm; \
    if (interf) delete interf; \
}

void remote_action(RA action, const char *value);
void httpCallback(const String &param, const String &value);