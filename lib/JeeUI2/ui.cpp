#include "JeeUI2.h"


void jeeui2::pub(const String &id, const String &label, const String &value, const String &unit, const String &bg_color, const String &text_color){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"id\":\"%s\",\"type\":\"pub\",\"value\":\"%s\",\"bg_color\":\"%s\",\"text_color\":\"%s\",\"label\":\"%s\",\"unit\":\"%s\"},")
      ,id.c_str(), value.c_str(), bg_color.c_str(), text_color.c_str(), label.c_str(), unit.c_str());

    jeeui2::buf.concat(buffer);

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
    button(F("bMQTTform"), F("gray"), F("Reconnect"));
}

void jeeui2::formWifi(){
    text(F("ssid"), F("SSID"));
    password(F("pass"), F("Password"));
    button(F("_sysReset"), F("gray"), F("Connect"));
}


void jeeui2::app(const String &name){
    mn = 0;
    pg = 0;
    op = F("");
    buf = F("");
    //buf.reserve(512); // резервируем сразу большой кусок
    buf.concat(F("{\"app\":\"")); buf.concat(name); buf.concat(F("\","));
    buf.concat(F("\"ID\":\"")); buf.concat(mc); buf.concat(F("\","));
}

void jeeui2::text(const String &id, const String &label){
    char buffer[512]; buffer[511]='\0';
    snprintf_P(buffer,sizeof(buffer)-1,PSTR("{\"html\":\"input\",\"id\":\"%s\",\"type\":\"text\",\"value\":\"%s\",\"label\":\"%s\"},"), id.c_str(), param(id).c_str(), label.c_str());
    jeeui2::buf.concat(buffer);
}

void jeeui2::number(const String &id, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"input\",\"id\":\"%s\",\"type\":\"number\",\"value\":\"%s\",\"label\":\"%s\"},"), id.c_str(), param(id).c_str(), label.c_str());

    jeeui2::buf.concat(buffer);
}

void jeeui2::time(const String &id, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"input\",\"id\":\"%s\",\"type\":\"time\",\"value\":\"%s\",\"label\":\"%s\"},"), id.c_str(), param(id).c_str(), label.c_str());

    jeeui2::buf.concat(buffer);
}

void jeeui2::date(const String &id, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"input\",\"id\":\"%s\",\"type\":\"date\",\"value\":\"%s\",\"label\":\"%s\"},"), id.c_str(), param(id).c_str(), label.c_str());

    jeeui2::buf.concat(buffer);
}

void jeeui2::datetime(const String &id, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"input\",\"id\":\"%s\",\"type\":\"datetime-local\",\"value\":\"%s\",\"label\":\"%s\"},"), id.c_str(), param(id).c_str(), label.c_str());

    jeeui2::buf.concat(buffer);
}

void jeeui2::range(const String &id, int min, int max, float step, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"input\",\"id\":\"%s\",\"min\":\"%d\",\"max\":\"%d\",\"step\":\"%.2f\",\"type\":\"range\",\"value\":\"%s\",\"label\":\"%s\"},")
      , id.c_str(), min, max, step, param(id).c_str(), label.c_str());

    jeeui2::buf.concat(buffer);
}

void jeeui2::email(const String &id, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"input\",\"id\":\"%s\",\"type\":\"email\",\"value\":\"%s\",\"label\":\"%s\"},"), id.c_str(), param(id).c_str(), label.c_str());

    jeeui2::buf.concat(buffer);
}

void jeeui2::password(const String &id, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"input\",\"id\":\"%s\",\"type\":\"password\",\"value\":\"%s\",\"label\":\"%s\"},"), id.c_str(), param(id).c_str(), label.c_str());

    jeeui2::buf.concat(buffer);
}

void jeeui2::option(const String &value, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"label\":\"%s\",\"value\":\"%s\"},"),label.c_str(), value.c_str());

    jeeui2::op.concat(buffer);
}

void jeeui2::select(const String &id, const String &label){
    char buffer[256];
    int lastIndex = op.length() - 1;
    op.remove(lastIndex);
    sprintf_P(buffer,PSTR("{\"html\":\"select\",\"id\":\"%s\",\"value\":\"%s\",\"label\":\"%s\",\"options\":["),id.c_str(),param(id).c_str(),label.c_str());
    jeeui2::buf.concat(buffer);
    jeeui2::buf.concat(op);
    jeeui2::buf.concat(F("]},"));

    op = F("");
}

void jeeui2::checkbox(const String &id, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"input\",\"id\":\"%s\",\"type\":\"checkbox\",\"value\":\"%s\",\"label\":\"%s\"},"), id.c_str(), param(id).c_str(), label.c_str());

    jeeui2::buf.concat(buffer);
}
void jeeui2::color(const String &id, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"input\",\"id\":\"%s\",\"type\":\"color\",\"value\":\"%s\",\"label\":\"%s\"},"), id.c_str(), param(id).c_str(), label.c_str());

    jeeui2::buf.concat(buffer);
}
void jeeui2::button(const String &id, const String &color, const String &label){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"button\",\"id\":\"%s\",\"color\":\"%s\",\"label\":\"%s\"},"), id.c_str(), color.c_str(), label.c_str());

    jeeui2::buf.concat(buffer);

    // if(!btn_id.containsKey(id)){
    //     String tmp;
    //     btn_id.add(id); //btn_id[btn_num] = String(F("BTN_")) + id;
    //     serializeJson(btn_id, tmp);
    //     deserializeJson(btn_id, tmp);
    // }
}

void jeeui2::button(const String &id, const String &color, const String &label, int column){
    char buffer[256];
    sprintf_P(buffer,PSTR("{\"html\":\"button\",\"id\":\"%s\",\"color\":\"%s\",\"label\":\"%s\",\"col\":\"%d\"},"), id.c_str(), color.c_str(), label.c_str(), column);

    jeeui2::buf.concat(buffer);
    
    // if(!btn_id.containsKey(id)){
    //     String tmp;
    //     btn_id.add(id); //btn_id[btn_num] = String(F("BTN_")) + id;
    //     serializeJson(btn_id, tmp);
    //     deserializeJson(btn_id, tmp);
    // }
}

void jeeui2::textarea(const String &id, const String &label){
    char buffer[512]; buffer[511]='\0';
    snprintf_P(buffer,sizeof(buffer)-1,PSTR("{\"html\":\"textarea\",\"id\":\"%s\",\"type\":\"text\",\"value\":\"%s\",\"label\":\"%s\"},"), id.c_str(), param(id).c_str(), label.c_str());

    jeeui2::buf.concat(buffer);
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