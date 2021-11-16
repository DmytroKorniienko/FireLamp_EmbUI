// Скрипт для автоматического сокрытия пункта температуры (если нет датчика) в боковом меню
let count = 0;
function showTemp(click){
	let interval = 100;
	let delay = 5000;
	if (count*interval > delay && !click)
		return;
	count++;
	let temp = document.getElementById("pTemp");
	if(temp == null){
    if (!click)
		  setTimeout(showTemp, interval);
		return;
	}
	else if(!temp.textContent) {
		temp.parentNode.className = "hidden";
    if (!click)
		  setTimeout(showTemp, interval);
		return;
	}
	temp.parentNode.className = "";
};
window.addEventListener('click', function(el){
  showTemp(true);})

showTemp();