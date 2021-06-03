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

    for(var i=0; i<this.height; i++){
      ihtml += "<tr>"
      for(var j=0; j<this.width; j++){
        ihtml += "<td class=\"dc_cells\" id=\"c"+(this.height-i-1)+"_"+j+"\"></td>"
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
 
	    for(var i=0; i<draw_ctrl.height; i++){
	      for(var j=0; j<draw_ctrl.width; j++){
		elem = document.getElementById("c"+i+"_"+j);
		elem.style.backgroundColor = elemColor;
	      }
	    }
	    var data = {}; data[draw_ctrl.ctrl+"_fill"]=draw_ctrl.selcolor;
      
	    ws.send_post(data);
	}
  else if(tid==id+"-d"){
    for(var i=0; i<draw_ctrl.height; i++){
      for(var j=0; j<draw_ctrl.width; j++){
        elem = document.getElementById("c"+i+"_"+j);
         elem.style.backgroundColor = "";
      }
    }
    var data = {drawing_ctrl_fill: ""};
    ws.send_post(data);
  }
}

function sendpost(obj_id){
    id_num = obj_id.substring(1);
 
    //������� ������ �����������
    var ind_ = id_num.indexOf('_');
    //���������� ����� ������ ������
    var n_row = id_num.substring(0, ind_);   
    //���������� ����� ������� ������
    var n_col = id_num.substring(ind_ + 1);
    
    var color = draw_ctrl.color;
    if (color.slice(0, 1) != '#' && color!='')
    	color = rgbToHex(color);

    console.log(n_col,n_row,color);
    var data = {}; data[draw_ctrl.ctrl] = "[\""+color+"\","+n_col+","+n_row+"]"; 
    ws.send_post(data);
}

function onTouch(evt) {
  evt.preventDefault();
  if (evt.touches.length > 1 || (evt.type == "touchend" && evt.touches.length > 0))
    return;

var touch = evt.changedTouches[0];
var xpos = Math.trunc(draw_ctrl.height-(touch.pageY-evt.currentTarget.offsetTop)*(draw_ctrl.height/evt.currentTarget.clientHeight));
var ypos = Math.trunc((touch.pageX-evt.currentTarget.offsetLeft)*(draw_ctrl.width/evt.currentTarget.clientWidth));
var cell_id="c"+xpos+"_"+ypos;

  switch (evt.type) {
    case "touchstart":
      starttouch(cell_id);
      break;
    case "touchmove":
      touchsel(cell_id);
      break;
    case "touchend":
      break;
  }
//console.log(evt.target.id, xpos, ypos, cell_id);
}

function onMouse(evt) {
  evt.preventDefault();
  var cell_id=evt.target.id; //"c"+xpos+"_"+ypos;
  if(draw_ctrl.ctrl+'-tbl'==cell_id)
	return;

  switch (evt.type) {
    case "mousedown":
      startsel(cell_id);
      break;
    case "mouseover":
      movesel(cell_id);
      break;
    case "mouseup":
      endsel(cell_id);
      break;
  }
//console.log(evt.target.id);
}


function starttouch(obj_id) {
  draw_ctrl.ispressed = true
  startsel(obj_id);
}

function touchsel(obj_id) {
  draw_ctrl.ispressed = true;
  movesel(obj_id);
}

function init_draw(obj_id, width, height){
  var tbl = document.getElementById(obj_id+'-tbl');
  draw_ctrl.selcolor = document.getElementById(obj_id+'-c').value;
  draw_ctrl.ctrl = obj_id;
  draw_ctrl.width = width;
  draw_ctrl.height = height;

  tbl.style.height = tbl.parentElement.clientWidth * (draw_ctrl.height/draw_ctrl.width) +"px";
  tbl.style.width = tbl.parentElement.clientWidth;
  tbl.innerHTML = draw_ctrl.getdrawhtml;
  tbl.addEventListener("touchstart", onTouch, false);
  tbl.addEventListener("touchend", onTouch, false);
  tbl.addEventListener("touchcancel", onTouch, false);
  tbl.addEventListener("touchmove", onTouch, false);

  tbl.addEventListener("mousedown", onMouse, false);
  tbl.addEventListener("mouseup", onMouse, false);
  tbl.addEventListener("mouseover", onMouse, false);

}

function startsel(obj_id) {
  var elem = document.getElementById(obj_id);

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
