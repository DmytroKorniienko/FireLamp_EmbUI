#include "JeeUI2.h"
#include "interface.h"

bool jeeui2::mqtt_connected = false;
bool jeeui2::mqtt_connect = false;
bool jeeui2::mqtt_remotecontrol = false;
char jeeui2::m_pref[16];

extern jeeui2 jee;

void jeeui2::connectToMqtt() {
  Serial.println(F("Connecting to MQTT..."));
  mqttClient.connect();
}

String jeeui2::id(const String &topic){
    if (!*m_pref) return topic;

    String ret = m_pref; ret += '/'; ret += topic;
    return ret;
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


    if (host.length()==0){
      Serial.println(F("MQTT host is empty - disabled!"));
      return;   // выходим если host не задан
    }

    if(param(F("m_pref")) == F("null")) var(F("m_pref"), pref);
    if(param(F("m_host")) == F("null")) var(F("m_host"), host);
    if(param(F("m_port")) == F("null")) var(F("m_port"), String(port));
    if(param(F("m_user")) == F("null")) var(F("m_user"), user);
    if(param(F("m_pass")) == F("null")) var(F("m_pass"), pass);

    if (remotecontrol) mqtt_remotecontrol = true;
    mqt = mqttFunction;

    strncpy(m_pref, param(F("m_pref")).c_str(), sizeof(m_pref)-1);
    strncpy(m_host, param(F("m_host")).c_str(), sizeof(m_host)-1);
    strncpy(m_user, param(F("m_user")).c_str(), sizeof(m_user)-1);
    strncpy(m_pass, param(F("m_pass")).c_str(), sizeof(m_pass)-1);
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

    mqtt_enable = true;
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
    mqtt(pref, host, port, user, pass, emptyFunction, remotecontrol);
    onConnect = mqttConnect;

}
void jeeui2::mqtt(const String &pref, const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) ()){
    getAPmac();
    mqtt(pref, host, port, user, pass, mqttFunction, false);
    onConnect = mqttConnect;
}
void jeeui2::mqtt(const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) ()){
    getAPmac();
    mqtt(mc, host, port, user, pass, mqttFunction, false);
    onConnect = mqttConnect;
}
void jeeui2::mqtt(const String &host, int port, const String &user, const String &pass, void (*mqttFunction) (const String &topic, const String &payload), void (*mqttConnect) (), bool remotecontrol){
    getAPmac();
    mqtt(mc, host, port, user, pass, mqttFunction, remotecontrol);
    onConnect = mqttConnect;
}

void jeeui2::mqtt_handle(){
    if (!wifi_sta || !*m_host) return;
    if (mqtt_connect) onMqttConnect();
    mqtt_reconnect();
}

/*
 * TODO: убрать этот бардак в планировщик
 */
void jeeui2::mqtt_reconnect(){
    static unsigned long tmout = 0;
    if (tmout + 15000 > millis()) return;
    tmout = millis();
    if ( wifi_sta && !mqtt_connected) connectToMqtt();
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
    if(mqtt_remotecontrol){
        subscribeAll();
    }
}

void jeeui2::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    Serial.print(F("Publish received: "));
    Serial.println(topic);

    char buffer[len + 2];
    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, payload, len);

    String tpc = String(topic);
    if (*m_pref) tpc = tpc.substring(strlen(m_pref) + 1, tpc.length());

    if (tpc.equals(F("jee/get/config"))) {
        jee.publish(F("jee/pub/config"), jee.deb(), false);    
    } else
    if (tpc.startsWith(F("jee/get/"))) {
       String param = tpc.substring(8);
       jee.publish(String(F("jee/pub/")) + param, jee.param(param), false);
    } else
    if (mqtt_remotecontrol && tpc.startsWith(F("jee/set/"))) {
       String cmd = tpc.substring(8);
       httpCallback(cmd, String(buffer)); // нельзя напряму передавать payload, это не ASCIIZ
    } else
    if (mqtt_remotecontrol && tpc.startsWith(F("jee/jsset/"))) {
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload, len);
        jee.post(doc.as<JsonObject>());
    } else {
        mqt(tpc, String(buffer));
    }
}

void jeeui2::subscribeAll(){
    mqttClient.subscribe(id(F("jee/#")).c_str(), 0);
    LOG(println, F("Subscribe All"));
}

void jeeui2::publish(const String &topic, const String &payload, bool retained){
    if (!wifi_sta || !mqtt_enable) return;
    mqttClient.publish(id(topic).c_str(), 0, retained, payload.c_str());
}

void jeeui2::publish(const String &topic, const String &payload){
    if (!wifi_sta || !mqtt_enable) return;
    mqttClient.publish(id(topic).c_str(), 0, false, payload.c_str());
}

void jeeui2::pub_mqtt(const String &key, const String &value){
    if(!mqtt_remotecontrol) return;
    publish(key, value, true);
}

void jeeui2::subscribe(const String &topic){
    mqttClient.subscribe(id(topic).c_str(), 0);
}
