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
static const char TEFF_008[] PROGMEM = "Светлячки";
static const char TEFF_009[] PROGMEM = "Светлячки со шлейфом";
static const char TEFF_010[] PROGMEM = "Блуждающий кубик";
static const char TEFF_011[] PROGMEM = "Пульс";
static const char TEFF_012[] PROGMEM = "Эффектопад";
static const char TEFF_013[] PROGMEM = "Огненная Лампа"; 
static const char TEFF_014[] PROGMEM = "Пейнтбол";
static const char TEFF_015[] PROGMEM = "Эффекты 3D-шума";
static const char TEFF_016[] PROGMEM = "Клеточки";
static const char TEFF_017[] PROGMEM = "Геометрический Вальс";
static const char TEFF_018[] PROGMEM = "Тикси Ленд";
static const char TEFF_019[] PROGMEM = "LDIRKO Ленд";
static const char TEFF_020[] PROGMEM = "Осцилятор";
static const char TEFF_021[] PROGMEM = "Шторм";
static const char TEFF_022[] PROGMEM = "";
static const char TEFF_023[] PROGMEM = "";
static const char TEFF_024[] PROGMEM = "Прыгающие мячики";
static const char TEFF_025[] PROGMEM = "Синусоид 3";
static const char TEFF_026[] PROGMEM = "Метасферы";
static const char TEFF_027[] PROGMEM = "Спираль";
static const char TEFF_028[] PROGMEM = "Кометы, Парящие Огни +";
static const char TEFF_029[] PROGMEM = "";
static const char TEFF_030[] PROGMEM = "Призмата";
static const char TEFF_031[] PROGMEM = "Стая и Хищник";
static const char TEFF_032[] PROGMEM = "Водоворот";
static const char TEFF_033[] PROGMEM = "Дрифт";
static const char TEFF_034[] PROGMEM = "Попкорн";
static const char TEFF_035[] PROGMEM = "Мерцание";
static const char TEFF_036[] PROGMEM = "Радар";
static const char TEFF_037[] PROGMEM = "Волны";
static const char TEFF_038[] PROGMEM = "Огонь 2012";
static const char TEFF_039[] PROGMEM = ""; 
static const char TEFF_040[] PROGMEM = ""; 
static const char TEFF_041[] PROGMEM = "ДНК";
static const char TEFF_042[] PROGMEM = "Огонь 2018";
static const char TEFF_043[] PROGMEM = "Кодовый замок";
static const char TEFF_044[] PROGMEM = "Куб 2D";
static const char TEFF_045[] PROGMEM = "Дым";
static const char TEFF_046[] PROGMEM = "Пикассо";
static const char TEFF_047[] PROGMEM = "Пикассо2";
static const char TEFF_048[] PROGMEM = "Пикассо3";
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
static const char TEFF_064[] PROGMEM = "Nexus"; //"Змеиный Остров";
static const char TEFF_065[] PROGMEM = "";
static const char TEFF_066[] PROGMEM = "";
static const char TEFF_067[] PROGMEM = "";
static const char TEFF_068[] PROGMEM = "Цветение";
static const char TEFF_069[] PROGMEM = "Змеиный Остров"; //"Змейки от Сотнег";
static const char TEFF_070[] PROGMEM = "Детские Сны";
static const char TEFF_071[] PROGMEM = "Меташары";
static const char TEFF_072[] PROGMEM = "Лавовая лампа";
static const char TEFF_253[] PROGMEM = "Часы";
static const char TEFF_254[] PROGMEM = "Частотный анализатор";
static const char TEFF_255[] PROGMEM = "Осциллограф";

