class DRAW_CTRL {
  constructor(height=16, width=16, color='', ispressed=false) {
    this.height = height;
    this.width = width;
    this.ispressed = ispressed;
    this.color = color;
    this.selcolor = color;
    this.ctrl = "";
	}
 
  get getdrawhtml() {
    var ihtml = "";

    for(var i=0; i<this.width; i++){
      ihtml += "<tr>"
      for(var j=0; j<this.height; j++){ // ontouchstart=\"startsel(this.id)\" ontouchmove=\"movesel(this.id)\" 
        ihtml += "<td class=\"dc_cells\" id=\"c"+(this.width-i-1)+"_"+j+"\" onmousedown=\"startsel(this.id)\" onmouseover=\"movesel(this.id)\" onmouseup=\"endsel(this.id)\" ></td>"
      }
      ihtml += "</tr>"
    }
    return ihtml;
  }
}

document.body.onmousedown = function() { 
  draw_ctrl.ispressed = true
}

document.body.onmouseup = function() {
  draw_ctrl.ispressed = false;
}

/*
document.body.onmouseleave = function() { 
  draw_ctrl.ispressed = false
}
*/

let draw_ctrl = new DRAW_CTRL(16,16);

function hexToRgb(hex) {
    // Expand shorthand form (e.g. "03F") to full form (e.g. "0033FF")
    var shorthandRegex = /^#?([a-f\d])([a-f\d])([a-f\d])$/i;
    hex = hex.replace(shorthandRegex, function(m, r, g, b) {
        return r + r + g + g + b + b;
    });

    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? "rgb(" + [
        parseInt(result[1], 16),
        parseInt(result[2], 16),
        parseInt(result[3], 16)
    ].join(', ') + ")" : null;
}

function rgbToHex(color) {
  var bg = color.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);
  function hex(x) {
    return ("0" + parseInt(x).toString(16)).slice(-2);
  }
  return     "#" + hex(bg[1]) + hex(bg[2]) + hex(bg[3]);
}

function custom_hook(tid, d, id){
	if(tid==id+"-c"){
		var cctrl = document.getElementById(tid);
		if(cctrl.value=="#000000")
			draw_ctrl.selcolor = "";
  		else
			draw_ctrl.selcolor = cctrl.value;
	} else if(tid==id+"-b"){
	    var elem;
	    var elemColor = draw_ctrl.selcolor;
	    if (elemColor === '#000000') elemColor = "";
	    //if (elemColor.slice(0, 1) === '#') elemColor = hexToRgb(elemColor);
 
	    for(var i=0; i<draw_ctrl.width; i++){
	      for(var j=0; j<draw_ctrl.height; j++){
		elem = document.getElementById("c"+i+"_"+j);
		elem.style.backgroundColor = elemColor;
	      }
	    }
	    var data = {}; data[draw_ctrl.ctrl+"_fill"]=draw_ctrl.selcolor; 
	    ws.send_post(data);
	}
}

function sendpost(obj_id){
    id_num = obj_id.substring(1);
 
    //находим индекс разделител€
    var ind_ = id_num.indexOf('_');
    //определ€ем номер строки €чейки
    var n_row = id_num.substring(0, ind_);   
    //определ€ем номер столбца €чейки
    var n_col = id_num.substring(ind_ + 1);
    
    var color = draw_ctrl.color;
    if (color.slice(0, 1) != '#' && color!='')
    	color = rgbToHex(color);

    console.log(n_col,n_row,color);
    var data = {}; data[draw_ctrl.ctrl] = "[\""+color+"\","+n_col+","+n_row+"]"; 
    ws.send_post(data);
}

function init_draw(obj_id, width, height){
  var tbl = document.getElementById(obj_id+'-tbl');
  draw_ctrl.selcolor = document.getElementById(obj_id+'-c').value;
  draw_ctrl.ctrl = obj_id;
  draw_ctrl.width = width;
  draw_ctrl.height = height;

  tbl.style.height = tbl.parentElement.clientWidth * (draw_ctrl.height/draw_ctrl.width) +"px";
  tbl.innerHTML = draw_ctrl.getdrawhtml;
}

function startsel(obj_id) {
  var elem = document.getElementById(obj_id);
  elem.draggable=false;
  var elemColor = draw_ctrl.selcolor;
  if (elemColor.slice(0, 1) === '#') elemColor = hexToRgb(elemColor);
  
  if(elem.style.backgroundColor!=elemColor){
    //console.log(elem.style.backgroundColor,elemColor);
    draw_ctrl.color = elemColor;
    sendpost(obj_id);
  } else if(elem.style.backgroundColor!=''){
    draw_ctrl.color = '';
    sendpost(obj_id);
  } else {
  	return;
  }
  elem.style.backgroundColor = draw_ctrl.color;
}

function movesel(obj_id) {
  if(!draw_ctrl.ispressed) return;
  var elem = document.getElementById(obj_id);
  if(elem.style.backgroundColor != draw_ctrl.color){
  	elem.style.backgroundColor = draw_ctrl.color;
    sendpost(obj_id);
  }
}

function endsel(obj_id) {
	if(draw_ctrl.ispressed)  
		draw_ctrl.ispressed = false;
}

//---------------------------------------------
