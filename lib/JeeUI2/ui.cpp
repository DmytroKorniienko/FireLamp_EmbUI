#include "JeeUI2.h"


void jeeui2::pub(const String &id, const String &label, const String &value, const String &unit, const String &bg_color, const String &text_color){

    buf += String(F("{\"id\":\"")) + id + "\",";
    buf += F("\"type\":\"pub\",");
    buf += String(F("\"value\":\"")) + value + "\",";
    buf += String(F("\"bg_color\":\"")) + bg_color + "\",";
    buf += String(F("\"text_color\":\"")) + text_color + "\",";
    buf += String(F("\"label\":\"")) + label + "\",";
    buf += String(F("\"unit\":\"")) + unit + "\"";
    buf += "},";

    pub_enable = true;
    pub_transport[id] = "";
}

void jeeui2::pub(const String &id, const String &label, const String &value, const String &unit, const String &bg_color){
    pub(id, label, value, unit, bg_color, F("#ffffff"));
}

void jeeui2::pub(const String &id, const String &label, const String &value, const String &unit){
    pub(id, label, value, unit, F("#6060ff"), F("#ffffff"));
}

void jeeui2::pub(const String &id, const String &label, const String &value){
    pub(id, label, value, F(""), F("#6060ff"), F("#ffffff"));
}

void jeeui2::pub(const String &id, const String &label){
    pub(id, label, F(""), F(""), F("#6060ff"), F("#ffffff"));
}

void jeeui2::formMqtt(){
    text(F("m_host"), F("MQTT host"));
    number(F("m_port"), F("MQTT port"));
    text(F("m_user"), F("User"));
    text(F("m_pass"), F("Password"));
    button(F("bWF"), F("gray"), F("Reconnect"));
}

void jeeui2::formWifi(){
    text(F("ssid"), F("SSID"));
    password(F("pass"), F("Password"));
    button(F("bWF"), F("gray"), F("Connect"));
}


void jeeui2::app(const String &name){
    btn_num = 0;
    mn = 0;
    pg = 0;
    op = "";

    buf = String(F("{\"app\":\"")) + name + "\",";
    buf += String(F("\"ID\":\"")) + mc + "\",";
}

void jeeui2::text(const String &id, const String &label){
    buf += F("{\"html\":\"input\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += F("\"type\":\"text\",");
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");
}

void jeeui2::number(const String &id, const String &label){
    buf += F("{\"html\":\"input\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += F("\"type\":\"number\",");
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");
}

void jeeui2::time(const String &id, const String &label){
    buf += "{\"html\":\"input\",";
    buf += "\"id\":\"" + id + "\",";
    buf += "\"type\":\"time\",";
    buf += "\"value\":\"" + param(id) + "\",";
    buf += "\"label\":\"" + label + "\"";
    buf += "},";
}

void jeeui2::date(const String &id, const String &label){
    buf += F("{\"html\":\"input\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += F("\"type\":\"date\",");
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");
}

void jeeui2::datetime(const String &id, const String &label){
    buf += F("{\"html\":\"input\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += F("\"type\":\"datetime-local\",");
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");
}

void jeeui2::range(const String &id, int min, int max, float step, const String &label){
    buf += F("{\"html\":\"input\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += String(F("\"min\":\"")) + String(min) + "\",";
    buf += String(F("\"max\":\"")) + String(max) + "\",";
    buf += String(F("\"step\":\"")) + String(step) + "\",";
    buf += F("\"type\":\"range\",");
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");
}

void jeeui2::email(const String &id, const String &label){
    buf += F("{\"html\":\"input\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += F("\"type\":\"email\",");
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");
}

void jeeui2::password(const String &id, const String &label){
    buf += F("{\"html\":\"input\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += F("\"type\":\"password\",");
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");
}

void jeeui2::option(const String &value, const String &label){
    op += String(F("{\"label\":\"")) + label + String(F("\",\"value\":\"")) + value + String(F("\"},"));
}

void jeeui2::select(const String &id, const String &label){

    int lastIndex = op.length() - 1;
    op.remove(lastIndex);
    buf += F("{\"html\":\"select\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\",";
    buf += String(F("\"options\":[")) + op + "]";
    buf += F("},");
    op = "";
}

void jeeui2::checkbox(const String &id, const String &label){
    buf += F("{\"html\":\"input\",");
    buf += F("\"type\":\"checkbox\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");
}
void jeeui2::color(const String &id, const String &label){

    buf += F("{\"html\":\"input\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += F("\"type\":\"color\",");
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");
}
void jeeui2::button(const String &id, const String &color, const String &label){
    buf += F("{\"html\":\"button\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += String(F("\"color\":\"")) + color + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");

    btn_id[btn_num] = "BTN_" + id;
    btn_num++;
}

void jeeui2::button(const String &id, const String &color, const String &label, int column){
    buf += F("{\"html\":\"button\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += String(F("\"color\":\"")) + color + "\",";
    buf += String(F("\"label\":\"")) + label + "\",";
    buf += String(F("\"col\":\"")) + String(column) + "\"";
    buf += F("},");

    btn_id[btn_num] = "BTN_" + id;
    btn_num++;
}

void jeeui2::textarea(const String &id, const String &label){
    buf += F("{\"html\":\"textarea\",");
    buf += String(F("\"id\":\"")) + id + "\",";
    buf += F("\"type\":\"text\",");
    buf += String(F("\"value\":\"")) + param(id) + "\",";
    buf += String(F("\"label\":\"")) + label + "\"";
    buf += F("},");
}

void jeeui2::menu(const String &name){
    
    if (mn == 0)
        buf += F("\"menu\":[");
    else{
        int lastIndex = buf.length() - 1;
        buf.remove(lastIndex);
        buf.remove(lastIndex - 1);
        buf += ",";
    }
    buf += "\"" + name + String(F("\"],"));
    mn++;
}

void jeeui2::page(){
    if (pg == 0)
        buf += F("\"content\":[[");
    else{
        int lastIndex = buf.length() - 1;
        buf.remove(lastIndex);
        buf += F("],[");
    }

    if (pg == mn){
        int lastIndex = buf.length() - 1;
        buf.remove(lastIndex);
        buf.remove(lastIndex - 1);
        buf.remove(lastIndex - 2);
        //buf.remove(lastIndex - 3);
        buf += F("]]}");
    }
    else
        pg++;
}