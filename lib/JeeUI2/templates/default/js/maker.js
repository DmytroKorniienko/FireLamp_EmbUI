
var global = {current:{idx:0}};

var render = function(){
    var tmpl_menu = new mustache(go("#tmpl_menu")[0],{
        on_page: function(d,p) {
            global.current = {idx: p, menu: global.menu[p], content: global.content[p]};
            out.menu();
            out.content();
            // return false;
        }
    }),
    tmpl_main = new mustache(go("#tmpl_main")[0],{
        on_change: function(d, id, label, i) {
            var value = this.value, type = this.type;
            if (type == "range") document.getElementById(id + "-val").innerHTML = label + ":&nbsp" + value;
            if (type == "text" || type == "password") document.getElementById(id + "-val").innerHTML = label + "&nbsp(" + value.length + ")";

            if (type == "checkbox"){
                var chbox=document.getElementById(id);
                if (chbox.checked) value = "true";
                else  value = "false";
            }
            global.current.content[i].value = value;
            ws.send_post(id, value);
        }
    }),
    out = {
        menu: function(){
            go("#menu").clear().append(tmpl_menu.parse(global));
        },
        content: function(){
            go("#main").clear().append(tmpl_main.parse(global));
        },
        make: function(obj){
            //go.merge(global, obj);
            var idx = global.current.idx;
            global = obj;
            global.current = {idx: idx, menu: global.menu[idx], content: global.content[idx]};
            this.menu();
            this.content();
        }
    };
    return out;
};

function datarange(id, value, label){
    document.getElementById(id + "-val").innerHTML = label + ":&nbsp" + value;
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

window.addEventListener("load", function(ev){
    var rdr = this.rdr = render();
    var ws = this.ws = wbs("ws://"+location.host+"/ws");
    ws.oninterface = function(msg) {
        rdr.make(msg);
    }

    var active = false, layout =  go("#layout");
    go("#menuLink").bind("click", function(){
        active = !active;
        if (active) layout.addClass("active");
        else layout.removeClass("active");
        return false;
    });
    go("#layout, #menu, #main").bind("click", function(){
        active = false;
        layout.removeClass("active");
    });
}.bind(window));
