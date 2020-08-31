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
static const char TEFF_005[] PROGMEM = "Снегопад";
static const char TEFF_006[] PROGMEM = "Метель + Звездопад";
static const char TEFF_007[] PROGMEM = "Матрица";
static const char TEFF_008[] PROGMEM = "Светлячки";
static const char TEFF_009[] PROGMEM = "Светлячки со шлейфом";
static const char TEFF_010[] PROGMEM = "Блуждающий кубик";
static const char TEFF_011[] PROGMEM = "Пульс";
static const char TEFF_012[] PROGMEM = "Эффектопад";
static const char TEFF_013[] PROGMEM = "Огненная лампа";
static const char TEFF_014[] PROGMEM = "Пейнтбол";
static const char TEFF_015[] PROGMEM = "Безумие 3D";
static const char TEFF_016[] PROGMEM = "Облака 3D";
static const char TEFF_017[] PROGMEM = "Лава 3D";
static const char TEFF_018[] PROGMEM = "Плазма 3D";
static const char TEFF_019[] PROGMEM = "Радуга 3D";
static const char TEFF_020[] PROGMEM = "Павлин 3D";
static const char TEFF_021[] PROGMEM = "Зебра 3D";
static const char TEFF_022[] PROGMEM = "Лес 3D";
static const char TEFF_023[] PROGMEM = "Океан 3D";
static const char TEFF_024[] PROGMEM = "Прыгающие мячики";
static const char TEFF_025[] PROGMEM = "Синусоид 3";
static const char TEFF_026[] PROGMEM = "Метасферы";
static const char TEFF_027[] PROGMEM = "Спираль";
static const char TEFF_028[] PROGMEM = "Радужная комета";
static const char TEFF_029[] PROGMEM = "Три кометы";
static const char TEFF_030[] PROGMEM = "Призмата";
static const char TEFF_031[] PROGMEM = "Стая";
static const char TEFF_032[] PROGMEM = "Водоворот";
static const char TEFF_033[] PROGMEM = "Дрифт";
static const char TEFF_034[] PROGMEM = "Дрифт 2";
static const char TEFF_035[] PROGMEM = "Мерцание";
static const char TEFF_036[] PROGMEM = "Радар";
static const char TEFF_037[] PROGMEM = "Волны";
static const char TEFF_038[] PROGMEM = "Огонь 2012";
static const char TEFF_039[] PROGMEM = "Дождь";
static const char TEFF_040[] PROGMEM = "Цветной дождь";
static const char TEFF_041[] PROGMEM = "Тучка в банке";
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
static const char TEFF_052[] PROGMEM = "Блики на воде, цвета";
static const char TEFF_053[] PROGMEM = "Фейерверк";
static const char TEFF_054[] PROGMEM = "Тихий Океан";
static const char TEFF_055[] PROGMEM = "Вышиванка";
static const char TEFF_056[] PROGMEM = "Цветной шум";
static const char TEFF_057[] PROGMEM = "Мотыльки";
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
static const char TINTF_007[] PROGMEM = "Пересоздать индекс из ФС";
static const char TINTF_008[] PROGMEM = "Сохранить";
static const char TINTF_009[] PROGMEM = "Управление";
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
static const char TINTF_014[] PROGMEM = "Режимы";
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
static const char TINTF_041[] PROGMEM = "Копии под базовым";
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
static const char TINTF_050[] PROGMEM = "UNIX дата/время нового года";
static const char TINTF_051[] PROGMEM = "Время";
static const char TINTF_052[] PROGMEM = "Правила TZSET учета поясного/сезонного времени (напр 'MSK-3' для Europe/Moscow) Нужную строку можно взять тут https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h";
static const char TINTF_053[] PROGMEM = "правило TZone (рекоммендуется задать!)";
static const char TINTF_054[] PROGMEM = "резервный NTP-сервер (не обязательно)";
static const char TINTF_055[] PROGMEM = "Дата/время в формате YYYY-MM-DDThh:mm:ss (если нет интернета)";
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
static const char TINTF_085[] PROGMEM = "В алфавитном порядке";
static const char TINTF_086[] PROGMEM = "События активны";
static const char TINTF_087[] PROGMEM = "Скорость";
static const char TINTF_088[] PROGMEM = "Масштаб";
static const char TINTF_089[] PROGMEM = "";
static const char TINTF_08A[] PROGMEM = "";
static const char TINTF_08B[] PROGMEM = "";
static const char TINTF_08C[] PROGMEM = "";
static const char TINTF_08D[] PROGMEM = "";
static const char TINTF_08E[] PROGMEM = "";
static const char TINTF_08F[] PROGMEM = "";
static const char TINTF_090[] PROGMEM = "Нумерация в списке эффектов";
static const char TINTF_091[] PROGMEM = "Символ микрофона в списке";
static const char TINTF_092[] PROGMEM = "";
static const char TINTF_093[] PROGMEM = "";
static const char TINTF_094[] PROGMEM = "";
static const char TINTF_095[] PROGMEM = "";
static const char TINTF_096[] PROGMEM = "";
static const char TINTF_097[] PROGMEM = "";
static const char TINTF_098[] PROGMEM = "";
static const char TINTF_099[] PROGMEM = "";
static const char TINTF_09A[] PROGMEM = "";
static const char TINTF_09B[] PROGMEM = "";
static const char TINTF_09C[] PROGMEM = "";
static const char TINTF_09D[] PROGMEM = "";
static const char TINTF_09E[] PROGMEM = "";
static const char TINTF_09F[] PROGMEM = "";
static const char TINTF_0A0[] PROGMEM = "";

