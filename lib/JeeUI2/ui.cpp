#include "JeeUI2.h"


void jeeui2::pub(const String &id, const String &label, const String &value, const String &unit, const String &bg_color, const String &text_color){
    buf.concat(F("{\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"pub\","));
    buf.concat(F("\"value\":\"")); buf.concat(value); buf.concat(F("\","));
    buf.concat(F("\"bg_color\":\"")); buf.concat(bg_color); ; buf.concat(F("\","));
    buf.concat(F("\"text_color\":\"")); buf.concat(text_color); ; buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); ; buf.concat(F("\","));
    buf.concat(F("\"unit\":\"")); buf.concat(unit); ; buf.concat(F("\""));
    buf.concat(F("},"));

    pub_enable = true;
    pub_transport[id] = value;
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
    op = F("");

    buf = F("");
    //buf.reserve(4096); // резервируем сразу большой кусок
    buf.concat(F("{\"app\":\"")); buf.concat(name); buf.concat(F("\","));
    buf.concat(F("\"ID\":\"")); buf.concat(mc); buf.concat(F("\","));
}

void jeeui2::text(const String &id, const String &label){
    buf.concat(F("{\"html\":\"input\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"text\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}

void jeeui2::number(const String &id, const String &label){
    buf.concat(F("{\"html\":\"input\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"number\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}

void jeeui2::time(const String &id, const String &label){
    buf.concat(F("{\"html\":\"input\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"time\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}

void jeeui2::date(const String &id, const String &label){
    buf.concat(F("{\"html\":\"input\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"date\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}

void jeeui2::datetime(const String &id, const String &label){
    buf.concat(F("{\"html\":\"input\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"datetime-local\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}

void jeeui2::range(const String &id, int min, int max, float step, const String &label){
    buf.concat(F("{\"html\":\"input\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"min\":\"")); buf.concat(min); buf.concat(F("\","));
    buf.concat(F("\"max\":\"")); buf.concat(max); buf.concat(F("\","));
    buf.concat(F("\"step\":\"")); buf.concat(step); buf.concat(F("\","));
    buf.concat(F("\"type\":\"range\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}

void jeeui2::email(const String &id, const String &label){
    buf.concat(F("{\"html\":\"input\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"email\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}

void jeeui2::password(const String &id, const String &label){
    buf.concat(F("{\"html\":\"input\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"password\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}

void jeeui2::option(const String &value, const String &label){
    op.concat(F("{\"label\":\""));
    op.concat(label);
    op.concat(F("\",\"value\":\""));
    op.concat(value);
    op.concat(F("\"},"));
}

void jeeui2::select(const String &id, const String &label){

    int lastIndex = op.length() - 1;
    op.remove(lastIndex);

    buf.concat(F("{\"html\":\"select\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\","));
    buf.concat(F("\"options\":[")); buf.concat(op); buf.concat(F("]"));
    buf.concat(F("},"));

    op = F("");
}

void jeeui2::checkbox(const String &id, const String &label){
    buf.concat(F("{\"html\":\"input\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"checkbox\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}
void jeeui2::color(const String &id, const String &label){
    buf.concat(F("{\"html\":\"input\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"color\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}
void jeeui2::button(const String &id, const String &color, const String &label){
    buf.concat(F("{\"html\":\"button\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"color\":\"")); buf.concat(color); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));

    btn_id[btn_num] = String(F("BTN_")) + id;
    btn_num++;
}

void jeeui2::button(const String &id, const String &color, const String &label, int column){
    buf.concat(F("{\"html\":\"button\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"color\":\"")); buf.concat(color); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\","));
    buf.concat(F("\"col\":\"")); buf.concat(column); buf.concat(F("\""));
    buf.concat(F("},"));

    btn_id[btn_num] = String(F("BTN_")) + id;
    btn_num++;
}

void jeeui2::textarea(const String &id, const String &label){
    buf.concat(F("{\"html\":\"textarea\","));
    buf.concat(F("\"id\":\"")); buf.concat(id); buf.concat(F("\","));
    buf.concat(F("\"type\":\"text\","));
    buf.concat(F("\"value\":\"")); buf.concat(param(id)); buf.concat(F("\","));
    buf.concat(F("\"label\":\"")); buf.concat(label); buf.concat(F("\""));
    buf.concat(F("},"));
}

void jeeui2::menu(const String &name){
    
    if (mn == 0)
        buf.concat(F("\"menu\":["));
    else{
        int lastIndex = buf.length() - 1;
        buf.remove(lastIndex-1,2);
        buf.concat(F(","));
    }
    buf.concat(F("\"")); buf.concat(name); buf.concat(F("\"],"));
    mn++;
}

void jeeui2::page(){
    if (pg == 0)
        buf.concat(F("\"content\":[["));
    else{
        int lastIndex = buf.length() - 1;
        buf.remove(lastIndex);
        buf.concat(F("],["));
    }

    if (pg == mn){
        int lastIndex = buf.length() - 1;
        buf.remove(lastIndex-2,3);
        buf.concat(F("]]}"));
    }
    else
        pg++;
}