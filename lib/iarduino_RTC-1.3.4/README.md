
[![](https://iarduino.ru/img/logo.svg)](https://iarduino.ru)[![](https://wiki.iarduino.ru/img/git-shop.svg?3)](https://iarduino.ru) [![](https://wiki.iarduino.ru/img/git-wiki.svg?2)](https://wiki.iarduino.ru) [![](https://wiki.iarduino.ru/img/git-lesson.svg?2)](https://lesson.iarduino.ru)[![](https://wiki.iarduino.ru/img/git-forum.svg?2)](http://forum.trema.ru)

# iarduino_RTC [СКАЧАТЬ](https://github.com/tremaru/iarduino_RTC/archive/1.2.1.zip)
Библиотека позволяет читать и записывать время RTC модулей на базе чипов: DS1302, [DS1307](http://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul.html),  [DS3231](http://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul-v2-0.html), ...  
Преимуществом данной библиотеки является удобная реализация получения времени.
> Подробнее про установку библиотеки читайте в нашей [инструкции](https://wiki.iarduino.ru/page/Installing_libraries/).

Пример подключения к [Arduino](https://iarduino.ru/shop/boards/arduino-uno-r3.html)/[Piranha UNO](https://iarduino.ru/shop/boards/piranha-uno-r3.html) с помощью [Trema Shield](https://iarduino.ru/shop/Expansion-payments/trema-shield.html)
Подробнее о [подключении модуля](https://wiki.iarduino.ru/page/chasy-realnogo-vremeni-rtc-trema-modul/) 
![enter image description here](https://iarduino.ru/img/upload/6a83fe7f3deb2f48d1408daac49284ba.png)

| Модель | Ссылка на магазин|
|--|--|
| DS1307 ![enter image description here](https://wiki.iarduino.ru/img/resources/643/643.svg) | https://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul.html|
| DS3231 ![enter image description here](https://wiki.iarduino.ru/img/resources/877/877.svg) | https://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul-v2-0.html |



## Описание библиотеки:
Библиотека позволяет читать и записывать время RTC модулей на базе чипов: DS1302, [DS1307](http://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul.html), [DS3231](http://iarduino.ru/shop/Expansion-payments/chasy-realnogo-vremeni-rtc-trema-modul-v2-0.html), …  
Преимуществом данной библиотеки является удобная реализация получения времени.
**[  Подробное описание, подключение и примеры работы ](https://wiki.iarduino.ru/page/chasy-realnogo-vremeni-rtc-trema-modul/)**

## Назначение функций и переменных:
Подробное описание работы с библиотекой, находится в разделе  https://wiki.iarduino.ru/page/chasy-realnogo-vremeni-rtc-trema-modul/

**Подключаем библиотеку.**  

    #include  <iarduino_RTC.h>  // Подключаем библиотеку.  

 **Создаём объект.**
 

    iarduino_RTC  ОБЪЕКТ ( НАЗВАНИЕ [, ВЫВОД_RST [, ВЫВОД_CLK [, ВЫВОД_DAT ]]]  );  // Создаём объект.

**Инициализация работы RTC модуля.**

    Функция  begin();  // Инициализация работы RTC модуля.

 **Установка времени.**

    Функция  settime( СЕК [, МИН [, ЧАС [, ДЕНЬ [, МЕС [, ГОД [, ДН ]]]]]]  );  // Установка времени.

 **Чтение времени.**

    Функция  gettime( [ СТРОКА ]  );  // Чтение времени.

**Заставляет функцию gettime «мигать» указанным параметром времени.**

    функция  blinktime( ПАРАМЕТР [ ЧАСТОТА ]  );  // Заставляет функцию gettime «мигать» указанным параметром времени.

**Указывает минимальный период обращения к модулю в минутах.**

    функция  period( МИНУТЫ  );  // Указывает минимальный период обращения к модулю в минутах.

**Возвращает секунды от 0 до 59.**

    Переменная  seconds // Возвращает секунды от 0 до 59.

 **Возвращает минуты от 0 до 59.**

    Переменная  minutes // Возвращает минуты от 0 до 59.

**Возвращает часы от 1 до 12.**

    Переменная  hours // Возвращает часы от 1 до 12.

**Возвращает часы от 0 до 23.**

    Переменная  Hours // Возвращает часы от 0 до 23.

**Возвращает полдень 0 или 1 (0-am, 1-pm).**

    Переменная  midday // Возвращает полдень 0 или 1 (0-am, 1-pm).

**Возвращает день месяца от 1 до 31.**

    Переменная  day // Возвращает день месяца от 1 до 31.

**Возвращает день недели от 0 до 6 (0-воскресенье, 6-суббота).**

    Переменная   weekday // Возвращает день недели от 0 до 6 (0-воскресенье, 6-суббота).

**Возвращает месяц от 1 до 12.**

    Переменная  month // Возвращает месяц от 1 до 12.

**Возвращает год от 0 до 99.**

    Переменная  year // Возвращает год от 0 до 99.













