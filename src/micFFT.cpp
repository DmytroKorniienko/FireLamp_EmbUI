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

#include "micFFT.h"

void MICWORKER::read_data()
{
  unsigned long m=micros(), _m=m;

  // /* Build raw data */
  // double cycles = (((samples-1) * signalFrequency) / samplingFrequency); //Number of signal cycles that the sampling will read
  // for (uint16_t i = 0; i < samples; i++)
  // {
  //   vReal[i] = int8_t((amplitude * (sin((i * (TWO_PI * cycles)) / samples))) / 2.0);/* Build data with positive and negative values*/
  //   //vReal[i] = uint8_t((amplitude * (sin((i * (twoPi * cycles)) / samples) + 1.0)) / 2.0);/* Build data displaced on the Y axis to include only positive values*/
  //   vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  // }

  for(uint16_t i=0; i<samples; i++){
    vReal[i] = analogRead(MIC_PIN); // ESP8266 Analog Pin ADC0 = A0

    if(useFixedFreq){ // используется фиксированное семплирование, организуем задержку
      while((micros() - m < sampling_period_us)){
        //empty loop
      }
      m += sampling_period_us;
    }
  }
  if(!useFixedFreq)
    samplingFrequency = ((1000UL*1000UL)/(micros()-_m))*(samples);
  //LOG.println(samplingFrequency);
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
    LOG.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      LOG.print(F("Hz"));
    LOG.print(F(" "));
    LOG.println(vData[i], 4);
  }
  LOG.println();
}

double MICWORKER::process(MIC_NOISE_REDUCE_LEVEL level)
{
  read_data();

  int minVal = 255.0;
  int maxVal = 0.0;
  for(uint16_t i=0; i<samples; i++){
    vReal[i]*=scale; // нормализация
    switch (level)
    {
    case MIC_NOISE_REDUCE_LEVEL::BIT_1:
      vReal[i] = map((uint16_t)vReal[i]&0xFE, 0, 1023, -127, 127); // маскируем один бит
      break;
    case MIC_NOISE_REDUCE_LEVEL::BIT_2:
      vReal[i] = map((uint16_t)vReal[i]&0xFC, 0, 1023, -127, 127); // маскируем два бита
      break;
    default:
      vReal[i] = map(vReal[i], 0, 1023, -127, 127); // без преобразований
      break;
    }
    minVal = min(minVal,abs(vReal[i]));
    maxVal = max(maxVal,abs(vReal[i]));
    vImag[i] = 0.0; // обнулить массив предыдущих измерений
  }
  minPeak = minVal; // минимальное амплитудное
  maxPeak = maxVal; // максимальное амплитудное
  return samplingFrequency; // частота семплирования
}

double MICWORKER::analyse()
{
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
  FFT.compute(FFTDirection::Forward); /* Compute FFT */
  FFT.complexToMagnitude(); /* Compute magnitudes */
  signalFrequency = FFT.majorPeak();
  return signalFrequency; // измеренная частота главной гармоники
}

void MICWORKER::debug()
{
  /* Print the results of the simulated sampling according to time */
  // LOG.println(F("Data:"));
  // PrintVector(data, samples, SCL_TIME);

  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);	/* Weigh data */
  // LOG.println(F("Weighed data:"));
  // PrintVector(data, samples, SCL_TIME);

  FFT.compute(FFTDirection::Forward); /* Compute FFT */
  // LOG.println(F("Computed Real values:"));
  // PrintVector(data, samples, SCL_INDEX);

  // LOG.println(F("Computed Imaginary values:"));
  // PrintVector(vImag, samples, SCL_INDEX);
  
  FFT.complexToMagnitude(); /* Compute magnitudes */
  LOG.println(F("Computed magnitudes:"));
  // PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);

  double x = FFT.majorPeak();
  LOG.println(x, 6);
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
  //LOG.print(F("dbgAVG Count="));LOG.print(count);LOG.print(F(", sum="));LOG.print(sum);LOG.print(F(", sum/count="));LOG.println(sum/count);

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
  //LOG.print(F("dispersion="));LOG.print(D);LOG.print(F(", standard error="));LOG.println(sqrt(D));

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
    scale = 512.0*count/sum; // смещение
    noise = (sum2/count2)*scale; //+/- единиц шума
    LOG.print(F("AVG="));LOG.print(sum/count);LOG.print(F(", noise="));LOG.println(sum2/count2);
  }
}