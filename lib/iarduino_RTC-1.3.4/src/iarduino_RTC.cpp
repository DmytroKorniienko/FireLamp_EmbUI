#include "iarduino_RTC.h"

//		Вывод даты и времени
char*	iarduino_RTC::gettime(String i){char j[i.length()+1]; i.toCharArray(j, i.length()); j[i.length()]=0; return gettime(j);}
char*	iarduino_RTC::gettime(const char* i){
			uint8_t j, k, n;																				//	Объявляем локальные переменные
			if(valRequest > millis()){valRequest=0;}														//	Избавляемся от переполнения millis()
//			Получаем текущее время:
			if(valPeriod == 0)											{funcReadTime();}else				//	Если минимальный период опроса модуля == 0 минут, то получаем время из регистров модуля, иначе ...
			if(valRequest == 0 || (valPeriod+valRequest <= millis()))	{funcReadTime();}else				//	Если время последнего опроса модуля превысило минимальный период опроса модуля, то получаем время из регистров модуля, иначе ...
																		{funcCalculationTime();}			//	Получаем время без обращения к модулю (время рассчитывая исходя из millis и valRequest)
			funcSetMoreTime();																				//	Преобразуем переменные не читаемые из модуля
//			Вычисляем размер блока памяти под строку с ответом:
			n=strlen(i)+1;																					//	Определяем размер равный введённой строке (i) + 1 символ конца строки
			for(j=0; j<strlen(i);         j++)	{															//	Проходим по символам полученной строки
			for(k=0; k<strlen(charInput); k++)	{															//	Проходим по служебным символам строки charInput
			if (i[j]==charInput[k])				{															//	Если символы обеих строк совпали, то ...
				if(k>0){n++;} if(k>9){n++;} if(k>11){n++;}													//	Увеличиваем размер (n) в соостветствии со значениём найденного служебного символа
			}}}
//			Выделяем блок памяти под строку с ответом:
			free(charReturn);																				//	Освобождаем ранее созданный блок памяти
			charReturn = (char*) malloc(n);																	//	Выделяем новый блок памяти размером n байт
//			Заполняем выделенный блок памяти:
			n=0;																							//	Определяем позицию в блоке памяти для следующего подставляемого значения
			for(j=0; j<strlen(i); j++)	{																	//	Проходим по символам полученной строки
				if(i[j]==charInput[0]	/*	w	*/	)	{funcFillChar( weekday		,0,n,7);	n+=1;}else	//	Подставляем день недели		от 0 до 6		(один знак: 0-воскресенье, 6-суббота)
				if(i[j]==charInput[1]	/*	a	*/	)	{funcFillChar( midday*2		,2,n,8);	n+=2;}else	//	Подставляем полдень			am или pm		(два знака, в нижнем регистре)
				if(i[j]==charInput[2]	/*	A	*/	)	{funcFillChar((midday+2)*2	,2,n,8);	n+=2;}else	//	Подставляем полдень			AM или PM		(два знака, в верхнем регистре)
				if(i[j]==charInput[3]	/*	d	*/	)	{funcFillChar( day			,1,n,4);	n+=2;}else	//	Подставляем день месяца		от 01 до 31		(два знака)
				if(i[j]==charInput[4]	/*	h	*/	)	{funcFillChar( hours		,1,n,3);	n+=2;}else	//	Подставляем часы			от 01 до 12		(два знака)
				if(i[j]==charInput[5]	/*	H	*/	)	{funcFillChar( Hours		,1,n,3);	n+=2;}else	//	Подставляем часы			от 00 до 23		(два знака)
				if(i[j]==charInput[6]	/*	i	*/	)	{funcFillChar( minutes		,1,n,2);	n+=2;}else	//	Подставляем минуты			от 00 до 59 	(два знака)
				if(i[j]==charInput[7]	/*	m	*/	)	{funcFillChar( month		,1,n,5);	n+=2;}else	//	Подставляем месяц			от 01 до 12 	(два знака)
				if(i[j]==charInput[8]	/*	s	*/	)	{funcFillChar( seconds		,1,n,1);	n+=2;}else	//	Подставляем секунды			от 00 до 59 	(два знака)
				if(i[j]==charInput[9]	/*	y	*/	)	{funcFillChar( year			,1,n,6);	n+=2;}else	//	Подставляем год				от 00 до 99 	(два знака)
				if(i[j]==charInput[10]	/*	M	*/	)	{funcFillChar((month+6)*3	,3,n,5);	n+=3;}else	//	Подставляем имя месяца		от Jan до Dec	(три знака: Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec)
				if(i[j]==charInput[11]	/*	D	*/	)	{funcFillChar( weekday*3	,3,n,7);	n+=3;}else	//	Подставляем имя деня недели	от Mon до Sun	(три знака: Mon Tue Wed Thu Fri Sat Sun)
				if(i[j]==charInput[12]	/*	Y	*/	)	{funcFillChar( year			,4,n,6);	n+=4;}else	//	Подставляем год				от 2000 до 2099	(четыре знака)
														{charReturn[n]=i[j];					n+=1;}		//	Если символ полученной строки не совпал со служебными, то подставляем его в блок памяти как есть, без изменений
			}	charReturn[n]='\0'; return charReturn;														//	Устанавливаем символ конца строки и возвращаем указатель на блок памяти с результатом
}

