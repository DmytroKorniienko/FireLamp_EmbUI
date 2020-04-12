#ifndef JeeUI2_h
#define JeeUI2_h


#include "Arduino.h"

#ifdef ESP8266
#include <ESPAsyncTCP.h>
#include <FS.h>
#else
#include <AsyncTCP.h>
#include "SPIFFS.h"
#endif

#include <ESPAsyncWebServer.h>
#include "ArduinoJson.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266mDNS.h>        // Include the mDNS library
#else
#include <WiFi.h>
#include <ESPmDNS.h>
#include <Update.h>
#endif

#include <AsyncMqttClient.h>
//#include "AsyncJson.h"

#ifndef __DISABLE_BUTTON0
#define __BUTTON 0 // Кнопка "FLASH" на NODE_MCU
#endif

#ifndef __IDPREFIX
#define __IDPREFIX F("JeeUI2-")
#endif

class jeeui2
{
    DynamicJsonDocument cfg;
    DynamicJsonDocument pub_transport;
    DynamicJsonDocument btn_transport;
    AsyncMqttClient mqttClient;

    typedef void (*buttonCallback) ();
    typedef void (*uiCallback) ();
    typedef void (*updateCallback) ();
    typedef void (*mqttCallback) ();

  public:
    jeeui2() : cfg(4096), pub_transport(256), btn_transport(128), btn_id(1024) {
      *ip='\0'; 
      *mc='\0'; 
      *mac='\0'; 
      *m_pref='\0'; 
      *m_host='\0';
      *m_user='\0';
      *m_pass='\0';
      *m_host='\0';
      *udpRemoteIP='\0';
      *incomingPacket='\0';
      *btnui='\0';
      *udpMessage='\0';
      *_t_tpc_current='\0';
      *_t_pld_current='\0';
    }

    void var(const String &key, const String &value, bool pub = false);
    void var_create(const String &key, const String &value);
    void btn_create(const String &btn, buttonCallback response);
    String param(const String &key);
    void led(uint8_t pin, bool invert);
    String deb();
    void ap(unsigned long interval);
    void begin();
    void begin(bool debug);
    void handle();
    void btnCallback(const String &name ,buttonCallback response);

    void app(const String &name);
    void menu(const String &name);
    void page();

    void text(const String &id, const String &label);
    void password(const String &id, const String &label);
    void number(const String &id, const String &label);
    void time(const String &id, const String &label);
    void date(const String &id, const String &label);
    void datetime(const String &id, const String &label);
    void email(const String &id, const String &label);
    void range(const String &id, int min, int max, float step, const String &label);
    void select(const String &id, const String &label);
    void option(const String &value, const String &label);
    void checkbox(const String &id, const String &label);
    void color(const String &id, const String &label);
    void button(const String &id, const String &color, const String &label);
    void button(const String &id, const String &color, const String &label, int column);
    void textarea(const String &id, const String &label);
    void save(const char *_cfg = nullptr);
    void load(const char *_cfg = nullptr);
    void udp(const String &message);
    void udp();
    void pub(const String &id, const String &label);
    void pub(const String &id, const String &label, const String &value);
    void pub(const String &id, const String &label, const String &value, const String &unit);
    void pub(const String &id, const String &label, const String &value, const String &unit, const String &bg_color);
    void pub(const String &id, const String &label, const String &value, const String &unit, const String &bg_color, const String &text_color);
    void formWifi();
    void formMqtt();

    uiCallback foo;
    void ui(void (*uiFunction) ());

    void mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), bool remotecontrol);
    void mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload));
    void mqtt(const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload));
    void mqtt(const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), bool remotecontrol);
    void mqtt(const String &host, int port, const String &user, const String &pass, bool remotecontrol);
    void mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, bool remotecontrol);

    mqttCallback onConnect;

    void mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) (), bool remotecontrol);
    void mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) ());
    void mqtt(const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) ());
    void mqtt(const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) (), bool remotecontrol);

    void subscribe(const String &topic);
    void publish(const String &topic, const String &payload);
    void publish(const String &topic, const String &payload, bool retained);

    void remControl();

    updateCallback upd;
    void update(void (*updateFunction) ());

    char ip[16]; //"255.255.255.255"
    char mc[13]; // id "ffffffffffff"
    char mac[18]; // "ff:ff:ff:ff:ff:ff"
    bool _refresh = false;
    bool connected = false;

    bool isLoading() {
      if(loading && tm_loading+5000<millis()){
        loading = false;
        if(dbg) Serial.println(F("LOADING UNBLOCK"));
        _refresh = true;
      }
      return loading;
    }

    String id(const String &tpoic);
    static char m_pref[16]; // префикс MQTT
    
    void setDelayedSave(unsigned int ms) { asave = ms; astimer = millis(); sv=true; } // Отложенное сохранение

  private:

    bool loading = true; // признак попытки загрузки начальной страницы index.htm
    unsigned long tm_loading = millis();

    void arr(const String &key, const String &value);
    void wifi_connect();
    void button_handle();
    void led_handle();
    void nonWifiVar();
    void led_on();
    void led_off();
    void led_inv();
    void autosave();
    void pre_autosave();
    void as();
    void udpBegin();
    void udpLoop();
    void btn();
    void getAPmac();

    void pub_mqtt(const String &key, const String &value);

    void mqtt_update();
    void mqtt_handle();
    void mqtt_send();
    void mqtt_reconnect();
    bool mqtt_ok = false;
    bool mqtt_enable = false;

    void _connected();
    void subscribeAll();
   
    char m_host[256]; // MQTT
    int m_port = 0;
    char m_user[64];
    char m_pass[64];
    bool m_params;

    int sendConfig = 0;

    bool pub_enable;

    //int btn_num = 0;
    DynamicJsonDocument btn_id; // json с ид кнопок (для подписки через MQQT)

    char udpRemoteIP[16];
    unsigned int localUdpPort = 4243;
    char incomingPacket[64];

    unsigned int asave = 1000;
    bool sv = false;
    bool isConfSaved = true; // признак сохраненного конфига
    unsigned long astimer;
    
    uint8_t wifi_mode;
    int LED_PIN = -1;
    bool LED_INVERT = false;
    uint8_t mn = 0;
    unsigned long a_ap = 0;
    bool wf = false;
    uint8_t pg = 0;
    char btnui[32]; // Последняя нажатая кнопка (аппаратная или UI), после обработки - сброс значения
    char udpMessage[65]; // Обмен по UDP
    
    bool dbg = false;
    bool rc;

    void connectToMqtt();
    void onMqttConnect();
    static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    static void onMqttSubscribe(uint16_t packetId, uint8_t qos);
    static void onMqttUnsubscribe(uint16_t packetId);
    static void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    static void onMqttPublish(uint16_t packetId);
    static void _onMqttConnect(bool sessionPresent);
    void check_wifi_state();

    static bool mqtt_connected;
    static bool mqtt_connect;
    static char _t_tpc_current[128]; // топик без префикса
    static char _t_pld_current[128]; // сообщение
    static bool _t_inc_current;
    static bool _t_remotecontrol;
    String op; // опции для выпадающего списка <-- весьма желательно очищать сразу же...
public:
    String buf; // борьба с фрагментацией кучи, буффер должен быть объявлен последним <-- весьма желательно очищать сразу же...
};

#endif