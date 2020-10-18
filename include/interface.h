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

void section_main_frame(Interface *interf, JsonObject *data);
void section_effects_frame(Interface *interf, JsonObject *data);
void section_lamp_frame(Interface *interf, JsonObject *data);
void section_settings_frame(Interface *interf, JsonObject *data);
void pubCallback(Interface *interf);
void save_lamp_flags();

void remote_action(RA action, ...);
String httpCallback(const String &param, const String &value, bool isset);
#ifdef ESP_USE_BUTTON
void default_buttons();
#endif

void uploadProgress(size_t len, size_t total);
