
//bool _wifi_connected = false;
#include "JeeUI2.h"
#include "user_interface.h"
#include "wi-fi.h"


#ifdef ESP8266
void jeeui2::onSTAConnected(WiFiEventStationModeConnected ipInfo)
{
    LOG(printf_P, PSTR("WiFi: connected to %s\r\n"), ipInfo.ssid.c_str());
}

void jeeui2::onSTAGotIP(WiFiEventStationModeGotIP ipInfo)
{
    wifi_setmode(WIFI_STA);            // Shutdown internal Access Point
    wifi_sta = true;
    jeeschedw.detach();
    LOG(printf_P, PSTR("WiFi: Got IP: %s\r\n"), ipInfo.ip.toString().c_str());
}

void jeeui2::onSTADisconnected(WiFiEventStationModeDisconnected event_info)
{
    wifi_sta = false;
    LOG(printf_P, PSTR("WiFi: Disconnected from SSID: %s, reason: %d\n"), event_info.ssid.c_str(), event_info.reason);
    if (jeeschedw.active() && WiFi.getMode()==WIFI_AP)
        return;

    jeeschedw.once_scheduled(WIFI_CONNECT_TIMEOUT, [this](){
            LOG(println, F("WiFi: enabling internal AP"));
            wifi_setmode(WIFI_AP);  // Enable internal AP if station connection is lost
            jeeschedw.once_scheduled(WIFI_RECONNECT_TIMER, [this](){wifi_setmode(WIFI_AP_STA); WiFi.begin();} );
            } );
}
#else
// need to test it under ESP32 (might not need any scheduler to handle both Client and AP at the same time)
void WiFiEvent(WiFiEvent_t event, system_event_info_t info)
{
    switch (event)
    {
    case SYSTEM_EVENT_STA_START:
        LOG(println, F("Station Mode Started"));
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        WiFi.mode(WIFI_STA);            // Shutdown internal Access Point
        LOG(printf_P, PSTR("Connected to: %s, got IP: %s\n", WiFi.SSID(), WiFi.localIP());
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        LOG(printf_P, PSTR("Disconnected from SSID: %s, reason: %d\n"), event_info.ssid.c_str(), event_info.reason);
        if(wifi_get_opmode() != WIFI_AP_STA){
            WiFi.mode(WIFI_AP_STA);         // Enable internal AP if station connection is lost
            LOG(println, F("Enabling internal AP"));
        }
        break;
    default:
        break;
    }
}
#endif

void jeeui2::wifi_connect(const char *ssid, const char *pwd)
{
    String hn = param(F("hostname"));
    String appwd = param(F("APpwd"));

    if (!hn.length())
        var(F("hostname"), String(__IDPREFIX) + mc, true);
    WiFi.hostname(hn);

    if (appwd.length()<WIFI_PSK_MIN_LENGTH)
        appwd = "";

    LOG(printf_P, PSTR("WiFi: set AP params to SSID:%s, pwd:%s\n"), hn.c_str(), appwd.c_str());
    WiFi.softAP(hn.c_str(), appwd.c_str());

    String apmode = param(F("APonly"));

    LOG(print, F("WiFi: start in "));
    if (apmode == F("true")){
        LOG(println, F("AP-only mode"));
        wifi_setmode(WIFI_AP);

    } else {
        LOG(println, F("AP/STA mode"));
        wifi_setmode(WIFI_AP_STA);

        if (ssid) {
            WiFi.begin(ssid, pwd);
            LOG(printf_P, PSTR("WiFi: client connecting to SSID:%s, pwd:%s\n"), ssid, pwd ? pwd : "");
        } else {
            WiFi.begin();   // use internaly stored last known credentials for connection
            LOG(println, F("WiFi reconecting..."));
        }
    }
}

void jeeui2::wifi_setmode(WiFiMode mode){
    LOG(printf_P, PSTR("WiFi: set mode: %d\n"), mode);
    WiFi.mode(mode);
}

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
    wifi_get_macaddr(SOFTAP_IF, _mac);

    sprintf_P(mc, PSTR("%02X%02X%02X%02X%02X%02X"), _mac[0], _mac[1], _mac[2], _mac[3], _mac[4], _mac[5]);
}