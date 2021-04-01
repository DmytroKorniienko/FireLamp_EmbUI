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
#ifndef __CONFIG_H
#define __CONFIG_H
#if defined __has_include
#  if __has_include("user_config.h")
#    include "user_config.h" // <- пользовательские настройки, пожалуйста меняйте все что требуется там, ЭТОТ ФАЙЛ (config.h) НЕ МЕНЯЙТЕ
#  endif
#endif

#define SF(s) __SFSTR(s)
#define __SFSTR(s) #s
//#define SF(...) #__VA_ARGS__

#include <GyverButton.h>
// переместил в platformio.ini
// #ifdef ESP8266
// #define FASTLED_USE_PROGMEM             (1)
// #endif
#define FASTLED_INTERRUPT_RETRY_COUNT   (0)                 // default: 2; // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
#define FASTLED_ESP8266_RAW_PIN_ORDER                       // FASTLED_ESP8266_RAW_PIN_ORDER, FASTLED_ESP8266_D1_PIN_ORDER or FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ALLOW_INTERRUPTS      (0)                   // default: 1; // Use this to force FastLED to allow interrupts in the clockless chipsets (or to force it to disallow), overriding the default on platforms that support this. Set the value to 1 to allow interrupts or 0 to disallow them.
#include <FastLED.h>

//-----------------------------------
//#define ESP_USE_BUTTON                                      // если строка не закомментирована, должна быть подключена кнопка (иначе ESP может регистрировать "фантомные" нажатия и некорректно устанавливать яркость)
//#define LAMP_DEBUG                                          // режим отладки, можно также включать в platformio.ini
//#define DEBUG_TELNET_OUTPUT  (true)                         // true - отладочные сообщения будут выводиться в telnet вместо Serial порта (для удалённой отладки без подключения usb кабелем) // Deprecated
//#define USE_FTP                                             // доступ к LittleFS по FTP, логин/пароль: esp8266
//#define OTA                                                 // Обновление по ОТА
//#define MIC_EFFECTS                                         // Включить использование микрофона для эффектов
//#define MP3PLAYER                                           // Включить использование MP3 плеера (DF Player)
//#define SHOWSYSCONFIG                                       // Показывать системное меню
//#define DISABLE_LED_BUILTIN                                 // Отключить встроенный в плату светодиод, если нужно чтобы светил - закомментировать строку
typedef enum {NR_NONE,BIT_1,BIT_2,BIT_3,BIT_4} MIC_NOISE_REDUCE_LEVEL;
//-----------------------------------
#ifndef LANG
#define LANG                  "text_res-RUS.h"                  // Языковой файл по дефолту
#endif

#ifndef MIC_PIN
#ifdef ESP8266
#define MIC_PIN               (A0)                          // ESP8266 Analog Pin ADC0 = A0
#else
#define MIC_PIN               (GPIO_NUM_34)                 // ESP32 Analog Pin
#endif
#define FAST_ADC_READ                                       // использовать полный диапазон звуковых частот, если закомментировано, то будет до 5кГц, но сэкономит память и проще обсчитать...
#endif

#ifndef MIC_POLLRATE
#define MIC_POLLRATE          (50U)                         // как часто опрашиваем микрофон, мс
#endif

#ifndef HIGH_MAP_FREQ
#define HIGH_MAP_FREQ         (20000U)                      // верхняя граница слышимого диапазона, используется для мапинга, дефолтное и общепринятое значение 20000Гц
#endif

#ifdef FAST_ADC_READ
#ifndef SAMPLING_FREQ
#define SAMPLING_FREQ         (18000U*2U)
#endif
#else
#define SAMPLING_FREQ         (5000U*2U)
#endif

#ifndef LOW_FREQ_MAP_VAL
#define LOW_FREQ_MAP_VAL      (35U)                         // Граница логарифмически приведенных значений выше которой уже идут средние частоты (микрофон) ~150Гц
#endif

#ifndef HI_FREQ_MAP_VAL
#define HI_FREQ_MAP_VAL       (188U)                        // Граница логарифмически приведенных значени выше которой уже идут высокие частоты (микрофон) ~4.5кГц
#endif

#ifndef MIN_PEAK_LEVEL
#define MIN_PEAK_LEVEL        (50U)                         // Минимальный амплитудный уровень, для эффектов зависящих от микрофона
#endif

#ifdef MP3PLAYER
#ifdef ESP32
 #error ESP32 with DfPlayer is not (yet) supported due to softwareserial dependency (to be fixed)
