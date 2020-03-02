const char temp_js[] PROGMEM = R"=====(

var json = "none";
var page = 0;
var obj;
var app;
var pages;
var pub_int = []
var pub_int_cnt = 0

function make(){
    obj = JSON.parse(json);
    app = obj.app;
    ID = obj.ID;
    pages = obj.menu.length;
    menu();
    content();
}

function parse(){
    var xhr = new XMLHttpRequest();
    xhr.open('GET', '/echo', true);
    xhr.onload = function () {
        json = xhr.responseText;
        make();
    };
    xhr.send(null);
}

function new_page(p){
    page = p;
    menu();
    content();
}

function inner(id, content){
    document.getElementById(id).innerHTML += content;
}

function menu_item(name, current, p){
    if (!current)
        return "<li class=\"pure-menu-item\" onclick=\"new_page(" + p + ")\"><a href=\"#\" class=\"pure-menu-link\">" + name + "</a></li>";
    else
        return "<li class=\"pure-menu-item menu-item-divided pure-menu-selected\" onclick=\"new_page(" + p + ")\"><a href=\"#\" class=\"pure-menu-link\">" + name + "</a></li>";
}

function menu(){
    var content = "<div class=\"pure-menu\"><a class=\"pure-menu-heading\" href=\"#\">" + app + "</a><ul class=\"pure-menu-list\">";
    for(var i = 0; i < pages; i++){
        var current;
        if(page == i) current = true;
        else current = false;
        content += menu_item(obj.menu[i], current, i);
    }
    content += "<br><br>";
    content += "<p>&nbsp&nbspID:&nbsp<b>" + ID + "</b><p>";
    content += "</ul>";
    content += "</div>";
    document.getElementById("menu").innerHTML = content;
}

function content(){
    for(var i = 0; i < pub_int_cnt; i++){
        clearInterval(pub_int[i])
    }
    document.getElementById("main").innerHTML = "";
    var content = "<div class=\"header\"><h1>" + obj.menu[page] + "</h1></div>";
    content += "<div class=\"content\">";
    content += "<form class=\"pure-form pure-form-stacked\">";
    content += "<fieldset>";
    content += "<div class=\"pure-g\">";

    var items = obj.content[page].length;
    for(var i = 0; i < items; i++){
        content += content_item(obj.content[page][i], i);
    }

    content += "</div>";
    content += "</fieldset>";
    content += "</form>";
    content += "</div>";
    document.getElementById("main").innerHTML = content;
}

