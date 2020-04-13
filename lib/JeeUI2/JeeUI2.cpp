#include "JeeUI2.h"

AsyncWebServer server(80);
bool __shouldReboot; // OTA update reboot flag

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
    if(rc)publish(String(F("jee/set/")) + key, value, true);
    if(dbg)Serial.print(F("WRITE: "));
    if(dbg)Serial.printf_P(PSTR("key (%s) value (%s) RAM: %d\n"), key.c_str(), value.substring(0, 15).c_str(), ESP.getFreeHeap());
    cfg[key] = value;
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
    request->send(404, F("text/plain"), F("Not found"));
}

void jeeui2::begin() { 
    wifi_connect();

    /*use mdns for host name resolution*/
    char tmpbuf[32];
    sprintf_P(tmpbuf,PSTR("%s%s"),(char*)__IDPREFIX, mc);    
    if (!MDNS.begin(tmpbuf)) {
        Serial.println(F("Error setting up MDNS responder!"));
        while (1) {
        delay(1000);
        }
    }
    MDNS.addService(F("http"), F("tcp"), 80);
    Serial.printf_P(PSTR("mDNS responder started: %s.local\n"),tmpbuf);

    server.on(PSTR("/post"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        uint8_t params = request->params();
        AsyncWebParameter *p;
        for (uint8_t i = 0; i < params; i++)
        {
          p = request->getParam(i);
          if (p->name().indexOf(F("BTN_")) != -1){
                strncpy(btnui,p->name().substring(4, p->name().length()).c_str(),sizeof(btnui)-1); // btnui = p->name().substring(4, p->name().length());
                if(dbg) Serial.printf_P(PSTR("BUTTON PRESS: %s\n"),btnui);
                if(strcmp_P(btnui, PSTR("_sysReset"))==0){
                    var(F("wifi"), F("STA"));
                    save();
                    ESP.restart();
                }
          } 
          else {
            var(String(p->name()), String(p->value()));
            as();
          }
        }
        request->send(200, F("text/plain"), F("OK"));
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
        request->send(200, F("text/plain"), pub_transport[p->name()].isNull()?p->value():pub_transport[p->name()].as<String>()); //p->value());
    });

    server.on(PSTR("/echo"), HTTP_ANY, [this](AsyncWebServerRequest *request) { 
        if(dbg)Serial.println(F("Вызов /echo"));
        foo();

        String res = buf;
        AsyncWebServerResponse *response = request->beginResponse(200, F("text/plain"), res);

        response->addHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
        //response->addHeader(F("Pragma"),F("no-cache"));
        //response->addHeader(F("Expires"),F("0"));
        request->send(response);
        //request->send(200, F("text/plain"), buf);

        // AsyncJsonResponse * response = new AsyncJsonResponse();

        // response->addHeader(F("Server"),F("ESP Async Web Server"));
        // response->addHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));

        // DynamicJsonDocument doc(4096);
        // serializeJson(doc,buf);
        // response->getRoot().set(doc.to<JsonVariant>());
        // response->setLength();
        // request->send(response);

        if(dbg)Serial.println(buf);
        buf = F("");
    });

    server.on(PSTR("/_refresh"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        static unsigned long echoTm; // сброс только через секунду
        char buffer[20];

        if(!_refresh)
            {echoTm = millis();}
        else if(echoTm+1500<millis()){ // 1.5 секунды при цикле опроса в 1 секунду
            _refresh = false;
            echoTm = millis();
        }
        sprintf_P(buffer,PSTR("{\"_refresh\":%d}"), _refresh);
        AsyncWebServerResponse *response = request->beginResponse(200, F("text/plain"), buffer);

        response->addHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
        //response->addHeader(F("Pragma"),F("no-cache"));
        //response->addHeader(F("Expires"),F("0"));
        request->send(response);
    });

    server.on(PSTR("/config"), HTTP_ANY, [this](AsyncWebServerRequest *request) { 
        String config = deb();
        AsyncWebServerResponse *response = request->beginResponse(200, F("text/plain"), config);

        response->addHeader(F("Cache-Control"), F("no-cache, no-store, must-revalidate"));
        //response->addHeader(F("Pragma"),F("no-cache"));
        //response->addHeader(F("Expires"),F("0"));
        request->send(response);
        //config = F("");
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

    server.on(PSTR("/js/maker.js"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, F("/js/maker.js.gz"), F("application/javascript"));
        response->addHeader(F("Content-Encoding"), F("gzip"));
        request->send(response);
    });

    server.on(PSTR("/"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        if(loading) {
            request->send(500, F("text/plain"), F("Server busy... Try again later"));
            return;
        }
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, F("/index.html.gz"), F("text/html"));
        response->addHeader(F("Content-Encoding"), F("gzip"));
        request->send(response);
        //if(dbg) Serial.println(F("LOADING BLOCK: index.htm"));
        tm_loading = millis();
        loading = true;
    });

    server.on(PSTR("/css/pure-min.css"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, F("/css/pure-min.css.gz"), F("text/css"));
        response->addHeader(F("Content-Encoding"), F("gzip"));
        request->send(response);
    });

    server.on(PSTR("/css/side-menu.css"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, F("/css/side-menu.css.gz"), F("text/css"));
        response->addHeader(F("Content-Encoding"), F("gzip"));
        request->send(response);
    });

    server.on(PSTR("/css/range.css"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, F("/css/range.css.gz"), F("text/css"));
        response->addHeader(F("Content-Encoding"), F("gzip"));
        request->send(response);
    });

    server.on(PSTR("/css/grids.css"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, F("/css/grids.css.gz"), F("text/css"));
        response->addHeader(F("Content-Encoding"), F("gzip"));
        request->send(response);
    });

    server.on(PSTR("/css/chk.css"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, F("/css/chk.css.gz"), F("text/css"));
        response->addHeader(F("Content-Encoding"), F("gzip"));
        request->send(response);
    });

    server.on(PSTR("/js/ui.js"), HTTP_ANY, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, F("/js/ui.js.gz"), F("application/javascript"));
        response->addHeader(F("Content-Encoding"), F("gzip"));
        request->send(response);
    });

    server.on(PSTR("/favicon.ico"), HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(SPIFFS, F("/favicon.ico.gz"), F("image/x-icon"));
        response->addHeader(F("Content-Encoding"), F("gzip"));
        request->send(response);
    });

    server.on(PSTR("/heap"), HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, F("text/plain"), String(ESP.getFreeHeap()));
    });

    // Simple Firmware Update Form
    server.on(PSTR("/update"), HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, F("text/html"), F("<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>"));
    });

    server.on(PSTR("/update"), HTTP_POST, [](AsyncWebServerRequest *request){
        __shouldReboot = !Update.hasError();
        AsyncWebServerResponse *response = request->beginResponse(200, F("text/plain"), (__shouldReboot?F("OK"):F("FAIL")));
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
    request->send(200, F("application/json"), json);
    json = String();
    });

    server.onNotFound(notFound);

    server.begin();
    foo();
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
