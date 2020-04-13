#include "JeeUI2.h"

bool jeeui2::mqtt_connected = false;
bool jeeui2::mqtt_connect = false;
bool jeeui2::_t_inc_current = false;
bool jeeui2::_t_remotecontrol = false;
char jeeui2::_t_tpc_current[128];
char jeeui2::_t_pld_current[128];
char jeeui2::m_pref[16];

// bool _t_inc_current = false;
// String _t_tpc_current;
// String _t_pld_current;
// String _t_prf_current;
// bool _t_remotecontrol;

// String _m_host;
// int _m_port;
// String _m_user;
// String _m_pass;
// bool _m_params;

void jeeui2::connectToMqtt() {
  Serial.println(F("Connecting to MQTT..."));
  mqttClient.connect();
}
       
String jeeui2::id(const String &topic){
    if(!*m_pref) return topic;
    else {
        String ret;
        ret = m_pref; ret += '/'; ret += topic;
        return ret;
    }
}

void jeeui2::onMqttSubscribe(uint16_t packetId, uint8_t qos) {

}

void jeeui2::onMqttUnsubscribe(uint16_t packetId) {

}

void jeeui2::onMqttPublish(uint16_t packetId) {

}

typedef void (*mqttCallback) (const String &topic, const String &payload);
mqttCallback mqt;

void fake(){}
void emptyFunction(const String &, const String &){}

void jeeui2::mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), bool remotecontrol){
    
    
    if(param(F("m_pref")) == F("null")) var(F("m_pref"), pref);
    if(param(F("m_host")) == F("null")) var(F("m_host"), host);
    if(param(F("m_port")) == F("null")) var(F("m_port"), String(port));
    if(param(F("m_user")) == F("null")) var(F("m_user"), user);
    if(param(F("m_pass")) == F("null")) var(F("m_pass"), pass);

    mqtt_enable = true;
    mqtt_update();

    //_t_prf_current = m_pref;
    if (remotecontrol) _t_remotecontrol = true; rc = true;
    mqt = mqttFunction;

}

void jeeui2::mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload)){
    mqtt(pref, host, port, user, pass, mqttFunction, false);
    onConnect = fake;
}

void jeeui2::mqtt(const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload)){
    getAPmac();
    mqtt(mc, host, port, user, pass, mqttFunction, false);
    onConnect = fake;
}

void jeeui2::mqtt(const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), bool remotecontrol){
    getAPmac();
    mqtt(mc, host, port, user, pass, mqttFunction, remotecontrol);
    onConnect = fake;
}

void jeeui2::mqtt(const String &host, int port, const String &user, const String &pass, bool remotecontrol){
    getAPmac();
    mqtt(mc, host, port, user, pass, emptyFunction, remotecontrol);
    onConnect = fake;
}

void jeeui2::mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, bool remotecontrol){
    getAPmac();
    mqtt(pref, host, port, user, pass, emptyFunction, remotecontrol);
    onConnect = fake;
}

void jeeui2::mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) (), bool remotecontrol){
    getAPmac();
    onConnect = mqttConnect;
    mqtt(pref, host, port, user, pass, emptyFunction, remotecontrol);

}
void jeeui2::mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) ()){
    getAPmac();
    onConnect = mqttConnect;
    mqtt(pref, host, port, user, pass, mqttFunction, false);
}
void jeeui2::mqtt(const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) ()){
    getAPmac();
    onConnect = mqttConnect;
    mqtt(mc, host, port, user, pass, mqttFunction, false);
}
void jeeui2::mqtt(const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) (), bool remotecontrol){
    getAPmac();
    onConnect = mqttConnect;
    mqtt(mc, host, port, user, pass, mqttFunction, remotecontrol);
}

