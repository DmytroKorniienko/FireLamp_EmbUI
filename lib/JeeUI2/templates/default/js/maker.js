
var global = {menu_id:0, menu: []};

var render = function(){
	var tmpl_menu = new mustache(go("#tmpl_menu")[0],{
		on_page: function(d,id) {
			out.menu_change(id);
		}
	}),
	tmpl_main = new mustache(go("#tmpl_main")[0],{
		on_input: function(d, id, label){
			var value = this.value, type = this.type;
			if (type == "range") go("#"+id+"-val").html(label + ":&nbsp" + value);
			if (type == "text" || type == "password") go("#"+id+"-val").html(label+"&nbsp("+value.length+")");
		},
		on_change: function(d, id) {
			var value = this.value, type = this.type;
			if (type == "checkbox"){
				var chbox=document.getElementById(id);
				if (chbox.checked) value = "true";
				else value = "false";
			}
			ws.send_post(id, value);
		}
	}),
	out = {
		menu_change: function(menu_id){
			global.menu_id = menu_id;
			this.menu();
			go("#main > div").display("none");
			go("#main #"+menu_id).display("block");
		},
		menu: function(){
			go("#menu").clear().append(tmpl_menu.parse(global));
		},
		content: function(obj){
			var name = "", sel = false;
			for (var i = 0; i < global.menu.length;  i++) if (global.menu[i].value == obj.section) {
				sel = (global.menu[i].value == global.menu_id);
				name = global.menu[i].label;
				break;
			}
			obj.selected = sel;
			obj.menu = name;
			go("#"+obj.section).remove();
			go("#main").append(tmpl_main.parse(obj));
		},
		make: function(obj){
			var frame = obj.block;
			if (!obj.block) return;
			for (var i = 0; i < frame.length; i++) if (typeof frame[i] == "object") {
				if (frame[i].section == "menu") {
					global.menu =  frame[i].block;
					global.app = obj.app;
					global.mc = obj.mc;
					if (!global.menu_id) global.menu_id = global.menu[0].value
					this.menu();
				} else {
					this.content(frame[i]);
				}
			}
		},
		value: function(obj){
			var frame = obj.block;
			if (!obj.block) return;
			for (var i = 0; i < frame.length; i++) if (typeof frame[i] == "object") {
				var el = go("#"+frame[i].id)[0];
				if (el) el.value = frame[i].value;
			}
		}
	};
	return out;
};

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
	ws.onvalue = function(msg){
		rdr.value(msg);
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