function content_item(item, i){
    var content = ''
    if(item.type == "pub"){
        content += "<div class=\"pure-u-1\" style=\"background-color: " + item.bg_color + "; text-align: center; color: " + item.text_color + ";\">"
        if(item.label != "") content += "<h2>" + item.label + "</h2>"
        content += "<p style=\"font-size: 32pt\" id=\"" + item.id + "\">" + item.value + item.unit + "</p>"
        content += "</div>"
        pub_int[pub_int_cnt] = setInterval(function () {
            pub(item.id, item.unit);
        }, 3000);
        pub_int_cnt++
        return content
    }

    content += "<div class=\"pure-u-1\">";

    if (item.type == "checkbox"){
        content += "<br>"
        content += "<div style=\"height: 40px;\">"
        content += "<input "
        if (typeof item.type == 'string') content += "type=\"" + item.type + "\""
        if (typeof item.id == 'string') content += "id=\"" + item.id + "\""
        if (typeof item.value == 'string' && item.value == "true") content += " checked "
        content += "class='checkbox' onclick=\"data('" + item.type + "', this.id, this.value, '" + item.label + "', " + page + ", " + i + ")\">";
        content += "<label class='switch' for='" + item.id +  "'>"
        content +=  "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp" + item.label
        content +=  "</label></input>"
        content += "</div>";
    }
    else if(item.html == "input"){
        content += "<label id=\"" + item.id + "-val\">" + item.label + " "
        if(item.type == "text" || item.type == "password") content += "(" + item.value.length + ")"
        else content += item.value
        content +=  "</label>";
        content += "<input ";
        if (typeof item.type == 'string') content += "type=\"" + item.type + "\"";
        if (typeof item.id == 'string') content += "id=\"" + item.id + "\"";
        if (typeof item.name == 'string') content += "name=\"" + item.name + "\"";
        if (typeof item.min == 'string') content += "min=\"" + item.min + "\"";
        if (typeof item.max == 'string') content += "max=\"" + item.max + "\"";
        if (typeof item.step == 'string') content += "step=\"" + item.step + "\"";
        if (typeof item.value == 'string') content += "value=\"" + item.value + "\"";
        if (typeof item.placeholder == 'string') content += "placeholder=\"" + item.placeholder + "\"";
        content += "class=\"pure-u-5-5\""

        if(item.type == "range"){
            content += "onchange=\"data('" + item.type + "', this.id, this.value, '" + item.label + "', " + page + ", " + i + ")\">";
        }

        if(item.type == "text" || item.type == "password" || item.type == "number" || item.type == "time" || item.type == "date" || item.type == "datetime-local"){
            content += "oninput=\"data('" + item.type + "', this.id, this.value, '" + item.label + "', " + page + ", " + i + ")\">";
        }

        if(item.type == "color"){
            content += "style=\"height: 50px\" oninput=\"data('" + item.type + "', this.id, this.value, '" + item.label + "', " + page + ", " + i + ")\">";
        }

    }
    if(item.html == "button"){
        if (typeof item.color != 'string') item.color = ''
        content += "<br>";
        content += "<input type=\"button\" ";
        content += "id=\"" + item.id + "\" style=\"background-color: " + item.color + "\" value=\"" + item.label + "\" class=\"pure-u-5-5 pure-button pure-button-primary\" onclick=\"data('" + item.type + "', 'BTN_' + this.id, this.value, '" + item.label + "', " + page + ", " + i + ")\">";
    }
    if(item.html == "textarea"){
        content += "<textarea class=\"pure-u-24-24\" maxlength=\"255\" id=\"" + item.id + "\" oninput=\"data('" + item.type + "', this.id, this.value, '" + item.label + "', " + page + ", " + i + ")\">" + item.value + "</textarea>";
    }
    if(item.html == "select"){
        content += "<label id=\"" + item.id + "-val\">" + item.label + "</label>";
        content += "<select ";
        if (typeof item.id == 'string') content += "id=\"" + item.id + "\"";
        if (typeof item.name == 'string') content += "name=\"" + item.name + "\"";
        if (typeof item.value == 'string') content += "value=\"" + item.value + "\"";
        content += "class=\"pure-u-5-5\"";
        content += "onchange=\"data(" + item.type + ", this.id, this.value, '" + item.label + "', '" + page + "', '" + i + "')\">";
        for(var i = 0; i < item.options.length; i++){
            if (item.options[i].value == item.value) content += "<option value=\"" + item.options[i].value + "\" selected>";
            else content += "<option value=\"" + item.options[i].value + "\">";
            content += item.options[i].label;
            content += "</option >";
        }
        content += "</select>";
    }
    content += "</div>";
    return content;
}

function datarange(id, value, label){
    document.getElementById(id + "-val").innerHTML = label + ":&nbsp" + value;
}

function data(type, id, value, label, page, i){
    if (type == "range") document.getElementById(id + "-val").innerHTML = label + ":&nbsp" + value;
    if (type == "text" || type == "password") document.getElementById(id + "-val").innerHTML = label + "&nbsp(" + value.length + ")";

    if (type == "checkbox"){
        var chbox=document.getElementById(id);
        if (chbox.checked) value = "true";
        else  value = "false";
    }
    obj.content[page][i].value = value;
    send(id, value)
}

function send(id, value){
    var formData = new FormData();
    formData.append(id, value);
    var xhr = new XMLHttpRequest();
    xhr.open('POST', '/post', true);
    xhr.onload = function () {
        var res = xhr.responseText;
    };
    xhr.send(formData);
}

function pub(id, unit){
    var xhr = new XMLHttpRequest();
    xhr.open('GET', '/pub?' + id, true);
    xhr.onload = function () {
        document.getElementById(id).innerHTML = xhr.responseText + unit;
    };
    xhr.send();
}
<!-- -->
document.addEventListener("DOMContentLoaded", function () {
  parse();
  var timerId = setInterval(function () {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', '/_refresh', true);
    xhr.onload = function () {
        var res = xhr.responseText;
        var needRefresh = JSON.parse(res);
        if(needRefresh._refresh == "1"){
            parse();
            console.log(needRefresh);
        }
    };
    xhr.send(null);
  }, 1000);
});
<!-- -->
)=====";