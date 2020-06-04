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


#include "fire_effect.h"

/*
void fire_drawFrame(bool isColored);

void fireRoutine(CRGB *leds, const char *param)
{
  if((millis() - myLamp.getEffDelay() - EFFECTS_RUN_TIMER) < (unsigned)((255-myLamp.effects.getSpeed())/3)){
    return;
  } else {
    myLamp.setEffDelay(millis());
  }
  
  fire_drawFrame(true);
}

void fire_shiftUp() {
  const uint8_t _plumestep = FIRE_HEIGHT + 1U;
  const uint8_t _plumeinc_min = (255.0 - FIRE_PLUMELIGHTMAX) / (float) FIRE_PLUMEHEIGHTMIN;
  const uint8_t _plumeinc_max = (255.0 - FIRE_PLUMELIGHTMIN) / (float) FIRE_PLUMEHEIGHTMAX ;
  const uint8_t _plumedec_min = (255.0 - FIRE_PLUMELIGHTMAX) / 2.0 ;
  const uint8_t _plumedec_max = (255.0 - FIRE_PLUMELIGHTMIN) / 2.0 ;

  for (uint8_t y = FIRE_HEIGHT; y > 0U; y--) {
    for (uint8_t x = 0U; x < WIDTH; x++) {
      matrixValue[y][x] = matrixValue[y - 1U][x];
    }
  }

  for (uint8_t x = 0U; x < WIDTH; x++) {
    if (matrixValue[_plumestep][x]) {
      matrixValue[0U][x] = (matrixValue[_plumestep][x] == 1U )
                           ? qsub8(matrixValue[0U][x], random8(_plumedec_min, _plumedec_max))
                           : qadd8(matrixValue[0U][x], random8(_plumeinc_min, _plumeinc_max));
      matrixValue[_plumestep][x]--;
    } else {
      uint8_t x1max = random8(FIRE_PLUMEWIDTHMIN, FIRE_PLUMEWIDTHMAX);
      uint8_t _plumeheight = random8(FIRE_PLUMEHEIGHTMIN, FIRE_PLUMEHEIGHTMAX);
      uint8_t mValue = random8(FIRE_PLUMELIGHTMIN, FIRE_PLUMELIGHTMAX);

      for (uint8_t x1 = 0U; x1 < x1max; x1++) {
        uint8_t x2 = (x + x1) % WIDTH;
        matrixValue[0U][x2] = ((x1 == 0) || (x1 == x1max)) ? qsub8(mValue, 24U) : mValue;
        matrixValue[_plumestep][x2] = _plumeheight;
      }
      x += x1max;
    }
  }
}

void fire_drawFrame(bool isColored) {                                         // прорисовка нового кадра

const uint8_t _coreshiftmin = FIRE_FLAMEWIDTH - FIRE_CORESHIFT ;
const uint8_t _coreshiftmax = FIRE_FLAMEWIDTH + FIRE_CORESHIFT ;
const uint8_t _flameshiftmin = FIRE_FLAMEWIDTH - FIRE_FLAMEAMPLITUDE ;
const uint8_t _flameshiftmax = FIRE_FLAMEWIDTH + FIRE_FLAMEAMPLITUDE ;
const uint8_t _corestrength = FIRE_CORESTRENGTH + 1U ;
const uint8_t _coreresist = FIRE_CORERESIST + 1U ;
const uint8_t _coreamplitude = FIRE_COREAMPLITUDE + 1U ;
const uint8_t _flamestrength = FIRE_FLAMESTRENGTH + 1U ;
const uint8_t _flameresist = FIRE_FLAMERESIST + 1U ;
const uint8_t _flameamplitude = FIRE_FLAMEAMPLITUDE + 1U ;
const uint8_t _sparkstrength = FIRE_SPARKSTRENGTH + 1U ;
const uint8_t _flameseed = FIRE_FLAMESEED ;
const uint8_t _coreoffset = FIRE_COREWIDTH + FIRE_OFFSET ;
const uint8_t _nofmasks = WIDTH / FIRE_COREWIDTH + (WIDTH % FIRE_COREWIDTH) ? 2U : 1U ;
const uint8_t _iseven = (FIRE_HEIGHT & 2U) ? 0U : 1U ;
const uint8_t _sparkdelay = FIRE_SPARKDELAY - 1U;
const uint8_t _plumestep = FIRE_HEIGHT + 1U;


uint8_t _mirrorstate [_nofmasks];
uint8_t _seedstep = 256.0 / _flameseed;
#if FIRE_FLAMEMIRROR < 201U                             // FLAMEMIRROR = 201
uint8_t _briefValue;
uint8_t _briefNewValue;
#else
uint8_t _briefValue[_nofmasks];
uint8_t _briefNewValue[_nofmasks];
#endif

// #if FIRE_SMOOTH
// static uint8_t _smooth = FIRE_SMOOTH;
// static uint8_t _smoothstep = 256.0 / FIRE_SMOOTH;
// #endif
// static uint8_t _sdelay = 128U;
// static uint8_t _darkness;
// static uint8_t _value;
// static uint8_t _windspeed = WIDTH;

#if FIRE_FLAMEMIRROR                                      // ***** FLAMEMIRROR Инициализация *****
#if FIRE_FLAMEMIRROR < 50U                                // FLAMEMIRROR = 1...49
uint8_t _shiftHue = FIRE_FLAMEWIDTH - 1U;
const uint8_t _flamecount = FIRE_FLAMEMIRROR ;
#elif FIRE_FLAMEMIRROR < 100U                             // FLAMEMIRROR = 50...99
uint8_t _shiftHue = FIRE_FLAMEWIDTH - 1U ;
const uint8_t _flamemirror = (FIRE_FLAMEMIRROR - 50U) * 2U ;
const uint8_t _flamecount = (_flamemirror / FIRE_FLAMECOEFF) + 1U ;
#elif FIRE_FLAMEMIRROR < 101U                             // FLAMEMIRROR = 100
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
uint8_t shiftHue[FIRE_HEIGHT] = {_shiftHue} ;
#elif FIRE_FLAMEMIRROR < 150U                             // FLAMEMIRROR = 101...149
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
const uint8_t _flamecount = FIRE_FLAMEMIRROR - 100U ;
uint8_t shiftHue[FIRE_HEIGHT] = {_shiftHue} ;           // массив дороожки горизонтального смещения пламени (hueMask)
#elif FIRE_FLAMEMIRROR < 200U                             // FLAMEMIRROR = 150...199
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
const uint8_t _flamemirror = (FIRE_FLAMEMIRROR - 150U) * 2U  ;
const uint8_t _flamecount = (_flamemirror / FIRE_FLAMECOEFF) + 1U ;
uint8_t shiftHue[FIRE_HEIGHT] = {_shiftHue} ;           // массив дороожки горизонтального смещения пламени (hueMask)
#elif FIRE_FLAMEMIRROR < 201U                             // FLAMEMIRROR = 200
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
uint8_t shiftHue[FIRE_HEIGHT] = {_shiftHue} ;           // массив дороожки горизонтального смещения пламени (hueMask)
#elif FIRE_FLAMEMIRROR < 202U                             // FLAMEMIRROR = 201
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
uint8_t shiftHue[FIRE_HEIGHT][_nofmasks] ;                // массив дороожки горизонтального смещения пламени (hueMask)
#else
uint8_t _shiftHue = FIRE_FLAMEWIDTH;
const uint8_t _flamemirror = (FIRE_FLAMEMIRROR - 201U) * 2U ;
const uint8_t _flamecount = (_flamemirror / FIRE_FLAMECOEFF) + 1U ;
uint8_t shiftHue[FIRE_HEIGHT][_nofmasks] ;                // массив дороожки горизонтального смещения пламени (hueMask)
#endif
#endif

#ifdef FIRE_UNIVERSE                                                          // если определен универсальный огонь вычисляем его базовый цвет  
  uint8_t baseHue = myLamp.effects.getScale();
  uint8_t baseSat = (myLamp.effects.getScale() == FIRE_WHITEVALUE) ? 0U : 255U ;  // отключение насыщенности при определенном положении колеса
#else
  uint8_t baseHue = isColored ? 255U : 0U;                                    // старое определение цвета
  uint8_t baseSat = 255;
#endif

#if FIRE_SMOOTH
  uint8_t _framemod = _framecounter % FIRE_SMOOTH;
  if (!_framemod) fire_shiftUp();
#else
  fire_shiftUp();
#endif

#if FIRE_BREATHSPEED
  for (uint8_t i = 0; i < _nofmasks; i++) {
    if (_briefValue[i] == _briefNewValue[i]) {
      _briefNewValue[i]  = random(FIRE_BREATHDEEP);
    } else {
      _briefValue[i] = (_briefValue[i] < _briefNewValue[i])
                       ?  min(qadd8(_briefValue[i], FIRE_BREATHSPEED ), _briefNewValue[i])
                       : max(qsub8(_briefValue[i], FIRE_BREATHSPEED), _briefNewValue[i]);
    }
  }
#else
  if (_briefValue == _briefNewValue) {
    _briefNewValue  = random(FIRE_BREATHDEEP);
  } else {
    _briefValue = (_briefValue < _briefNewValue)
                  ?  min(qadd8(_briefValue, FIRE_BREATHSPEED ), _briefNewValue)
                  : max(qsub8(_briefValue, FIRE_BREATHSPEED), _briefNewValue);
  }
#endif

#if FIRE_FLAMEMIRROR                                  // ***** HUE_mirror *****
#if FIRE_FLAMEMIRROR < 50U                            // _mirror = 1...49
  if (!(_rowcounter % _flamecount) && (_rowcounter % FIRE_HEIGHT)) _mirror = !_mirror ;
#elif FIRE_FLAMEMIRROR < 100U                         // _mirror = 50...99
  if (!random8(_flamemirror) && !(_rowcounter % _flamecount)) {
    _mirror = !_mirror ;
  }
#elif FIRE_FLAMEMIRROR < 101U
  _shiftHue = shiftHue[0];
#elif FIRE_FLAMEMIRROR < 150U                         // _mirror = 100...149  
  if (!(_rowcounter % _flamecount) && (_rowcounter % FIRE_HEIGHT)) _mirror = !_mirror ;
  _shiftHue = shiftHue[0];
#elif FIRE_FLAMEMIRROR < 200U                         // _mirror = 150...199  
  if (!random8(_flamemirror) && !(_rowcounter % _flamecount)) {
    _mirror = !_mirror ;
  }
  _shiftHue = shiftHue[0];
#elif FIRE_FLAMEMIRROR < 201U                         // _mirror = 200
  _shiftHue = shiftHue[0];
#elif FIRE_FLAMEMIRROR < 202U                         // _mirror = 201

#else                                                 // _mirror = 202...255
  if (!random8(_flamemirror) && !(_rowcounter % _flamecount)) {
    _mirror = !_mirror ;
  }
#endif
#endif

  //each row interpolates with the one before it
  for (uint8_t y = FIRE_HEIGHT - 1U; y < FIRE_HEIGHT; y--) {   // Work остальных строк
#if FIRE_FLAMEMIRROR                                  // ***** HUE_mirror *****
#if FIRE_FLAMEMIRROR < 50U                            // _mirror = 1...49
    if (!(_rowcounter % _flamecount) ) _mirror = !_mirror ;
#elif FIRE_FLAMEMIRROR < 100U                         // _mirror = 50...99
    if (!random8(_flamemirror) && !(_rowcounter % _flamecount)) _mirror = !_mirror ;
#elif FIRE_FLAMEMIRROR < 101U                         // _mirror = 100
    _shiftHue = shiftHue[y];
#elif FIRE_FLAMEMIRROR < 150U                         // _mirror = 101...149 
    if (!(_rowcounter % FIRE_FLAMEMIRROR)) _mirror = !_mirror ;
    _shiftHue = shiftHue[y];
#elif FIRE_FLAMEMIRROR < 151U                         // _mirror = 150
    _shiftHue = shiftHue[y];
#elif FIRE_FLAMEMIRROR < 200U                         // _mirror = 151...199 
    if (!random8(_flamemirror) && (_rowcounter % _flamecount)) _mirror = !_mirror ;
    _shiftHue = shiftHue[y];
#elif FIRE_FLAMEMIRROR < 201U                         // _mirror = 200
    _shiftHue = shiftHue[y];
#elif FIRE_FLAMEMIRROR < 202U                         // _mirror = 201
#else
    if (!random8(_flamemirror) && !(_rowcounter % _flamecount)) {
      _mirror = !_mirror ;
    }
#endif
#endif

    for (uint8_t x = 0U; x < WIDTH; x++) {            // прорисовка пламени
#if FIRE_FLAMEMIRROR == 0U                            // ***** HUE_mirror *****
      uint8_t _offset = x + _coreoffset;
      uint8_t z = _offset % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 50U                          // _mirror = 1...49
      uint8_t _offset = x + _coreoffset;
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 100U                         // _mirror = 50...99
      uint8_t _offset = x + _coreoffset;
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 101U                         // _mirror = 100
      //_shiftHue = shiftHue[y];
      uint8_t _offset = x + _coreoffset - _shiftHue;
      uint8_t z = _offset % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 150U                         // _mirror = 101...149      
      //_shiftHue = shiftHue[y];
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 151U                         // _mirror = 150
      //_shiftHue = shiftHue[y];
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 200U                         // _mirror = 151...199      
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 201U                         // _mirror = 200
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      _mirror = (x / FIRE_FLAMEWIDTH) % 2U;
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#elif FIRE_FLAMEMIRROR < 202U                         // _mirror = 201
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      _shiftHue = shiftHue[y][x / FIRE_FLAMEWIDTH];
      uint8_t z = qadd8(_offset, _shiftHue) % FIRE_FLAMEWIDTH;
#else                                                 // _mirror = 202...255
      uint8_t _offset = x + _coreoffset + shiftValue[y];
      _shiftHue = shiftHue[y][x / FIRE_FLAMEWIDTH];
      uint8_t z = (_mirror ? qsub8(_offset, _shiftHue) : qadd8(_offset, _shiftHue)) % FIRE_FLAMEWIDTH;
#endif

#if FIRE_BREATHSPEED
      _darkness = qadd8(_briefValue[x / FIRE_FLAMEWIDTH], random8(FIRE_CORESEED));
#else
      _darkness = qadd8(_briefValue, random8(FIRE_CORESEED));
#endif

#if FIRE_SMOOTH
      uint8_t _scale = qmul8(_smoothstep, _framemod);
      _value = qadd8( scale8( matrixValue[y + 1U][x] , qsub8( 255, _scale)), scale8( matrixValue[y][x] , _scale ) )  ;
#elif FIRE_SMOOTH == 1
      _value = avg8( matrixValue[y][x], matrixValue[y + 1U][x]);
#else
      _value = matrixValue[y + 1U][x];
#endif
      uint8_t _hue =  qadd8(random8(_flameseed), pgm_read_byte(&hueMask[y][z]));
#if FIRE_COREEDGE
      uint8_t _white = qsub8(_hue, FIRE_COREEDGE);
      uint8_t _whiteness = qsub8( baseSat, scale8( FIRE_COREDEEP, qsub8( 255, qmul8(_seedstep , _white))));
      uint8_t _saturation = _white ?  _whiteness : baseSat;
#else
      uint8_t _saturation = baseSat;
#endif

      myLamp.drawPixelXY(x, y, CHSV(                    // определение цвета пикселя
                    qadd8(_hue, baseHue) , // H - смещение всполохов
                    _saturation,           // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                    qsub8(_value, qadd8(pgm_read_byte(&valueMask[y][_offset % FIRE_FLAMEWIDTH]), _darkness)))); // V
    }
    if (y) {
      shiftValue[y] = shiftValue[y - 1];                 // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем

#if FIRE_FLAMEMIRROR                                   // ***** HUE_mirror *****
#if FIRE_FLAMEMIRROR < 50U
#elif FIRE_FLAMEMIRROR < 100U                          // _mirror = 51...99   
#elif FIRE_FLAMEMIRROR < 200U                          // _mirror = 151...199    
      shiftHue[y] = shiftHue[y - 1];                     // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем
#elif FIRE_FLAMEMIRROR < 201U                          // _mirror = 200
      shiftHue[y] = shiftHue[y - 1];
#elif FIRE_FLAMEMIRROR < 202U                          // _mirror = 201
      for (uint8_t i = 0; i < _nofmasks; i++) {
        shiftHue[y][i] = shiftHue[y - 1][i];
      }
#else
      for (uint8_t i = 0; i < _nofmasks; i++) {
        shiftHue[y][i] = shiftHue[y - 1][i];
      }
#endif
#endif
    }
    _rowcounter++;

  }
#if FIRE_CORESTRENGTH                                     // смещение очага 
  if (!random8(_corestrength) && !(_rowcounter % _coreresist)) {
    shiftValue[0] = constrain(qsub8(qadd8(shiftValue[0],  random8(_coreamplitude)),  random8(_coreamplitude)), _coreshiftmin, _coreshiftmax);
  }
#endif

#if FIRE_FLAMEMIRROR                                      // ***** HUE_mirror *****
#if FIRE_FLAMEMIRROR < 50U
#elif FIRE_FLAMEMIRROR < 100U                             // _mirror = 51...99
#elif FIRE_FLAMEMIRROR < 200U                             // _mirror = 151...199  
  if (!random8(_flamestrength) && !(_rowcounter % _flameresist)) {
    shiftHue[0] = constrain(qsub8(qadd8(shiftHue[0],  random8(_flameamplitude)),  random8(_flameamplitude)), _flameshiftmin, _flameshiftmax);
  }
#elif FIRE_FLAMEMIRROR < 201U                             // _mirror = 200  
  if (!random8(_flamestrength)  && !(_rowcounter % _flameresist)) {
    shiftHue[0] = constrain(qsub8(qadd8(shiftHue[0],  random8(_flameamplitude)),  random8(_flameamplitude)), _flameshiftmin, _flameshiftmax);
  }
#elif FIRE_FLAMEMIRROR < 202U                             // _mirror = 201
  for (uint8_t i = 0; i < _nofmasks; i++) {
    if (!random8(_flamestrength) && !((_rowcounter + i) % _flameresist) ) {
      shiftHue[0][i] = constrain(qsub8(qadd8(shiftHue[0][i],  random8(_flameamplitude)),  random8(_flameamplitude)), _flameshiftmin, _flameshiftmax);
    }
  }
#else                                                     // _mirror = 202...255 
  for (uint8_t i = 0; i < _nofmasks; i++) {
    if (!random8(_flamestrength) && !((_rowcounter + i) % _flameresist) ) {
      shiftHue[0][i] = constrain(qsub8(qadd8(shiftHue[0][i],  random8(_flameamplitude)),  random8(_flameamplitude)), _flameshiftmin, _flameshiftmax);
    }
  }
#endif
#endif

#if FIRE_SPARKLES  // если это самая нижняя строка искр - формитуем искорку из пламени
  if (!(_framecounter % _sparkdelay)) {
    for (uint8_t y = HEIGHT - 1U; y > FIRE_HEIGHT; y--) {
      for (uint8_t x = 0U; x < WIDTH; x++) {      // стираем строчку с искрами (очень не оптимально)
        myLamp.drawPixelXY(x, y, 0U);
      }
      for (uint8_t x = 0U; x < WIDTH; x++) {

        // #if FIRE_WINDCASE  // TODO
        //         if  (!random8(_sparkstrength) && ((_framecounter >> 8) % FIRE_WINDCASE )) {
        //           _windspeed = constrain(_windspeed + random8(2U) - random8(2U), WIDTH - 1U, WIDTH + 1U) % WIDTH;
        //         }
        //         _windspeed = 15U;
        //   #endif
#if FIRE_SPARKSTRENGTH
        uint8_t newX = (random8(_sparkstrength)) ? x : (x + WIDTH + random8(2U) - random8(2U)) % WIDTH ;   // с вероятностью 1/3 смещаем искорку влево или вправо
#else
        uint8_t newX = x;
#endif
        if (myLamp.getPixColorXY(x, y - 1U) > 0U) myLamp.drawPixelXY(newX, y, myLamp.getPixColorXY(x, y - 1U));    // рисуем искорку на новой строчке 
      }
    }
    for (uint8_t x = 0U; x < WIDTH; x++) {                  // если это не самая нижняя строка искр - перемещаем искорку выше
      if (random8(FIRE_EVERYNSPARK) == 0 && myLamp.getPixColorXY(x, FIRE_HEIGHT - 1U) != 0U) {
        uint16_t px = myLamp.getPixelNumber (x, FIRE_HEIGHT);
        myLamp.setLeds(px, CRGB(myLamp.getPixColorXY(x, FIRE_HEIGHT - random(0,FIRE_SPARKBORNDEEP)))/(uint8_t)(255/(!FIRE_SPARKSBRIGHT?1:FIRE_SPARKSBRIGHT)+1));
        //myLamp.setLeds(px, (unsigned int)myLamp.getLeds(px) % FIRE_SPARKSBRIGHT);
      } else myLamp.drawPixelXY(x, FIRE_HEIGHT, 0U);
    }
  }
#endif
  if (_rowcounter > 65521U) {
    _rowcounter = _rowcounter % 65521 + _iseven  ;
  }
  _framecounter++;
}

*/

