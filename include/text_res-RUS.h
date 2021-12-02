/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_JeeUI.

    FireLamp_JeeUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_JeeUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_JeeUI.  If not, see <https://www.gnu.org/licenses/>.

  (Этот файл — часть FireLamp_JeeUI.

   FireLamp_JeeUI - свободная программа: вы можете перераспространять ее и/или
   изменять ее на условиях Стандартной общественной лицензии GNU в том виде,
   в каком она была опубликована Фондом свободного программного обеспечения;
   либо версии 3 лицензии, либо (по вашему выбору) любой более поздней
   версии.

   FireLamp_JeeUI распространяется в надежде, что она будет полезной,
   но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА
   или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной
   общественной лицензии GNU.

   Вы должны были получить копию Стандартной общественной лицензии GNU
   вместе с этой программой. Если это не так, см.
   <https://www.gnu.org/licenses/>.)
*/

#ifndef __TEXT_RES_H
#define __TEXT_RES_H
#include <Arduino.h>

// Здесь строковые константы общего назначения, включая константы из интерфейса, что позволит локализировать при необходимости
// Ничего иного в данном файле не предполагается

/** набор строк с именами эффектов
 *  ссылки на эти строки собираются во флеш-массив в файле effect_types.h
 */
static const char TEFF_000[] PROGMEM = "";  // "пустышка"
static const char TEFF_001[] PROGMEM = "Белая лампа";
static const char TEFF_002[] PROGMEM = "Цвета";
static const char TEFF_003[] PROGMEM = "Радуга 2D";
static const char TEFF_004[] PROGMEM = "Конфетти";
static const char TEFF_005[] PROGMEM = "Цветные драже";
static const char TEFF_006[] PROGMEM = "Метель, Звездопад, Метеоры";
static const char TEFF_007[] PROGMEM = "New Матрица";
static const char TEFF_008[] PROGMEM = "ДНК Вариант";
static const char TEFF_009[] PROGMEM = "Светлячки со шлейфом";
static const char TEFF_010[] PROGMEM = "Блуждающий кубик";
static const char TEFF_011[] PROGMEM = "Пульс";
static const char TEFF_012[] PROGMEM = "Эффектопад";
static const char TEFF_013[] PROGMEM = "Огненная Лампа"; 
static const char TEFF_014[] PROGMEM = "Пейнтбол";
static const char TEFF_015[] PROGMEM = "Эффекты 3D-шума";
static const char TEFF_016[] PROGMEM = "Ёлки-Палки";
static const char TEFF_017[] PROGMEM = "Геометрический Вальс";
static const char TEFF_018[] PROGMEM = "Тикси Ленд";
static const char TEFF_019[] PROGMEM = "LDIRKO Ленд";
static const char TEFF_020[] PROGMEM = "Осцилятор";
static const char TEFF_021[] PROGMEM = "Шторм";
static const char TEFF_022[] PROGMEM = "Фея";
static const char TEFF_023[] PROGMEM = "Источник";
static const char TEFF_024[] PROGMEM = "Прыгающие мячики";
static const char TEFF_025[] PROGMEM = "Синусоид";
static const char TEFF_026[] PROGMEM = "Метасферы";
static const char TEFF_027[] PROGMEM = "Спираль";
static const char TEFF_028[] PROGMEM = "Кометы, Парящие Огни +";
static const char TEFF_029[] PROGMEM = "Бульбулятор";
static const char TEFF_030[] PROGMEM = "Призмата";
static const char TEFF_031[] PROGMEM = "Стая и Хищник";
static const char TEFF_032[] PROGMEM = "Водоворот";
static const char TEFF_033[] PROGMEM = "Дрифт";
static const char TEFF_034[] PROGMEM = "Попкорн";
static const char TEFF_035[] PROGMEM = "Мерцание";
static const char TEFF_036[] PROGMEM = "Радар";
static const char TEFF_037[] PROGMEM = "Волны";
static const char TEFF_038[] PROGMEM = "Огонь 2012";
static const char TEFF_039[] PROGMEM = "Бенгальские Огни"; 
static const char TEFF_040[] PROGMEM = "Шары"; 
static const char TEFF_041[] PROGMEM = "ДНК";
static const char TEFF_042[] PROGMEM = "Огонь 2018";
static const char TEFF_043[] PROGMEM = "Кодовый замок";
static const char TEFF_044[] PROGMEM = "Куб 2D";
static const char TEFF_045[] PROGMEM = "Магма";
static const char TEFF_046[] PROGMEM = "Пикассо";
static const char TEFF_047[] PROGMEM = "Звездный Десант";
static const char TEFF_048[] PROGMEM = "Флаги";
static const char TEFF_049[] PROGMEM = "Прыгуны";
static const char TEFF_050[] PROGMEM = "Вихри";
static const char TEFF_051[] PROGMEM = "Звезды";
static const char TEFF_052[] PROGMEM = "Блики на воде, Цвета";
static const char TEFF_053[] PROGMEM = "Фейерверк";
static const char TEFF_054[] PROGMEM = "Тихий Океан";
static const char TEFF_055[] PROGMEM = "Вышиванка";
static const char TEFF_056[] PROGMEM = "Цветной шум";
static const char TEFF_057[] PROGMEM = "Мотыльки";
static const char TEFF_058[] PROGMEM = "Тени";
static const char TEFF_059[] PROGMEM = "Узоры";
static const char TEFF_060[] PROGMEM = "Стрелки";
static const char TEFF_061[] PROGMEM = "Дикие шарики";
static const char TEFF_062[] PROGMEM = "Притяжение";
static const char TEFF_063[] PROGMEM = "Змейки";
static const char TEFF_064[] PROGMEM = "Nexus"; 
static const char TEFF_065[] PROGMEM = "Лабиринт";
static const char TEFF_066[] PROGMEM = "Вьющийся Цвет";
static const char TEFF_067[] PROGMEM = "Северное Сияние";
static const char TEFF_068[] PROGMEM = "Цветение";
static const char TEFF_069[] PROGMEM = "Змеиный Остров"; 
static const char TEFF_070[] PROGMEM = "Детские Сны";
static const char TEFF_071[] PROGMEM = "Меташары";
static const char TEFF_072[] PROGMEM = "Лавовая лампа";
static const char TEFF_073[] PROGMEM = "Космо-Гонщик";
static const char TEFF_074[] PROGMEM = "Дым";
static const char TEFF_075[] PROGMEM = "Огонь 2021";
static const char TEFF_076[] PROGMEM = "Мираж";
static const char TEFF_250[] PROGMEM = "Часы";
static const char TEFF_254[] PROGMEM = "Частотный анализатор";
static const char TEFF_255[] PROGMEM = "Осциллограф";

//-------------------------
#define DFTINTF_00A "Эффект"
#define DFTINTF_020 "Микрофон"
#define DFTINTF_084 "Палитра"
#define DFTINTF_088 "Масштаб"
#define DFTINTF_0D0 "Цвет"
#define DFTINTF_0D1 "Палитра/Масштаб"
#define DFTINTF_0D2 "Эффект (0 = циклично)"
#define DFTINTF_0D3 "Смещение цвета (0 = циклично)"
#define DFTINTF_0D4 "Cубпиксель"
#define DFTINTF_0D5 "Количество"
#define DFTINTF_0D6 "Генератор цвета"
#define DFTINTF_0D7 "Оттенок"
#define DFTINTF_0D8 "Жогово!"
#define DFTINTF_0D9 "Количество / Цвет(ночнник)"
#define DFTINTF_0DA "Насыщенность"
#define DFTINTF_0DB "Усиление"
#define DFTINTF_0DC "Развертка"
#define DFTINTF_0DD "Пульсация"
#define DFTINTF_0DE "Размытие"
#define DFTINTF_0DF "Порхание"
#define DFTINTF_0E0 "Ночник"
#define DFTINTF_0E1 "Кольца"
#define DFTINTF_0E2 "Блики (0:Откл.)"
#define DFTINTF_0E3 "Режим"
#define DFTINTF_0E4 "Линейная скорость"
#define DFTINTF_0E5 "Режим (1 = случайный)"
#define DFTINTF_0E6 "Смещение"
#define DFTINTF_0E7 "Количество/Направление/Положение"
#define DFTINTF_0E8 "Хищник"
#define DFTINTF_0E9 "Масса"
#define DFTINTF_0EA "Скорость (Верт.)"
#define DFTINTF_0EB "Скорость (Гориз.)"
#define DFTINTF_0EC "Узор (0 = случайный)"
#define DFTINTF_0ED "Затенение"
#define DFTINTF_0EE "Энергия"
#define DFTINTF_0EF "Плотность"
#define DFTINTF_0F0 "Цвет/Вращение"
#define DFTINTF_0F1 "Размер/Шлейф(1-85-170-255)"
#define DFTINTF_0F2 "Частота запусков"
#define DFTINTF_0F3 "Поворот"
#define DFTINTF_0F4 "Палитра (0 = генератор)"
#define DFTINTF_0F5 "Сглаживание"
#define DFTINTF_0F6 "Заполнение"
#define DFTINTF_0F7 "Толщина"
#define DFTINTF_0F9 "Поведение: 1/2"
#define DFTINTF_0FA "Цвет: 1/2"
#define DFTINTF_0FB "Палитра (0 = без цвета)"
#define DFTINTF_0FC "Цвет (1 = случайный, 255 = белый)"
#define DFTINTF_0FD "Снегопад/Звездопад/Метеоры"
#define DFTINTF_0FE "Кол-во"
#define DFTINTF_0FF "Тип 1/2, Размытие 3/4"
#define DFTINTF_100 "Палитра (0 = случайный цвет)"
#define DFTINTF_101 "Цвет 1 (0 = циклично)"
#define DFTINTF_102 "Тучка"
#define DFTINTF_103 "Размытие (0 = нет)"
#define DFTINTF_104 "Размах"
#define DFTINTF_105 "Фильтр"
#define DFTINTF_106 "Физика"
#define DFTINTF_107 "Круги"
#define DFTINTF_108 "Вариант 1/2"
#define DFTINTF_109 "Вспышки"
#define DFTINTF_10A "Глюк"
#define DFTINTF_10B "Шторм"
#define DFTINTF_10C "Количество снарядов"
#define DFTINTF_10D "Размер (ширина)"
#define DFTINTF_10E "Сдвиг цвета (0 = случайный)"
#define DFTINTF_10F "Размер (высота)"
#define DFTINTF_110 "Классика"
#define DFTINTF_111 "Один цвет"
#define DFTINTF_112 "Обратные Цвета"
#define DFTINTF_113 "Цвет 2 (0 = циклично)"
#define DFTINTF_114 "Качество/Скорость"
#define DFTINTF_115 "Размытие (1-4 с ветром)"
#define DFTINTF_116 "Перемещение по синусу"
#define DFTINTF_117 "Направление (0 = циклично)"
#define DFTINTF_118 "Порог сигнала"


/** набор строк с текстовыми константами интерфейса
 */
static const char TINTF_000[] PROGMEM = "Эффекты";
static const char TINTF_001[] PROGMEM = "Вывод текста";
static const char TINTF_002[] PROGMEM = "Настройки";
static const char TINTF_003[] PROGMEM = "В списке выбора";
static const char TINTF_004[] PROGMEM = "В списке демо";
static const char TINTF_005[] PROGMEM = "Копировать";
static const char TINTF_006[] PROGMEM = "Удалить";
static const char TINTF_007[] PROGMEM = "Обновить список эффектов";
static const char TINTF_008[] PROGMEM = "Сохранить";
static const char TINTF_009[] PROGMEM = "Управление списком эффектов";
static const char TINTF_00A[] PROGMEM = DFTINTF_00A;
static const char TINTF_00B[] PROGMEM = "Выход";
static const char TINTF_00C[] PROGMEM = "Глоб. яркость";
static const char TINTF_00D[] PROGMEM = "Яркость";
static const char TINTF_00E[] PROGMEM = "Включить";
static const char TINTF_00F[] PROGMEM = "Демо";
static const char TINTF_010[] PROGMEM = "Гл.Яркость";
static const char TINTF_011[] PROGMEM = "События";
static const char TINTF_012[] PROGMEM = "Микр.";
static const char TINTF_013[] PROGMEM = "Кнопка";
static const char TINTF_014[] PROGMEM = "Еще...";
static const char TINTF_015[] PROGMEM = "<<<";
static const char TINTF_016[] PROGMEM = ">>>";
static const char TINTF_017[] PROGMEM = "Ожидает...";
static const char TINTF_018[] PROGMEM = "Конфигурации";
static const char TINTF_019[] PROGMEM = "Загрузить";
static const char TINTF_01A[] PROGMEM = "Конфигурация";
static const char TINTF_01B[] PROGMEM = "Создать";
static const char TINTF_01C[] PROGMEM = "Вывести на лампе";
static const char TINTF_01D[] PROGMEM = "Текст";
static const char TINTF_01E[] PROGMEM = "Цвет сообщения";
static const char TINTF_01F[] PROGMEM = "Отправить";
static const char TINTF_020[] PROGMEM = DFTINTF_020;
static const char TINTF_021[] PROGMEM = "Микрофон активен";
static const char TINTF_022[] PROGMEM = "Коэф. коррекции нуля";
static const char TINTF_023[] PROGMEM = "Уровень шума, ед";
static const char TINTF_024[] PROGMEM = "Шумодав";
static const char TINTF_025[] PROGMEM = "Калибровка микрофона";
static const char TINTF_026[] PROGMEM = "Включите микрофон";
static const char TINTF_027[] PROGMEM = "... в процессе ...";
static const char TINTF_028[] PROGMEM = "WiFi";
static const char TINTF_029[] PROGMEM = "Клиент (STA)";
static const char TINTF_02A[] PROGMEM = "Настройки WiFi-клиента";
static const char TINTF_02B[] PROGMEM = "Имя лампы (mDNS Hostname/AP-SSID)";
static const char TINTF_02C[] PROGMEM = "WiFi SSID";
static const char TINTF_02D[] PROGMEM = "Password";
static const char TINTF_02E[] PROGMEM = "Подключиться";
static const char TINTF_02F[] PROGMEM = "Точка доступа (AP)";
static const char TINTF_030[] PROGMEM = "Отображение";
static const char TINTF_031[] PROGMEM = "Настройки WiFi и точки доступа";
static const char TINTF_032[] PROGMEM = "В режиме AP лампа всегда работает как точка доступа и не будет подключаться к другим WiFi-сетям, STA - режим клиента, AP+STA - смешанный";
static const char TINTF_033[] PROGMEM = "Режим WiFi";
static const char TINTF_034[] PROGMEM = "Защитить AP паролем";
static const char TINTF_035[] PROGMEM = "MQTT";
static const char TINTF_036[] PROGMEM = "MQTT host";
static const char TINTF_037[] PROGMEM = "MQTT port";
static const char TINTF_038[] PROGMEM = "User";
static const char TINTF_039[] PROGMEM = "Интервал mqtt, сек";
static const char TINTF_03A[] PROGMEM = "Подключение";
static const char TINTF_03B[] PROGMEM = "Отзеркаливание H";
static const char TINTF_03C[] PROGMEM = "Отзеркаливание V";
static const char TINTF_03D[] PROGMEM = "Плавное переключение эффектов";
static const char TINTF_03E[] PROGMEM = "Случайный эффект в Демо";
static const char TINTF_03F[] PROGMEM = "Смена эффекта в Демо, сек";
static const char TINTF_040[] PROGMEM = "Сортировка эффектов";
static const char TINTF_041[] PROGMEM = "Копии под оригинальным";
static const char TINTF_042[] PROGMEM = "Копии в конце";
static const char TINTF_043[] PROGMEM = "В порядке индекса";
static const char TINTF_044[] PROGMEM = "Скорость прокрутки текста";
static const char TINTF_045[] PROGMEM = "Смещение вывода текста";
static const char TINTF_046[] PROGMEM = "Смешанный (AP+STA)";
static const char TINTF_047[] PROGMEM = "Не выводить";
static const char TINTF_048[] PROGMEM = "Каждый час";
static const char TINTF_049[] PROGMEM = "Каждые полчаса";
static const char TINTF_04A[] PROGMEM = "Каждые 15 минут";
static const char TINTF_04B[] PROGMEM = "Каждые 10 минут";
static const char TINTF_04C[] PROGMEM = "Каждые 5 минут";
static const char TINTF_04D[] PROGMEM = "Каждую минуту";
static const char TINTF_04E[] PROGMEM = "Новогоднее поздравление";
static const char TINTF_04F[] PROGMEM = "Период вывода в минутах (0 - не выводить)";
static const char TINTF_050[] PROGMEM = "Дата/время нового года в формате YYYY-MM-DDThh:mm";
static const char TINTF_051[] PROGMEM = "Дата / Время / Часовая зона";
static const char TINTF_052[] PROGMEM = "Установки часовой зоны. Правила смены поясного/сезонного времени применяются автоматически, ручной коррекции не требуется. Если в вашей зоны нет в списке, можно выбрать общую зону сдвига от Гринвича";
static const char TINTF_053[] PROGMEM = "Часовая зона";
static const char TINTF_054[] PROGMEM = "Резервный NTP-сервер (не обязательно)";
static const char TINTF_055[] PROGMEM = "Дата/время в формате YYYY-MM-DDThh:mm:ss (без интернета, если пусто - время с устройства)";
static const char TINTF_056[] PROGMEM = "Обновление";
static const char TINTF_057[] PROGMEM = "Обновление по ОТА-PIO";
static const char TINTF_058[] PROGMEM = "Начать";
static const char TINTF_059[] PROGMEM = "Загрузка прошивки";
static const char TINTF_05A[] PROGMEM = "Upload";
static const char TINTF_05B[] PROGMEM = "Событие";
static const char TINTF_05C[] PROGMEM = "Редактировать";
static const char TINTF_05D[] PROGMEM = "Добавить";
static const char TINTF_05E[] PROGMEM = "Активно";
static const char TINTF_05F[] PROGMEM = "Тип события";
static const char TINTF_060[] PROGMEM = "Включить лампу";
static const char TINTF_061[] PROGMEM = "Выключить лампу";
static const char TINTF_062[] PROGMEM = "Включить DEMO";
static const char TINTF_063[] PROGMEM = "Будильник";
static const char TINTF_064[] PROGMEM = "Загрузка конф. лампы";
static const char TINTF_065[] PROGMEM = "Загрузка конф. эффектов";
static const char TINTF_066[] PROGMEM = "Загрузка конф. событий";
static const char TINTF_067[] PROGMEM = "Вывести текст";
static const char TINTF_068[] PROGMEM = "Вывести время";
static const char TINTF_069[] PROGMEM = "Состояние пина";
static const char TINTF_06A[] PROGMEM = "Включить AUX";
static const char TINTF_06B[] PROGMEM = "Выключить AUX";
static const char TINTF_06C[] PROGMEM = "Переключить AUX";
static const char TINTF_06D[] PROGMEM = "Дата/время события";
static const char TINTF_06E[] PROGMEM = "Повтор, мин";
static const char TINTF_06F[] PROGMEM = "Останов через, мин";
static const char TINTF_070[] PROGMEM = "Параметр (текст)";
static const char TINTF_071[] PROGMEM = "Повтор, дни недели";
static const char TINTF_072[] PROGMEM = "Понедельник";
static const char TINTF_073[] PROGMEM = "Вторник";
static const char TINTF_074[] PROGMEM = "Среда";
static const char TINTF_075[] PROGMEM = "Четверг";
static const char TINTF_076[] PROGMEM = "Пятница";
static const char TINTF_077[] PROGMEM = "Суббота";
static const char TINTF_078[] PROGMEM = "Воскресенье";
static const char TINTF_079[] PROGMEM = "Обновить";
static const char TINTF_07A[] PROGMEM = "Действие";
static const char TINTF_07B[] PROGMEM = "Кнопка активна";
static const char TINTF_07C[] PROGMEM = "OFF/ON";
static const char TINTF_07D[] PROGMEM = "Удержание";
static const char TINTF_07E[] PROGMEM = "Нажатия";
static const char TINTF_07F[] PROGMEM = "Однократно";
static const char TINTF_080[] PROGMEM = "Огненная лампа";
#if defined(EMBUI_USE_FTP) && defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi, MQTT & FTP";
#elif defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi & MQTT";
#elif defined(EMBUI_USE_FTP)
static const char TINTF_081[] PROGMEM = "WiFi & FTP";
#else
static const char TINTF_081[] PROGMEM = "WiFi";
#endif
static const char TINTF_082[] PROGMEM = "Другие";
static const char TINTF_083[] PROGMEM = "Очищать лампу при смене эффектов";
static const char TINTF_084[] PROGMEM = DFTINTF_084;
static const char TINTF_085[] PROGMEM = "В алфавитном порядке (внутренние)";
static const char TINTF_086[] PROGMEM = "События активны";
static const char TINTF_087[] PROGMEM = "Скорость";
static const char TINTF_088[] PROGMEM = DFTINTF_088;
static const char TINTF_089[] PROGMEM = "Имя эффекта";
static const char TINTF_08A[] PROGMEM = "В алфавитном порядке (конфигурация)";
static const char TINTF_08B[] PROGMEM = "Сброс установок эффекта";
static const char TINTF_08C[] PROGMEM = "Префикс топиков";
static const char TINTF_08D[] PROGMEM = "Сортировка по микрофону";
static const char TINTF_08E[] PROGMEM = "Отладка";
static const char TINTF_08F[] PROGMEM = "Настройки ESP";
static const char TINTF_090[] PROGMEM = "Нумерация в списке эффектов";
static const char TINTF_091[] PROGMEM = "Символ микрофона в списке";
static const char TINTF_092[] PROGMEM = "Настройка пинов";
static const char TINTF_093[] PROGMEM = "Показывать системное меню";
static const char TINTF_094[] PROGMEM = "Пин кнопки";
static const char TINTF_095[] PROGMEM = "Лимит по току (mA)";
static const char TINTF_096[] PROGMEM = "Перезагрузка";
static const char TINTF_097[] PROGMEM = "Пин RX плеера";
static const char TINTF_098[] PROGMEM = "Пин TX плеера";
static const char TINTF_099[] PROGMEM = "DFPlayer";
static const char TINTF_09A[] PROGMEM = "Название эффекта в Демо";
static const char TINTF_09B[] PROGMEM = "Громкость";
static const char TINTF_09C[] PROGMEM = "Время";
static const char TINTF_09D[] PROGMEM = "Имя эффекта";
static const char TINTF_09E[] PROGMEM = "Эффект(сброс)";
static const char TINTF_09F[] PROGMEM = "Отключен";
static const char TINTF_0A0[] PROGMEM = "Первый";
static const char TINTF_0A1[] PROGMEM = "Случайный";
static const char TINTF_0A2[] PROGMEM = "Случайный MP3";
static const char TINTF_0A3[] PROGMEM = "Звук будильника";
static const char TINTF_0A4[] PROGMEM = "Второй";
static const char TINTF_0A5[] PROGMEM = "Третий";
static const char TINTF_0A6[] PROGMEM = "Четвертый";
static const char TINTF_0A7[] PROGMEM = "Пятый";
static const char TINTF_0A8[] PROGMEM = "Эквалайзер";
static const char TINTF_0A9[] PROGMEM = "Номальный";
static const char TINTF_0AA[] PROGMEM = "Поп";
static const char TINTF_0AB[] PROGMEM = "Рок";
static const char TINTF_0AC[] PROGMEM = "Джаз";
static const char TINTF_0AD[] PROGMEM = "Классика";
static const char TINTF_0AE[] PROGMEM = "Бас";
static const char TINTF_0AF[] PROGMEM = "MP3 плеер";
static const char TINTF_0B0[] PROGMEM = "Кол-во файлов в папке MP3";
static const char TINTF_0B1[] PROGMEM = "Режимы озвучивания";
static const char TINTF_0B2[] PROGMEM = "Папка\\номер звукового файла (примеры: MP3\\17, 5\\19)";
static const char TINTF_0B3[] PROGMEM = "Ограничить громкость будильника";
static const char TINTF_0B4[] PROGMEM = "По умолчанию";
static const char TINTF_0B5[] PROGMEM = "Удалить из списка";
static const char TINTF_0B6[] PROGMEM = "Отключено";
static const char TINTF_0B7[] PROGMEM = "Вариант 1";
static const char TINTF_0B8[] PROGMEM = "Вариант 2";
static const char TINTF_0B9[] PROGMEM = "Параметр";
static const char TINTF_0BA[] PROGMEM = "Будильник Рассвет";
static const char TINTF_0BB[] PROGMEM = "Длительность рассвета, мин";
static const char TINTF_0BC[] PROGMEM = "Светить после рассвета, мин";
static const char TINTF_0BD[] PROGMEM = "<";
static const char TINTF_0BE[] PROGMEM = ">";
static const char TINTF_0BF[] PROGMEM = "<+5";
static const char TINTF_0C0[] PROGMEM = "+5>";
static const char TINTF_0C1[] PROGMEM = "секунд";
static const char TINTF_0C2[] PROGMEM = "минут";
static const char TINTF_0C3[] PROGMEM = "часов";
static const char TINTF_0C4[] PROGMEM = "дней";
static const char TINTF_0C5[] PROGMEM = "дня";
static const char TINTF_0C6[] PROGMEM = "день";
static const char TINTF_0C7[] PROGMEM = "часа";
static const char TINTF_0C8[] PROGMEM = "час";
static const char TINTF_0C9[] PROGMEM = "Случ.";
static const char TINTF_0CA[] PROGMEM = "Затемнение фона";
static const char TINTF_0CB[] PROGMEM = "Предупреждение";
static const char TINTF_0CC[] PROGMEM = "минуты";
static const char TINTF_0CD[] PROGMEM = "минута";
static const char TINTF_0CE[] PROGMEM = "Рисование";
static const char TINTF_0CF[] PROGMEM = "Заливка";
static const char TINTF_0D0[] PROGMEM = DFTINTF_0D0;
static const char TINTF_0D1[] PROGMEM = "Звук вначале";
static const char TINTF_0D2[] PROGMEM = "Ограничить громкость";
static const char TINTF_0D3[] PROGMEM = "Коэф. скорости (1.0 по умолчанию)";
static const char TINTF_0D4[] PROGMEM = "Настройки дисплея";
static const char TINTF_0D5[] PROGMEM = "Яркость (0 - выключить)";
static const char TINTF_0D6[] PROGMEM = "Яркость с выкл. лампой (0 - выключить)";
static const char TINTF_0D7[] PROGMEM = "24х часовой формат";
static const char TINTF_0D8[] PROGMEM = "Ведущий 0";
static const char TINTF_0D9[] PROGMEM = "Очистить";
static const char TINTF_0DA[] PROGMEM = "Сканировать";
static const char TINTF_0DB[] PROGMEM = "FTP";
static const char TINTF_0DC[] PROGMEM = "Управление";
static const char TINTF_0DD[] PROGMEM = "Верт.шкала";
static const char TINTF_0DE[] PROGMEM = "Цвет шкалы";
static const char TINTF_0DF[] PROGMEM = "Цвет текста";
static const char TINTF_0E0[] PROGMEM = "Отображение температуры";
static const char TINTF_0E1[] PROGMEM = "Уверены?";
static const char TINTF_0E2[] PROGMEM = "Трансляция";
static const char TINTF_0E3[] PROGMEM = "Режим";
static const char TINTF_0E4[] PROGMEM = "E1.31";
static const char TINTF_0E5[] PROGMEM = "SoulMate";
static const char TINTF_0E6[] PROGMEM = "Блок.Эффект";
static const char TINTF_0E7[] PROGMEM = "Разметка";
static const char TINTF_0E8[] PROGMEM = "Начальный Universe";

static const char NY_MDG_STRING1[] PROGMEM = "До нового года осталось %d %s";
static const char NY_MDG_STRING2[] PROGMEM = "C новым %d годом!";
//-------------------------

#endif