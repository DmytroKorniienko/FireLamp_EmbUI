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

(Цей файл є частиною FireLamp_JeeUI.

   FireLamp_JeeUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_JeeUI поширюється в надії, що вона буде корисною,
   але БЕЗ ВСЯКИХ ГАРАНТІЙ; навіть без неявної гарантії ТОВАРНОГО ВИГЛЯДУ
   або ПРИДАТНОСТІ ДЛЯ ВИЗНАЧЕНИХ ЦІЛЕЙ. Докладніше див. у Стандартній
   громадська ліцензія GNU.

   Ви повинні були отримати копію Стандартної громадської ліцензії GNU
   разом із цією програмою. Якщо це не так, див.
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
static const char TEFF_001[] PROGMEM = "Біла лампа";
static const char TEFF_002[] PROGMEM = "Кольори";
static const char TEFF_003[] PROGMEM = "Веселка 2D";
static const char TEFF_004[] PROGMEM = "Конфеті";
static const char TEFF_005[] PROGMEM = "Кольорові драже";
static const char TEFF_006[] PROGMEM = "Хурделиця, Зіркопад, Метеори";
static const char TEFF_007[] PROGMEM = "Матриця";
static const char TEFF_008[] PROGMEM = "Акварель";
static const char TEFF_009[] PROGMEM = "Світлячки зі шлейфом";
static const char TEFF_010[] PROGMEM = "Блукаючий кубик";
static const char TEFF_011[] PROGMEM = "Пульс";
static const char TEFF_012[] PROGMEM = "Ефектопад";
static const char TEFF_013[] PROGMEM = "Неопалима купина"; 
static const char TEFF_014[] PROGMEM = "Пейнтбол";
static const char TEFF_015[] PROGMEM = "Шум 3D";
static const char TEFF_016[] PROGMEM = "Клітинки-Ялинки";
static const char TEFF_017[] PROGMEM = "П'ятнашки";
static const char TEFF_018[] PROGMEM = "Тіксі Ленд";
static const char TEFF_019[] PROGMEM = "Нікола Тесла";
static const char TEFF_020[] PROGMEM = "Осцилятор";
static const char TEFF_021[] PROGMEM = "Шторм";
static const char TEFF_022[] PROGMEM = "Фея";
static const char TEFF_023[] PROGMEM = "Джерело";
static const char TEFF_024[] PROGMEM = "Стрибаючі м'ячики";
static const char TEFF_025[] PROGMEM = "Синусоїд";
static const char TEFF_026[] PROGMEM = "Метасфери";
static const char TEFF_027[] PROGMEM = "Спіраль";
static const char TEFF_028[] PROGMEM = "Комети, Витаючі Вогні ++";
static const char TEFF_029[] PROGMEM = "Бульбулятор";
static const char TEFF_030[] PROGMEM = "Призмата";
static const char TEFF_031[] PROGMEM = "Стадо та Хижак";
static const char TEFF_032[] PROGMEM = "Водоверть";
static const char TEFF_033[] PROGMEM = "Дрифт";
static const char TEFF_034[] PROGMEM = "Попкорн";
static const char TEFF_035[] PROGMEM = "Мерехтіння";
static const char TEFF_036[] PROGMEM = "Радар";
static const char TEFF_037[] PROGMEM = "Хвилі";
static const char TEFF_038[] PROGMEM = "Вогонь 2012";
static const char TEFF_039[] PROGMEM = "Бенгальські Вогні";
static const char TEFF_040[] PROGMEM = "Кульки";
static const char TEFF_041[] PROGMEM = "ДНК";
static const char TEFF_042[] PROGMEM = "Вогонь 2018";
static const char TEFF_043[] PROGMEM = "Кодовий замок";
static const char TEFF_044[] PROGMEM = "Куб 2D";
static const char TEFF_045[] PROGMEM = "Магма";
static const char TEFF_046[] PROGMEM = "Пікассо";
static const char TEFF_047[] PROGMEM = "Зоряний Десант";
static const char TEFF_048[] PROGMEM = "Прапори";
static const char TEFF_049[] PROGMEM = "Стрибуни";
static const char TEFF_050[] PROGMEM = "Вихори";
static const char TEFF_051[] PROGMEM = "Зірки";
static const char TEFF_052[] PROGMEM = "Відблиски води, Кольори";
static const char TEFF_053[] PROGMEM = "Феєрверк";
static const char TEFF_054[] PROGMEM = "Тихий Океан";
static const char TEFF_055[] PROGMEM = "Вишиванка";
static const char TEFF_056[] PROGMEM = "Кольоровий шум";
static const char TEFF_057[] PROGMEM = "Метелики";
static const char TEFF_058[] PROGMEM = "Тіні";
static const char TEFF_059[] PROGMEM = "Візерунки";
static const char TEFF_060[] PROGMEM = "Стрілки";
static const char TEFF_061[] PROGMEM = "Дикі кульки";
static const char TEFF_062[] PROGMEM = "Притягування";
static const char TEFF_063[] PROGMEM = "Змійки";
static const char TEFF_064[] PROGMEM = "Nexus";
static const char TEFF_065[] PROGMEM = "Лабіринт";
static const char TEFF_066[] PROGMEM = "В'юнкі кольори";
static const char TEFF_067[] PROGMEM = "Північне Сяйво";
static const char TEFF_068[] PROGMEM = "Цвітіння";
static const char TEFF_069[] PROGMEM = "Зміїний Острів";
static const char TEFF_070[] PROGMEM = "Дитячі сни";
static const char TEFF_071[] PROGMEM = "Метабульки";
static const char TEFF_072[] PROGMEM = "Лавова Лампа";
static const char TEFF_073[] PROGMEM = "Космо-Рейсер";
static const char TEFF_074[] PROGMEM = "Дим";
static const char TEFF_075[] PROGMEM = "Вогняна Лампа";
static const char TEFF_076[] PROGMEM = "Міраж";
static const char TEFF_250[] PROGMEM = "Годинник";
static const char TEFF_251[] PROGMEM = "Програвач RGB 332/565";
static const char TEFF_254[] PROGMEM = "Частотний аналізатор";
static const char TEFF_255[] PROGMEM = "Осцилограф";

#define DFTINTF_00A "Ефект"
#define DFTINTF_020 "Мікрофон"
#define DFTINTF_084 "Палітра"
#define DFTINTF_088 "Масштаб"
#define DFTINTF_0D0 "Колір"
#define DFTINTF_0D1 "Палітра/Масштаб"
#define DFTINTF_0D2 "Ефект (0: циклічно)"
#define DFTINTF_0D3 "Зміщення кольору (0: циклічно)"
#define DFTINTF_0D4 "Cубпіксель"
#define DFTINTF_0D5 "Кількість"
#define DFTINTF_0D6 "Генератор кольору"
#define DFTINTF_0D7 "Відтінок"
#define DFTINTF_0D8 "Запал!"
#define DFTINTF_0D9 "Кількість/Колір (нічник)"
#define DFTINTF_0DA "Насиченість"
#define DFTINTF_0DB "Підсилення"
#define DFTINTF_0DC "Розгортка"
#define DFTINTF_0DD "Пульсація"
#define DFTINTF_0DE "Розмиття"
#define DFTINTF_0DF "Порхання"
#define DFTINTF_0E0 "Нічник"
#define DFTINTF_0E1 "Кільця"
#define DFTINTF_0E2 "Відблиски (0: Вимкнути)"
#define DFTINTF_0E3 "Режим"
#define DFTINTF_0E4 "Лінійна швидкість"
#define DFTINTF_0E5 "Режим (1: випадковий)"
#define DFTINTF_0E6 "Зсув"
#define DFTINTF_0E7 "Кількість/Напрям/Положення"
#define DFTINTF_0E8 "Хижак"
#define DFTINTF_0E9 "Маса"
#define DFTINTF_0EA "Швидкість (Верт.)"
#define DFTINTF_0EB "Швидкість (Гориз.)"
#define DFTINTF_0EC "Візерунок (0: випадковий)"
#define DFTINTF_0ED "Затемнення"
#define DFTINTF_0EE "Енергія"
#define DFTINTF_0EF "Щільність"
#define DFTINTF_0F0 "Колір/Обертання"
#define DFTINTF_0F1 "Розмір/Шлейф (1-85-170-255)"
#define DFTINTF_0F2 "Частота запусків"
#define DFTINTF_0F3 "Поворот"
#define DFTINTF_0F4 "Палітра (0: генератор)"
#define DFTINTF_0F5 "Згладжування"
#define DFTINTF_0F6 "Заповнення"
#define DFTINTF_0F7 "Товщина"
#define DFTINTF_0F9 "Поведінка: 1/2"
#define DFTINTF_0FA "Колір: 1/2"
#define DFTINTF_0FB "Палітра (0: без кольору)"
#define DFTINTF_0FC "Колір (1: випадковий, 255: білий)"
#define DFTINTF_0FD "Снігопад/Зорепад/Метеори"
#define DFTINTF_0FE "Кількість"
#define DFTINTF_0FF "Тип 1/2, Розмиття 3/4"
#define DFTINTF_100 "Палітра (0: випадковий колір)"
#define DFTINTF_101 "Колір 1 (0: циклічно)"
#define DFTINTF_102 "Хмарка"
#define DFTINTF_103 "Розмиття (0: немає)"
#define DFTINTF_104 "Розмах"
#define DFTINTF_105 "Фільтр"
#define DFTINTF_106 "Фізика"
#define DFTINTF_107 "Кола"
#define DFTINTF_108 "Варіант 1/2"
#define DFTINTF_109 "Спалахи"
#define DFTINTF_10A "Глюк"
#define DFTINTF_10B "Блискавка"
#define DFTINTF_10C "Кількість снарядів"
#define DFTINTF_10D "Розмір (ширина)"
#define DFTINTF_10E "Зміщення кольору (0: випадковий)"
#define DFTINTF_10F "Розмір (висота)"
#define DFTINTF_110 "Класика"
#define DFTINTF_111 "Один колір"
#define DFTINTF_112 "Зворотні Кольори"
#define DFTINTF_113 "Колір 2 (0: циклічно)"
#define DFTINTF_114 "Якість / Швидкість"
#define DFTINTF_115 "Розмиття (1-4 з вітром)"
#define DFTINTF_116 "Переміщення по синусу"
#define DFTINTF_117 "Напрямок (0: циклічно)"
#define DFTINTF_118 "Поріг сигналу"
#define DFTINTF_11A "Колір (1 = циклічно)"


/** набор строк с текстовыми константами интерфейса
 */
static const char TINTF_000[] PROGMEM = "Ефекти";
static const char TINTF_001[] PROGMEM = "Вивід тексту";
static const char TINTF_002[] PROGMEM = "Налаштування";
static const char TINTF_003[] PROGMEM = "В загальному списку";
static const char TINTF_004[] PROGMEM = "В списку \"Демо\"";
static const char TINTF_005[] PROGMEM = "Копіювати";
static const char TINTF_006[] PROGMEM = "Видалити";
static const char TINTF_007[] PROGMEM = "Оновити список ефектів";
static const char TINTF_008[] PROGMEM = "Зберегти";
static const char TINTF_009[] PROGMEM = "Керування списком ефектів";
static const char TINTF_00A[] PROGMEM = DFTINTF_00A;
static const char TINTF_00B[] PROGMEM = "Вихід";
static const char TINTF_00C[] PROGMEM = "Глоб.яскравість";
static const char TINTF_00D[] PROGMEM = "Яскравість";
static const char TINTF_00E[] PROGMEM = "Увімкнути";
static const char TINTF_00F[] PROGMEM = "\"Демо\"";
static const char TINTF_010[] PROGMEM = "Гл.Яскрав";
static const char TINTF_011[] PROGMEM = "Події";
static const char TINTF_012[] PROGMEM = "Мікр.";
static const char TINTF_013[] PROGMEM = "Кнопка";
static const char TINTF_014[] PROGMEM = "Додатково...";
static const char TINTF_015[] PROGMEM = "<<<";
static const char TINTF_016[] PROGMEM = ">>>";
static const char TINTF_017[] PROGMEM = "Очікуйте...";
static const char TINTF_018[] PROGMEM = "Конфігурації";
static const char TINTF_019[] PROGMEM = "Завантажити";
static const char TINTF_01A[] PROGMEM = "Конфігурація";
static const char TINTF_01B[] PROGMEM = "Створити";
static const char TINTF_01C[] PROGMEM = "Вивести на лампу";
static const char TINTF_01D[] PROGMEM = "Текст";
static const char TINTF_01E[] PROGMEM = "Колір повідомлення";
static const char TINTF_01F[] PROGMEM = "Надіслати";
static const char TINTF_020[] PROGMEM = DFTINTF_020;
static const char TINTF_021[] PROGMEM = "Мікрофон активний";
static const char TINTF_022[] PROGMEM = "Коеф.корекції нуля";
static const char TINTF_023[] PROGMEM = "Рівень шуму, од";
static const char TINTF_024[] PROGMEM = "Подавлення шуму";
static const char TINTF_025[] PROGMEM = "Калібровка мікрофона";
static const char TINTF_026[] PROGMEM = "Включіть мікрофон";
static const char TINTF_027[] PROGMEM = "... в процесі ...";
static const char TINTF_028[] PROGMEM = "WiFi";
static const char TINTF_029[] PROGMEM = "Клієнт (STA)";
static const char TINTF_02A[] PROGMEM = "Налаштування клієнта WiFi";
static const char TINTF_02B[] PROGMEM = "Ім'я лампи (mDNS Hostname/AP-SSID)";
static const char TINTF_02C[] PROGMEM = "Мережа WiFi";
static const char TINTF_02D[] PROGMEM = "Пароль";
static const char TINTF_02E[] PROGMEM = "Підключитися";
static const char TINTF_02F[] PROGMEM = "Точка доступу (AP)";
static const char TINTF_030[] PROGMEM = "Відображення";
static const char TINTF_031[] PROGMEM = "Налаштування WiFi та точки доступу";
static const char TINTF_032[] PROGMEM = "В режимі AP лампа завжди працює як точка доступу, і не буде підключатися до інших WiFi-мереж";
static const char TINTF_033[] PROGMEM = "Режим WiFi";
static const char TINTF_034[] PROGMEM = "Захистити AP паролем";
static const char TINTF_035[] PROGMEM = "MQTT";
static const char TINTF_036[] PROGMEM = "MQTT хост";
static const char TINTF_037[] PROGMEM = "MQTT порт";
static const char TINTF_038[] PROGMEM = "Користувач";
static const char TINTF_039[] PROGMEM = "Інтервал MQTT сек.";
static const char TINTF_03A[] PROGMEM = "Підключити";
static const char TINTF_03B[] PROGMEM = "Горизонтальне відзеркалення";
static const char TINTF_03C[] PROGMEM = "Вертикальне відзеркалення";
static const char TINTF_03D[] PROGMEM = "Плавна зміна ефектів";
static const char TINTF_03E[] PROGMEM = "Випадковий ефект в Демо";
static const char TINTF_03F[] PROGMEM = "Зміна ефекта в Демо. (сек)";
static const char TINTF_040[] PROGMEM = "Сортуваня ефектів";
static const char TINTF_041[] PROGMEM = "Копії під оригіналом";
static const char TINTF_042[] PROGMEM = "Копії в кінці списку";
static const char TINTF_043[] PROGMEM = "Відповідно номеру";
static const char TINTF_044[] PROGMEM = "Швидкість відтворення тексту";
static const char TINTF_045[] PROGMEM = "Зміщення текста вверх";
static const char TINTF_046[] PROGMEM = "Змішаний (AP+STA)";
static const char TINTF_047[] PROGMEM = "Не показувати";
static const char TINTF_048[] PROGMEM = "Кожну годину";
static const char TINTF_049[] PROGMEM = "Кожні півгодинни";
static const char TINTF_04A[] PROGMEM = "Кожні 15 хвилин";
static const char TINTF_04B[] PROGMEM = "Кожні 10 хвилин";
static const char TINTF_04C[] PROGMEM = "Кожні 5 хвилин";
static const char TINTF_04D[] PROGMEM = "Кожну хвилину";
static const char TINTF_04E[] PROGMEM = "Новорічне привітання";
static const char TINTF_04F[] PROGMEM = "Період відображення, хвилин (0 - не відображати)";
static const char TINTF_050[] PROGMEM = "Дата/час Нового Року у форматі YYYY-MM-DDThh:mm";
static const char TINTF_051[] PROGMEM = "Дата / Час / Часова зона";
static const char TINTF_052[] PROGMEM = "Налаштування часової зони. Правила поясного та сезонного часу застосовуються автоматично, ручна корекція не потрібна. Якщо у списку немає вашої зоні, оберіть загальну зону зміщення від Грінвича";
static const char TINTF_053[] PROGMEM = "Часова зона";
static const char TINTF_054[] PROGMEM = "Резервний NTP-сервер (не обов'язково)";
static const char TINTF_055[] PROGMEM = "Дата/час у форматі YYYY-MM-DDThh:mm:ss (при відсутності Інтернет, якщо пусто, то отримати з клієнта)";
static const char TINTF_056[] PROGMEM = "Оновлення";
static const char TINTF_057[] PROGMEM = "Оновлення ОТА-PIO";
static const char TINTF_058[] PROGMEM = "Почати";
static const char TINTF_059[] PROGMEM = "Завантаження прошивки";
static const char TINTF_05A[] PROGMEM = "Upload";
static const char TINTF_05B[] PROGMEM = "Подія";
static const char TINTF_05C[] PROGMEM = "Редагувати";
static const char TINTF_05D[] PROGMEM = "Додати";
static const char TINTF_05E[] PROGMEM = "Активно";
static const char TINTF_05F[] PROGMEM = "Тип події";
static const char TINTF_060[] PROGMEM = "Увімкнути лампу";
static const char TINTF_061[] PROGMEM = "Вимкнути лампу";
static const char TINTF_062[] PROGMEM = "Режим \"Демо\"";
static const char TINTF_063[] PROGMEM = "Будильник";
static const char TINTF_064[] PROGMEM = "Завантажити налаштування лампи";
static const char TINTF_065[] PROGMEM = "Завантажити налаштування ефектів";
static const char TINTF_066[] PROGMEM = "Завантажити налаштування подій";
static const char TINTF_067[] PROGMEM = "Відобразити текст";
static const char TINTF_068[] PROGMEM = "Відобразити час";
static const char TINTF_069[] PROGMEM = "Стан піна";
static const char TINTF_06A[] PROGMEM = "Увімкнути AUX";
static const char TINTF_06B[] PROGMEM = "Вимкнути AUX";
static const char TINTF_06C[] PROGMEM = "Переключити стан AUX";
static const char TINTF_06D[] PROGMEM = "Дата/час події";
static const char TINTF_06E[] PROGMEM = "Повтор, хв";
static const char TINTF_06F[] PROGMEM = "Зупинити через, хв";
static const char TINTF_070[] PROGMEM = "Параметр (текст)";
static const char TINTF_071[] PROGMEM = "Повтор";
static const char TINTF_072[] PROGMEM = "Понеділок";
static const char TINTF_073[] PROGMEM = "Вівторок";
static const char TINTF_074[] PROGMEM = "Середа";
static const char TINTF_075[] PROGMEM = "Четверг";
static const char TINTF_076[] PROGMEM = "П'ятница";
static const char TINTF_077[] PROGMEM = "Субота";
static const char TINTF_078[] PROGMEM = "Неділя";
static const char TINTF_079[] PROGMEM = "Оновити";
static const char TINTF_07A[] PROGMEM = "Дія";
static const char TINTF_07B[] PROGMEM = "Кнопка активна";
static const char TINTF_07C[] PROGMEM = "OFF/ON";
static const char TINTF_07D[] PROGMEM = "Утримання";
static const char TINTF_07E[] PROGMEM = "Натискання";
static const char TINTF_07F[] PROGMEM = "Одноразово";
static const char TINTF_080[] PROGMEM = "Вогняна Лампа";
#if defined(EMBUI_USE_FTP) && defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi, MQTT & FTP";
#elif defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi & MQTT";
#elif defined(EMBUI_USE_FTP)
static const char TINTF_081[] PROGMEM = "WiFi & FTP";
#else
static const char TINTF_081[] PROGMEM = "WiFi";
#endif
static const char TINTF_082[] PROGMEM = "Інші";
static const char TINTF_083[] PROGMEM = "Очистити лампу при зміні ефектів";
static const char TINTF_084[] PROGMEM = DFTINTF_084;
static const char TINTF_085[] PROGMEM = "В алфавітному порядку (внутрішнє)";
static const char TINTF_086[] PROGMEM = "Події активні";
static const char TINTF_087[] PROGMEM = "Швидкість";
static const char TINTF_088[] PROGMEM = DFTINTF_088;
static const char TINTF_089[] PROGMEM = "Назва ефекту";
static const char TINTF_08A[] PROGMEM = "В алфавітному порядку (конфігурація)";
static const char TINTF_08B[] PROGMEM = "Скинути налаштування ефекта";
static const char TINTF_08C[] PROGMEM = "Префікс топіків";
static const char TINTF_08D[] PROGMEM = "Сортуваня по мікрофону";
static const char TINTF_08E[] PROGMEM = "Налаштування";
static const char TINTF_08F[] PROGMEM = "Налаштування ESP";
static const char TINTF_090[] PROGMEM = "Нумерація в списку ефектів";
static const char TINTF_091[] PROGMEM = "Символ мікрофона в списку";
static const char TINTF_092[] PROGMEM = "Налаштування пінів";
static const char TINTF_093[] PROGMEM = "Показати налаштування ESP";
static const char TINTF_094[] PROGMEM = "Пін кнопки";
static const char TINTF_095[] PROGMEM = "Обмеження струму (mA)";
static const char TINTF_096[] PROGMEM = "Перезавантаження";
static const char TINTF_097[] PROGMEM = "Пін RX DFPlayer";
static const char TINTF_098[] PROGMEM = "Пін TX DFPlayer";
static const char TINTF_099[] PROGMEM = "DFPlayer";
static const char TINTF_09A[] PROGMEM = "Назва ефекту в Демо";
static const char TINTF_09B[] PROGMEM = "Гучність";
static const char TINTF_09C[] PROGMEM = "Час";
static const char TINTF_09D[] PROGMEM = "Назва ефекту";
static const char TINTF_09E[] PROGMEM = "Відповідно ефекту";
static const char TINTF_09F[] PROGMEM = "Відключений";
static const char TINTF_0A0[] PROGMEM = "Перший";
static const char TINTF_0A1[] PROGMEM = "Випадковий";
static const char TINTF_0A2[] PROGMEM = "Випадковий MP3";
static const char TINTF_0A3[] PROGMEM = "Звук будильника";
static const char TINTF_0A4[] PROGMEM = "Другий";
static const char TINTF_0A5[] PROGMEM = "Третій";
static const char TINTF_0A6[] PROGMEM = "Четвертий";
static const char TINTF_0A7[] PROGMEM = "Пятий";
static const char TINTF_0A8[] PROGMEM = "Еквалайзер";
static const char TINTF_0A9[] PROGMEM = "Нормальний";
static const char TINTF_0AA[] PROGMEM = "Поп";
static const char TINTF_0AB[] PROGMEM = "Рок";
static const char TINTF_0AC[] PROGMEM = "Джаз";
static const char TINTF_0AD[] PROGMEM = "Класика";
static const char TINTF_0AE[] PROGMEM = "Бас";
static const char TINTF_0AF[] PROGMEM = "MP3 плеєр";
static const char TINTF_0B0[] PROGMEM = "Кількість файлів в теці MP3";
static const char TINTF_0B1[] PROGMEM = "Режими озвучення";
static const char TINTF_0B2[] PROGMEM = "Тека\\номер звукового файла (приклади: MP3\\17, 5\\19)";
static const char TINTF_0B3[] PROGMEM = "Обмежити гучність будильника";
static const char TINTF_0B4[] PROGMEM = "За замовчуванням";
static const char TINTF_0B5[] PROGMEM = "Видалити зі списку";
static const char TINTF_0B6[] PROGMEM = "Відключено";
static const char TINTF_0B7[] PROGMEM = "Варіант 1";
static const char TINTF_0B8[] PROGMEM = "Варіант 2";
static const char TINTF_0B9[] PROGMEM = "Параметр";
static const char TINTF_0BA[] PROGMEM = "Будильник \"Світанок\"";
static const char TINTF_0BB[] PROGMEM = "Тривалість світанку";
static const char TINTF_0BC[] PROGMEM = "Свічення після світанку";
static const char TINTF_0BD[] PROGMEM = "<";
static const char TINTF_0BE[] PROGMEM = ">";
static const char TINTF_0BF[] PROGMEM = "<+5";
static const char TINTF_0C0[] PROGMEM = "+5>";
static const char TINTF_0C1[] PROGMEM = "секунд";
static const char TINTF_0C2[] PROGMEM = "хвилин";
static const char TINTF_0C3[] PROGMEM = "годин";
static const char TINTF_0C4[] PROGMEM = "днів";
static const char TINTF_0C5[] PROGMEM = "дні";
static const char TINTF_0C6[] PROGMEM = "день";
static const char TINTF_0C7[] PROGMEM = "годин";
static const char TINTF_0C8[] PROGMEM = "година";
static const char TINTF_0C9[] PROGMEM = "Випадк.";
static const char TINTF_0CA[] PROGMEM = "Затемнення фону";
static const char TINTF_0CB[] PROGMEM = "Попередження";
static const char TINTF_0CC[] PROGMEM = "хвилини";
static const char TINTF_0CD[] PROGMEM = "хвилина";
static const char TINTF_0CE[] PROGMEM = "Малювання";
static const char TINTF_0CF[] PROGMEM = "Заливка";
static const char TINTF_0D0[] PROGMEM = DFTINTF_0D0;
static const char TINTF_0D1[] PROGMEM = "Звук спочатку";
static const char TINTF_0D2[] PROGMEM = "Обмежити гучність";
static const char TINTF_0D3[] PROGMEM = "Коеф. швидкості";
static const char TINTF_0D4[] PROGMEM = "Налаштування дисплея";
static const char TINTF_0D5[] PROGMEM = "Яскравість (0 - виключити)";
static const char TINTF_0D6[] PROGMEM = "Яскравість з викл. лампою (0 - виключити)";
static const char TINTF_0D7[] PROGMEM = "24х годинний формат";
static const char TINTF_0D8[] PROGMEM = "Ведучий 0";
static const char TINTF_0D9[] PROGMEM = "Очистити";
static const char TINTF_0DA[] PROGMEM = "Сканувати";
static const char TINTF_0DB[] PROGMEM = "FTP";
static const char TINTF_0DC[] PROGMEM = "Управління";
static const char TINTF_0DD[] PROGMEM = "Тип шкали";
static const char TINTF_0DE[] PROGMEM = "Колір шкали";
static const char TINTF_0DF[] PROGMEM = "Колір тексту";
static const char TINTF_0E0[] PROGMEM = "Показувати температуру";
static const char TINTF_0E1[] PROGMEM = "Впевнені?";
static const char TINTF_0E2[] PROGMEM = "Трансляція";
static const char TINTF_0E3[] PROGMEM = "Режим";
static const char TINTF_0E4[] PROGMEM = "E1.31";
static const char TINTF_0E5[] PROGMEM = "SoulMate";
static const char TINTF_0E6[] PROGMEM = "Блок.Ефект";
static const char TINTF_0E7[] PROGMEM = "Розмітка";
static const char TINTF_0E8[] PROGMEM = "Початковий Universe";
static const char TINTF_0E9[] PROGMEM = "Завантажити конфіг кнопок";
static const char TINTF_0EA[] PROGMEM = "Нічник";
static const char TINTF_0EB[] PROGMEM = "Світильник";
static const char TINTF_0EC[] PROGMEM = "У будь-якому випадку";
static const char TINTF_0ED[] PROGMEM = "Промовити час";
static const char TINTF_0EE[] PROGMEM = "Відключено";
static const char TINTF_0EF[] PROGMEM = "Вертикальна";
static const char TINTF_0F0[] PROGMEM = "Горизонтальна";

static const char NY_MDG_STRING1[] PROGMEM = "До нового року залишилось %d %s";
static const char NY_MDG_STRING2[] PROGMEM = "З новим %d роком!";
//-------------------------

#endif