#include "JeeUI2.h"
#include "ui.h"
#include "../../include/misc.h"

#ifdef LAMP_DEBUG
#include "MemoryInfo.h"
#endif

bool __shouldReboot; // OTA update reboot flag

extern jeeui2 jee;

static const char PGmimetxt[] PROGMEM  = "text/plain";
static const char PGmimecss[] PROGMEM  = "text/css";
static const char PGmimehtml[] PROGMEM = "text/html; charset=utf-8";
static const char PGmimejson[] PROGMEM = "application/json";
static const char PGhdrcontentenc[] PROGMEM = "Content-Encoding";
static const char PGhdrcachec[] PROGMEM = "Cache-Control";
static const char PGgzip[] PROGMEM = "gzip";
static const char PGnocache[] PROGMEM = "no-cache, no-store, must-revalidate";    // 10 days cache
static const char PGpmaxage[] PROGMEM = "public, max-age=864000";    // 10 days cache

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
    if(type == WS_EVT_CONNECT){
        LOG(printf, "ws[%s][%u] connect MEM: %u\n", server->url(), client->id(), ESP.getFreeHeap());

        Interface interf(&jee, client);
        block_main_frame(&interf);

    } else
    if(type == WS_EVT_DISCONNECT){
        LOG(printf, "ws[%s][%u] disconnect\n", server->url(), client->id());
    } else
    if(type == WS_EVT_ERROR){
        LOG(printf, "ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
    } else
    if(type == WS_EVT_PONG){
        LOG(printf, "ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
    } else
    if(type == WS_EVT_DATA){
        AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if(info->final && info->index == 0 && info->len == len){
            DynamicJsonDocument doc(200);
            deserializeJson(doc, data, info->len);

            const char *pkg = doc["pkg"];
            if (!pkg) return;
            if (!strcmp(pkg, "post")) {
                JsonArray arr = doc["data"];
                jee.post(arr);
            }
        }
  }
}

void jeeui2::post(JsonArray data){
    // В передаваемых параметрах может быть только один с обработчиком
    section_handle_t *section = nullptr;
    bool hide = false;

    Interface *interf = new Interface(this, &ws, 1024);
    interf->json_frame_value();

    for (size_t i=0; i < data.size(); i++) {
        JsonObject item = data[i];
        String key = item["key"], val = item["val"];

        for (int i = 0; !section && i < section_handle.size(); i++) {
            if (section_handle[i]->name == key) {
                hide = true;
                section = section_handle[i];
                LOG(printf_P, PSTR("post section: %s\n"), key.c_str());
            }
            var(key, val);
        };

        if (!hide) interf->value(key, val);
        hide = false;
    }
    interf->json_frame_flush();
    delete interf;

    jee.save();

    if (section) {
        Interface interf(this, &ws);
        section->callback(&interf);
    }
    // as();
}

void jeeui2::refresh(){
    if (!ws.count()) return;
    // send(&ws);
}

void jeeui2::send_pub(){
    Interface interf(this, &ws, 512);
    pubCallback(&interf);
}

void jeeui2::var(const String &key, const String &value)
{
    // JsonObject of N element	8 + 16 * N
    unsigned len = key.length() + value.length() + 16;
    size_t cap = cfg.capacity(), mem = cfg.memoryUsage();

    if (dbg) Serial.printf_P(PSTR("WRITE: key (%s) value (%s) "), key.c_str(), value.substring(0, 15).c_str());

    if (cap - mem < len) {
        cfg.garbageCollect();
        if (dbg) Serial.printf_P(PSTR("garbage cfg %u(%u) of %u\n"), mem, cfg.memoryUsage(), cap);

    }
    if (cap - mem < len) {
        if (dbg) Serial.printf_P(PSTR("KEY (%s) NOT WRITE !!!!!!!!\n"), key.c_str());
        return;
    }

    cfg[key] = value;

    if (dbg) Serial.printf_P(PSTR("FREE: %u\n"), cap - cfg.memoryUsage());

    if (_t_remotecontrol) {
        publish(String(F("jee/set/")) + key, value, true);
    }
}

void jeeui2::var_create(const String &key, const String &value)
{
    if(cfg[key].isNull()){
        cfg[key] = value;
        LOG(print, F("CREATE: "));
        LOG(printf_P, PSTR("key (%s) value (%s) RAM: %d\n"), key.c_str(), value.substring(0, 15).c_str(), ESP.getFreeHeap());
    }
}

void jeeui2::section_handle_add(const String &name, buttonCallback response)
{
    section_handle_t *section = new section_handle_t;
    section->name = name;
    section->callback = response;
    section_handle.add(section);

    LOG(print, F("REGISTER: "));
    LOG(printf_P, PSTR("BTN (%s) RAM: %d\n"), name.c_str(), ESP.getFreeHeap());
}

String jeeui2::param(const String &key)
{
    String value = cfg[key].as<String>();
    LOG(print, F("READ: "));
    LOG(printf_P, PSTR("key (%s) value (%s) MEM: %u\n"), key.c_str(), value.c_str(), ESP.getFreeHeap());
    return value;
}

String jeeui2::deb()
{
    String cfg_str;
    serializeJson(cfg, cfg_str);
    return cfg_str;
}

void jeeui2::begin(bool debug) {
    dbg = debug;
    nonWifiVar();
    load();
    LOG(println, String(F("CONFIG: ")) + deb());
    begin();
    LOG(println, String(F("RAM: ")) + String(ESP.getFreeHeap()));
    LOG(println, String(F("MAC: ")) + mac);
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, FPSTR(PGmimetxt), F("Not found"));
}

