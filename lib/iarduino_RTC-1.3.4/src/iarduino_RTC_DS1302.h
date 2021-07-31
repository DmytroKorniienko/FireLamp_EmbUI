#ifndef iarduino_RTC_DS1302_h																	//
#define iarduino_RTC_DS1302_h																	//
#define	RTC_DS1302 1																			//	Модуль часов реального времени с протоколом передачи данных SI3, памятью 040x8 (31 байт которой доступны для хранения данных пользователя)
																								//
class iarduino_RTC_DS1302: public iarduino_RTC_BASE{											//
	public:																						//
	/**	Конструктор класса **/																	//
		iarduino_RTC_DS1302(uint8_t i=SS, uint8_t j=SCK, uint8_t k=MOSI){pinRES=i; pinCLK=j; pinDAT=k;}	//	(вывод RST, вывод CLK, вывод DAT)
	/**	функции доступные пользователю **/														//
//		Инициализация модуля:																	//
		void	begin(void){																	//	(без параметров)
//					Инициализация работы с трехпроводной шиной:									//
					funcBegin();																//	(без параметров)
//					Установка флагов управления и состояния модуля:								//
					varI=funcReadReg(0x81); if( varI & 0b10000000){funcWriteReg(0x81, (varI&~0b10000000));}	//		(если установлен 7 бит в 129 регистре, то сбрасываем его - запускаем генератор)
					varI=funcReadReg(0x85); if( varI & 0b10000000){funcWriteReg(0x85, (varI&~0b10000000));}	//		(если установлен 7 бит в 133 регистре, то сбрасываем его - переводим модуль в 24 часовой режим)
					varI=funcReadReg(0x8F); if( varI & 0b10000000){funcWriteReg(0x8F, (varI&~0b10000000));}	//		(если установлен 7 бит в 143 регистре, то сбрасываем его - разрешаем запись в регистры модуля)
		}																						//
																								//
//		Чтение одного значения из регистров даты и времени модуля:								//
		uint8_t	funcReadTimeIndex(uint8_t i){													//	(i = 0-секунды / 1-минуты / 2-часы / 3-день / 4-месяц / 5-год / 6-день недели)
					delay(1); return funcReadReg(arrTimeRegAddr[i]) & arrTimeRegMack[i];		//
		}																						//
																								//
//		Запись одного значения в регистры даты и времени модуля:								//
		void	funcWriteTimeIndex(uint8_t i, uint8_t j){										//	(i = 0-секунды / 1-минуты / 2-часы / 3-день / 4-месяц / 5-год / 6-день недели, j = значение)
					varI=funcReadTimeIndex(i);													//	Читаем данные из регистра i
					j |= ~arrTimeRegMack[i] & varI;												//	Устанавливаем биты значения j по маске arrTimeRegMack[i] в прочитанные из регистра i
					j &=  arrTimeRegMack[i] | varI;												//	Сбрасываем    биты значения j по маске arrTimeRegMack[i] в прочитанные из регистра i
					funcWriteReg(arrTimeRegAddr[i], j);											//	Сохраняем значение j в регистр arrTimeRegAddr[i]
		}																						//
																								//
	private:																					//
	/**	Внутренние переменные **/																//
		uint8_t	pinRES					=	 0;													//	Определяем переменную для хранения номера вывода RST трехпроводной шины
		uint8_t	pinCLK					=	 0;													//	Определяем переменную для хранения номера вывода CLK трехпроводной шины
		uint8_t	pinDAT					=	 0;													//	Определяем переменную для хранения номера вывода DAT трехпроводной шины
		uint8_t	arrTimeRegAddr[7]		=	{0x81,0x83,0x85,0x87,0x89,0x8D,0x8B};				//	Определяем массив с адресами регистров чтения даты и времени		(сек, мин, час, день, месяц, год, день недели)
		uint8_t	arrTimeRegMack[7]		=	{0x7F,0x7F,0x3F,0x3F,0x1F,0xFF,0x07};				//	Определяем маскировочный массив для регистров даты и времени		(при чтении/записи, нужно совершить побитовое «и»)
		uint8_t	busRate					=	10;													//	Скорость передачи данных трехпроводной шине в кГц					(до 255 кГц)
		uint8_t	varI;																			//
																								//
	/**	Внутренние функции **/																	//
//		Функция чтения данных из регистра модуля:												//
		uint8_t	funcReadReg(uint8_t i){															//	Определяем функцию читения данных из регистра модуля				(аргумент: адрес_регистра)
					varI=1;																		//	Предустанавливаем переменную varI в значение 1, чтоб не вывести: 45 апреля 255 часов 127 минут и 200 секунд
					digitalWrite		(pinRES, 1);											//	Устанавливаем линию RES в активное состояние						
					funcWriteByte		(i);													//	Отправляем адрес регистра
					varI=funcReadByte	(true);													//	Читаем байт из регистра с учётом предустановленного бита
					digitalWrite		(pinRES, 0);											//	Устанавливаем линию RES в неактивное состояние
					return	varI;																//	Возвращаем значение переменной varI
		}																						//
//		Функция записи данных в регистр модуля:													//
		void	funcWriteReg(uint8_t i, uint8_t j){												//	Определяем функцию записи данных в регистр модуля					(аргументы: адрес_регистра, байт_данных)
					digitalWrite		(pinRES, 1);											//	Устанавливаем линию RES в активное состояние
					funcWriteByte		(i-1);													//	Отправляем адрес регистра											(при чтении указывается адрес-1)
					funcWriteByte		(j);													//	Отправляем байт данных
					digitalWrite		(pinRES, 0);											//	Устанавливаем линию RES в неактивное состояние
		}																						//
																								//
	/**	функции для работы с трехпроводной шиной **/											//
		void	funcWriteByte	(uint8_t j)														/*	Передача  одного байта		(байт для передачи)									*/	{uint8_t i=0, n=500/busRate+1; pinMode(pinDAT, OUTPUT); while(i>=0 && i<8){digitalWrite(pinDAT, (j & _BV(i))); delayMicroseconds(n); digitalWrite(pinCLK, 1); delayMicroseconds(n); digitalWrite(pinCLK, 0); i++;} pinMode(pinDAT, INPUT);}
		uint8_t	funcReadByte	(bool j)														/*	Получение одного байта		(флаг чтения предустановленного бита с линии DAT)	*/	{uint8_t i=0, k=0, n=500/busRate+1; pinMode(pinDAT, INPUT); if(j){if(digitalRead(pinDAT)){k |= _BV(i);} i++;} while(i>=0 && i<8){digitalWrite(pinCLK, 1); delayMicroseconds(n); digitalWrite(pinCLK, 0); delayMicroseconds(n); if(digitalRead(pinDAT)){k |= _BV(i);} i++;} return k;}
		void	funcBegin		(void)															/*	Подготовка выводов шины		(без параметров)									*/	{pinMode(pinRES, OUTPUT); pinMode(pinCLK, OUTPUT); pinMode(pinDAT, INPUT); digitalWrite(pinCLK, 0); digitalWrite(pinRES, 0);}
};																								//
																								//
#endif																							//