#ifndef JeeUI2_h
#define JeeUI2_h


#include "Arduino.h"

#ifdef ESP8266
#include <ESPAsyncTCP.h>
#include <FS.h>
#include "LittleFS.h"
#else
#include <AsyncTCP.h>
#include "LittleFS.h"
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

#include <Ticker.h>   // esp планировщик

#include <AsyncMqttClient.h>
#include "../../include/LList.h"
#include "../../include/misc.h"

#ifndef __DISABLE_BUTTON0
#define __BUTTON 0 // Кнопка "FLASH" на NODE_MCU
#endif

#ifndef __IDPREFIX
#define __IDPREFIX F("JeeUI2-")
#endif

class Interface;

class jeeui2
{
    typedef void (*buttonCallback) (Interface *interf, JsonObject *data);
    typedef void (*mqttCallback) ();

    typedef struct section_handle_t{
      String name;
      buttonCallback callback;
    } section_handle_t;

    DynamicJsonDocument cfg;
    LList<section_handle_t*> section_handle;
    AsyncMqttClient mqttClient;

  public:
    jeeui2() : cfg(4096), section_handle(), server(80), ws("/ws"){
//      *ip='\0';
      *mc='\0';
//      *mac='\0';
      *m_pref='\0';
      *m_host='\0';
      *m_user='\0';
      *m_pass='\0';
      *m_host='\0';
      *udpRemoteIP='\0';
      *incomingPacket='\0';
      *udpMessage='\0';
    }

    AsyncWebServer server;
    AsyncWebSocket ws;

    void var(const String &key, const String &value, bool force = false);
    void var_create(const String &key, const String &value);
    void section_handle_add(const String &btn, buttonCallback response);
    const char* param(const char* key);
    String param(const String &key);
    void led(uint8_t pin, bool invert);
    String deb();
    void init();
    void begin();
    void handle();

    void save(const char *_cfg = nullptr, bool force = false);
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
    void mqtt_reconnect();

    void subscribe(const String &topic);
    void publish(const String &topic, const String &payload);
    void publish(const String &topic, const String &payload, bool retained);

    void remControl();

    /**
     * Подключение к WiFi AP в клиентском режиме
     */
    void wifi_connect(const char *ssid=nullptr, const char *pwd=nullptr);
    bool wifi_sta = false;    // флаг успешного подключения к нешне WiFi-AP, (TODO: переделать на события с коллбеками)

    void post(JsonObject data);
    void send_pub();

    char mc[13]; // id из mac-адреса "ffffffffffff"

    String id(const String &tpoic);

    static char m_pref[16]; // префикс MQTT
    char m_host[256]; // MQTT
    int m_port = 0;
    char m_user[64];
    char m_pass[64];

  private:
    void led_handle();
    //void nonWifiVar();
    void led_on();
    void led_off();
    void led_inv();
    void autosave();
    void udpBegin();
    void udpLoop();
    void btn();
    void getAPmac();

    void pub_mqtt(const String &key, const String &value);

    void mqtt_handle();
    bool mqtt_enable = false;

    //void _connected();
    void subscribeAll();

    char udpRemoteIP[16];
    unsigned int localUdpPort = 4243;
    char incomingPacket[64];

    unsigned int asave = 1000;
    bool isNeedSave = false;
    unsigned long astimer;

    // WiFi related
    WiFiEventHandler e1, e2, e3;
    WiFiMode wifi_mode;
    void onSTAConnected(WiFiEventStationModeConnected ipInfo);
    void onSTAGotIP(WiFiEventStationModeGotIP ipInfo);
    void onSTADisconnected(WiFiEventStationModeDisconnected event_info);
    /**
      * устанавлием режим WiFi
      */
    void wifi_setmode(WiFiMode mode);

    int LED_PIN = -1;
    bool LED_INVERT = false;
    uint8_t mn = 0;
    unsigned long a_ap = 0;
    uint8_t pg = 0;
    char udpMessage[65]; // буфер для сообщений Обмена по UDP

    void connectToMqtt();
    void onMqttConnect();
    static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    static void onMqttSubscribe(uint16_t packetId, uint8_t qos);
    static void onMqttUnsubscribe(uint16_t packetId);
    static void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    static void onMqttPublish(uint16_t packetId);
    static void _onMqttConnect(bool sessionPresent);

    static bool mqtt_connected;
    static bool mqtt_connect;
    static bool mqtt_remotecontrol;

    Ticker jeeschedw;        // планировщик WiFi
};

#endif