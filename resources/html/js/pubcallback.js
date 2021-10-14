// Скрипт для автоматического сокрытия пункта температуры (если нет датчика) в боковом меню
let count = 0;
function showTemp(){
	let interval = 100;
	let delay = 5000;
	if (count*interval > delay)
		return;
	count++;
	let temp = document.getElementById("pTemp");
	if(temp == null){
		setTimeout(showTemp, interval);
		return;
	}
	else if(!temp.textContent) {
		temp.parentNode.className = "hidden";
		setTimeout(showTemp, interval);
		return;
	}
	temp.parentNode.className = "";
};
showTemp();