void jeeui2::begin() {
    wifi_connect();

    /*use mdns for host name resolution*/
    char tmpbuf[32]; // Используем ap_ssid если задан, иначе конструируем вручную
    if(param(F("ap_ssid")).length()>0)
        strncpy_P(tmpbuf,param(F("ap_ssid")).c_str(),sizeof(tmpbuf)-1);
    else
        sprintf_P(tmpbuf,PSTR("%s%s"),(char*)__IDPREFIX, mc);
    if (!MDNS.begin(tmpbuf)) {
        Serial.println(F("Error setting up MDNS responder!"));
        while (1) {
        delay(1000);
        }
    }
    MDNS.addService(F("http"), F("tcp"), 80);
    Serial.printf_P(PSTR("mDNS responder started: %s.local\n"),tmpbuf);
    // MDNS.setHostname();
    // Serial.printf_P(PSTR("mDNS responder started: %s.local\n"), hostname);

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    // Добавлено для отладки, т.е. возможности получить JSON интерфейса для анализа
    server.on(PSTR("/echo"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        // jee.send(request);
    });

    server.on(PSTR("/version"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        String buf;
        buf = F("VERSION: "); buf+=F(VERSION);
        buf += F("\nGIT: "); buf+=F(PIO_SRC_REV);
        buf += F("\nOK\n");
        request->send(200, FPSTR(PGmimetxt), buf.c_str());
    });

    server.on(PSTR("/cmd"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebParameter *p;
        p = request->getParam(0);
        strncpy(httpParam,p->name().c_str(),sizeof(httpParam)-1);
        strncpy(httpValue,p->value().c_str(),sizeof(httpValue)-1);
        _isHttpCmd = true;
        request->send(200, FPSTR(PGmimetxt), F("Ok"));
    });

    server.on(PSTR("/config"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        String config = deb();
        AsyncWebServerResponse *response = request->beginResponse(200, FPSTR(PGmimejson), config);

        response->addHeader(FPSTR(PGhdrcachec), FPSTR(PGnocache));
        request->send(response);
    });

    server.on(PSTR("/eff_config.json"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        request->send(SPIFFS, F("/eff_config.json"), String(), true);
    });

    server.on(PSTR("/config.json"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        request->send(SPIFFS, F("/config.json"), String(), true);
    });

    server.on(PSTR("/events_config.json"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        request->send(SPIFFS, F("/events_config.json"), String(), true);
    });

    // server all files from SPIFFS root
    server.serveStatic("/", SPIFFS, "/")
        .setDefaultFile("index.html")
        .setCacheControl("max-age=864000");

    server.on(PSTR("/restart"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        request->redirect("/heap");
        delay(1000);
        ESP.restart();
    });

    server.on(PSTR("/heap"), HTTP_GET, [this](AsyncWebServerRequest *request){
        String out = "Heap: "+String(ESP.getFreeHeap());
#ifdef LAMP_DEBUG
        out += "\nFrac: " + String(getFragmentation());
        out += "\nClient: " + String(ws.count());
#endif
        request->send(200, FPSTR(PGmimetxt), out);
    });

    // Simple Firmware Update Form
    server.on(PSTR("/update"), HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, FPSTR(PGmimehtml), F("<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>"));
    });

    server.on(PSTR("/update"), HTTP_POST, [](AsyncWebServerRequest *request){
        __shouldReboot = !Update.hasError();
        AsyncWebServerResponse *response = request->beginResponse(200, FPSTR(PGmimetxt), (__shouldReboot?F("OK"):F("FAIL")));
        response->addHeader(F("Connection"), F("close"));
        request->send(response);
    },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
        if(!index){
            Serial.printf_P(PSTR("Update Start: %s\n"), filename.c_str());
#ifndef ESP32
            Update.runAsync(true);
#endif
            if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)){
                Update.printError(Serial);
            }
        }
        if(!Update.hasError()){
            if(Update.write(data, len) != len){
                Update.printError(Serial);
            }
        }
        if(final){
            if(Update.end(true)){
                Serial.printf_P(PSTR("Update Success: %uB\n"), index+len);
            } else {
                Update.printError(Serial);
            }
        }
    });

    //First request will return 0 results unless you start scan from somewhere else (loop/setup)
    //Do not request more often than 3-5 seconds
    server.on(PSTR("/scan"), HTTP_GET, [](AsyncWebServerRequest *request){
    String json = F("[");
    int n = WiFi.scanComplete();
    if(n == -2){
        WiFi.scanNetworks(true);
    } else if(n){
        for (int i = 0; i < n; ++i){
        if(i) json += F(",");
        json += F("{");
        json += String(F("\"rssi\":"))+String(WiFi.RSSI(i));
        json += String(F(",\"ssid\":\""))+WiFi.SSID(i)+F("\"");
        json += String(F(",\"bssid\":\""))+WiFi.BSSIDstr(i)+F("\"");
        json += String(F(",\"channel\":"))+String(WiFi.channel(i));
        json += String(F(",\"secure\":"))+String(WiFi.encryptionType(i));
#ifdef ESP8266
        json += String(F(",\"hidden\":"))+String(WiFi.isHidden(i)?F("true"):F("false")); // что-то сломали и в esp32 больше не работает...
#endif
        json += F("}");
        }
        WiFi.scanDelete();
        if(WiFi.scanComplete() == -2){
        WiFi.scanNetworks(true);
        }
    }
    json += F("]");
    request->send(200, FPSTR(PGmimejson), json);
    json = String();
    });

    server.onNotFound(notFound);

    server.begin();

    //fcallback_upd();
    mqtt_update();
}