//-------------------------

/** набор служебных текстовых констант (не для локализации)
 */
static const char TCONST_0000[] PROGMEM = "effects";
static const char TCONST_0001[] PROGMEM = "pTime";
static const char TCONST_0002[] PROGMEM = "pMem";
static const char TCONST_0003[] PROGMEM = "lamp";
static const char TCONST_0004[] PROGMEM = "settings";
static const char TCONST_0005[] PROGMEM = "set_effect";
static const char TCONST_0006[] PROGMEM = "eff_sel";
static const char TCONST_0007[] PROGMEM = "eff_fav";
static const char TCONST_0008[] PROGMEM = "gray";
static const char TCONST_0009[] PROGMEM = "copy";
static const char TCONST_000A[] PROGMEM = "del";
static const char TCONST_000B[] PROGMEM = "makeidx";
static const char TCONST_000C[] PROGMEM = "red";
static const char TCONST_000D[] PROGMEM = "black";
static const char TCONST_000E[] PROGMEM = "AUX";
static const char TCONST_000F[] PROGMEM = "effects_config";
static const char TCONST_0010[] PROGMEM = "effListConf";
static const char TCONST_0011[] PROGMEM = "effects_param";
static const char TCONST_0012[] PROGMEM = "bright";
static const char TCONST_0013[] PROGMEM = "speed";
static const char TCONST_0014[] PROGMEM = "scale";
static const char TCONST_0015[] PROGMEM = "dynCtrl";
static const char TCONST_0016[] PROGMEM = "effList";
static const char TCONST_0017[] PROGMEM = "nofade";
static const char TCONST_0018[] PROGMEM = "GlobBRI";
static const char TCONST_0019[] PROGMEM = "flags";
static const char TCONST_001A[] PROGMEM = "ONflag";
static const char TCONST_001B[] PROGMEM = "Demo";
static const char TCONST_001C[] PROGMEM = "GBR";
static const char TCONST_001D[] PROGMEM = "Events";
static const char TCONST_001E[] PROGMEM = "Mic";
static const char TCONST_001F[] PROGMEM = "Btn";
static const char TCONST_0020[] PROGMEM = "show_flags";
static const char TCONST_0021[] PROGMEM = "mode";
static const char TCONST_0022[] PROGMEM = "eff_prev";
static const char TCONST_0023[] PROGMEM = "eff_next";
static const char TCONST_0024[] PROGMEM = "#708090";
static const char TCONST_0025[] PROGMEM = "#5f9ea0";
static const char TCONST_0026[] PROGMEM = "DTimer";
static const char TCONST_0027[] PROGMEM = "OTA";
static const char TCONST_0028[] PROGMEM = "lamp_config";
static const char TCONST_0029[] PROGMEM = "edit_lamp_config";
static const char TCONST_002A[] PROGMEM = "fileName";
static const char TCONST_002B[] PROGMEM = "/cfg";
static const char TCONST_002C[] PROGMEM = "/cfg/";
static const char TCONST_002D[] PROGMEM = "load";
static const char TCONST_002E[] PROGMEM = "save";
static const char TCONST_002F[] PROGMEM = "green";
static const char TCONST_0030[] PROGMEM = "add_lamp_config";
static const char TCONST_0031[] PROGMEM = "/glb/";
static const char TCONST_0032[] PROGMEM = "/evn/";
static const char TCONST_0033[] PROGMEM = "/btn/";
static const char TCONST_0034[] PROGMEM = "textsend";
static const char TCONST_0035[] PROGMEM = "msg";
static const char TCONST_0036[] PROGMEM = "txtColor";
static const char TCONST_0037[] PROGMEM = "settings_mic";
static const char TCONST_0038[] PROGMEM = "set_mic";
static const char TCONST_0039[] PROGMEM = "micScale";
static const char TCONST_003A[] PROGMEM = "micNoise";
static const char TCONST_003B[] PROGMEM = "micnRdcLvl";
static const char TCONST_003C[] PROGMEM = "mic_cal";
static const char TCONST_003D[] PROGMEM = "settings_wifi";
static const char TCONST_003E[] PROGMEM = "set_wifi";
static const char TCONST_003F[] PROGMEM = "hostname";
static const char TCONST_0040[] PROGMEM = "wcssid";
static const char TCONST_0041[] PROGMEM = "wcpass";
static const char TCONST_0042[] PROGMEM = "set_wifiAP";
static const char TCONST_0043[] PROGMEM = "APonly";
static const char TCONST_0044[] PROGMEM = "APpwd";
static const char TCONST_0045[] PROGMEM = "set_mqtt";
static const char TCONST_0046[] PROGMEM = "m_host";
static const char TCONST_0047[] PROGMEM = "m_port";
static const char TCONST_0048[] PROGMEM = "m_user";
static const char TCONST_0049[] PROGMEM = "m_pass";
static const char TCONST_004A[] PROGMEM = "mqtt_int";
static const char TCONST_004B[] PROGMEM = "set_other";
static const char TCONST_004C[] PROGMEM = "MIRR_H";
static const char TCONST_004D[] PROGMEM = "MIRR_V";
static const char TCONST_004E[] PROGMEM = "isFaderON";
static const char TCONST_004F[] PROGMEM = "DRand";
static const char TCONST_0050[] PROGMEM = "effSort";
static const char TCONST_0051[] PROGMEM = "txtSpeed";
static const char TCONST_0052[] PROGMEM = "txtOf";
static const char TCONST_0053[] PROGMEM = "perTime";
static const char TCONST_0054[] PROGMEM = "ny_period";
static const char TCONST_0055[] PROGMEM = "ny_unix";
static const char TCONST_0056[] PROGMEM = "set_time";
static const char TCONST_0057[] PROGMEM = "TZSET";
static const char TCONST_0058[] PROGMEM = "userntp";
static const char TCONST_0059[] PROGMEM = "setdatetime";
static const char TCONST_005A[] PROGMEM = "update";
static const char TCONST_005B[] PROGMEM = "blue";
static const char TCONST_005C[] PROGMEM = "show_event";
static const char TCONST_005D[] PROGMEM = "event_conf";
static const char TCONST_005E[] PROGMEM = "eventList";
static const char TCONST_005F[] PROGMEM = "edit";
static const char TCONST_0060[] PROGMEM = "enabled";
static const char TCONST_0061[] PROGMEM = "d1";
static const char TCONST_0062[] PROGMEM = "d2";
static const char TCONST_0063[] PROGMEM = "d3";
static const char TCONST_0064[] PROGMEM = "d4";
static const char TCONST_0065[] PROGMEM = "d5";
static const char TCONST_0066[] PROGMEM = "d6";
static const char TCONST_0067[] PROGMEM = "d7";
static const char TCONST_0068[] PROGMEM = "evList";
static const char TCONST_0069[] PROGMEM = "repeat";
static const char TCONST_006A[] PROGMEM = "stopat";
static const char TCONST_006B[] PROGMEM = "tmEvent";
static const char TCONST_006C[] PROGMEM = "set_event";
static const char TCONST_006D[] PROGMEM = "show_button";
static const char TCONST_006E[] PROGMEM = "butt_conf";
static const char TCONST_006F[] PROGMEM = "buttList";
static const char TCONST_0070[] PROGMEM = "on";
static const char TCONST_0071[] PROGMEM = "hold";
static const char TCONST_0072[] PROGMEM = "onetime";
static const char TCONST_0073[] PROGMEM = "clicks";
static const char TCONST_0074[] PROGMEM = "bactList";
static const char TCONST_0075[] PROGMEM = "set_butt";
static const char TCONST_0076[] PROGMEM = "show_butt";
static const char TCONST_0077[] PROGMEM = "show_time";
static const char TCONST_0078[] PROGMEM = "show_wifi";
static const char TCONST_0079[] PROGMEM = "show_mic";
static const char TCONST_007A[] PROGMEM = "show_other";
static const char TCONST_007B[] PROGMEM = "m_pref";
static const char TCONST_007C[] PROGMEM = "#ffffff";
static const char TCONST_007D[] PROGMEM = "1609459200";
static const char TCONST_007E[] PROGMEM = "main";
static const char TCONST_007F[] PROGMEM = "dynCtrl*";
static const char TCONST_0080[] PROGMEM = "STA";
static const char TCONST_0081[] PROGMEM = "off";
static const char TCONST_0082[] PROGMEM = "effect";
static const char TCONST_0083[] PROGMEM = "move_next";
static const char TCONST_0084[] PROGMEM = "move_prev";
static const char TCONST_0085[] PROGMEM = "move_rnd";
static const char TCONST_0086[] PROGMEM = "reboot";
static const char TCONST_0087[] PROGMEM = "alarm";
static const char TCONST_0088[] PROGMEM = "aux_on";
static const char TCONST_0089[] PROGMEM = "aux_off";
static const char TCONST_008A[] PROGMEM = "aux_toggle";
static const char TCONST_008B[] PROGMEM = "jee/pub/";
static const char TCONST_008C[] PROGMEM = "pin";
static const char TCONST_008D[] PROGMEM = "act";
static const char TCONST_008E[] PROGMEM = "isClearing";
static const char TCONST_008F[] PROGMEM = "pUptime";
static const char TCONST_0090[] PROGMEM = "numInList";
static const char TCONST_0091[] PROGMEM = "effHasMic";
static const char TCONST_0092[] PROGMEM = "";
static const char TCONST_0093[] PROGMEM = "";
static const char TCONST_0094[] PROGMEM = "";
static const char TCONST_0095[] PROGMEM = "";
static const char TCONST_0096[] PROGMEM = "";
static const char TCONST_0097[] PROGMEM = "";
static const char TCONST_0098[] PROGMEM = "";
static const char TCONST_0099[] PROGMEM = "";
static const char TCONST_009A[] PROGMEM = "";
static const char TCONST_009B[] PROGMEM = "";
static const char TCONST_009C[] PROGMEM = "";
static const char TCONST_009D[] PROGMEM = "";
static const char TCONST_009E[] PROGMEM = "";
static const char TCONST_009F[] PROGMEM = "";
static const char TCONST_00A0[] PROGMEM = "";

static const char TCONST_FFFE[] PROGMEM = "false";
static const char TCONST_FFFF[] PROGMEM = "true";
#endif