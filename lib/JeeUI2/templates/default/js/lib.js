var go = function(param, context){
	if (!arguments.length) return new GO(document);
	if (typeof param == "string") return (new GO(context || document)).go(param);
	if (typeof param == "undefined") return new GO();
	return new GO(param);
}

go.formdata = function(form){
	var controls = {},
	checkValue = function(element){
		switch (element.type.toLowerCase()){
			case 'checkbox':
				return element.checked?	"true" : "false";
			case 'radio':
				if(element.checked) return element.value;
				break;
			default:
				return element.value;
		}
	};

	for(var i = 0; i < form.length; i++){
		var el = form[i];
		if (el.disabled) continue;
		switch (el.tagName.toLowerCase()){
			case 'input':
				var val = checkValue(el);
				if (typeof val != "undefined") controls[el.name || el.id]= val;
				break;
			case 'textarea':
			case 'select':
				controls[el.name || el.id]= el.value;
				break;
			default:
				continue;
		}
	}
	return controls;
}

go.linker = function(fn,th){
	var arg = Array.prototype.slice.call(arguments,2);
	return function(){ return fn.apply(th, Array.prototype.slice.call(arguments).concat(arg)); };
}
go.listevent = function(el, evn, fn, opt){
	var self = ((opt instanceof Object)? opt.self : 0) || this;
	el = (el instanceof Array)? el : [el];
	evn = (evn instanceof Array)? evn : [evn];
	var rfn = function(arg){
		return function(e){
			e = e || event;
			var a = arg.slice(); a.unshift(e);
			var retval = fn.apply(self, a);
			if (typeof retval !== 'undefined' && !retval){
				if(e.preventDefault) e.preventDefault();
				else e.returnValue = false;

				if(e.stopPropagation) e.stopPropagation();
				else e.cancelBubble = true;
			}
			return retval;
		};
	}([].slice.call(arguments,4));

	for(var i = 0; i < el.length; i++) for(var j = 0; j < evn.length; j++){
		if(window.addEventListener){
			el[i].addEventListener(evn[j], rfn, opt);
		}else{
			el[i].attachEvent("on"+evn[j], rfn);
		}
	}
};
go.path = function(el, p, st){
	p = (p instanceof Array)? p : p.split(".");
	var i = p.shift();
	if (!i && st && st._parent) return this.path(st._parent._this, p, st._parent);
	if (i == "_this" || i == "_this2" || i == "_this3") {
		return p.length? this.path(st._this, p, st) : el;
	}
	if (i == "_index") return st._index;
	if (i == "_key") return st._key;
	if (i == "_val") return st._val;

	if (!el) return el;
	if (el[i] instanceof Object && p.length) return this.path(el[i], p, st);
	return el[i];
}
go.merge = function(base, data, idx) {
	for (var i in data) {
		var id = idx? idx + parseInt(i) : i;
		if (typeof data[i] == 'object') {
			if (typeof base[id] != 'object') base[id] = (data[i] instanceof Array)? [] : {};
			this.merge(base[id], data[i], (i == "block")? data.idx : undefined);
		} else base[id] = data[i];
	}
	return base;
}
go.eval = function(th, data) {
	return (function(){ return eval(data); }).call(th);
}

