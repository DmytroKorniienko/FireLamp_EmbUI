#include "JeeUI2.h"
#include "interface.h"
#include "ui.h"

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

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
    if(type == WS_EVT_CONNECT){
        LOG(printf, "ws[%s][%u] connect MEM: %u\n", server->url(), client->id(), ESP.getFreeHeap());

        Interface *interf = new Interface(&jee, client);
        section_main_frame(interf, nullptr);
        delete interf;

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
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, data, info->len);

            const char *pkg = doc["pkg"];
            if (!pkg) return;
            if (!strcmp(pkg, "post")) {
                JsonObject data = doc["data"];
                jee.post(data);
            }
        }
  }
}

void jeeui2::post(JsonObject data){
    section_handle_t *section = nullptr;
    int count = 0;
    Interface *interf = new Interface(this, &ws, 512);
    interf->json_frame_value();

    for (JsonPair kv : data) {
        String key = kv.key().c_str(), val = kv.value();
        if (val != F("null")) {
            interf->value(key, val);
            ++count;
        }

        const char *kname = key.c_str();
        for (int i = 0; !section && i < section_handle.size(); i++) {
            const char *sname = section_handle[i]->name.c_str();
            const char *mall = strchr(sname, '*');
            unsigned len = mall? mall - sname - 1 : strlen(kname);
            if (strncmp(sname, kname, len) == 0) {
                section = section_handle[i];
            }
        };
    }

    if (count) {
        interf->json_frame_flush();
    } else {
        interf->json_frame_clear();
    }
    delete interf;

    if (section) {
        LOG(printf_P, PSTR("\nPOST SECTION: %s\n\n"), section->name.c_str());
        Interface *interf = new Interface(this, &ws);
        section->callback(interf, &data);
        delete interf;
    }
}

void jeeui2::send_pub(){
    if (!ws.count()) return;
    Interface *interf = new Interface(this, &ws, 512);
    pubCallback(interf);
    delete interf;
}

void jeeui2::var(const String &key, const String &value, bool force)
{
    // JsonObject of N element	8 + 16 * N
    unsigned len = key.length() + value.length() + 16;
    size_t cap = cfg.capacity(), mem = cfg.memoryUsage();

    LOG(printf_P, PSTR("WRITE: key (%s) value (%s) "), key.c_str(), value.substring(0, 15).c_str());
    if (!force && !cfg.containsKey(key)) {
        LOG(printf_P, PSTR("ERROR: KEY (%s) NOT INIT !!!!!!!!\n"), key.c_str());
        return;
    }

    if (cap - mem < len) {
        cfg.garbageCollect();
        LOG(printf_P, PSTR("garbage cfg %u(%u) of %u\n"), mem, cfg.memoryUsage(), cap);

    }
    if (cap - mem < len) {
        LOG(printf_P, PSTR("ERROR: KEY (%s) NOT WRITE !!!!!!!!\n"), key.c_str());
        return;
    }

    cfg[key] = value;
    isNeedSave = true;

    LOG(printf_P, PSTR("FREE: %u\n"), cap - cfg.memoryUsage());

    // if (mqtt_remotecontrol) {
    //     publish(String(F("jee/set/")) + key, value, true);
    // }
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

    LOG(printf_P, PSTR("REGISTER: %s\n"), name.c_str());
}

/**
 * Возвращает указатель на строку со значением параметра из конфига
 * В случае отсутствующего параметра возвращает пустой указатель
 */
const char* jeeui2::param(const char* key)
{
    const char* value = cfg[key];
    if (value){
        LOG(printf_P, PSTR("READ key (%s) value (%s) MEM: %u\n"), key, value, ESP.getFreeHeap());
    }

    return value;
}

/**
 * обертка над param в виде String
 * В случае несуществующего ключа возвращает пустую строку
 */
String jeeui2::param(const String &key)
{
    String value(param(key.c_str()));
    return value;
}

String jeeui2::deb()
{
    String cfg_str;
    serializeJson(cfg, cfg_str);
    return cfg_str;
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, FPSTR(PGmimetxt), F("Not found"));
}