void jeeui2::mqtt_update(){
    if(!mqtt_enable) return;
    strncpy(m_pref,param(F("m_pref")).c_str(),sizeof(m_pref)-1); // m_pref = param(F("m_pref"));
    strncpy(m_host,param(F("m_host")).c_str(),sizeof(m_host)-1); // m_host = param(F("m_host"));
    strncpy(m_user,param(F("m_user")).c_str(),sizeof(m_user)-1); // m_user = param(F("m_user"));
    strncpy(m_pass,param(F("m_pass")).c_str(),sizeof(m_pass)-1); // m_pass = param(F("m_pass"));
    m_port = param(F("m_port")).toInt();
    if(!*m_host) m_params = false;
    else m_params = true;
}

//bool first_connect = true;

void jeeui2::mqtt_handle(){
    if(!connected || !m_params) return;
    static bool st = false;
    if(!st){
        st = true;
        strncpy(m_host,param(F("m_host")).c_str(),sizeof(m_host)-1); // m_host = param(F("m_host"));
        strncpy(m_user,param(F("m_user")).c_str(),sizeof(m_user)-1); // m_user = param(F("m_user"));
        strncpy(m_pass,param(F("m_pass")).c_str(),sizeof(m_pass)-1); // m_pass = param(F("m_pass"));

        m_port = param(F("m_port")).toInt();
        
        mqttClient.onConnect(_onMqttConnect);
        mqttClient.onDisconnect(onMqttDisconnect);
        mqttClient.onSubscribe(onMqttSubscribe);
        mqttClient.onUnsubscribe(onMqttUnsubscribe);
        mqttClient.onMessage(onMqttMessage);
        mqttClient.onPublish(onMqttPublish);
        mqttClient.setCredentials(m_user, m_pass);
        mqttClient.setServer(m_host, m_port);
        //mqttClient.setMaxTopicLength(48);
    }

    if(mqtt_connect) onMqttConnect();
    check_wifi_state();
    remControl();
}

void jeeui2::check_wifi_state(){
    //static bool old_wifi_connected = false;
    static unsigned long i;
    if(i + 5000 > millis()) return;
    i = millis();
    if(connected && !mqtt_connected) connectToMqtt();
}

void jeeui2::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println(F("Disconnected from MQTT."));
  mqtt_connect = false;
  mqtt_connected = false;

}

void jeeui2::_onMqttConnect(bool sessionPresent) {
    mqtt_connect = true;
    
}

void jeeui2::onMqttConnect(){
    mqtt_connect = false;
    mqtt_connected = true;
    Serial.println(F("Connected to MQTT."));
    if(_t_remotecontrol){
        subscribeAll();
    }
}

void jeeui2::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    Serial.print(F("Publish received: "));
    Serial.println(topic);

    String tpc = String(topic);
    if(*m_pref) tpc = tpc.substring(strlen(m_pref) + 1, tpc.length());
    char buffer[len+2];
    memset(buffer,'\0',len+2);
    strncpy(buffer,payload,sizeof(buffer)-2);
    
    if(tpc.startsWith(F("jee/get/")) || tpc.startsWith(F("jee/set/"))){
        tpc = tpc.substring(4, tpc.length());
        strncpy(_t_tpc_current, tpc.c_str(), sizeof(_t_tpc_current)-1);
        strncpy(_t_pld_current, buffer, sizeof(_t_pld_current)-1);
        
        _t_inc_current = true;
    }
    else mqt(tpc, String(_t_pld_current)); // msg
}

