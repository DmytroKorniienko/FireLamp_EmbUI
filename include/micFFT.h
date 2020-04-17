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
#ifndef __MICFFT_H
#define __MICFFT_H


// Define this to use reciprocal multiplication for division and some more speedups that might decrease precision
#define FFT_SPEED_OVER_PRECISION

// Define this to use a low-precision square root approximation instead of the regular sqrt() call
// This might only work for specific use cases, but is significantly faster. Only works for ArduinoFFT<float>.
#define FFT_SQRT_APPROXIMATION

#include "arduinoFFT.h"
#include "config.h"
#include "misc.h"

class MICWORKER {
private:
  static const uint16_t samples = 128; //This value MUST ALWAYS be a power of 2
  bool useFixedFreq = false; // использовать фиксированное семплирование, либо максимально возможное (false)
  bool _isCaliblation = false;
  float scale = 1.28; // 400 как средняя точка у меня, но надо будет калибравать для каждого случая отдельно калибровкой :)
  float noise = 0; // заполняется калибровкой, это уровень шума микрофона
  double signalFrequency = 700;
  double samplingFrequency = 9000; // частота семплирования для esp8266 (без разгонов) скорее всего не может быть выше 9500... иначе расчет будет неправильным
  const unsigned int sampling_period_us = round(1000000*(1.0/samplingFrequency));
  const uint8_t amplitude = 100;
  float *vReal = nullptr;
  float *vImag = nullptr;
  int8_t minPeak = 0.0;
  int8_t maxPeak = 0.0;
  ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency); /* Create FFT object */
  void PrintVector(float *vData, uint16_t bufferSize, uint8_t scaleType);
  void read_data();
  void debug();
public:
  MICWORKER(float scale = 1.28, float noise = 0) { this->vReal = new float[samples]; this->vImag = new float[samples]; this->scale=scale; this->noise=noise; }
  ~MICWORKER() { delete [] vReal; delete [] vImag; }
  bool isCaliblation() {return _isCaliblation;}
  void calibrate();
  double process(MIC_NOISE_REDUCE_LEVEL level=MIC_NOISE_REDUCE_LEVEL::NONE);
  double analyse();
  float getScale() {return scale;}
  float getNoise() {return noise;}
  float getFreq() {return signalFrequency;}
  int8_t getMinPeak() {return minPeak;}
  int8_t getMaxPeak() {return maxPeak;}
};

#endif
