#ifndef iarduino_RTC_DS1307_h																	//
#define iarduino_RTC_DS1307_h																	//
#define	RTC_DS1307 2																			//	Модуль часов реального времени с протоколом передачи данных I2C, памятью 064x8 (56 байт которой доступны для хранения данных пользователя)
#include "iarduino_RTC_I2C.h"																	//	Подключаем файл iarduino_RTC_I2C.h - для работы с шиной I2C
																								//
class iarduino_RTC_DS1307: public iarduino_RTC_BASE{											//
	public:																						//
	/**	функции доступные пользователю **/														//
//		Инициализация модуля:																	//
		void	begin(void){																	//	(без параметров)
//					Инициализация работы с шиной I2C:											//
					objI2C.begin(100);															//	(скорость шины в кГц)
//					Установка флагов управления и состояния модуля:								//
					varI=objI2C.readByte(valAddress, 0x00); if( varI & 0b10000000                         ){objI2C.writeByte(valAddress, 0x00, (varI&~0b10000000)            );}	//		(если установлен 7 бит в 0 регистре, то сбрасываем его - запускаем генератор)
					varI=objI2C.readByte(valAddress, 0x02); if( varI & 0b01000000                         ){objI2C.writeByte(valAddress, 0x02, (varI&~0b01000000)            );}	//		(если установлен 6 бит в 2 регистре, то сбрасываем его - переводим модуль в 24 часовой режим)
					varI=objI2C.readByte(valAddress, 0x07); if((varI & 0b00000011) || !(varI & 0b00010000)){objI2C.writeByte(valAddress, 0x07, (varI&~0b00000011)|0b00010000 );}	//		(если установлены 1 и 0 биты или сброшен 4 бит в 7 регистре, то сбрасываем 1 с 0 битами, а 4 устанавливаем - выводим меандр с частотой 1 Гц на выводе SQW/OUT модуля)
		}																						//
																								//
//		Чтение одного значения из регистров даты и времени модуля:								//
		uint8_t	funcReadTimeIndex(uint8_t i){													//	(i = 0-секунды / 1-минуты / 2-часы / 3-день / 4-месяц / 5-год / 6-день недели)
					delay(1);																	//
					return objI2C.readByte(valAddress, arrTimeRegAddr[i]) & arrTimeRegMack[i];	//
		}																						//
																								//
//		Запись одного значения в регистры даты и времени модуля:								//
		void	funcWriteTimeIndex(uint8_t i, uint8_t j){										//	(i = 0-секунды / 1-минуты / 2-часы / 3-день / 4-месяц / 5-год / 6-день недели, j = значение)
					varI=funcReadTimeIndex(i);													//	Читаем данные из регистра i
					j |= ~arrTimeRegMack[i] & varI;												//	Устанавливаем биты значения j по маске arrTimeRegMack[i] в прочитанные из регистра i
					j &=  arrTimeRegMack[i] | varI;												//	Сбрасываем    биты значения j по маске arrTimeRegMack[i] в прочитанные из регистра i
					objI2C.writeByte(valAddress, arrTimeRegAddr[i], j);							//	Сохраняем значение j в регистр arrTimeRegAddr[i]
		}																						//
																								//
	private:																					//
	/**	Внутренние переменные **/																//
		iarduino_I2C objI2C;																	//	Создаём объект для работы с шиной I2C
		uint8_t	valAddress				=	 0x68;												//	Адрес модуля на шине I2C
		uint8_t	arrTimeRegAddr[7]		=	{0x00,0x01,0x02,0x04,0x05,0x06,0x03};				//	Определяем массив с адресами регистров даты и времени				(сек, мин, час, день, месяц, год, день недели)
		uint8_t	arrTimeRegMack[7]		=	{0x7F,0x7F,0x3F,0x3F,0x1F,0xFF,0x07};				//	Определяем маскировочный массив для регистров даты и времени		(при чтении/записи, нужно совершить побитовое «и»)
		uint8_t	varI;																			//
};																								//
																								//
#endif																							//