void drawFrame(uint8_t pcnt, bool isColored);
void generateLine();
void shiftUp();

void fireRoutine(CRGB *leds, const char *param)
{
  if ((micros() - myLamp.getEffDelay_uS() - 5000U) < (50000U - map(myLamp.effects.getSpeed(), 1, 255, 0, 50000U))) {
    return;
  } else {
    myLamp.setEffDelay_uS(micros());
  }


  if (myLamp.isLoading()) {
    // лень обнулять :)
    // GSHMEM.line[WIDTH];
    // GSHMEM.shiftHue[HEIGHT];                              // массив дороожки горизонтального смещения пламени (hueMask)
    // GSHMEM.shiftValue[HEIGHT];                            // массив дороожки горизонтального смещения пламени (hueValue)
    // char GSHMEM.matrixValue[8][16];
    GSHMEM.pcnt = 0U;
    
    generateLine();
  }

  if (GSHMEM.pcnt >= 30) {                                  // внутренний делитель кадров для поднимающегося пламени
    shiftUp();                                              // смещение кадра вверх
    generateLine();                                         // перерисовать новую нижнюю линию случайным образом
    GSHMEM.pcnt = 0;
  }
  //  drawFrame(GSHMEM.pcnt, (strcmp(isColored, "C") == 0));           // прорисовка экрана
  drawFrame(GSHMEM.pcnt, true);                              // для прошивки где стоит логический параметр
  // GSHMEM.pcnt += speed_scale;  // делитель кадров: задает скорость подъема пламени 25/100 = 1/4
  GSHMEM.pcnt += 25;
}