//			Заполняем значением определённую позицию блока памяти:
void		iarduino_RTC::funcFillChar(uint8_t i, uint8_t j, uint8_t n, uint8_t k){							//													(данные, тип данных, позиция для вставки, мигание)
			bool f=valBlink==k; if((millis()%valFrequency)<(valFrequency/2)){f=false;}						//	Устанавливаем флаг мигания, если значение valBlink равно значению параметра k
			switch (j){
				/* 1 знак	*/	case 0: if(i>6 ){i=6; }	charReturn[n]=f?32:i+48;																																		break;
				/* 2 знака	*/	case 1: if(i>99){i=99;}	charReturn[n]=f?32:i/10+48;					charReturn[n+1]=f?32:i%10+48;																						break;
				/* AM / PM	*/	case 2: if(i>6 ){i=6; }	charReturn[n]=f?32:charMidday[i];			charReturn[n+1]=f?32:charMidday[i+1];																				break;
				/* дн / мес	*/	case 3: if(i>54){i=54;}	charReturn[n]=f?32:charDayMon[i];			charReturn[n+1]=f?32:charDayMon[i+1];		charReturn[n+2]=f?32:charDayMon[i+2];									break;
				/* 4 знака	*/	case 4: if(i>99){i=99;}	charReturn[n]=f?32:(valCentury-1)/10+48;	charReturn[n+1]=f?32:(valCentury-1)%10+48;	charReturn[n+2]=f?32:i/10+48;			charReturn[n+3]=f?32:i%10+48;	break;
			}
}

//		Установка даты и времени
void	iarduino_RTC::settime(int i1, int i2, int i3, int i4, int i5, int i6, int i7){						//													(сек, мин, час, день, мес, год, день_недели)
			funcWriteTime(i1, i2, i3, i4, i5, i6, i7);														//	Записываем дату и время в регистры модуля
			funcReadTime();																					//	Читаем дату и время из регистров модуля
			funcSetMoreTime();																				//	Корректируем переменные не читаемые из модуля	(hours, midday)
}

