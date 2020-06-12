#include "ui.h"
#include "../../include/misc.h"

void Interface::formMqtt(){
    text(F("m_host"), F("MQTT host"));
    number(F("m_port"), F("MQTT port"));
    text(F("m_user"), F("User"));
    text(F("m_pass"), F("Password"));
    button(F("bMQTTform"), F("gray"), F("Reconnect"));
}

void Interface::formWifi(){
    text(F("ssid"), F("SSID"));
    password(F("pass"), F("Password"));
    button(F("sysReset"), F("gray"), F("Connect"));
}

void Interface::text(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("text");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        text(id, label);
    }
}

void Interface::number(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("number");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        number(id, label);
    }
}

void Interface::time(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("time");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        time(id, label);
    }
}

void Interface::date(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("date");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        date(id, label);
    }
}

void Interface::datetime(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("datetime-local");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        datetime(id, label);
    }
}

void Interface::range(const String &id, int min, int max, float step, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("range");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    obj[F("min")] = min;
    obj[F("max")] = max;
    obj[F("step")] = step;

    if (!json_frame_add(obj.as<JsonObject>())) {
        range(id, min, max, step, label);
    }
}

void Interface::email(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("email");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        email(id, label);
    }
}

void Interface::password(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("password");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        password(id, label);
    }
}

void Interface::option(const String &value, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("label")] = label;
    obj[F("value")] = value;

    if (!json_frame_add(obj.as<JsonObject>())) {
        option(value, label);
    }
}

void Interface::select(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("select");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        select(id, label);
        return;
    }
    section_stack.end()->idx--;
    json_section_begin(F("options"), F(""), section_stack.end()->block.getElement(section_stack.end()->idx));
}

void Interface::checkbox(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("checkbox");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        checkbox(id, label);
    }
}
void Interface::color(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("input");
    obj[F("type")] = F("color");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        color(id, label);
    }
}
void Interface::button(const String &id, const String &color, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("button");
    obj[F("id")] = id;
    obj[F("color")] = color;
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        button(id, color, label);
    }
}

void Interface::button(const String &id, const String &color, const String &label, int column){
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

void Interface::textarea(const String &id, const String &label){
    StaticJsonDocument<256> obj;
    obj[F("html")] = F("textarea");
    obj[F("id")] = id;
    obj[F("value")] = jee->param(id);
    obj[F("label")] = label;

    if (!json_frame_add(obj.as<JsonObject>())) {
        textarea(id, label);
    }
}

void Interface::value(const String &id, const String &val){
    StaticJsonDocument<256> obj;
    obj[F("id")] = id;
    obj[F("value")] = val;

    if (!json_frame_add(obj.as<JsonObject>())) {
        value(id, val);
    }
}

///////////////////////////////////////
void Interface::json_frame_value(){
    json[F("pkg")] = F("value");
    json[F("final")] = false;

    json_section_begin("root" + String(rand()));
}

void Interface::json_frame_interface(const String &name){
    json[F("pkg")] = F("interface");
    json[F("app")] = name;
    json[F("mc")] = jee->mc;
    json[F("final")] = false;

    json_section_begin("root" + String(rand()));
}

bool Interface::json_frame_add(JsonObject obj) {
    LOG(printf_P, PSTR("json_frame_add: %u = %u "), obj.memoryUsage(), json.capacity() - json.memoryUsage());
    if (json.capacity() - json.memoryUsage() > obj.memoryUsage() + 20 && section_stack.end()->block.add(obj)) {
        section_stack.end()->idx++;
        LOG(printf, PSTR("OK [%u]  MEM: %u\n"), section_stack.end()->idx, ESP.getFreeHeap());
        return true;
    }
    LOG(printf_P, PSTR("BAD  MEM: %u\n"), ESP.getFreeHeap());

    json_frame_send();
    json_frame_next();
    return false;
}

void Interface::json_frame_next(){
    json.clear();
    JsonObject obj = json.to<JsonObject>();
    for (int i = 0; i < section_stack.size(); i++) {
        if (i) obj = section_stack[i - 1]->block.createNestedObject();
        obj[F("section")] = section_stack[i]->name;
        obj[F("idx")] = section_stack[i]->idx;
        LOG(printf_P, PSTR("section %u %s %u\n"), i, section_stack[i]->name.c_str(), section_stack[i]->idx);
        section_stack[i]->block = obj.createNestedArray(F("block"));
    }
    LOG(printf_P, PSTR("json_frame_next: [%u] %u = %u\n"), section_stack.size(), obj.memoryUsage(), json.capacity() - json.memoryUsage());
}

void Interface::json_frame_clear(){
    while(section_stack.size()) {
        section_stack_t *section = section_stack.shift();
        delete section;
    }
    json.clear();
}

void Interface::json_frame_flush(){
    LOG(println, F("json_frame_flush"));
    json[F("final")] = true;
    json_section_end();
    json_frame_send();
    json_frame_clear();
}

void Interface::json_frame_send(){
    if (buf.length()) buf = "";
    serializeJson(json, buf);
    LOG(println, buf);
    if (send_hndl) send_hndl->send(buf);
    buf = "";
}

void Interface::json_section_begin(const String &name, const String &label){
    JsonObject obj;
    if (section_stack.size()) {
        obj = section_stack.end()->block.createNestedObject();
    } else {
        obj = json.as<JsonObject>();
    }
    json_section_begin(name, label, obj);
}

void Interface::json_section_begin(const String &name, const String &label, JsonObject obj){
    obj[F("section")] = name;
    if (label != "") obj[F("label")] = label;

    section_stack_t *section = new section_stack_t;
    section->name = name;
    section->block = obj.createNestedArray(F("block"));
    section->idx = 0;
    section_stack.add(section);
    LOG(printf, PSTR("section begin %s [%u]\n"), name.c_str(), section_stack.size());
}

void Interface::json_section_end(){
    if (!section_stack.size()) return;

    section_stack_t *section = section_stack.pop();
    if (section_stack.size()) {
        section_stack.end()->idx++;
    }
    LOG(printf, PSTR("section end %s [%u]\n"), section->name.c_str(), section_stack.size());
    delete section;
}