#endif
#ifndef MP3_TX_PIN
#define MP3_TX_PIN            (D5)                         // TX mp3 player RX (D5)
#endif
#ifndef MP3_RX_PIN
#define MP3_RX_PIN            (D6)                         // RX mp3 player TX (D6)
#endif
#ifndef MP3_SERIAL_TIMEOUT
#define MP3_SERIAL_TIMEOUT    (300U)                       // 300мс по умолчанию, диапазон 200...1000, подбирается экспериментально, не желательно сильно повышать
#endif
#ifndef DFPALYER_START_DELAY
#define DFPALYER_START_DELAY  (500U)                       // 500мс по умолчанию, диапазон 10...1000, подбирается экспериментально, не желательно сильно повышать, безусловная задержка до инициализации
#endif
#endif

#ifndef LAMP_PIN
//#define LAMP_PIN              (2U)                          // пин ленты                (D4)
#ifdef ESP8266
#define LAMP_PIN              (D3)                          // пин ленты                (D3)
#endif
#endif

#ifndef BTN_PIN
#define BTN_PIN               (4U)                          // пин кнопки               (D2)
//#define BTN_PIN               (0U)                          // пин кнопки "FLASH" NodeMCU (ОТЛАДКА!!!) , подтяжка должна быть PULL_MODE=HIGH_PULL
#endif

#ifndef MOSFET_PIN
#define MOSFET_PIN            (5U)                          // пин MOSFET транзистора   (D1) - может быть использован для управления питанием матрицы/ленты
#endif
#ifndef ALARM_PIN
#define ALARM_PIN             (16U)                         // пин состояния будильника (D0) - может быть использован для управления каким-либо внешним устройством на время работы будильника
#endif
#ifndef MOSFET_LEVEL
#define MOSFET_LEVEL          (HIGH)                        // логический уровень, в который будет установлен пин MOSFET_PIN, когда матрица включена - HIGH или LOW
#endif
#ifndef ALARM_LEVEL
#define ALARM_LEVEL           (HIGH)                        // логический уровень, в который будет установлен пин ALARM_PIN, когда "рассвет"/будильник включен
#endif

#ifndef WIDTH
#define WIDTH                 (16U)                         // ширина матрицы
#endif
#ifndef HEIGHT
#define HEIGHT                (16U)                         // высота матрицы
#endif

#ifndef COLOR_ORDER
#define COLOR_ORDER           (GRB)                         // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB
#endif

#ifndef MATRIX_TYPE
#define MATRIX_TYPE           (0U)                          // тип матрицы: 0 - зигзаг, 1 - параллельная
#endif
#ifndef CONNECTION_ANGLE
#define CONNECTION_ANGLE      (1U)                          // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#endif
#ifndef STRIP_DIRECTION
#define STRIP_DIRECTION       (3U)                          // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
                                                            // при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
                                                            // шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/
#endif

#define NUM_LEDS              (uint16_t)(WIDTH * HEIGHT)    // не менять и не переопределять, служебный!

#ifndef SEGMENTS
#define SEGMENTS              (1U)                          // диодов в одном "пикселе" (для создания матрицы из кусков ленты)
#endif

//#define VERTGAUGE             (1U)                          // вертикальный/горизонтальный(1/0) индикатор, закомментировано - отключен
#ifndef NUMHOLD_TIME
#define NUMHOLD_TIME          (3000U)                       // время запоминания последней комбинации яркости/скорости/масштаба в мс
#endif

#ifndef BRIGHTNESS
#define BRIGHTNESS            (255U)                        // стандартная максимальная яркость (0-255)
#endif
#ifndef CURRENT_LIMIT
#define CURRENT_LIMIT         (2000U)                       // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#endif

#ifndef FADE_STEPTIME
#define FADE_STEPTIME         (50U)                         // default time between fade steps, ms (2 seconds with max steps)
#endif
#ifndef FADE_TIME
#define FADE_TIME             (2000U)                       // Default fade time, ms
#endif
#ifndef FADE_MININCREMENT
#define FADE_MININCREMENT     (3U)                          // Minimal increment for brightness fade
#endif
#ifndef FADE_MINCHANGEBRT
#define FADE_MINCHANGEBRT     (30U)                         // Minimal brightness for effects changer
#endif

#ifndef MAX_FPS
#define MAX_FPS               (60U)                         // Максимальное число обсчитываемых и выводимых кадров в секунду
#endif

#ifndef SPEED_ADJ
#define SPEED_ADJ (float)NUM_LEDS/256                         // Поправка скорости риал-тайм эффектов относительно размеров метрицы.
#endif

#define EFFECTS_RUN_TIMER   (uint16_t)(1000 / MAX_FPS)     // период обработки эффектов - при 10 это 10мс, т.е. 1000/10 = 100 раз в секунду, при 20 = 50 раз в секунду, желательно использовать диапазон 10...40

#ifndef DEFAULT_DEMO_TIMER
  #define DEFAULT_DEMO_TIMER  (60U)                         // интервал смены демо по-умолчанию
#endif