//-------------------------

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
static const char TINTF_00A[] PROGMEM = "Эффект";
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
static const char TINTF_020[] PROGMEM = "Микрофон";
static const char TINTF_021[] PROGMEM = "Микрофон активен";
static const char TINTF_022[] PROGMEM = "Коэф. коррекции нуля";
static const char TINTF_023[] PROGMEM = "Уровень шума, ед";
static const char TINTF_024[] PROGMEM = "Шумодав";
static const char TINTF_025[] PROGMEM = "Калибровка микрофона";
static const char TINTF_026[] PROGMEM = "Включите микрофон";
static const char TINTF_027[] PROGMEM = "... в процессе ...";
static const char TINTF_028[] PROGMEM = "WiFi";
static const char TINTF_029[] PROGMEM = "WiFi Client";
static const char TINTF_02A[] PROGMEM = "Настройки WiFi-клиента";
static const char TINTF_02B[] PROGMEM = "Имя лампы (mDNS Hostname/AP-SSID)";
static const char TINTF_02C[] PROGMEM = "WiFi SSID";
static const char TINTF_02D[] PROGMEM = "Password";
static const char TINTF_02E[] PROGMEM = "Подключиться";
static const char TINTF_02F[] PROGMEM = "WiFi AP";
static const char TINTF_030[] PROGMEM = "Отображение";
static const char TINTF_031[] PROGMEM = "Настройки WiFi-точки доступа";
static const char TINTF_032[] PROGMEM = "В режиме AP-only лампа всегда работает как точка доступа и не будет подключаться к другим WiFi-сетям";
static const char TINTF_033[] PROGMEM = "Режим AP-only";
static const char TINTF_034[] PROGMEM = "Защитить AP паролем";
static const char TINTF_035[] PROGMEM = "MQTT";
static const char TINTF_036[] PROGMEM = "MQTT host";
static const char TINTF_037[] PROGMEM = "MQTT port";
static const char TINTF_038[] PROGMEM = "User";
static const char TINTF_039[] PROGMEM = "Интервал mqtt сек.";
static const char TINTF_03A[] PROGMEM = "Connect";
static const char TINTF_03B[] PROGMEM = "Отзеркаливание H";
static const char TINTF_03C[] PROGMEM = "Отзеркаливание V";
static const char TINTF_03D[] PROGMEM = "Плавное переключение эффектов";
static const char TINTF_03E[] PROGMEM = "Случайный эффект в Демо";
static const char TINTF_03F[] PROGMEM = "Смена эффекта в Демо. (сек)";
static const char TINTF_040[] PROGMEM = "Сортировка эффектов";
static const char TINTF_041[] PROGMEM = "Копии под оригинальным";
static const char TINTF_042[] PROGMEM = "Копии в конце";
static const char TINTF_043[] PROGMEM = "В порядке индекса";
static const char TINTF_044[] PROGMEM = "Задержка прокрутки текста";
static const char TINTF_045[] PROGMEM = "Смещение вывода текста";
static const char TINTF_046[] PROGMEM = "Периодический вывод времени";
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
static const char TINTF_051[] PROGMEM = "Время";
static const char TINTF_052[] PROGMEM = "Правила TZSET учета поясного/сезонного времени (напр 'MSK-3' для Europe/Moscow) Нужную строку можно взять тут https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h";
static const char TINTF_053[] PROGMEM = "правило TZone (рекоммендуется задать!)";
static const char TINTF_054[] PROGMEM = "резервный NTP-сервер (не обязательно)";
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
static const char TINTF_06F[] PROGMEM = "Остановить через, мин";
static const char TINTF_070[] PROGMEM = "Параметр (текст)";
static const char TINTF_071[] PROGMEM = "Повтор";
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
static const char TINTF_081[] PROGMEM = "WiFi & MQTT";
static const char TINTF_082[] PROGMEM = "Другие";
static const char TINTF_083[] PROGMEM = "Очищать лампу при смене эффектов";
static const char TINTF_084[] PROGMEM = "Палитра";
static const char TINTF_085[] PROGMEM = "В алфавитном порядке (внутренние)";
static const char TINTF_086[] PROGMEM = "События активны";
static const char TINTF_087[] PROGMEM = "Скорость";
static const char TINTF_088[] PROGMEM = "Масштаб";
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
static const char TINTF_0B4[] PROGMEM = "Удалить все";
static const char TINTF_0B5[] PROGMEM = "Удалить из списка";
static const char TINTF_0B6[] PROGMEM = "Отключено";
static const char TINTF_0B7[] PROGMEM = "Вариант 1";
static const char TINTF_0B8[] PROGMEM = "Вариант 2";
static const char TINTF_0B9[] PROGMEM = "Параметр";
static const char TINTF_0BA[] PROGMEM = "Будильник Рассвет";
static const char TINTF_0BB[] PROGMEM = "Длительность рассвета";
static const char TINTF_0BC[] PROGMEM = "Длительность свечения после рассвета";
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
static const char TINTF_0C7[] PROGMEM = "";
static const char TINTF_0C8[] PROGMEM = "";
static const char TINTF_0C9[] PROGMEM = "";
static const char TINTF_0CA[] PROGMEM = "";
static const char TINTF_0CB[] PROGMEM = "";
static const char TINTF_0CC[] PROGMEM = "";
static const char TINTF_0CD[] PROGMEM = "";
static const char TINTF_0CE[] PROGMEM = "";
static const char TINTF_0CF[] PROGMEM = "";
static const char TINTF_0D0[] PROGMEM = "";


