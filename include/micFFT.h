/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_EmbUI.

    FireLamp_EmbUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_EmbUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_EmbUI.  If not, see <https://www.gnu.org/licenses/>.

(Цей файл є частиною FireLamp_EmbUI.

   FireLamp_EmbUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_EmbUI поширюється в надії, що вона буде корисною,
   але БЕЗ ВСЯКИХ ГАРАНТІЙ; навіть без неявної гарантії ТОВАРНОГО ВИГЛЯДУ
   або ПРИДАТНОСТІ ДЛЯ ВИЗНАЧЕНИХ ЦІЛЕЙ. Докладніше див. у Стандартній
   громадська ліцензія GNU.

   Ви повинні були отримати копію Стандартної громадської ліцензії GNU
   разом із цією програмою. Якщо це не так, див.
   <https://www.gnu.org/licenses/>.)
*/
#ifndef __MICFFT_H
#define __MICFFT_H

#ifdef CONFIG_IDF_TARGET_ESP32
#include "driver/adc_common.h"
#include "esp_adc_cal.h"
#define DEFAULT_VREF 1100
#endif

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
#if defined(FAST_ADC_READ) && defined(ESP8266)
  bool useFixedFreq = true; // использовать фиксированное семплирование, либо максимально возможное (false)
#else
  bool useFixedFreq = false; // использовать фиксированное семплирование, либо максимально возможное (false)
  esp_adc_cal_characteristics_t *adc_chars;
#endif
  bool _isCaliblation = false;
  float scale = 1.27; // 400 как средняя точка у меня, но надо будет калибравать для каждого случая отдельно калибровкой :)
  float noise = 0; // заполняется калибровкой, это уровень шума микрофона
  double signalFrequency = 700;
  uint32_t samplingFrequency = SAMPLING_FREQ; // частота семплирования для esp8266 (без разгонов) скорее всего не может быть выше 9500 если чтение через analogRead(MIC_PIN);

  const unsigned int sampling_period_us = round(1000000*(1.0/samplingFrequency));
  const uint8_t amplitude = 100;
  float *vReal = nullptr;
  float *vImag = nullptr;
  uint8_t minPeak = 0;
  uint8_t maxPeak = 0;
  float curVal = 0.0;
  ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency); /* Create FFT object */
  void PrintVector(float *vData, uint16_t bufferSize, uint8_t scaleType);
  void read_data();
  void debug();
public:
#ifdef FAST_ADC_READ
#ifdef ESP8266
  static const uint16_t samples=256U;     //This value MUST ALWAYS be a power of 2
#else
  static const uint16_t samples=256U;     //This value MUST ALWAYS be a power of 2
#endif
#else
  static const uint16_t samples=64U;     //This value MUST ALWAYS be a power of 2
#endif
  MICWORKER(float scale = 1.28, float noise = 0, bool withAnalyse=true) {
#if CONFIG_IDF_TARGET_ESP32
    //analogSetAttenuation(ADC_11db);
    //analogSetClockDiv(1); // fastest == 1
    //analogReadResolution(9);

  //Range 0-4096
  adc1_config_width(ADC_WIDTH_BIT_12);
  // full voltage range
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);

  // // check to see what calibration is available
  // if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
  // {
  //   Serial.println("Using voltage ref stored in eFuse");
  // }
  // if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
  // {
  //   Serial.println("Using two point values from eFuse");
  // }
  // if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_DEFAULT_VREF) == ESP_OK)
  // {
  //   Serial.println("Using default VREF");
  // }
  //Characterize ADC
  adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars); 
#endif
    this->vReal = new float[samples];
    if(withAnalyse)
      this->vImag = new float[samples];
    else
      this->vImag = nullptr;
    this->scale=scale;
    this->noise=noise;
  }
  ~MICWORKER() { if(vReal) delete [] vReal; if(vImag) delete [] vImag; }
  bool isCaliblation() {return _isCaliblation;}
  void calibrate();
  double process(MIC_NOISE_REDUCE_LEVEL level=MIC_NOISE_REDUCE_LEVEL::NR_NONE);
  double analyse();
  float getScale() {return scale;}
  float getNoise() {return noise;}
  float getFreq() {return signalFrequency;}
  float getCurVal() {return curVal;}
  uint8_t getMinPeak() {return minPeak;}
  uint8_t getMaxPeak() {return maxPeak;}
  float fillSizeScaledArray(float *arr, size_t size, bool bound=true);
};

#endif
