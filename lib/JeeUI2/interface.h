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
    RA_SEND_TEXT,
#ifdef AUX_PIN
    RA_AUX_ON,
    RA_AUX_OFF,
    RA_AUX_TOGLE,
#endif
#ifdef OTA
    RA_OTA,
#endif
    RA_REBOOT,
    RA_MV_NEXT,
    RA_MV_PREV,
    RA_MV_RAND,
    RA_BRIGHT,
    RA_SPEED,
    RA_SCALE,
    RA_EFFECT,
} RA;

void remote_action(RA action, const String &value);
void httpCallback(const String &param, const String &value);