var GO = function(context){
	if (context) {
		context = ((context instanceof Object || typeof context == 'object') &&
			typeof context.length !== 'undefined' && context.nodeType == undefined
		)? context : [context];
		for(var i = 0; i < context.length; i++) this.push(context[i]);
	}
}
GO.prototype = new Array();
;(function($_){
	$_._alias = {},
	$_.alias = function(alias){
		for(var a in alias){
			$_[a] = function(path){
				path = (typeof path == 'string')? path.split(".") : path;
				return function(){
					var m = null, out = new GO([]);
					for (var i = 0; i < this.length; i++) {
						if (typeof path == 'function') {
							m = path.apply(this[i], arguments);
						} else {
							var obj = this[i], len = path.length - 1;
							for (var j = 0; obj && j < len; j++) obj = obj[path[j]];
							if (obj && typeof obj[path[len]] !== 'undefined') {
								if (typeof obj[path[len]] == 'function' || typeof obj[path[len]].toString == 'undefined'){
									m = obj[path[len]].apply(obj, arguments);
								} else if (typeof obj[path[len]] == 'object'){
									if (arguments.length == 2) obj[path[len]][arguments[0]] = arguments[1];
									m = this[i];//obj[path[len]];
								} else {
									if (arguments.length == 1) obj[path[len]] = arguments[0];
									m = this[i];//obj[path[len]];
								}
							}
						}
						if (m === null) continue;
						if (m === undefined) m = this[i];
						if (!m || typeof m === 'string' || typeof m === 'function' || typeof m.length === 'undefined') {
							m = [m];
						}
						for (var n = 0; n < m.length; n++) out.push(m[n]);
					}
					return out;
				};
			}(alias[a]);
			$_._alias[a] = alias[a];
		}
		return this;
	}
	$_.alias({
		'go':'querySelectorAll', 'query':'querySelectorAll',
		'attr':'getAttribute','sattr':'setAttribute','rattr':'removeAttribute',
		'byid':'getElementById', 'bytag':'getElementsByTagName',
		'byname':'getElementsByName', 'byclass':'getElementsByClassName',
		'css':'style', 'style':'style', 'display':'style.display',
		'left': 'style.left', 'right': 'style.right', 'top': 'style.top', 'bottom': 'style.bottom',
		'height': 'style.height', 'width': 'style.width',
		'innerHTML': 'innerHTML', 'src': 'src', 'href': 'href',
		'addClass':'classList.add', 'removeClass':'classList.remove', 'className':'className',
		'toggleClass':'classList.toggle', 'contClass':'classList.contains',
		'each': function(fn){
			var r = fn.apply(this, [].slice.call(arguments,1));
			return (typeof r != 'undefined')? r : this;
		},
		'exec': function(){ return this.apply(arguments); },
		'event': function(n,dt,ps) {
			var evt = null;
			ps = ps || {bubbles: false, cancelable: true, detail: undefined, view: undefined};
			ps.detail = dt;
			if (typeof window.CustomEvent != "function") {
				evt = document.createEvent('CustomEvent');
				evt.initCustomEvent(n, ps.bubbles, ps.cancelable, ps.detail);
			} else {
				evt = new CustomEvent(n, ps);
			}
			this.dispatchEvent(evt);
			return this;
		},
		'bind': function(ev, fn){
			return go.listevent.apply(this, [this, ev, fn, {self: this}].concat([].slice.call(arguments, 2)));
		},
		'find': function(k,v){
			return (typeof this == 'object' && this[k] && (!v || this[k] === v))? this : null;
		},
		'id': function(v){
			return (typeof this == 'object' && this["id"] === v)? this : null;
		},
		'set': function(k,v){
			if (typeof this == 'object') {
				if (typeof k == 'object') {
					for (var i in k) this[i] = k[i];
				} else this[k] = v;
				return this;
			}
			return null;
		},
		'cstyle': function(ps){
			if (typeof ps == 'object') for (var i in ps) {
				var st = i.split("-");
				for (var n = 1; n < st.length; n++) st[n] = st[n][0].toUpperCase() + st[n].slice(1);
				this.style[st.join("")] = ps[i];
			}
			return [window.getComputedStyle? window.getComputedStyle(this, (typeof ps == 'string')? ps : "") : this.currentStyle];
		},
		'showhide': function(){
			if (!this.style.display) this.style.display = go(this).cstyle()[0].display;
			this.style.display = (this.style.display === "none")? "block" : "none";
			return this;
		},
		'clone': function(){ return this.cloneNode(true); },
		'fragment': function(el){ return document.createDocumentFragment();	},
		'remove': function(){ var p = this.parentNode; p.removeChild(this); return this; },
		'clear': function(){ while(this.firstChild) this.removeChild(this.firstChild); return this; },
		'append': function(el){
			el = (el instanceof Array)? el : [el];
			for(var i = 0; i < el.length; i++) this.appendChild(el[i]);
			return this;
		},
		'prepend': function(el){
			el = (el instanceof Array)? el : [el];
			for(var i =  el.length; i > 0; i--) this.insertBefore(el[i-1], this.firstChild);
			return this;
		},
		'replace': function(el){
			el = (el instanceof Array)? el : [el];
			for(var i = 0; i < el.length; i++) {
				this.parentNode.insertBefore(el[i], this);
				if (i == el.length - 1) this.parentNode.removeChild(this);
			}
			return this;
		},
		'before': function(el){
			el = (el instanceof Array)? el : [el];
			for(var i =  el.length; i > 0; i--) this.parentNode.insertBefore(el[i-1], this);
			return this;
		},
		'after': function(el, t){
			el = (el instanceof Array)? el : [el];
			var ns = t? this.nextSibling : this.nextElementSibling;
			if (ns) {
				for(var i = el.length; i > 0; i--) this.parentNode.insertBefore(el[i-1], ns);
			} else {
				for(var i = 0; i < el.length; i++) this.parentNode.appendChild(el[i]);
			}
			return this;
		},
		'create': function(tg,a1,a2){
			tg = document.createElement(tg);
			var dt = (typeof a2 == "string")? a2 : ((typeof a1 == "string")? a1 : 0),
			at = (typeof a1 == "object")? a1 : 0;
			if(dt) tg.innerHTML = dt;
			if(at) for(var i in at) tg.setAttribute(i, at[i]);
			if (this !== document) this.appendChild(tg);
			return tg;
		},
		'html': function(vl,ap){ if(ap) this.innerHTML += vl; else this.innerHTML = vl; return this; },
		'children': function(){ return this.children; },
		'nodes': function(type){
			return (this.nodeType == (type? type : 1))? this : null;
		}
	});
}(GO.prototype));

