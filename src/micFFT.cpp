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


#include "config.h"

#ifdef MIC_EFFECTS
#include "micFFT.h"
#ifdef ESP8266
#include "user_interface.h"
ADC_MODE(ADC_TOUT);
#endif


void MICWORKER::read_data()
{
  //uint16_t adc_addr[samples]; // point to the address of ADC continuously fast sampling output
  uint16_t adc_addr[1]; // point to the address of ADC continuously fast sampling output
  //uint16_t adc_num = samples; // sampling number of ADC continuously fast sampling, range [1, 65535]
  const uint8_t adc_clk_div = 8; // ADC working clock = 80M/adc_clk_div, range [1, 23], the recommended value is 8

  unsigned long m=micros(), _m=m;
  // /* Build raw data */
  // double cycles = (((samples-1) * signalFrequency) / samplingFrequency); //Number of signal cycles that the sampling will read
  // for (uint16_t i = 0; i < samples; i++)
  // {
  //   vReal[i] = int8_t((amplitude * (sin((i * (TWO_PI * cycles)) / samples))) / 2.0);/* Build data with positive and negative values*/
  //   //vReal[i] = uint8_t((amplitude * (sin((i * (twoPi * cycles)) / samples) + 1.0)) / 2.0);/* Build data displaced on the Y axis to include only positive values*/
  //   vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  // }

// #ifdef ESP8266
//   system_adc_read_fast(adc_addr, samples, adc_clk_div);
//   if(!useFixedFreq)
//     samplingFrequency = ((1000UL*1000UL)/(micros()-_m))*(samples)*1;
//   // EVERY_N_SECONDS(3) {
//   //   LOG(println, samplingFrequency);
//   // }
// #endif
  for(uint16_t i=0; i<samples; i++){
#if defined(ESP8266) && defined(FAST_ADC_READ)
    system_adc_read_fast(adc_addr, 1, adc_clk_div);
    vReal[i] = adc_addr[0]; // использую system_adc_read_fast для бОльшей скорости
#else
    vReal[i] = analogRead(MIC_PIN); // ESP8266 Analog Pin ADC0 = A0
#endif
    if(useFixedFreq){ // используется фиксированное семплирование, организуем задержку
      while((micros() - m < sampling_period_us)){
        //empty loop
      }
      m += sampling_period_us;
    }
  }
  if(!useFixedFreq)
    samplingFrequency = ((1000UL*1000UL)/(micros()-_m))*(samples);
  // EVERY_N_SECONDS(3) {
  //   LOG(println, samplingFrequency);
  // }
  FFT = ArduinoFFT<float>(vReal, vImag, samples, samplingFrequency);
}

void MICWORKER::PrintVector(float *vData, uint16_t bufferSize, uint8_t scaleType)
{
  typedef enum {
    SCL_INDEX=0x00,
    SCL_TIME=0x01,
    SCL_FREQUENCY=0x02,
    SCL_PLOT=0x03
  } FFT_DEBUG_EN;

  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa = 0.0;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
	      break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
	      break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
	      break;
      default:
        abscissa = (i * 1.0);
        break;
    }
    LOG(print, abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      LOG(print, F("Hz"));
    LOG(print, F(" "));
    LOG(println, vData[i], 4);
  }
  LOG(println, );
}