//		Установка даты и времени от начала эпохи Unix
void	iarduino_RTC::settimeUnix(uint32_t i){																//															(сек)
			uint32_t j;	uint8_t k; bool f=true;																//	Объявляем временные переменные.
			seconds		=  i          % 60;																	//  Получаем текущее значение секунд.						(остаток от деления секунд прошедших с начала эпохи Unix на количество секунд в минуте)
			i			= (i-seconds) / 60;																	//  Получаем количество минут прошедших с начала эпохи Unix.
			minutes		=  i          % 60;																	//  Получаем текущее значение минут.						(остаток от деления минут  прошедших с начала эпохи Unix на количество минут в часе)
			i			= (i-minutes) / 60;																	//  Получаем количество часов прошедших с начала эпохи Unix.
			Hours		=  i          % 24;																	//  Получаем текущее значение часов.						(остаток от деления часов  прошедших с начала эпохи Unix на количество часов в дне)
			i			= (i-Hours)   / 24;																	//  Получаем количество  дней прошедших с начала эпохи Unix.
			j			=  0; while((((j+1)*365)+((j+2)/4))<=i){j++;}										//	Получаем количество   лет прошедших с начала эпохи Unix.
			weekday		= (i+4) % 7;																		//	Получаем текущий день недели.							(0-воскресенье, 1-понедельник, ... , 6-суббота)
			i			=  i - (j*365) - ((j+1)/4);															//	Получаем количество  дней прошедших в текущем году.
			valCentury	= ((1970+j)/100)+1;																	//	Получаем текущий век.
			year		=  (1970+j)%100;																	//	Получаем две последние цифры текущего года.
			k			= ((1970+j)%4)==0?29:28;															//	Получаем количество дней в феврале текущего года.
			month		= 0; while(f){month++; j=month; j=(((j+1)%2)^(j<8))?31:(j==2?k:30); if(i>=j){i-=j;}else{f=false;}}
			day			= i+1;																				//	Получаем текущий день.
		//	Устанавливаем время:
			settime(seconds, minutes, Hours, day, month, year, weekday);
}

//		Чтение даты и времени в переменные из регистров модуля:
void	iarduino_RTC::funcReadTime(void){																	//													(без параметров)
			seconds    = arrCalculationTime[0] = funcConvertCodeToNum(objClass -> funcReadTimeIndex(0));	//	Получаем секунды
			minutes    = arrCalculationTime[1] = funcConvertCodeToNum(objClass -> funcReadTimeIndex(1));	//	Получаем минуты
			Hours      = arrCalculationTime[2] = funcConvertCodeToNum(objClass -> funcReadTimeIndex(2));	//	Получаем часы
			day        = arrCalculationTime[3] = funcConvertCodeToNum(objClass -> funcReadTimeIndex(3));	//	Получаем день
			month      = arrCalculationTime[4] = funcConvertCodeToNum(objClass -> funcReadTimeIndex(4));	//	Получаем месяц
			year       = arrCalculationTime[5] = funcConvertCodeToNum(objClass -> funcReadTimeIndex(5));	//	Получаем год
			weekday    = arrCalculationTime[6] = funcConvertCodeToNum(objClass -> funcReadTimeIndex(6))-1;	//	Получаем день недели							(в регистре значение от 1 до 7, а в переменной от 0 до 6)
			Unix       = funcCalculationUnix();																//  Получаем количество секунд						(прошедших с начала эпохи Unix)
			valRequest = millis();																			//	Сохраняем время данного запроса
}

//		Запись даты и времени в регистры модуля:
void	iarduino_RTC::funcWriteTime(int i1, int i2, int i3, int i4, int i5, int i6, int i7){				//	
			if(i1<=60 && i1>=0){objClass -> funcWriteTimeIndex(0, funcConvertNumToCode(i1  ));}				//	Сохраняем секунды
			if(i2<=60 && i2>=0){objClass -> funcWriteTimeIndex(1, funcConvertNumToCode(i2  ));}				//	Сохраняем минуты
			if(i3<=23 && i3>=0){objClass -> funcWriteTimeIndex(2, funcConvertNumToCode(i3  ));}				//	Сохраняем часы
			if(i4<=31 && i4>=1){objClass -> funcWriteTimeIndex(3, funcConvertNumToCode(i4  ));}				//	Сохраняем день
			if(i5<=12 && i5>=1){objClass -> funcWriteTimeIndex(4, funcConvertNumToCode(i5  ));}				//	Сохраняем месяц
			if(i6<=99 && i6>=0){objClass -> funcWriteTimeIndex(5, funcConvertNumToCode(i6  ));}				//	Сохраняем год
			if(i7<= 6 && i7>=0){objClass -> funcWriteTimeIndex(6, funcConvertNumToCode(i7+1));}				//	Сохраняем день недели							(в регистре значение от 1 до 7, а в переменной от 0 до 6)
}