void jeeui2::init(){
//#ifdef LAMP_DEBUG
//    nonWifiVar();
    load();
    LOG(println, String(F("CONFIG: ")) + jee.deb());
//#endif
    //ap(20000); // если в течении 20 секунд не удастся подключиться к Точке доступа - запускаем свою (параметр "wifi" сменится с AP на STA)

    //WiFi.persistent(false);     // не сохраняем креды от WiFi во флеш, т.к. они у нас уже лежат в конфиге
    #ifdef ESP8266
        e1 = WiFi.onStationModeGotIP(std::bind(&jeeui2::onSTAGotIP, this, std::placeholders::_1));
        e2 = WiFi.onStationModeDisconnected(std::bind(&jeeui2::onSTADisconnected, this, std::placeholders::_1));
        e3 = WiFi.onStationModeConnected(std::bind(&jeeui2::onSTAConnected, this, std::placeholders::_1));
    #else
        WiFi.onEvent(std::bind(&jeeui2::WiFiEvent, this, std::placeholders::_1));
    #endif

    wifi_connect();
    //LOG(println, String(F("MAC: ")) + jee.mac);
}

void jeeui2::begin(){
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
        AsyncWebParameter *prm = request->getParam(0);
        if (prm) {
            httpCallback(prm->name(), prm->value());
        }
        request->send(200, FPSTR(PGmimetxt), F("Ok"));
    });

    server.on(PSTR("/config"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        String config = deb();
        AsyncWebServerResponse *response = request->beginResponse(200, FPSTR(PGmimejson), config);

        response->addHeader(FPSTR(PGhdrcachec), FPSTR(PGnocache));
        request->send(response);
    });

    server.on(PSTR("/eff_config.json"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        request->send(LittleFS, F("/eff_config.json"), String(), true);
    });

    server.on(PSTR("/config.json"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        request->send(LittleFS, F("/config.json"), String(), true);
    });

    server.on(PSTR("/events_config.json"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        request->send(LittleFS, F("/events_config.json"), String(), true);
    });

    server.on(PSTR("/restart"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        __shouldReboot = true;
        request->send(200, FPSTR(PGmimetxt), F("Ok"));
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
        if (__shouldReboot) {
            request->redirect(F("/"));
        } else {
            AsyncWebServerResponse *response = request->beginResponse(200, FPSTR(PGmimetxt), F("FAIL"));
            response->addHeader(F("Connection"), F("close"));
            request->send(response);
        }
    },[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
        if (!index) {
#ifndef ESP32
            Update.runAsync(true);
#endif
            int type = (data[0] == 0xe9 || data[0] == 0x1f)? U_FLASH : U_FS;
            size_t size = (type == U_FLASH)? ((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000) : (uintptr_t)&_FS_end - (uintptr_t)&_FS_start;
            Serial.printf_P(PSTR("Update %s Start (%u)\n"), (type == U_FLASH)? F("FLASH") : F("FS"), request->contentLength());

            if (!Update.begin(size, type)) {
                Update.printError(Serial);
            }
        }
        if (!Update.hasError()) {
            if(Update.write(data, len) != len){
                Update.printError(Serial);
            }
        }
        if (final) {
            if(Update.end(true)){
                Serial.printf_P(PSTR("Update Success: %uB\n"), index+len);
            } else {
                Update.printError(Serial);
            }
        }
        uploadProgress(index + len, request->contentLength());
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

    // server all files from LittleFS root
    server.serveStatic("/", LittleFS, "/")
        .setDefaultFile("index.html")
        .setCacheControl("max-age=14400");

    server.onNotFound(notFound);

    server.begin();
}

void jeeui2::led(uint8_t pin, bool invert){
    if (pin == -1) return;
    LED_PIN = pin;
    LED_INVERT = invert;
    pinMode(LED_PIN, OUTPUT);
}

void jeeui2::handle(){
    if (__shouldReboot) {
        Serial.println(F("Rebooting..."));
        delay(100);
        ESP.restart();
    }
#ifdef ESP8266
    MDNS.update();
#endif
    //_connected();
    mqtt_handle();
    udpLoop();

    static unsigned long timer = 0;
    if (timer + 300U > millis()) return;
    timer = millis();

    btn();
    led_handle();
    autosave();
    ws.cleanupClients(4);

    //публикация изменяющихся значений
    static unsigned long timer_pub = 0;
    if (timer_pub + 10*1000 > millis()) return;
    timer_pub = millis();
    send_pub();
}

/*
void jeeui2::nonWifiVar(){
    getAPmac();
    if(param(F("wifi")) == F("null")) var(F("wifi"), F("AP"), true);
    if(param(F("ssid")) == F("null")) var(F("ssid"), F("JeeUI2"), true);
    if(param(F("pass")) == F("null")) var(F("pass"), "", true);
    if(param(F("ap_ssid")) == F("null")) var(F("ap_ssid"), String(__IDPREFIX) + mc, true);
    if(param(F("ap_pass")) == F("null")) var(F("ap_pass"), "", true);
}
*/