// Randomly generate the next line (matrix row)
void generateLine() {
  for (uint8_t x = 0U; x < WIDTH; x++) {
    GSHMEM.line[x] = random8(127, 255);                             // заполнение случайным образом нижней линии (127, 255) - менее контрастное, (64, 255) - оригинал
  }
}

void shiftUp() {                                            //подъем кадра
  for (uint8_t y = HEIGHT - 1U; y > 0U; y--) {
    for (uint8_t x = 0U; x < WIDTH; x++) {
      uint8_t newX = x % 16U;                               // сократил формулу без доп. проверок
      if (y > 7U) continue;
      GSHMEM.matrixValue[y][newX] = GSHMEM.matrixValue[y - 1U][newX];     //смещение пламени (только для зоны очага)
    }
  }

  for (uint8_t x = 0U; x < WIDTH; x++) {                    // прорисовка новой нижней линии
    uint8_t newX = x % 16U;                                 // сократил формулу без доп. проверок
    GSHMEM.matrixValue[0U][newX] = GSHMEM.line[newX];
  }
}

// draw a frame, interpolating between 2 "key frames"
// @param pcnt percentage of interpolation

void drawFrame(uint8_t pcnt, bool isColored) {                  // прорисовка нового кадра
  int32_t nextv;
#ifdef UNIVERSE_FIRE                                            // если определен универсальный огонь  
  //  uint8_t baseHue = (float)modes[currentMode].Scale * 2.57;
  uint8_t baseHue = (myLamp.effects.getScale() - 1U) * 2.6;
#else
  uint8_t baseHue = isColored ? 255U : 0U;
#endif
  uint8_t baseSat = (myLamp.effects.getScale() < 255) ? 255U : 0U;  // вычисление базового оттенка

  uint8_t deltaValue = random8(0U, 3U) ? constrain (GSHMEM.shiftValue[0] + random8(0U, 2U) - random8(0U, 2U), 15U, 17U) : GSHMEM.shiftValue[0]; // random(0U, 3U)= скорость смещения очага чем больше 3U - тем медленнее                          // текущее смещение пламени (hueValue)
 
  //first row interpolates with the "next" line
  uint8_t deltaHue = random8(0U, 2U) ? constrain (GSHMEM.shiftHue[0] + random8(0U, 2U) - random8(0U, 2U), 15U, 17U) : GSHMEM.shiftHue[0]; // random(0U, 2U)= скорость смещения языков чем больше 2U - тем медленнее
  // 15U, 17U - амплитуда качания -1...+1 относительно 16U
  // высчитываем плавную дорожку смещения всполохов для нижней строки
  // так как в последствии координаты точки будут исчисляться из остатка, то за базу можем принять кратную ширину матрицы hueMask
  // ширина матрицы hueMask = 16, поэтому нам нужно получить диапазон чисел от 15 до 17
  // далее к предыдущему значению прибавляем случайную 1 и отнимаем случайную 1 - это позволит плавным образом менять значение смещения
  GSHMEM.shiftHue[0] = deltaHue;                                   // заносим это значение в стэк
  // 15U, 17U - амплитуда качания -1...+1 относительно 16U
  GSHMEM.shiftValue[0] = deltaValue;


  for (uint8_t x = 0U; x < WIDTH; x++) {                                          // прорисовка нижней строки (сначала делаем ее, так как потом будем пользоваться ее значением смещения)
    uint8_t newX = x % 16;                                                        // сократил формулу без доп. проверок
    nextv =                                                               // расчет значения яркости относительно valueMask и нижерасположенной строки.
      (((100.0 - pcnt) * GSHMEM.matrixValue[0][newX] + pcnt * GSHMEM.line[newX]) / 100.0)
      - pgm_read_byte(&valueMask[0][(x + deltaValue) % 16U]);
    CRGB color = CHSV(                                                            // вычисление цвета и яркости пикселя
                   baseHue + pgm_read_byte(&hueMask[0][(x + deltaHue) % 16U]),    // H - смещение всполохов
                   baseSat,                                                       // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                   (uint8_t)max(0, nextv)                                         // V
                 );
    myLamp.setLeds(myLamp.getPixelNumber(x, 0), color);                                            // прорисовка цвета очага
  }

  //each row interpolates with the one before it
  for (uint8_t y = HEIGHT - 1U; y > 0U; y--) {                                      // прорисовка остальных строк с учетом значения низлежащих
    deltaHue = GSHMEM.shiftHue[y];                                                         // извлекаем положение
    GSHMEM.shiftHue[y] = GSHMEM.shiftHue[y - 1];                                                  // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем
    deltaValue = GSHMEM.shiftValue[y];                                                     // извлекаем положение
    GSHMEM.shiftValue[y] = GSHMEM.shiftValue[y - 1];                                              // подготавлеваем значение смешения для следующего кадра основываясь на предыдущем


    if (y > 8U) {                                                                   // цикл стирания текущей строоки для искр
      for (uint8_t _x = 0U; _x < WIDTH; _x++) {                                     // стираем строчку с искрами (очень не оптимально)
        myLamp.drawPixelXY(_x, y, 0U);
      }
    }
    for (uint8_t x = 0U; x < WIDTH; x++) {                                          // пересчет координаты x для текущей строки
      uint8_t newX = x % 16U;                                                       // функция поиска позиции значения яркости для матрицы valueMask
      if (y < 8U) {                                                                 // если строка представляет очаг
        nextv =                                                                     // расчет значения яркости относительно valueMask и нижерасположенной строки.
          (((100.0 - pcnt) * GSHMEM.matrixValue[y][newX]
            + pcnt * GSHMEM.matrixValue[y - 1][newX]) / 100.0)
          - pgm_read_byte(&valueMask[y][(x + deltaValue) % 16U]);

        CRGB color = CHSV(                                                                  // определение цвета пикселя
                       baseHue + pgm_read_byte(&hueMask[y][(x + deltaHue) % 16U ]),         // H - смещение всполохов
                       baseSat,                                                             // S - когда колесо масштаба =100 - белый огонь (экономим на 1 эффекте)
                       (uint8_t)max(0, nextv)                                               // V
                     );
        myLamp.setLeds(myLamp.getPixelNumber(x, y), color);
      }
      else if (y == 8U && SPARKLES) {                                               // если это самая нижняя строка искр - формитуем искорку из пламени
        if (random8(0, 20) == 0 && myLamp.getPixColorXY(x, y - 1U) != 0U) myLamp.drawPixelXY(x, y, myLamp.getPixColorXY(x, y - 2U));  // 20 = обратная величина количества искр
        else myLamp.drawPixelXY(x, y, 0U);
      }
      else if (SPARKLES) {                                                          // если это не самая нижняя строка искр - перемещаем искорку выше
        // старая версия для яркости
        newX = (random8(0, 4)) ? x : (x + WIDTH + random8(0U, 2U) - random8(0U, 2U)) % WIDTH ;   // с вероятностью 1/3 смещаем искорку влево или вправо
        if (myLamp.getPixColorXY(x, y - 1U) > 0U) myLamp.drawPixelXY(newX, y, myLamp.getPixColorXY(x, y - 1U));    // рисуем искорку на новой строчке
      }
    }
  }
}