//		Расчёт времени без обращения к модулю:
void	iarduino_RTC::funcCalculationTime(void){															//													(без параметров)
			uint32_t i=(millis()-valRequest)/1000;															//	Определяем количество секунд					(прошедших после последнего обращения к модулю)
			uint8_t	 j=30 + ( (arrCalculationTime[4] + (arrCalculationTime[4]>7?1:0)) % 2 );				//	Определяем количество дней в месяце				(31, 30, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
			if(arrCalculationTime[4]==2){j=28+((((uint16_t)valCentury-1)*100+arrCalculationTime[5])%4?0:1);}//	Если текущий месяц - февраль, то меняем на ...	(31, 28/29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
			i+=arrCalculationTime[0]; seconds = i%60;		i/=60;											//	Добавляем к прошедним секундам (i) посление прочитанные секунды (arrCalculationTime[0]), результатом будет остаток, а значение i превращаем в минуты
			i+=arrCalculationTime[1]; minutes = i%60;		i/=60;											//	Добавляем к прошедним минутам  (i) посление прочитанные минуты  (arrCalculationTime[1]), результатом будет остаток, а значение i превращаем в часы
			i+=arrCalculationTime[2]; Hours   = i%24;		i/=24;											//	Добавляем к прошедним часам    (i) посление прочитанные часы    (arrCalculationTime[2]), результатом будет остаток, а значение i превращаем в дни
			                          weekday = arrCalculationTime[6]+i; if(weekday>6){weekday=0;}			//	День недели увеличится на      (i) от прочитанного дня недели   (arrCalculationTime[6])
			i+=arrCalculationTime[3]; day     = i%(j+1);	i/=(j+1);	day+=i;								//	Добавляем к прошедним дням     (i) посление прочитанные дни     (arrCalculationTime[3]), результатом будет остаток, а значение i превращаем в месяцы
			i+=arrCalculationTime[4]; month   = i%13;		i/=13;		month+=i;							//	Добавляем к прошедним месяцам  (i) посление прочитанные месяцы  (arrCalculationTime[4]), результатом будет остаток, а значение i превращаем в годы
			i+=arrCalculationTime[5]; year    = i%100;														//	Добавляем к прошедним годам    (i) посление прочитанные годы    (arrCalculationTime[5]), результатом будет остаток, а значение i превращаем в дни
			                          Unix    = funcCalculationUnix();										//  Получаем количество секунд прошедших с начала эпохи Unix
}

//		Расчёт количества cекунд прошедших с начала эпохи Unix:
uint32_t iarduino_RTC::funcCalculationUnix(void){															//													(без параметров)
			uint32_t i;																						//	Объявляем  переменную для хранения результата.	(рассчёты производятся из значений переменных: seconds, minutes, Hours, day, month, year и valCentury).
			uint32_t j = (uint32_t)(valCentury-1) * 100 + year;												//	Определяем текущий год с учётом века.			(valCentury - век, year - последние два знака текущего года).
			i  = j - 1970;																					//  Определяем количество прошедших лет				(с 01.01.1970 г.)
			i  = i * 365 + ((i+1)/4);																		//  Определяем количество дней в прошедших годах	((i+1)/4) - количество прошедших високосных лет (без учёта текущего года).
			i += (month-1)*30 + ( (month + (month<9?0:1) )/2 );												//  Добавляем  количество дней в прошедших месяцах	((month+(month<9?0:1))/2) - количество прошедших месяцев текущего года, содержащих 31 день.
			i -= month>2? (j%4==0?1:2) : 0;																	//  Вычитаем   1 или 2 дня из февраля текущего года	((month>2) - если февраль уже прошел, j%4==0 - если текущий год високосный)
			i += day-1;																						//  Добавляем  количество прошедших дней этого месяца
			i *= 86400;																						//	Получаем   количество секунд прошедших дней
			i += (uint32_t)Hours * 3600 + (uint32_t)minutes * 60 + seconds;									//	Добавляем  количество секунд текущего дня
			return i;																						//	Возвращаем результат
}
