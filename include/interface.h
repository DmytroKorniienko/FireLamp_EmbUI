//#pragma once
#ifndef __INTERFACE_H
#define __INTERFACE_H

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
#ifdef ESP_USE_BUTTON
    RA_BUTTONS_CONFIG,
#endif
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
    RA_BRIGHT_NF,
    RA_CONTROL,
    RA_MIC,
#ifdef MP3PLAYER
    RA_MP3_PREV,
    RA_MP3_NEXT,
    RA_MP3_SOUND,
    RA_PLAYERONOFF,
#endif
#ifdef MIC_EFFECTS
    RA_MICONOFF,
#endif
    RA_EFFECT,
    RA_SEND_TEXT,
    RA_SEND_TIME,
    RA_SEND_IP,
    RA_WHITE_HI,
    RA_WHITE_LO,
    RA_WIFI_REC,
    RA_GLOBAL_BRIGHT,
    RA_WARNING,
    RA_DRAW,
    RA_FILLMATRIX
} RA;

void remote_action(RA action, ...);
String httpCallback(const String &param, const String &value, bool isset);
#ifdef ESP_USE_BUTTON
void default_buttons();
#endif

// ---------------------

void section_main_frame(Interface *interf, JsonObject *data);
void section_effects_frame(Interface *interf, JsonObject *data);
void section_text_frame(Interface *interf, JsonObject *data);
// реализация настроек тут своя, отличная от фреймворка
void section_settings_frame(Interface *interf, JsonObject *data);
void pubCallback(Interface *interf);
void set_onflag(Interface *interf, JsonObject *data);
void save_lamp_flags();

uint8_t uploadProgress(size_t len, size_t total);

void delayedcall_show_effects();

// Вывод значка микрофона в списке эффектов
#ifdef MIC_EFFECTS
    #define MIC_SYMBOL (micSymb ? (pgm_read_byte(T_EFFVER + (uint8_t)eff->eff_nb) % 2 == 0 ? " \U0001F399" : "") : "")
    #define MIC_SYMB bool micSymb = myLamp.getLampSettings().effHasMic
#else
    #define MIC_SYMBOL ""
    #define MIC_SYMB
#endif

// Вывод номеров эффектов в списке, в WebUI
//#define EFF_NUMBER (numList ? (String(eff->eff_nb) + ". ") : "")
#define EFF_NUMBER   (numList ? (eff->eff_nb <= 255 ? (String(eff->eff_nb) + ". ") : (String((byte)(eff->eff_nb & 0xFF)) + "." + String((byte)(eff->eff_nb >> 8) - 1U) + ". ")) : "")

#ifdef DELAYED_EFFECTS
 #define INDEX_BUILD_DELAY 5
#else
 #define INDEX_BUILD_DELAY 1
#endif

#endif