var mustache = function(tmpl,func){
	this.nevent = 0; this.scope = [];
	this._tmpl = (typeof tmpl == 'string')? tmpl : tmpl.innerHTML.replace(/[\r\t\n]/g, "");
	this._func = func || this._func || {};

	this._func.exec = this._func.exec || function(d,fn){
		var arg = [].slice.call(arguments, 2),
		out = 'data-musfun'+this.nevent+'="'+fn+'" '+
		'data-musarg'+this.nevent+'="'+escape(JSON.stringify(arg))+'"';
		this.nevent++;
		return out;
	};
	this._func.onevent = this._func.onevent || function(d){
		var arg = [].slice.call(arguments, 1);
		arg.unshift(d, "event");
		return this._func.exec.apply(this, arg);
	};
	this._func.event = this._func.event || function(th,ev,fn){
		var arg = [].slice.call(arguments, 3);
		fn = th._func[fn];
		if (fn) go.listevent(this, ev.split(","), function(e){
			var a = arg.slice(); a.unshift(e); return fn.apply(this, a);
		}, {self: this});
	};
	this._func.if = this._func.if || function(d){
		for (var i = 1; i < arguments.length; i++) if (!arguments[i]) return 0;
		return 1;
	};
	this._func.if2 = this._func.if,
	this._func.if3 = this._func.if,
	this._func.calc = this._func.calc || function(d){
		return [].slice.call(arguments, 1).join();
	};
}
;(function($_){
	var cont = document.createElement("div"),
	rgobj = /\{\{(@|#|!|\^)([a-z0-9_.]+?)(?:\s+(.+?))?\}\}([\s\S]*?)\{\{\/\2\}\}/gi,
	rgitem = /\{\{(!|>)?([a-z0-9_.]+?)(?:\s+(.+?))?\}\}/gi,
	rgparam = /\s*,\s*/gi,
	rgfield = /[a-z0-9_.]+|".*?"|'.*?'|`.*?`/gi,
	strcon = ['true','false','null','undefined','typeof','instanceof','Array','Object'];

	$_._parse_param = function(val, data, stack){
		var m = val.match(rgfield), s = val.split(rgfield), out = "";
		while (s && (val = s.shift()) != undefined) {
			out += val;
			if (m && (val = m.shift())) {
				if (strcon.indexOf(val) != -1 || val[0] == '"' || val[0] == "'" || Number(val) == Number(val)) out += val;
				else if (val[0] == "`") out += val.substr(1, val.length - 2);
				else {
					this.scope.push(go.path(data, val, stack));
					out += "this["+(this.scope.length-1)+"]";
				}
			}
		}
		try{ return go.eval(this.scope, out); }catch(e){ return undefined; }
	}
	$_._parse_item = function(tmpl, data, stack){
		tmpl = (tmpl || "").replace(rgitem, go.linker(function(all, pref, name, argum){
			var arg = argum? argum.split(rgparam) : [], out;
			for (var i = 0; i < arg.length; i++) arg[i] = this._parse_param(arg[i], data, stack);
			if (pref == "!") return "";
			if (pref == ">"){
				var el = document.getElementById(name), dt = arg.length? arg[0] : data;
				return el? (new mustache(el, this._func)).parse(dt, this, stack) : "";
			}
			if (this._func[name]) {
				arg.unshift(data);
				return this._func[name].apply(this, arg);
			}
			out = go.path(data, name, stack);
			return (typeof out != 'undefined')? out : "";
		}, this));
		return tmpl;
	}
	$_._parse_obj = function(tmpl, data, stack){
		tmpl = (tmpl || "").replace(rgobj, go.linker(function(all, pref, name, argum, body){
			var out = body || argum || "", value = go.path(data, name, stack);
			if (pref == "!") return "";
			if (typeof value == 'function' || this._func[name]) {
				var arg = (body && argum)? argum.split(rgparam) : [];
				for (var i = 0; i < arg.length; i++) arg[i] = this._parse_param(arg[i], data, stack);
				arg.unshift(data);
				if (pref == "@") arg.unshift(body);
				var res = (this._func[name] || value).apply(this, arg);
				if (pref == "#") out = res? this._parse_obj(out, data, stack) : "";
				else if (pref == "^") out = res? "" : this._parse_obj(out, data, stack);
				else out = res;
			} else if (value instanceof Array) {
				if (pref != "^" && !value.length) return "";
				if (pref == "^" && value.length) return "";
				var res = "", stk = {_this:data, _parent:stack};
				for (var i = 0; i < value.length; i++) {
					stk._index = i; stk._val = value[i];
					var tmp = this._parse_obj(out, value[i], stk);
					res += this._parse_item(tmp, value[i], stk);
				}
				out = res;
			} else if (value instanceof Object) {
				if (!value && pref == "#" || value && pref == "^") return "";
				var stk = {_this:data, _parent:stack};
				if (pref == "#" || pref == "^") {
					out = this._parse_obj(out, value, stk);
					out = this._parse_item(out, value, stk);
				} else {
					var res = "", n = 0;
					for (var i in value) {
						stk._index = n++; stk._key = i;
						var tmp = this._parse_obj(out, value[i], stk);
						res += this._parse_item(tmp, value[i], stk);
					}
					out = res;
				}
			} else {
				if (!value && pref == "#" || value && pref == "^") return "";
				out = this._parse_obj(out, data, stack);
				out = this._parse_item(out, data, stack);
			}

			return out;
		}, this));
		return tmpl;
	}
	$_.parse = function(data, parent, stack){
		this.nevent = (typeof parent == "object")? parent.nevent : 0;
		stack = stack || {_this:data};
		var out = this._parse_obj(this._tmpl, data, stack);
		out = this._parse_item(out, data, stack);
		if (parent) {
			if (typeof parent == "object") parent.nevent = this.nevent;
			return out;
		}
		cont.innerHTML = out;
		for (var i = 0; i <= this.nevent; i++){
			go("[data-musfun"+i+"]",cont).each(function(th){
				var fn = th._func[this.getAttribute("data-musfun"+i)],
				arg = JSON.parse(unescape(this.getAttribute("data-musarg"+i)));
				this.removeAttribute("data-musarg"+i);
				this.removeAttribute("data-musfun"+i);
				try{ if (fn){ arg.unshift(th); fn.apply(this, arg); } }catch(e){}
			}, this);
		}
		return [].slice.call(cont.childNodes);
	}
}(mustache.prototype));

var wbs = function(url){
	var ws = null, frame = {}, connected = false, lastmsg = null, to = null,
	open = function(fnopen, fnerror){
		ws = new WebSocket(url);
		ws.onerror = function(err){
			console.log("WS Error", err);
			if (fnerror) fnerror(err);
			if (typeof out.onerror== "function") {
				try{ out.onerror(); } catch(e){ console.log('Error onerror', e); }
			}
		}
		ws.onopen = function(){
			console.log("WS Open");
			connected = true;
			if (fnopen) fnopen();
			if (typeof out.onopen== "function") {
				try{ out.onopen(); } catch(e){ console.log('Error onopen', e); }
			}
		},
		ws.onclose = function(){
			console.log("WS Close");
			connected = false;
			if (typeof out.onclose== "function") {
				try{ out.onclose(); } catch(e){ console.log('Error onclose', e); }
			}
		}
		ws.onmessage = function(msg){
			try{ msg = JSON.parse(msg.data); } catch(e){ console.log('Error message', e); return; }
			console.log('Received message:', msg);
			if (!(msg instanceof Object)) return;
			if (msg.section) {
				if (!frame[msg.section]) frame[msg.section] = {};
				go.merge(frame[msg.section], msg);
				if (msg.final) {
					receiv_msg(frame[msg.section]);
					delete frame[msg.section];
				}
			} else {
				receiv_msg(msg);
			}
		}
	},
	receiv_msg = function(msg){
		console.log('Received packet:', msg);
		if (msg.pkg && typeof out["on"+msg.pkg] == "function"){
			try{ out["on"+msg.pkg](msg); } catch(e){ console.log('Error on'+msg.pkg, e); }
		}
	},
	send = function(msg){ try{ ws.send(msg); }catch(e){} },
	send_msg = function(msg){
		console.log('Sending message:', msg);
		try{ lastmsg = JSON.stringify(msg); } catch(e){ console.log('Error stringify', e); return; }
		if (connected) send(lastmsg);
	},

	out = {
		connect: function(){
			console.log("WS Connect");
			ws = null;
			if (to) clearTimeout(to);
			to = setTimeout(function(){open(function(){
				to = null;
				if (lastmsg) send(lastmsg);
			})}, 500);
		},
		send_post: function(dt){
			send_msg({pkg:"post", data:dt});
		}
	};
	return out;
}
