
var global = {menu_id:0, menu: [], value:{}};

var render = function(){
	var tmpl_menu = new mustache(go("#tmpl_menu")[0],{
		on_page: function(d,id) {
			out.menu_change(id);
			return false;
		}
	}),
	fn_secrion = {
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
			var data = {}; data[id] = (value !== undefined)? value : null;
			ws.send_post(data);
		},
		on_showhide: function(d, id) {
			go("#"+id).showhide();
		},
		on_submit: function(d, id) {
			var data = go.formdata(go("input, textarea, select", go("#"+id)));
			data[id] = null;
			ws.send_post(data);
		}
	},
	tmpl_section = new mustache(go("#tmpl_section")[0], fn_secrion),
	tmpl_section_main = new mustache(go("#tmpl_section_main")[0], fn_secrion),
	out = {
		lockhist: false,
		history: function(hist){
			history.pushState({hist:hist}, '', '?'+hist);
		},
		menu_change: function(menu_id){
			global.menu_id = menu_id;
			this.menu();
			go("#main > div").display("none");
			// go("#main #"+menu_id).display("block");
			var data = {}; data[menu_id] = null
			ws.send_post(data);
		},
		menu: function(){
			go("#menu").clear().append(tmpl_menu.parse(global));
		},
		content: function(obj){
			if (obj.main) {
				go("#main > section").remove();
				go("#main").append(tmpl_section_main.parse(obj));
				if (!out.lockhist) out.history(obj.section);
			} else {
				var el = go("#"+obj.section);
				if (el.length) {
					el.replace(tmpl_section.parse(obj));
				}
			}
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
			out.lockhist = false;
		},
		value: function(obj){
			var frame = obj.block;
			if (!obj.block) return;
			for (var i = 0; i < frame.length; i++) if (typeof frame[i] == "object") {
				var el = go("#"+frame[i].id);
				if (el.length) {
					if (el[0].value != undefined) el[0].value = frame[i].value;
					else {
						global.value[frame[i].id] = frame[i].value
						el.html(frame[i].value);
					}
				}
			}
		}
	};
	return out;
};

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

window.addEventListener("popstate", function(e){
	if (e = e.state && e.state.hist) {
		rdr.lockhist = true;
		var data = {}; data[e] = null;
		ws.send_post(data);
	}
});