double MICWORKER::process(MIC_NOISE_REDUCE_LEVEL level)
{
  read_data();

#ifdef ESP8266
  const uint16_t RESOLUTION = 1023;
#else
  const uint16_t RESOLUTION = 4095;
#endif

  int minVal = 255.0;
  int maxVal = 0.0;
  for(uint16_t i=0; i<samples; i++){
    vReal[i]*=scale; // нормализация
    switch (level)
    {
    case MIC_NOISE_REDUCE_LEVEL::NR_NONE:
      vReal[i] = map(vReal[i], 0, RESOLUTION, -128, 127); // без преобразований
      break;
    case MIC_NOISE_REDUCE_LEVEL::BIT_1:
      vReal[i] = map((uint16_t)vReal[i], 0, RESOLUTION, -128, 127);
      vReal[i] = ((uint16_t)(abs((int16_t)vReal[i]))&0xFE)*(vReal[i]>0?1:-1); // маскируем один бита
      break;
    case MIC_NOISE_REDUCE_LEVEL::BIT_2:
      vReal[i] = map((uint16_t)vReal[i], 0, RESOLUTION, -128, 127);
      vReal[i] = ((uint16_t)(abs((int16_t)vReal[i]))&0xFC)*(vReal[i]>0?1:-1); // маскируем два бита
      break;
    case MIC_NOISE_REDUCE_LEVEL::BIT_3:
      vReal[i] = map((uint16_t)vReal[i], 0, RESOLUTION, -128, 127);
      vReal[i] = ((uint16_t)(abs((int16_t)vReal[i]))&0xF8)*(vReal[i]>0?1:-1); // маскируем три бита
      break;
    case MIC_NOISE_REDUCE_LEVEL::BIT_4:
      vReal[i] = map((uint16_t)vReal[i], 0, RESOLUTION, -128, 127);
      vReal[i] = ((uint16_t)(abs((int16_t)vReal[i]))&0xF0)*(vReal[i]>0?1:-1); // маскируем четыре бита
      break;
    default:
      break;
    }
    minVal = min(minVal,abs((int)vReal[i]));
    maxVal = max(maxVal,abs((int)vReal[i]));
  }
  minPeak = minVal; // минимальное амплитудное
  maxPeak = maxVal; // максимальное амплитудное
  curVal = vReal[0];
  return samplingFrequency; // частота семплирования
}

double MICWORKER::analyse()
{
  //memset(vImag,0,sizeof(samples*sizeof(*vImag))); // обнулить массив предыдущих измерений (так нельзя, ломает float)
  for(uint16_t i=0; i<samples; i++)
    vImag[i] = 0.0f;
  
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
  FFT.compute(FFTDirection::Forward); /* Compute FFT */
  FFT.complexToMagnitude(); /* Compute magnitudes */
  signalFrequency = FFT.majorPeak();
  return signalFrequency; // измеренная частота главной гармоники
}

float MICWORKER::fillSizeScaledArray(float *arr, size_t size, bool bound) // bound - ограничивать частотный диапазон или использовать 20-20000, по умолчанию - ограничивать
{
  float prevdata[size];
  for(size_t i=0; i<size; i++){
    prevdata[i] = arr[i];
    arr[i] = 0.0;
  }
  
  signalFrequency = (float)analyse(); // сюда запишем частоту главной гармоники

  // т.к. samplingFrequency удвоенная от реальной, то делим на 2, т.е -> samplingFrequency>>1
  float maxFreq = bound?log((samplingFrequency>>1)):log(20000);
  float minFreq = bound?log((samplingFrequency>>1)/samples):log(20);
  float scale = (size)/(maxFreq-minFreq);
  float step = samplingFrequency/samples;

  // EVERY_N_SECONDS(5){
  //   for(uint16_t i=0; i<samples; i++){
  //     LOG(printf_P, PSTR("x[%d]=%5.1f "),i, vReal[i]);
  //   }
  //   LOG(println);LOG(println);
  // }

  int16_t idx=0;
  for(uint16_t i=0; i<samples; i++){
    float idx_freq=step*(i+1);
    idx=(logf(idx_freq)-minFreq)*scale;
    idx=(idx<0?0:(idx>=(int16_t)size?size-1:(i<idx?i:idx)));
    arr[idx]+=(vReal[i]<0.0 ? 0.0 : vReal[i]);
  }

  // придушить ВЧ
  if(signalFrequency<(samplingFrequency>>1))
    arr[size-1]/=10.0;
  else if(signalFrequency<(samplingFrequency>>2))
    arr[size-1]/=20.0;
  
  float maxVal=0; // ищем максимум и усредняем с предыдущим измерением
  for(uint16_t i=0;i<size;i++){
    arr[i]=(arr[i]+prevdata[i])/2.0; // усредняем c предыдущим
    maxVal=max(maxVal,arr[i]);
  }

  // EVERY_N_SECONDS(1){
  //   for(uint16_t i=0;i<size;i++){
  //     LOG(printf_P, PSTR("x[%d]=%5.1f "),i, arr[i]);
  //   }
  //   LOG(println);
  // }

  return maxVal<0?0:maxVal;
}

