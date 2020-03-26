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
#ifndef __FIRE_EFFECT_H
#define __FIRE_EFFECT_H

#include "main.h"
/*
// ------------- The Flame / Огонь -----------------
// The Flame Effect(beta) for GyverLamp (FastLED) by PalPalych from 27-02-2020
#define FIRE_UNIVERSE                        // универсальный огонь 2-в-1 Цветной+Белый
#define FIRE_WHITEVALUE           (255U)     // значение регулятора "Масштаб" при котором отображается белое пламя #100U
#define FIRE_OFFSET               (4U)       // сдвиг маски огня относительно края. для того, чтобы разместить очаг по центру = FIRE_FLAMEWIDTH/2 #4U

#define FIRE_SMOOTH               (3U)       // сглаживание за счет увеличения промежуточных кадров (0...255, 0 - выключено), #3U
                                             // зависимые параметры: FIRE_CORESTRENGTH, FIRE_CORERESIST, FIRE_FLAMESTRENGTH,FIRE_CORERESIST,
                                             // FIRE_BREATHSPEED, FIRE_SPARKDELAY

#define FIRE_CORESHIFT            (8U)       // допустимый сдвиг очага в пикселях относительно его центральной позиции #4U
#define FIRE_CORESTRENGTH         (36U)      // устойчивость очага от сдвига по строкам (1...255 , 0 = отключить колыхание) #12U
#define FIRE_CORERESIST           (16U)      // устойчивость очага от сдвига по кадрам (1...255) #2U
#define FIRE_COREAMPLITUDE        (1U)       // амплитуда сдвига очага (1...FIRE_COREWIDTH/2) #1U
#define FIRE_CORESEED             (16U)      // величина зерна огня #16U  

#define FIRE_FLAMESTRENGTH        (8U)       // устойчивость пламени от сдвига по строкам (1...255 , 0 = отключить колыхание) #2U
#define FIRE_FLAMERESIST          (3U)       // устойчивость пламени от сдвига по кадрам (1...255) #1U
#define FIRE_FLAMEAMPLITUDE       (1U)       // амплитуда сдвига пламени (1...FIRE_FLAMEWIDTH/4) #1U
#define FIRE_FLAMESEED            (4U)       // величина зерна оттенка соседних пикселей огня #8U

#define FIRE_COREEDGE             (24U)      // порог срабатывания от hueMask с градиентом +FIRE_FLAMESEED для ообозначения сердцевины огня (1...255 , 0 = отключить) #1U
#define FIRE_COREDEEP             (64U)      // глубина белизны сердцевины огня #64U

#define FIRE_FLAMECOEFF           (4U)       // делитель фактов поворота зеркала для некоторых режимов FIRE_FLAMEMIRROR 
#define FIRE_FLAMEMIRROR          (201U)     // отзеркаливание маски оттенка очага #201U
                                             //   0...99 - столб пламени, без смещения оттенка по строкам,
                                             //     0 - выключение отзеркливания / столб пламени
                                             //     1...49 - шаговая диффузия с амплитудой FIRE_FLAMEAMPLITUDE,
                                             //     50...99 - случайная диффузия с амплитудой FIRE_FLAMEAMPLITUDE,
                                             //  101...199 - колышащее пламя, с учетом смещения оттенка по строкам,
                                             //     100 - выключение отзеркливания / колышащее пламя (базовое отключение)
                                             //     101...149 - диффузия со смещением FIRE_FLAMEAMPLITUDE,
                                             //     150...199 - случайная диффузия со смещением FIRE_FLAMEAMPLITUDE,
                                             //   200...255 - колышащее пламя для каждого очага
                                             //     200 - зеркальное для каждого второго очага
                                             //     201 - колышащее пламя для каждого очага
                                             //     202...254 - случайное отзеркаливание со скоростью переключения, 

#define FIRE_PLUMEHEIGHTMIN       (3U)      // высота языков пламени минимальная, влияет на визуальную скорость подъема пламени (0...255) #3U
#define FIRE_PLUMEHEIGHTMAX       (9U)      // высота языков пламени максимальная (FIRE_PLUMEHEIGHTMIN...255) #9U
#define FIRE_PLUMEWIDTHMIN        (1U)      // ширина языков пламени минимальная (0...255) #31U
#define FIRE_PLUMEWIDTHMAX        (4U)      // ширина языков пламени максимальная (FIRE_PLUMEWIDTHMIN...255) #31U
#define FIRE_PLUMELIGHTMIN        (192U)    // минимальная яркость темной части языка пламени (0...255) #160U
#define FIRE_PLUMELIGHTMAX        (224U)    // максимальная яркость темной части языка пламени (0...255) #192U

#define FIRE_BREATHDEEP           (64U)     // дыхание пламени (0...255) 0 - выключено  #64U
#define FIRE_BREATHSPEED          (2U)      // скорость дыхание пламени за 1 кадр (0...255) #4U

#define FIRE_SPARKLES             (true)    // вылетающие искры вкл/выкл
#define FIRE_SPARKSBRIGHT         (96U)    // первоначальная яркость искр (255...0) #200U
#define FIRE_EVERYNSPARK          (48U)     // только каждая единичная из указанных искр будет выводится (0...255) #64U
#define FIRE_SPARKSTRENGTH        (3U)      // стойкость искр относительно напора ветра (1... , 0 = отключить колыхание) #3U
#define FIRE_SPARKBORNDEEP        (2U)      // глубина от края пламени, на которой зарождаются искры #2U
#define FIRE_SPARKDELAY           (3U)      // замедлитель скорости искр #2U

//#define FIRE_WINDCASE             (2U)      // случайные порывы ветра каждый 256 кадр (1...255 , 0 = отключить)  #2U

// ----- неперемещаемые/неизменные оперделения -----
#define FIRE_COREWIDTH            (8U)      // ширина очага (маски)
#define FIRE_FLAMEWIDTH           (8U)      // ширина пламени (маски)
#define FIRE_HEIGHT               (9U)      // высота очага (маски)

//these values are substracetd from the generated values to give a shape to the animation
#if ((FIRE_COREWIDTH == 16U) && (FIRE_HEIGHT == 8U))
static const uint8_t valueMask[FIRE_HEIGHT][FIRE_COREWIDTH] PROGMEM = {
  {32 , 0  , 0  , 16 , 16 , 0  , 0  , 32 , 32 , 0  , 0  , 16 , 16 , 0  , 0  , 32 },
  {64 , 0  , 0  , 8  , 8  , 0  , 0  , 64 , 64 , 0  , 0  , 8  , 8  , 0  , 0  , 64 },
  {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
  {128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
  {160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
  {192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192},
  {255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255},
  {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}
};
#elif ((FIRE_COREWIDTH == 8U) && (FIRE_HEIGHT == 8U))
static const uint8_t valueMask[FIRE_HEIGHT][FIRE_COREWIDTH] PROGMEM = {
  {32 , 0  , 0  , 16  , 16  , 0  , 0  , 32 },
  {64 , 8  , 0  , 8   , 8   , 0  , 8  , 64 },
  {96 , 32 , 0  , 0   , 0   , 0  , 32 , 96 },
  {128, 64 , 32 , 0   , 0   , 32 , 64 , 128},
  {160, 96 , 64 , 32  , 32  , 64 , 96 , 160},
  {192, 128, 96 , 64  , 64  , 96 , 128, 192},
  {255, 160, 128, 96  , 96  , 128, 160, 255},
  {255, 192, 160, 128 , 128 , 160, 192, 255}
};
#elif ((FIRE_COREWIDTH == 8U) && (FIRE_HEIGHT == 9U)) //PalPalych's Optimal Mod for 16x16
static const uint8_t valueMask[FIRE_HEIGHT][FIRE_COREWIDTH] PROGMEM = {
  {16 , 0  , 8  , 16  , 16  , 8  , 0  , 16 },
  {32 , 0  , 0  , 16  , 16  , 0  , 0  , 32 },
  {64 , 8  , 0  , 8   , 8   , 0  , 8  , 64 },
  {96 , 32 , 0  , 0   , 0   , 0  , 32 , 96 },
  {128, 64 , 32 , 0   , 0   , 32 , 64 , 128},
  {160, 96 , 64 , 32  , 32  , 64 , 96 , 160},
  {192, 128, 96 , 64  , 64  , 96 , 128, 192},
  {255, 160, 128, 96  , 96  , 128, 160, 255},
  {255, 240, 192, 160 , 160 , 192, 240, 255}
};
#elif ((FIRE_COREWIDTH == 8U) && (FIRE_HEIGHT == 10U))
static const uint8_t valueMask[FIRE_HEIGHT][FIRE_COREWIDTH] PROGMEM = {
  {24 , 0  , 8  , 16  , 16  , 8  , 0  , 24 },
  {32 , 4  , 0  , 12  , 12  , 0  , 4  , 32 },
  {64 , 8  , 0  , 8   , 8   , 0  , 8  , 64 },
  {96 , 32 , 0  , 0   , 0   , 0  , 32 , 96 },
  {128, 64 , 32 , 0   , 0   , 32 , 64 , 128},
  {160, 96 , 64 , 32  , 32  , 64 , 96 , 160},
  {192, 128, 96 , 64  , 64  , 96 , 128, 192},
  {255, 160, 128, 96  , 96  , 128, 160, 255},
  {255, 192, 160, 128 , 128 , 160, 192, 255},
  {255, 240, 192, 160 , 160 , 192, 240, 255}
};
#else
#pragma message "Wrong value mask parameters!"
#endif

//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
#if ((FIRE_FLAMEWIDTH == 16U) && (FIRE_HEIGHT == 8U))
static const uint8_t hueMask[FIRE_HEIGHT][FIRE_FLAMEWIDTH] PROGMEM =  {
  {1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1},
  {1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1},
  {1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1},
  {1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1},
  {1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1},
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0},
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0},
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0}
};
#elif ((FIRE_FLAMEWIDTH == 8U) && (FIRE_HEIGHT == 8U))
static const uint8_t hueMask[FIRE_HEIGHT][FIRE_FLAMEWIDTH] PROGMEM = {
  {1 , 11, 19, 25, 25, 22, 11, 1 },
  {1 , 8 , 13, 19, 25, 19, 8 , 1 },
  {1 , 8 , 13, 16, 19, 16, 8 , 1 },
  {1 , 5 , 11, 13, 13, 13, 5 , 1 },
  {1 , 5 , 11, 11, 11, 11, 5 , 1 },
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
};
#elif ((FIRE_FLAMEWIDTH == 8U) && (FIRE_HEIGHT == 9U)) //PalPalych's Optimal Mod for 16x16
static const uint8_t hueMask[FIRE_HEIGHT][FIRE_FLAMEWIDTH] PROGMEM = {
  {7 , 21, 25, 26, 26, 25, 21, 7 },
  {3 , 17, 24, 26, 26, 24, 17, 3 },
  {2 , 13, 21, 25, 25, 21, 13, 2 },
  {1 , 11, 18, 23, 23, 18, 11, 1 },
  {1 , 9 , 14, 20, 20, 14, 9 , 1 },
  {1 , 5 , 10, 16, 16, 10, 5 , 1 },
  {0 , 2 , 7 , 11, 11, 7 , 2 , 0 },
  {0 , 1 , 3 , 6 , 6 , 3 , 1 , 0 },
  {0 , 0 , 1 , 2 , 2 , 1 , 0 , 0 }
};
#elif ((FIRE_FLAMEWIDTH == 8U) && (FIRE_HEIGHT == 10U))
static const uint8_t hueMask[FIRE_HEIGHT][FIRE_FLAMEWIDTH] PROGMEM = {
  {5 , 16, 25, 26, 26, 25, 16, 5 },
  {1 , 11, 22, 26, 26, 22, 11, 1 },
  {1 , 8 , 19, 25, 25, 19, 8 , 1 },
  {1 , 8 , 13, 19, 19, 16, 8 , 1 },
  {1 , 5 , 11, 13, 13, 13, 5 , 1 },
  {1 , 5 , 11, 11, 11, 11, 5 , 1 },
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
  {0 , 0 , 3 , 6 , 6 , 3 , 0 , 0 },
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
};
#else
#pragma message "Wrong hue mask parameters!"
#endif


uint8_t shiftValue[FIRE_HEIGHT];                          // массив дороожки горизонтального смещения пламени (hueValue)

uint8_t matrixValue[FIRE_HEIGHT + 2U][WIDTH];		// <- ******* для всех прошивок!!! закомментить предыдцщие инициализации и строки с упоминанием. Проверить глобальным поискоом!  *******
uint8_t line[WIDTH][2U];                                // буфер теневой маски
uint8_t pcnt;
uint16_t _rowcounter;
uint16_t _framecounter;
bool _mirror;

#if FIRE_SMOOTH
uint8_t _smooth = FIRE_SMOOTH;
uint8_t _smoothstep = 256.0 / FIRE_SMOOTH;
#endif
uint8_t _sdelay = 128U;
uint8_t _darkness;
uint8_t _value;
uint8_t _windspeed = WIDTH;
*/

#define SPARKLES              (1U)                     // вылетающие угольки вкл выкл
#define UNIVERSE_FIRE                                  // универсальный огонь 2-в-1 Цветной+Белый

//these values are substracetd from the generated values to give a shape to the animation
static const uint8_t valueMask[8][16] PROGMEM =
{
  {0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  },
  {0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  },
  {0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  },
  {0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  },
  {32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 },
  {64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 },
  {96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 },
  {128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128}
};

//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
static const uint8_t hueMask[8][16] PROGMEM =
{
  {25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25 },
  {25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19 },
  {19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16 },
  {13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13 },
  {11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11 },
  {8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8  },
  {5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5  },
  {1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1  }
};


#endif