void jeeui2::led(uint8_t pin, bool invert)
{
    if (pin == -1) return;
    LED_PIN = pin;
    LED_INVERT = invert;
    pinMode(LED_PIN, OUTPUT);
}

void jeeui2::handle()
{
    if(_isHttpCmd){
        if(fcallback_http != nullptr)
            fcallback_http(httpParam, httpValue);
        _isHttpCmd = false;
        *httpParam='\0'; *httpValue='\0';
    }

    if(__shouldReboot){
        Serial.println(F("Rebooting..."));
        delay(100);
        ESP.restart();
    }
#ifdef ESP8266
    MDNS.update();
#endif
    _connected();
    mqtt_handle();
    udpLoop();

    static unsigned long timer = 0;
    if (timer + 300U > millis()) return;
    timer = millis();

    btn();
    led_handle();
    button_handle();
    pre_autosave();
    autosave();
    ws.cleanupClients(4);

    //публикация изменяющихся значений
    static unsigned long timer_pub = 0;
    if (timer_pub + 10*1000 > millis()) return;
    timer_pub = millis();
    send_pub();
}

void jeeui2::nonWifiVar(){
    getAPmac();
    if(param(F("wifi")) == F("null")) var(F("wifi"), F("AP"));
    if(param(F("ssid")) == F("null")) var(F("ssid"), F("JeeUI2"));
    if(param(F("pass")) == F("null")) var(F("pass"), "");
    if(param(F("ap_ssid")) == F("null")) var(F("ap_ssid"), String(__IDPREFIX) + mc);
    if(param(F("ap_pass")) == F("null")) var(F("ap_pass"), "");
}

void jeeui2::getAPmac(){
    if(*mc) return;
    #ifdef ESP32
    WiFi.mode(WIFI_MODE_AP);
    #else
    WiFi.mode(WIFI_AP);
    #endif
    String _mac(WiFi.softAPmacAddress());
    _mac.replace(F(":"), F(""));
    strncpy(mc, _mac.c_str(), sizeof(mc)-1);
}
