var wbs = function(url){
    var ws = null,
    open = function(){
        ws = new WebSocket(url);
        ws.onerror = function(err){
            console.log(err);
        }
        ws.onopen = function(){
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
    ws.onrefrash = function(msg) {
        parse();
    }
    ws.oninterface = function(msg) {
        make(msg);
    }
    parse();
}.bind(window));