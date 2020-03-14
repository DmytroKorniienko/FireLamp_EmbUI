#include "JeeUI2.h"


bool _t_inc_current = false;
String _t_tpc_current;
String _t_pld_current;
String _t_prf_current;
bool _t_remotecontrol;

String _m_host;
int _m_port;
String _m_user;
String _m_pass;
bool _m_params;

void jeeui2::connectToMqtt() {
  Serial.println(F("Connecting to MQTT..."));
  mqttClient.connect();
}
       
String jeeui2::id(const String &topic){
    if(m_pref == F("")) return topic;
    else return String(m_pref + '/' + topic);
}

bool mqtt_connected = false;
bool mqtt_connect = false;

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

    _t_prf_current = m_pref;
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
    m_pref = param(F("m_pref"));
    m_host = param(F("m_host"));
    m_port = param(F("m_port")).toInt();
    m_user = param(F("m_user"));
    m_pass = param(F("m_pass"));
    if(m_host == F("") || m_host == F("null") || m_port == 0) m_params = false;
    else m_params = true;
}

bool first_connect = true;

void jeeui2::mqtt_handle(){
    if(!connected || !m_params) return;
    static bool st = false;
    if(!st){
        st = true;
        _m_host = param(F("m_host"));
        _m_port = param(F("m_port")).toInt();
        _m_user = param(F("m_user"));
        _m_pass = param(F("m_pass"));
        mqttClient.onConnect(_onMqttConnect);
        mqttClient.onDisconnect(onMqttDisconnect);
        mqttClient.onSubscribe(onMqttSubscribe);
        mqttClient.onUnsubscribe(onMqttUnsubscribe);
        mqttClient.onMessage(onMqttMessage);
        mqttClient.onPublish(onMqttPublish);
        mqttClient.setCredentials(_m_user.c_str(), _m_pass.c_str());
        mqttClient.setServer(_m_host.c_str(), _m_port);
    }

    if(mqtt_connect) onMqttConnect();
    check_wifi_state();
    remControl();
}

void jeeui2::check_wifi_state(){
    static bool old_wifi_connected = false;
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
    Serial.println(F("Publish received."));
    String tpc = String(topic);
    if(_t_prf_current != F("")) tpc = tpc.substring(_t_prf_current.length() + 1, tpc.length());
    String msg = F(""); 
    for (int i= 0; i < len; i++) {
        msg += (char)payload[i];
    }
    if(tpc.startsWith(F("jee/get/")) || tpc.startsWith(F("jee/set/"))){
        tpc = tpc.substring(4, tpc.length());
        _t_tpc_current = tpc;
        _t_pld_current = msg;
        _t_inc_current = true;
    }
    else mqt(tpc, msg);
}

void jeeui2::remControl(){

    if(!_t_remotecontrol || !_t_inc_current) return;
    _t_inc_current = false;
    
    if(dbg)Serial.printf_P(PSTR("RC [%s - %s]"), _t_tpc_current.c_str(), _t_pld_current.c_str());
    if(_t_tpc_current == F("get/")) publish(F("jee/cfg"), deb(), false);
    if(_t_tpc_current.indexOf(F("set/")) != -1){
        _t_tpc_current = _t_tpc_current.substring(4, _t_tpc_current.length());
        if(dbg) Serial.println(String(F("SET: ")) + _t_tpc_current);
        if (_t_tpc_current.indexOf(F("BTN_")) != -1){
            btnui = _t_tpc_current.substring(4, _t_tpc_current.length());
            if(btnui == F("bWF")){
                var(F("wifi"), F("STA"));
                save();
                ESP.restart();
            }
        } 
        else {
            if(param(_t_tpc_current) != _t_pld_current){
                var(_t_tpc_current, _t_pld_current);
                as();
            }
                
        }
    }
    _t_tpc_current = F("");
    _t_pld_current = F("");
    
}

void jeeui2::subscribeAll(){
    mqttClient.subscribe(id(F("jee/get/")).c_str(), 0);
    JsonObject root = cfg.as<JsonObject>();
    for (JsonPair kv : root) {
        String key = String(kv.key().c_str());
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
            mqttClient.subscribe(id(String(F("jee/set/")) + key).c_str(), 0);
        }
    }
    for(int i = 0; i < btn_num + 1; i++){
        if(dbg)Serial.println( String(F("BTN =>")) + id(String(F("jee/set/")) + btn_id[i]));
        mqttClient.subscribe(id(String(F("jee/set/")) + btn_id[i]).c_str(), 0);
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