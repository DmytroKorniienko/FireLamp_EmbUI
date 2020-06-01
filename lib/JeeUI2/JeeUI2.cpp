#include "JeeUI2.h"

#ifdef LAMP_DEBUG
#include "MemoryInfo.h"
#endif

bool __shouldReboot; // OTA update reboot flag

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

extern jeeui2 jee;

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
    if(type == WS_EVT_CONNECT){
       if(jee.dbg) Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
       client->text(jee.get_interface());
    } else
    if(type == WS_EVT_DISCONNECT){
        if(jee.dbg) Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
    } else
    if(type == WS_EVT_ERROR){
        if(jee.dbg) Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
    } else
    if(type == WS_EVT_PONG){
        if(jee.dbg) Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
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
                for (size_t i=0; i < arr.size(); i++) {
                    JsonObject item = arr[i];
                    jee.post(item["key"], item["val"]);
                }
            }
        }
  }
}

void jeeui2::post(const String &key, const String &value){
    if (!key || !value) return;
    if (key.indexOf(F("BTN_")) != -1){
        strncpy(btnui, key. substring(4, key.length()).c_str(), sizeof(btnui)-1);
        if(dbg) Serial.printf_P(PSTR("BUTTON PRESS: %s\n"), btnui);
        if(strcmp_P(btnui, PSTR("_sysReset")) == 0){
            var(F("wifi"), F("STA"));
            save();
            ESP.restart();
        }
    } else {
        var(String(key), String(value));
        as();
    }
}

String jeeui2::get_interface(){
    if (buf.length()) buf = "";
    foo();
    return buf;
}

void jeeui2::refresh()
{
    if (!ws.count()) return;

    //String b = get_interface(); // то есть мы перепысываем огромный буффер в новую переменную?
    if(dbg)Serial.printf_P(PSTR("WS BEFORE: [%u]\n"), ESP.getFreeHeap());
    //if (b.length()) ws.textAll(b);
    get_interface();
    if (buf.length()) ws.textAll(buf); // get_interface переписывает переменную buf, ИМХО ее можно использовать на прямую
    if(dbg)Serial.printf_P(PSTR("WS AFTER: [%u]\n"), ESP.getFreeHeap());
}

void jeeui2::var(const String &key, const String &value, bool pub)
{
    if(pub_transport.containsKey(key) || pub){
        String tmp;
        pub_transport[key] = value;
        serializeJson(pub_transport, tmp);
        deserializeJson(pub_transport, tmp);
        //if(dbg)Serial.printf_P(PSTR("serializeJson: [%s]: %s\n"), key.c_str(), pub_transport.as<String>().c_str());
        if(pub) return;
    }
    if(pub_enable){
        JsonVariant pub_key = pub_transport[key];
        if (!pub_key.isNull()) {
            pub_transport[key] = value;
            //if(dbg)Serial.printf_P(PSTR("Pub: [%s - %s]\n"), key.c_str(), value.c_str());
            //pub_mqtt(key, value);
            publish(String(F("jee/pub/")) + key, value, true);
            return;
        }
    }
    if(dbg)Serial.print(F("WRITE: "));
    if(dbg)Serial.printf_P(PSTR("key (%s) value (%s) RAM: %d\n"), key.c_str(), value.substring(0, 15).c_str(), ESP.getFreeHeap());
    cfg[key] = value;
    if(rc)publish(String(F("jee/set/")) + key, value, true);
}

void jeeui2::var_create(const String &key, const String &value)
{
    if(cfg[key].isNull()){
        cfg[key] = value;
        if(dbg)Serial.print(F("CREATE: "));
        if(dbg)Serial.printf_P(PSTR("key (%s) value (%s) RAM: %d\n"), key.c_str(), value.substring(0, 15).c_str(), ESP.getFreeHeap());
    }
}

void jeeui2::btn_create(const String &btn, buttonCallback response)
{
    //return;
    if(!btn_id.containsKey(btn)){
        JsonArray arr; // добавляем в очередь
        String tmp;

        if(!btn_id.isNull())
            arr = btn_id.as<JsonArray>(); // используем имеющийся
        else
            arr = btn_id.to<JsonArray>(); // создаем новый

        JsonObject var = arr.createNestedObject();
        var[F("b")]=btn;
        var[F("f")]=(unsigned long)response;

        if(dbg)Serial.print(F("REGISTER: "));
        if(dbg)Serial.printf_P(PSTR("BTN (%s) RAM: %d\n"), btn.c_str(), ESP.getFreeHeap());

        serializeJson(btn_id, tmp); // Тут шаманство, чтобы не ломало JSON
        deserializeJson(btn_id, tmp);
    }
}

String jeeui2::param(const String &key)
{
    //String value = cfg[key];
    if(dbg)Serial.print(F("READ: "));
    if(dbg)Serial.printf_P(PSTR("key (%s) value (%s) RAM: %d\n"), key.c_str(), cfg[key].as<String>().c_str(), ESP.getFreeHeap());
    return cfg[key];
}

String jeeui2::deb()
{
    String cfg_str;
    serializeJson(cfg, cfg_str);
    deserializeJson(cfg, cfg_str); // сильное колдунство #%@%#@$ (пытаемся починить ломающийся json если долго не было сохранений)
    return cfg_str;
}

void jeeui2::begin(bool debug) {
    dbg = debug;
    nonWifiVar();
    load();
    if(dbg)Serial.println(String(F("CONFIG: ")) + deb());
    begin();
    if(dbg)Serial.println(String(F("RAM: ")) + String(ESP.getFreeHeap()));
    if(dbg)Serial.println(String(F("MAC: ")) + mac);
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

    server.on(PSTR("/post"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        uint8_t params = request->params();
        for (uint8_t i = 0; i < params; i++) {
            AsyncWebParameter *p = request->getParam(i);
            post(p->name(), p->value());
        }
        request->send(200, FPSTR(PGmimetxt), F("OK"));
    });

    server.on(PSTR("/pub"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebParameter *p;
        String value = F("");
        p = request->getParam(0);
        JsonVariant pub_key = pub_transport[p->name()];
        if (!pub_key.isNull()) {
            value = pub_transport[p->name()].as<String>();
            if(dbg)Serial.printf_P(PSTR("pub: [%s - %s - %s]\n"),p->name().c_str(), p->value().c_str(), value.c_str());
        }
        request->send(200, FPSTR(PGmimetxt), pub_transport[p->name()].isNull()?p->value():pub_transport[p->name()].as<String>()); //p->value());
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

    server.on(PSTR("/heap"), HTTP_GET, [](AsyncWebServerRequest *request){
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
    //foo();    //WHY???
    //upd();
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
        if(httpfunc != nullptr)
            httpfunc(httpParam, httpValue);
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
    static unsigned long timer;
    if (timer + 300U > millis())
        return;
    timer = millis();
    btn();
    led_handle();
    button_handle();
    pre_autosave();
    autosave();
    ws.cleanupClients(4);
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

// push data to http-responce buffer
void jeeui2::uiPush(){
    if (httpstream == nullptr) return;  // nowhere to push

    httpstream->print(buf);
    buf = "";
}
