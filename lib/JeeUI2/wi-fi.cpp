
//bool _wifi_connected = false;
#include "JeeUI2.h"
#include "user_interface.h"


#ifdef ESP8266
void jeeui2::onSTAConnected(WiFiEventStationModeConnected ipInfo)
{
    Serial.printf_P(PSTR("Connected to %s\r\n"), ipInfo.ssid.c_str());
}

void jeeui2::onSTAGotIP(WiFiEventStationModeGotIP ipInfo)
{
    //strncpy(ip,WiFi.localIP().toString().c_str(),sizeof(ip)-1);
    //WiFi.localIP().printTo(ip);
    //ipInfo.ip.printTo(ip);
    Serial.printf_P(PSTR("Got IP: %s\r\n"), ipInfo.ip.toString().c_str());
    Serial.printf_P(PSTR("Connected: %s\r\n"), WiFi.status() == WL_CONNECTED ? F("yes") : F("no"));
    WiFi.mode(WIFI_STA);            // Shutdown internal Access Point
//    _wifi_connected = true;
}

void jeeui2::onSTADisconnected(WiFiEventStationModeDisconnected event_info)
{
    LOG(printf_P, PSTR("Disconnected from SSID: %s, reason: %d\n"), event_info.ssid.c_str(), event_info.reason);
    if(wifi_get_opmode() != WIFI_AP_STA){
        WiFi.mode(WIFI_AP_STA);         // Enable internal AP if station connection is lost
        LOG(println, F("Enabling internal AP"));
    }

//    _wifi_connected = false;
}
#else
void WiFiEvent(WiFiEvent_t event, system_event_info_t info)
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_START:
        Serial.println(F("Station Mode Started"));
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("Connected to :" + String(WiFi.SSID()));
        Serial.print(F("Got IP: "));
        Serial.println(WiFi.localIP());
        _wifi_connected = true;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println(F("Disconnected from station, attempting reconnection"));
        WiFi.reconnect();
        _wifi_connected = false;
        break;
    default:
        break;
    }
}
#endif

/*
 void jeeui2::_connected(){
     connected = _wifi_connected;
 }
*/

void jeeui2::wifi_connect(bool usecfg)
{
//    bool wf = false;
    String wifi = param(F("wifi"));
//    if (wifi == F("AP")) wifi_mode = 1;
//    if (wifi == F("STA")) wifi_mode = 2;
//    if (wifi == F("TMP_AP")) {wifi_mode = 1; var(F("wifi"), F("STA"));}
//    switch (wifi_mode)

    WiFi.softAP(param(F("ap_ssid")).c_str(), param(F("pass")).c_str());
    WiFi.hostname(param(F("ap_ssid")).c_str());

    if(wifi == F("AP"))
    {
        //WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        //wifi_mode = WIFI_AP;
        LOG(println, F("Start Wi-Fi AP mode!"));
//        _wifi_connected = false;
    } else {
        WiFi.mode(WIFI_AP_STA);

        if (usecfg) {
            WiFi.begin(param(F("ssid")).c_str(), param(F("pass")).c_str());
        } else {
            WiFi.begin();   // use internaly stored known credentials for connection
        }
        LOG(println, F("Connecting STA"));


        //WiFi.macAddress(macaddr);
// зачем нам собственный мак вообще?
/*
        uint8_t macaddr[6];
        WiFi.macAddress(macaddr);
        String _mac;
        for (byte i = 0; i < 6; ++i) {
            _mac += String(macaddr[i], HEX);
            if (i < 5)
                _mac += ':';
        }
        strncpy(mac,_mac.c_str(),sizeof(mac)-1);
*/
    }

    //WiFi.scanNetworks(true);      // зачем оно нам?
}

/*
void jeeui2::ap(unsigned long interval){
    a_ap = interval;
}
*/

/**
 * формирует chipid из MAC-адреса вида aabbccddeeff
 */
void jeeui2::getAPmac(){
    if(*mc) return;
/*
    #ifdef ESP32
    WiFi.mode(WIFI_MODE_AP);
    #else
    WiFi.mode(WIFI_AP);
    #endif
    String _mac(WiFi.softAPmacAddress());
    _mac.replace(F(":"), F(""));
    strncpy(mc, _mac.c_str(), sizeof(mc)-1);
*/
    uint8_t _mac[6];
    //char macStr[18] = { 0 };
    wifi_get_macaddr(SOFTAP_IF, _mac);

    sprintf_P(mc, PSTR("%02X%02X%02X%02X%02X%02X"), _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]);

    //String _mac(WiFi.softAPmacAddress());
}