void MICWORKER::debug()
{
  /* Print the results of the simulated sampling according to time */
  // LOG(println, F("Data:"));
  // PrintVector(data, samples, SCL_TIME);

  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
  // LOG(println, F("Weighed data:"));
  // PrintVector(data, samples, SCL_TIME);

  FFT.compute(FFTDirection::Forward); /* Compute FFT */
  // LOG(println, F("Computed Real values:"));
  // PrintVector(data, samples, SCL_INDEX);

  // LOG(println, F("Computed Imaginary values:"));
  // PrintVector(vImag, samples, SCL_INDEX);

  FFT.complexToMagnitude(); /* Compute magnitudes */
  LOG(println, F("Computed magnitudes:"));
  // PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);

  double x = FFT.majorPeak();
  LOG(println, x, 6);
}

void MICWORKER::calibrate()
{
  if(!_isCaliblation){
    // начальный вход в калибровку
    for(uint16_t i=0; i<samples; i++){
      vImag[i] = 0;
    }
    _isCaliblation = true;
  }

  // читаем сырые данные
  for(uint16_t i=0; i<samples; i++){
    vReal[i] = analogRead(MIC_PIN); // ESP8266 Analog Pin ADC0 = A0
  }

  double sum=0, count=0;
  for (uint16_t i=0;i<samples;i++) {
    double cnt=vReal[i];
    sum+=(double)vReal[i] * cnt;
    count+=cnt;
  }
  //LOG(print(F("dbgAVG Count="));LOG(print(count);LOG(print(F(", sum="));LOG(print(sum);LOG(print(F(", sum/count="));LOG(println, sum/count);

  double average = sum/count;
  double sumSq=0;
  count=0;
  for (uint16_t i=0;i<samples;i++) {
    double cnt=vReal[i];
    double a = sq(((double)vReal[i]-average)) * cnt;
    sumSq+=a;
    count+=cnt;
  }
  double D = sumSq / count; // dispersion
  if(D>500) return; // слишком большой разброс, не включаем данный замер...
  //sqrt(D); // standard error
  //LOG(print(F("dispersion="));LOG(print(D);LOG(print(F(", standard error="));LOG(println, sqrt(D));

  uint16_t step; // где мы сейчас
  for(step=0; step<samples/2; step++){ // делим на 2 диапазона AVG+stderr
    if(vImag[step]==0.0){
      vImag[step] = sum/count; // среднее
      vImag[samples/2+step] = sqrt(D); // среднеквадратическое отклонение
      return; // после первого найденного - на выход
    }
  }

  if(step==samples/2){ // массив заполнен, окончательный рассчет
    _isCaliblation = false;
    double sum=0, count=0, sum2=0, count2=0;
    for (uint16_t i=0;i<samples/2;i++) {
      double cnt=vImag[i];
      sum+=(double)vImag[i] * cnt;
      count+=cnt;

      double cnt2=vImag[samples/2+i];
      sum2+=(double)vImag[samples/2+i] * cnt2;
      count2+=cnt2;
    }
#ifdef ESP8266
    scale = 512.0*count/sum; // смещение
#else
    scale = 2048.0*count/sum; // смещение
#endif
    noise = (sum2/count2)*scale; //+/- единиц шума
    LOG(print, F("AVG=")); LOG(print, sum/count); LOG(print, F(", noise=")); LOG(println, sum2/count2);
  }
}

#endif  //def MIC_EFFECTS