void jeeui2::remControl(){
    if(!_t_remotecontrol || !_t_inc_current) return;
    _t_inc_current = false;
    
    if(dbg)Serial.printf_P(PSTR("RC [%s - %s]\n"), _t_tpc_current, _t_pld_current);
    if(strcmp_P(_t_tpc_current, PSTR("get/refresh"))==0){
        //publish(F("jee/get/refresh"), F("true"), false);
        _refresh = true;
    }
    else if(strcmp_P(_t_tpc_current, PSTR("get/"))==0)
        publish(F("jee/get/cfg"), deb(), false);
    char *pos = strstr_P(_t_tpc_current, PSTR("set/"));
    if(pos!=nullptr){
        strncpy(_t_tpc_current,pos+4,sizeof(_t_tpc_current)-1);
        if(dbg) Serial.printf(PSTR("SET: %s\n"), _t_tpc_current);
        pos = strstr_P(_t_tpc_current, PSTR("BTN_"));
        if(pos!=nullptr){
            //strncpy(btnui, _t_tpc_current.substring(4, _t_tpc_current.length()).c_str(), sizeof(btnui)-1); // btnui = _t_tpc_current.substring(4, _t_tpc_current.length());
            strncpy(btnui, pos+4, sizeof(btnui)-1); // btnui = _t_tpc_current.substring(4, _t_tpc_current.length());
            if(strcmp_P(btnui, PSTR("_sysReset"))==0){
                var(F("wifi"), F("STA"));
                save();
                ESP.restart();
            }
        } 
        else {
            //Serial.printf_P("PROCEED: %s - %s\n",_t_tpc_current,_t_pld_current);
            if(param(_t_tpc_current) != _t_pld_current){
                var(_t_tpc_current, _t_pld_current);
                as();
            }
                
        }
    }
    *_t_tpc_current = '\0'; // _t_tpc_current = F("");
    *_t_pld_current = '\0'; // _t_pld_current = F("");
}

void jeeui2::subscribeAll(){
    mqttClient.subscribe(id(F("jee/get/")).c_str(), 0);
    mqttClient.subscribe(id(F("jee/get/refresh")).c_str(), 0);
    JsonObject root = cfg.as<JsonObject>();
    for (JsonPair kv : root) {
        String key = String(kv.key().c_str());
        int retry=3;
        if( 
            key != F("wifi" )       &&
            key != F("m_pref")      &&
            key != F("ssid" )       &&
            key != F("pass")        &&
            key != F("ap_ssid")     &&
            key != F("ap_pass")     &&
            key != F("m_host")      &&
            key != F("m_port")      &&
            key != F("m_user")      &&
            key != F("m_pass") 
            ){
            if(dbg)Serial.println(id(String(F("jee/set/")) + key));
            delay(33); // задержка на отправку предыдущих пакетов
            while(!mqttClient.subscribe(id(String(F("jee/set/")) + key).c_str(), 0) && retry--){
                delay(33); // доп. задержка на отправку предыдущих пакетов
            }
            if(dbg && retry<=0)
                Serial.println(id(String(F("-> jee/set/")) + key + F(" not subscribed!")));
            retry=3;
        }
    }
    //if(dbg)Serial.println(btn_id.as<String>());
    if(dbg)Serial.println( String(F("BTN =>")));
    JsonArray arr = btn_id.as<JsonArray>();
    for (size_t i=0; i<arr.size(); i++) {
        JsonObject var=arr[i]; // извлекаем очередной
        String item = id(F("jee/set/")) + String(F("BTN_")) + var[F("b")].as<String>();
        if(dbg)Serial.println(item);

        int retry=2;
        delay(33); // задержка на отправку предыдущих пакетов
        while(!mqttClient.subscribe(item.c_str(), 0) && retry--){
            delay(33); // доп. задержка на отправку предыдущих пакетов
        }
        if(dbg && retry<=0)Serial.println(String(F("-> ")) + item + F(" not subscribed!"));
    }
    if(dbg)Serial.println(F("Subscribe All"));
}

void jeeui2::publish(const String &topic, const String &payload, bool retained){
    if (!connected || !mqtt_enable) return; 
    mqttClient.publish(id(topic).c_str(), 0, retained, payload.c_str());
}

void jeeui2::publish(const String &topic, const String &payload){
    if (!connected || !mqtt_enable) return; 
    mqttClient.publish(id(topic).c_str(), 0, false, payload.c_str());
}

void jeeui2::pub_mqtt(const String &key, const String &value){
    if(!_t_remotecontrol) return;
    publish(key, value, true);
}

void jeeui2::subscribe(const String &topic){
    mqttClient.subscribe(id(topic).c_str(), 0);
}