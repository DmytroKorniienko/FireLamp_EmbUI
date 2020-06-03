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

void jeeui2::text(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("text");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        text(id, label);
    }
}

void jeeui2::number(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("number");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        number(id, label);
    }
}

void jeeui2::time(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("time");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        time(id, label);
    }
}

void jeeui2::date(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("date");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        date(id, label);
    }
}

void jeeui2::datetime(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("datetime-local");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        datetime(id, label);
    }
}

void jeeui2::range(const String &id, int min, int max, float step, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("range");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    obj[F("min")] = min;
    obj[F("max")] = max;
    obj[F("step")] = step;

    if (!frame_add(obj.as<JsonObject>())) {
        range(id, min, max, step, label);
    }
}

void jeeui2::email(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("email");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        email(id, label);
    }
}

void jeeui2::password(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("password");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        password(id, label);
    }
}

void jeeui2::option(const String &value, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("label")] = label;
    obj[F("value")] = value;

    if (!frame_add(obj.as<JsonObject>())) {
        option(value, label);
    }
}

void jeeui2::select(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("select");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        select(id, label);
        return;
    }

    secbegin(F("options"));
}

void jeeui2::checkbox(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("checkbox");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        checkbox(id, label);
    }
}
void jeeui2::color(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("color");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        color(id, label);
    }
}
void jeeui2::button(const String &id, const String &color, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("button");
    obj[F("id")] = id;
    obj[F("color")] = color;
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        button(id, color, label);
    }
}

void jeeui2::button(const String &id, const String &color, const String &label, int column){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("button");
    obj[F("id")] = id;
    obj[F("color")] = color;
    obj[F("label")] = label;
    obj[F("col")] = column;

    if (!frame_add(obj.as<JsonObject>())) {
        button(id, color, label, column);
    }
}

void jeeui2::textarea(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("textarea");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!frame_add(obj.as<JsonObject>())) {
        textarea(id, label);
    }
}

///////////////////////////////////////
void jeeui2::app(const String &name){
    json[F("pkg")] = F("interface");
    json[F("app")] = name;
    json[F("id")] = mc;

    secbegin(F("root"));
}

int jeeui2::frame_add(JsonObject obj) {
    if (dbg) Serial.printf(PSTR("frame_add: %u = %u "), obj.memoryUsage(), json.capacity() - json.memoryUsage());
    if (json.capacity() - json.memoryUsage() > obj.memoryUsage() + 20 && section_list.end()->block.add(obj)) {
        if (dbg) Serial.println("OK ");
        return 1;
    }
    if (dbg) Serial.println("BAD ");

    frame_send();
    frame_next();
    return 0;
}

void jeeui2::frame_next(){
    json.clear();
    json.garbageCollect();
    JsonObject obj = json.to<JsonObject>();
    for (int i = 0; i < section_list.size(); i++) {
        if (i) obj = section_list[i - 1]->block.createNestedObject();
        obj[F("section")] = section_list[i]->section;
        section_list[i]->block = obj.createNestedArray(F("block"));
    }
    if (dbg) Serial.printf(PSTR("frame_next: [%u] %u = %u\n"), section_list.size(), obj.memoryUsage(), json.capacity() - json.memoryUsage());
}

void jeeui2::frame_clear(){
    for (int i = 0; i < section_list.size(); i++) {
        delete section_list[i];
    }
    section_list.clear();
}

void jeeui2::secbegin(const String &name){
    JsonObject obj;
    if (section_list.size()) {
        obj = section_list.end()->block.createNestedObject();
    } else {
        obj = json.as<JsonObject>();
    }
    obj[F("section")] = name;

    section_t *section = new section_t;
    section->section = name;
    section->block = obj.createNestedArray(F("block"));
    section_list.add(section);
}

void jeeui2::secend(){
    if (!section_list.size()) return;
    section_t *section = section_list.pop();
    delete section;
}

void jeeui2::flush(){

}