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

    if (!json_frame_add(obj.as<JsonObject>())) {
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

    if (!json_frame_add(obj.as<JsonObject>())) {
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

    if (!json_frame_add(obj.as<JsonObject>())) {
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

    if (!json_frame_add(obj.as<JsonObject>())) {
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

    if (!json_frame_add(obj.as<JsonObject>())) {
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

    if (!json_frame_add(obj.as<JsonObject>())) {
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

    if (!json_frame_add(obj.as<JsonObject>())) {
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

    if (!json_frame_add(obj.as<JsonObject>())) {
        password(id, label);
    }
}

void jeeui2::option(const String &value, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("label")] = label;
    obj[F("value")] = value;

    if (!json_frame_add(obj.as<JsonObject>())) {
        option(value, label);
    }
}

void jeeui2::select(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("select");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        select(id, label);
        return;
    }

    json_section_begin(F("options"), section_list.end()->block.getElement(section_list.end()->idx - 1));
}

void jeeui2::checkbox(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("checkbox");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
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

    if (!json_frame_add(obj.as<JsonObject>())) {
        color(id, label);
    }
}
void jeeui2::button(const String &id, const String &color, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("button");
    obj[F("id")] = id;
    obj[F("color")] = color;
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
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

    if (!json_frame_add(obj.as<JsonObject>())) {
        button(id, color, label, column);
    }
}

void jeeui2::textarea(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("textarea");
    obj[F("id")] = id;
    obj[F("value")] = param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        textarea(id, label);
    }
}

///////////////////////////////////////
void jeeui2::json_frame_interface(const String &name){
    json[F("pkg")] = F("interface");
    json[F("app")] = name;
    json[F("mc")] = mc;
    json[F("final")] = false;

    json_section_begin("root" + String(rand()));
}

bool jeeui2::json_frame_add(JsonObject obj) {
    if (dbg) Serial.printf(PSTR("json_frame_add: %u = %u "), obj.memoryUsage(), json.capacity() - json.memoryUsage());
    if (json.capacity() - json.memoryUsage() > obj.memoryUsage() + 20 && section_list.end()->block.add(obj)) {
        section_list.end()->idx++;
        if (dbg) Serial.printf(PSTR("OK [%u]  MEM: %u\n"), section_list.end()->idx, ESP.getFreeHeap());
        return true;
    }
    if (dbg) Serial.printf(PSTR("BAD  MEM: %u\n"), ESP.getFreeHeap());

    json_frame_send();
    json_frame_next();
    return false;
}

void jeeui2::json_frame_next(){
    json.clear();
    JsonObject obj = json.to<JsonObject>();
    for (int i = 0; i < section_list.size(); i++) {
        if (i) obj = section_list[i - 1]->block.createNestedObject();
        obj[F("section")] = section_list[i]->name;
        obj[F("idx")] = section_list[i]->idx;
        if (dbg) Serial.printf(PSTR("section %u %s %u\n"), i, section_list[i]->name.c_str(), section_list[i]->idx);
        section_list[i]->block = obj.createNestedArray(F("block"));
    }
    if (dbg) Serial.printf(PSTR("json_frame_next: [%u] %u = %u\n"), section_list.size(), obj.memoryUsage(), json.capacity() - json.memoryUsage());
}

void jeeui2::json_frame_clear(){
    while(section_list.size()) {
        section_t *section = section_list.shift();
        delete section;
    }
    json.clear();
}

void jeeui2::json_frame_flush(){
    if (dbg) Serial.println(F("json_frame_flush"));
    json[F("final")] = true;
    json_section_end();
    json_frame_send();
    json_frame_clear();
}

void jeeui2::json_frame_send(){
    if (buf.length()) buf = "";
    serializeJson(json, buf);
    if (dbg)Serial.println(buf);
    if (send_hndl) send_hndl->send(buf);
    buf = "";
}

void jeeui2::json_section_begin(const String &name){
    JsonObject obj;
    if (section_list.size()) {
        obj = section_list.end()->block.createNestedObject();
    } else {
        obj = json.as<JsonObject>();
    }
    json_section_begin(name, obj);
}

void jeeui2::json_section_begin(const String &name, JsonObject obj){
    obj[F("section")] = name;

    section_t *section = new section_t;
    section->name = name;
    section->block = obj.createNestedArray(F("block"));
    section->idx = 0;
    section_list.add(section);
    if (dbg) Serial.printf(PSTR("section begin %s [%u]\n"), name.c_str(), section_list.size());
}

void jeeui2::json_section_end(){
    if (!section_list.size()) return;

    section_t *section = section_list.pop();
    if (section_list.size()) {
        section_list.end()->idx++;
    }
    if (dbg) Serial.printf(PSTR("section end %s [%u]\n"), section->name.c_str(), section_list.size());
     delete section;
}