//-------------------------

/**
 * Набор статических строк, определяющих UI эффекта если он отличается от базового
 *
 *  GUI для эффекта по-умолчанию
 * Полный формат для пользовательского (id=3...7) параметра имеет вид: {\"id\":3,\"type\":0,\"val\":127,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Параметр\"}
 * @nb@ - будет заменен на реальный номер эффекта, @name@ - на дефолтное имя эффекта, @ver@ - версия, @pal@ - максимальная палитра
 * https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/post-48813
 * https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/post-48848
 */

// Дефолтные контролы
static const char E_DEFUI[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"},{\"id\":2,\"val\":\"127\"}]}";
// Добавил ко всем конфигам чекбокс "Микрофон" с индексом 7. Ввиду модифированного поведения функции myLamp.isMicOnOff(), он будет появляться только если глобально
// микрофон включен, и эффект обладает зависимостью от микрофона в принципе.
static const char E_DEFMICUI_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"},{\"id\":2,\"val\":\"127\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
// Общая конфигурация для эффектов с 3-им ползунком для палитр
static const char E_3PAL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_3PAL_MIC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_3PAL255_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Палитра/Масштаб\"}]}";
// Общая конфигурация для эффектов только с 2-мя ползунками "Яркость" и "Скорость", пример - эффект"Тихий Океан"
static const char E_2_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":48,\"val\":\"127\"}]}"; // 3*16+0 для 2 контрола
static const char E_2_MIC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":48,\"val\":\"127\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}"; // 3*16+0 для 2 контрола

// Общая конфигурация для эффектов с 4-им ползунком для палитр
static const char E_4PAL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_LLEND[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":0,\"min\":0,\"max\":10,\"step\":1,\"name\":\"Эффект (0 = циклично)\"}, {\"id\":3,\"type\":0,\"val\":0,\"min\":0,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}, {\"id\":4,\"type\":0,\"val\":16,\"min\":0,\"max\":32,\"step\":1,\"name\":\"Смещение цвета (0 = циклично)\"}]}";
static const char E_PILE[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":4,\"min\":1,\"max\":8,\"step\":1,\"name\":\"Заполнение (не моментально)\"}, {\"id\":1,\"type\":48,\"val\":\"127\"} ,{\"id\":2,\"type\":48,\"val\":\"127\"}, {\"id\":4,\"type\":0,\"val\":7,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_SPARCLES_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":16,\"val\":1,\"min\":1,\"max\":10,\"step\":1,\"name\":\"Цвет\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_LIGHTERS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":16,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Количество\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Cубпиксель\"}]}";
static const char E_RADAR255_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Палитра/Масштаб\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Cубпиксель\"}]}";
static const char E_WHITE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Оттенок\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_LEAPERS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":50,\"step\":1,\"name\":\"Жогово!\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_BUTTERFLY_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":64,\"step\":1,\"name\":\"Количество / Цвет(ночнник)\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Порхание\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Ночник\"}]}";
static const char E_PULS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Генератор цвета\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_BBALLS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Масштаб\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Кольца\"}]}";
static const char E_PRIZMATA_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"},{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Размытие\"}]}";
static const char E_AQUARIUM_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Насыщенность\"}, {\"id\":4,\"type\":0,\"val\":1,\"min\":0,\"max\":15,\"step\":1,\"name\":\"Блики (0:Откл. 1:Случ.)\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_FREQ_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":127,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Усиление\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}, {\"id\":4,\"type\":0,\"val\":2,\"min\":1,\"max\":3,\"step\":1,\"name\":\"Режим\"}]}";
static const char E_OSC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Развертка\"},{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Пульсация\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Уcиление\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
// размерность ползунка "Узор" должна быть MAX_PATTERN + 1 (patterns.h). При добавлении паттернов - менять и тут.
static const char E_PATT_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":33,\"min\":1,\"max\":65,\"step\":1,\"name\":\"Скорость (Верт.)\"}, {\"id\":2,\"type\":0,\"val\":33,\"min\":1,\"max\":65,\"step\":1,\"name\":\"Скорость (Гориз.)\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":39,\"step\":1,\"name\":\"Узор (1 = случайный)\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Cубпиксель\"}]}";
static const char E_SHAD_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Скорость\"}, {\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Затенение\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Линейная скорость\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_ARR_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":6,\"step\":1,\"name\":\"Режим (1 = случайный)\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Cубпиксель\"}]}";
static const char E_F2018_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Оттенок\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Линейная скорость\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_NBAL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"}, {\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":25,\"step\":1,\"name\":\"Смещение\"}]}";
static const char E_ATTRACT_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":100,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Энергия\"}, {\"id\":3,\"type\":0,\"val\":100,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Масса\"}, {\"id\":4,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_FLOCK_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Хищник\"}]}";
static const char E_WAVES_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":4,\"min\":1,\"max\":8,\"step\":1,\"name\":\"Количество/Направление/Положение\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_MUNCH_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}, {\"id\":3,\"type\":0,\"val\":4,\"min\":0,\"max\":8,\"step\":1,\"name\":\"Сдвиг цвета (0 = случайный)\"} ,{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
//static const char E_COM_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет (1-127 случайный)\"}]}";
static const char E_DRIFT_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":4,\"step\":1,\"name\":\"Тип 1/2, Размытие 3/4\"}]}";
static const char E_SMOKE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет/Вращение\"},{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":6,\"step\":1,\"name\":\"Заполнение\"}]}";
static const char E_CUBE2D_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":0,\"max\":22,\"step\":1,\"name\":\"Палитра (0 = случайный цвет)\"},{\"id\":3,\"type\":0,\"val\":4,\"min\":1,\"max\":7,\"step\":1,\"name\":\"Размер (ширина)\"},{\"id\":4,\"type\":0,\"val\":4,\"min\":1,\"max\":7,\"step\":1,\"name\":\"Размер (высота)\"} ,{\"id\":5,\"type\":2,\"val\":\"false\",\"name\":\"Классика\"}]}";
static const char E_FLAMP_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет\"}]}";
static const char E_LIGHT2_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":16,\"step\":1,\"name\":\"Количество\"}]}";
static const char E_CUBE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Размер/Шлейф(1-85-170-255)\"}]}";
static const char E_STARFAIL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":5,\"min\":1,\"max\":10,\"step\":1,\"name\":\"Плотность\"} , {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":3,\"step\":1,\"name\":\"Снегопад/Звездопад/Метеоры\"} ,{\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Вариант 1/2\"}]}"; 
static const char E_SNAKE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}, {\"id\":3,\"type\":0,\"val\":4,\"min\":1,\"max\":16,\"step\":1,\"name\":\"Кол-во\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Cубпиксель\"}, {\"id\":5,\"type\":2,\"val\":\"true\",\"name\":\"Один цвет\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_NEXUS[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":5,\"min\":1,\"max\":10,\"step\":1,\"name\":\"Плотность\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_FWORK_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Частота запусков\"}, {\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":8,\"step\":1,\"name\":\"Количество снарядов\"}, {\"id\":3,\"type\":2,\"val\":\"false\",\"name\":\"Вспышки\"}]}";
static const char E_MATRIX[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":12,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Плотность\"}, {\"id\":3,\"type\":0,\"val\":90,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет (1 = случайный, 255 = белый)\"}, {\"id\":4,\"type\":0,\"val\":20,\"min\":1,\"max\":60,\"step\":1,\"name\":\"Глюк\"}]}";
static const char E_RAIN_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":25,\"min\":1,\"max\":45,\"step\":1,\"name\":\"Плотность\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Тучка\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Шторм\"}]}";
static const char E_FIRE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":5,\"type\":0,\"val\":1,\"min\":1,\"max\":3,\"step\":1,\"name\":\"Вид задержки (текущая/dryrun/delay)\"}]}";
static const char E_NFIRE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":50,\"min\":1,\"max\":100,\"step\":1,\"name\":\"Масштаб\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":10,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_POPCORN_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":16,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Плотность\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Размытие\"}, {\"id\":5,\"type\":2,\"val\":\"true\",\"name\":\"Обратные Цвета\"}]}";
static const char E_MBL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":0,\"max\":@pal@,\"step\":1,\"name\":\"Палитра (0 = генератор)\"}, {\"id\":4,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"step\":1,\"name\":\"Генератор цвета\"}]}";
static const char E_LIQLAM_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":0,\"max\":18,\"step\":1,\"name\":\"Палитра (0 = генератор)\"}, {\"id\":4,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"step\":1,\"name\":\"Генератор цвета\"}, {\"id\":5,\"type\":0,\"val\":0,\"min\":0,\"max\":4,\"step\":1,\"name\":\"Фильтр\"}, {\"id\":6,\"type\":2,\"val\":\"true\",\"name\":\"Физика\"}]}";
static const char E_F2012_MIC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":64,\"min\":1,\"max\":128,\"step\":1,\"name\":\"Масштаб\"},{\"id\":3,\"type\":0,\"val\":6,\"min\":1,\"max\":10,\"step\":1,\"name\":\"Палитра\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_DNA_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":48,\"val\":\"127\"},{\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Поворот\"}]}";
static const char E_SNOW_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"val\":\"1\",\"min\":1,\"max\":3,\"name\":\"Сглаживание\"}]}";
static const char E_SMOKEBALLS[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":16,\"step\":1,\"name\":\"Количество\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_PALMICUI_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":0,\"max\":@pal@,\"step\":1,\"name\":\"Палитра (0 = без цвета)\"},{\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Сглаживание\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_COLORS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"},{\"id\":2,\"val\":\"127\"},{\"id\":3,\"type\":32,\"val\":\"2\",\"min\":1,\"max\":3,\"step\":1,\"name\":\"Режим\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";
static const char E_TLAND[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":25,\"min\":0,\"max\":35,\"step\":1,\"name\":\"Эффект (0 = циклично)\"}, {\"id\":3,\"type\":0,\"val\":160,\"min\":0,\"max\":255,\"step\":1,\"name\":\"Цвет 1 (0 = циклично)\"}, {\"id\":4,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"step\":1,\"name\":\"Цвет 2 (0 = циклично)\"}, {\"id\":5,\"type\":0,\"val\":1,\"min\":1,\"max\":4,\"step\":1,\"name\":\"Качество/Скорость\"}]}";
static const char E_FLYING[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":128,\"step\":1,\"name\":\"Размытие\"}, {\"id\":3,\"type\":0,\"val\":7,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Круги\"}]}";
static const char E_WRAIN[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":25,\"min\":1,\"max\":45,\"step\":1,\"name\":\"Плотность\"}, {\"id\":3,\"type\":0,\"val\":4,\"min\":0,\"max\":@pal@,\"step\":1,\"name\":\"Палитра (0 = случайный цвет)\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Тучка\"}, {\"id\":5,\"type\":2,\"val\":\"true\",\"name\":\"Шторм\"}, {\"id\":6,\"type\":0,\"val\":1,\"min\":1,\"max\":8,\"step\":1,\"name\":\"Размытие (1-4 с ветром) \"}]}";
static const char E_CLOCK[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":4,\"min\":1,\"max\":8,\"step\":1,\"name\":\"Толщина\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Палитра\"}]}";
static const char E_COMET[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":4,\"min\":1,\"max\":6,\"step\":1,\"name\":\"Эффект\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Цвет (1 = случайный, 255 = белый)\"}, {\"id\":4,\"type\":0,\"val\":6,\"min\":1,\"max\":12,\"step\":1,\"name\":\"Размах\"}, {\"id\":5,\"type\":0,\"val\":32,\"min\":1,\"max\":64,\"step\":1,\"name\":\"Размытие\"}]}";

// Инженерный
static const char E_TEST_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":16,\"step\":1,\"name\":\"Количество\"}]}";
static const char E_TEST2_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Палитра\"}, {\"id\":4,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Параметр 1\"}, {\"id\":5,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Параметр 2\"}, {\"id\":6,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Параметр 3\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Микрофон\"}]}";



#endif