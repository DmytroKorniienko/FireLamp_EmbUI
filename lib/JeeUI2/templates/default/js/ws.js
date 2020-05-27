var wbs = function(url){
    var ws = null,
    open = function(fnopen, fnerror){
        ws = new WebSocket(url);
        ws.onerror = function(err){
            console.log(err);
            if (fnerror) fnerror(err);
            if (typeof out.onerror== "function") {
                try{ out.onerror(); } catch(e){ console.log('Error onerror', e); }
            }
        }
        ws.onopen = function(){
            if (fnopen) fnopen();
            if (typeof out.onopen== "function") {
                try{ out.onopen(); } catch(e){ console.log('Error onopen', e); }
            }
        }
        ws.onmessage = function(msg){
            console.log('Received message:',  msg);
            try{ msg = JSON.parse(msg.data); } catch(e){ console.log('Error message', e); }
            if (msg.pkg && typeof out["on"+msg.pkg] == "function"){
                try{ out["on"+msg.pkg](msg); } catch(e){ console.log('Error on'+msg.pkg, e); }
            }
        }
    },
    send_msg = function(msg){
        if (!ws || (ws.readyState != 1 && ws.readyState != 0)){
            setTimeout(function(){open(function(){ send_msg(msg); })}, 1000);
            return;
        }
        try{ msg = JSON.stringify(msg); } catch(e){ console.log('Error stringify', e); }
        console.log('Sending message:', msg);
        ws.send(msg);
    },

    out = {
        send_post: function(id, value){
            send_msg({pkg:"post", data:[{key: id, val: value}]});
        }
    };
    open();
    return out;
}

window.addEventListener("load", function(ev){
    var ws = this.ws = wbs("ws://"+location.host+"/ws");
    ws.oninterface = function(msg) {
        make(msg);
    }
}.bind(window));