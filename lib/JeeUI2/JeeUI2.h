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
#include "../../include/LList.h"

#ifndef __DISABLE_BUTTON0
#define __BUTTON 0 // Кнопка "FLASH" на NODE_MCU
#endif

#ifndef __IDPREFIX
#define __IDPREFIX F("JeeUI2-")
#endif

class Interface;

class jeeui2
{
    typedef void (*buttonCallback) (Interface *interf);
    typedef void (*updateCallback) ();
    typedef void (*mqttCallback) ();
    typedef void (*httpCallback) (const char *param, const char *value);

    typedef struct section_handle_t{
      String name;
      buttonCallback callback;
    } section_handle_t;

    DynamicJsonDocument cfg;
    LList<section_handle_t*> section_handle;
    AsyncMqttClient mqttClient;

  public:
    jeeui2() : cfg(4096), section_handle(), server(80), ws("/ws"){
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

    AsyncWebServer server;
    AsyncWebSocket ws;

    void var(const String &key, const String &value);
    void var_create(const String &key, const String &value);
    void section_handle_add(const String &btn, buttonCallback response);
    String param(const String &key);
    void led(uint8_t pin, bool invert);
    String deb();
    void ap(unsigned long interval);
    void begin();
    void begin(bool debug);
    void handle();


    void save(const char *_cfg = nullptr);
    void load(const char *_cfg = nullptr);
    void udp(const String &message);
    void udp();



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

    updateCallback updateCallbackHndl();
    void updateCallbackHndl(updateCallback func);

    httpCallback httpCallbackHndl();
    void httpCallbackHndl(httpCallback func);

    void refresh();
    void post(JsonArray data);
    void send_pub();

    char ip[16]; //"255.255.255.255"
    char mc[13]; // id "ffffffffffff"
    char mac[18]; // "ff:ff:ff:ff:ff:ff"

    bool connected = false;
    bool dbg = false;

    String id(const String &tpoic);
    static char m_pref[16]; // префикс MQTT

    void setDelayedSave(unsigned int ms) { asave = ms; astimer = millis(); sv=true; } // Отложенное сохранение

  private:
    bool _isHttpCmd = false;
    char httpParam[32]; // буфер под параметр
    char httpValue[32]; // и его значение

    httpCallback fcallback_http = nullptr;
    updateCallback fcallback_update = nullptr;

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
    // bool httpstream = false;
    String op; // опции для выпадающего списка <-- весьма желательно очищать сразу же...
};

#endif