// настройка кнопки, если разрешена
#ifdef ESP_USE_BUTTON
#ifndef BUTTON_DEBOUNCE
#define BUTTON_DEBOUNCE       (30U)                         // Button debounce time, ms
#endif
#ifndef PULL_MODE
#define PULL_MODE             (LOW_PULL)                    // подтяжка кнопки к нулю (для сенсорных кнопок на TP223) - LOW_PULL, подтяжка кнопки к питанию (для механических кнопок НО, на массу) - HIGH_PULL
#endif
#ifndef BUTTON_STEP_TIMEOUT
#define BUTTON_STEP_TIMEOUT   (75U)                         // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#endif
#ifndef BUTTON_CLICK_TIMEOUT
#define BUTTON_CLICK_TIMEOUT  (500U)                        // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий
#endif
#ifndef BUTTON_TIMEOUT
#define BUTTON_TIMEOUT        (500U)                        // с какого момента начинает считаться, что кнопка удерживается в мс
#endif
#endif


// настройки времени
//#ifndef TZONE
//#define TZONE                 ("AUTO")
//#endif
#ifndef HTTPTIME_SYNC_INTERVAL
 #define HTTPTIME_SYNC_INTERVAL    (4)                           // интервал синхронизации времени по http, час
#endif

#ifndef CFG_AUTOSAVE_TIMEOUT
#define CFG_AUTOSAVE_TIMEOUT       (20*1000U)                   // таймаут сохранения конфигурации эффекта, по умолчанию - 20 секунд
#endif

#ifndef TEXT_OFFSET
#define TEXT_OFFSET           (4U)                          // высота, на которой бежит текст (от низа матрицы)
#endif
#ifndef LET_WIDTH
#define LET_WIDTH             (5U)                          // ширина буквы шрифта
#endif
#ifndef LET_HEIGHT
#define LET_HEIGHT            (8U)                          // высота буквы шрифта
#endif
#ifndef LET_SPACE
#define LET_SPACE             (1U)                          // пропуск между символами (кол-во пикселей)
#endif
#ifndef LETTER_COLOR
#define LETTER_COLOR          (CRGB::White)                 // цвет букв по умолчанию
#endif
#ifndef DEFAULT_TEXT_SPEED
#define DEFAULT_TEXT_SPEED    (100U)                        // скорость движения текста, в миллисекундах - меньше == быстрее
#endif
#ifndef FADETOBLACKVALUE
#define FADETOBLACKVALUE      (222U)                        // степень затенения фона под текстом, до 255, чем больше число - тем больше затенение.
#endif

// --- РАССВЕТ -------------------------
#ifndef DAWN_BRIGHT
#define DAWN_BRIGHT           (200U)                        // максимальная яркость рассвета (0-255)
#endif
#ifndef DAWN_TIMEOUT
#define DAWN_TIMEOUT          (1U)                          // сколько рассвет светит после времени будильника, минут
#endif
//#define PRINT_ALARM_TIME                                    // нужен ли вывод времени для будильника, если пустая строка в событии будильника

// ************* НАСТРОЙКА МАТРИЦЫ *****
#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X (MIRR_V ? (WIDTH - x - 1) : x)
#define THIS_Y (MIRR_H ? (HEIGHT - y - 1) : y)

#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define ROTATED_MATRIX
#define THIS_X (MIRR_V ? (HEIGHT - y - 1) : y)
#define THIS_Y (MIRR_H ? (WIDTH - x - 1) : x)

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X (MIRR_V ? (WIDTH - x - 1) : x)
#define THIS_Y (MIRR_H ?  y : (HEIGHT - y - 1))

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define ROTATED_MATRIX
#define THIS_X (MIRR_V ? y : (HEIGHT - y - 1))
#define THIS_Y (MIRR_H ? (WIDTH - x - 1) : x)

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (MIRR_V ?  x : (WIDTH - x - 1))
#define THIS_Y (MIRR_H ? y : (HEIGHT - y - 1))

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define ROTATED_MATRIX
#define THIS_X (MIRR_V ? y : (HEIGHT - y - 1))
#define THIS_Y (MIRR_H ?  x : (WIDTH - x - 1))

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (MIRR_V ?  x : (WIDTH - x - 1))
#define THIS_Y (MIRR_H ? (HEIGHT - y - 1) : y)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define ROTATED_MATRIX
#define THIS_X (MIRR_V ? (HEIGHT - y - 1) : y)
#define THIS_Y (MIRR_H ?  x : (WIDTH - x - 1))

#else
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y
#pragma message "Wrong matrix parameters! Set to default"

#endif

#ifdef TM1637
#ifndef TM_CLK_PIN
  #define TM_CLK_PIN D0 
#endif
#ifndef TM_DIO_PIN
  #define TM_DIO_PIN D7
#endif
#ifndef TM_BRIGHTNESS
  #define TM_BRIGHTNESS 7U //яркость дисплея, 0..7
#endif
#ifndef TM_SHOW_BANNER
  #define TM_SHOW_BANNER 0
#endif
#endif

#endif