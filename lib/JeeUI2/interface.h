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
    RA_MIC,
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

void remote_action(RA action, ...);
void httpCallback(const String &param, const String &value);

class Button{
    typedef union _bflags {
        uint8_t mask;
        struct {
            uint8_t on:1;
            uint8_t hold:1;
            uint8_t click:3;
        };
    } btnflags;
    btnflags flags;

    friend bool operator== (const Button &f1, const Button &f2) { return (f1.flags.mask == f2.flags.mask); }
    friend bool operator!= (const Button &f1, const Button &f2) { return (f1.flags.mask != f2.flags.mask); }

    public:
        Button(uint8_t on, uint8_t hold, uint8_t click) { flags.on = on; flags.hold = hold; flags.click = click; }
};

