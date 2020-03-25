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

#pragma once
#include <GyverButton.h>

#define FASTLED_INTERRUPT_RETRY_COUNT   (0)                 // default: 2; // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
#define FASTLED_ESP8266_RAW_PIN_ORDER                       // FASTLED_ESP8266_RAW_PIN_ORDER, FASTLED_ESP8266_D1_PIN_ORDER or FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ALLOW_INTERRUPTS      (0)                   // default: 1; // Use this to force FastLED to allow interrupts in the clockless chipsets (or to force it to disallow), overriding the default on platforms that support this. Set the value to 1 to allow interrupts or 0 to disallow them.
#include <FastLed.h>

//-----------------------------------
#define ESP_USE_BUTTON                                      // если строка не закомментирована, должна быть подключена кнопка (иначе ESP может регистрировать "фантомные" нажатия и некорректно устанавливать яркость)
#define LAMP_DEBUG
//#define DEBUG_TELNET_OUTPUT  (true)                       // true - отладочные сообщения будут выводиться в telnet вместо Serial порта (для удалённой отладки без подключения usb кабелем)
//#define USE_FTP                                             // доступ к SPIFFS по FTP, логин/пароль: esp8266
#define OTA                                                 // Обновление по ОТА
//-----------------------------------
#define LAMP_PIN              (2U)                          // пин ленты                (D4)
#define BTN_PIN               (4U)                          // пин кнопки               (D2)
//#define BTN_PIN               (0U)                          // пин кнопки "FLASH" NodeMCU (ОТЛАДКА!!!) , подтяжка должна быть PULL_MODE=HIGH_PULL
#define MOSFET_PIN            (5U)                          // пин MOSFET транзистора   (D1) - может быть использован для управления питанием матрицы/ленты
#define ALARM_PIN             (16U)                         // пин состояния будильника (D0) - может быть использован для управления каким-либо внешним устройством на время работы будильника
#define MOSFET_LEVEL          (HIGH)                        // логический уровень, в который будет установлен пин MOSFET_PIN, когда матрица включена - HIGH или LOW
#define ALARM_LEVEL           (HIGH)                        // логический уровень, в который будет установлен пин ALARM_PIN, когда "рассвет"/будильник включен

#define WIDTH                 (16U)                         // ширина матрицы
#define HEIGHT                (16U)                         // высота матрицы

#define COLOR_ORDER           (GRB)                         // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE           (0U)                          // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE      (1U)                          // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION       (3U)                          // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
                                                            // при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
                                                            // шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/
#define NUM_LEDS              (uint16_t)(WIDTH * HEIGHT)
#define SEGMENTS              (1U)                          // диодов в одном "пикселе" (для создания матрицы из кусков ленты)
#define VERTGAUGE             (1U)                          // вертикальный/горизонтальный(1/0) индикатор, закомментировано - отключен
#define NUMHOLD_TIME          (3000U)                       // время запоминания последней комбинации яркости/скорости/масштаба в мс

#define BRIGHTNESS            (255U)                        // стандартная максимальная яркость (0-255)
#define CURRENT_LIMIT         (3000U)                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define FADERTIMEOUT 1000                                   // длительность всего фейдера в мс
#define FADERSTEPTIME 50                                    // длительность шага фейдера, мс

#define RANDOM_DEMO           (1)                           // 0,1 - последовательный (0)/рандомный (1) выбор режима демо
#define DEMO_TIMEOUT          (33)                          // время в секундах для смены режима DEMO
#define USELEDBUF                                           // буфер под эффекты, можно закомментировать, в случае если нужно сэкономить память, но будут артефакты обработки
#define EFFECTS_RUN_TIMER     (20U)                         // период обработки эффектов - 20мс, т.е. 1000/20 = 50 раз в секунду

// настройка кнопки
#ifdef ESP_USE_BUTTON
const PROGMEM byte PULL_MODE=HIGH_PULL;                     // подтяжка кнопки к питанию (для механических кнопок НО, на массу)
//const byte PULL_MODE=LOW_PULL;                            // подтяжка кнопки к нулю (для сенсорных кнопок на TP223)
#define BUTTON_STEP_TIMEOUT   (75U)                         // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#define BUTTON_CLICK_TIMEOUT  (500U)                        // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий
#define BUTTON_TIMEOUT        (500U)                        // с какого момента начинает считаться, что кнопка удерживается в мс
#endif

#define TIME_SYNC_INTERVAL    (60*60*1000)                  // интервал синхронизации времени, 60*60*1000 => раз в час

//#define NTP_ADDRESS           "pool.ntp.org"              // сервер времени
const char NTP_ADDRESS[] PROGMEM = "pool.ntp.org";     // сервер времени, можно также попробовать "ntp2.colocall.net"

#define TEXT_OFFSET           (4U)                          // высота, на которой бежит текст (от низа матрицы)
#define LET_WIDTH             (5U)                          // ширина буквы шрифта
#define LET_HEIGHT            (8U)                          // высота буквы шрифта
#define LET_SPACE             (1U)                          // пробел
#define LETTER_COLOR          (CRGB::White)                 // цвет букв по умолчанию
#define DEFAULT_TEXT_SPEED    (100U)                        // скорость движения текста, в миллисекундах - меньше == быстрее
// ************* НАСТРОЙКА МАТРИЦЫ *****
#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X (MIRR_V ? (WIDTH - x - 1) : x)
#define THIS_Y (MIRR_H ? (HEIGHT - y - 1) : y)

#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X (MIRR_V ? (HEIGHT - y - 1) : y)
#define THIS_Y (MIRR_H ? (WIDTH - x - 1) : x)

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X (MIRR_V ? (WIDTH - x - 1) : x)
#define THIS_Y (MIRR_H ?  x : (WIDTH - x - 1))

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (MIRR_V ? y : (HEIGHT - y - 1))
#define THIS_Y (MIRR_H ? (WIDTH - x - 1) : x)

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (MIRR_V ?  x : (WIDTH - x - 1))
#define THIS_Y (MIRR_H ? y : (HEIGHT - y - 1))

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (MIRR_V ? y : (HEIGHT - y - 1))
#define THIS_Y (MIRR_H ?  x : (WIDTH - x - 1))

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (MIRR_V ?  x : (WIDTH - x - 1))
#define THIS_Y (MIRR_H ? (HEIGHT - y - 1) : y)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X (MIRR_V ? (HEIGHT - y - 1) : y)
#define THIS_Y (MIRR_H ?  x : (WIDTH - x - 1))

#else
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y
#pragma message "Wrong matrix parameters! Set to default"

#endif
