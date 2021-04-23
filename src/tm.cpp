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

#include "config.h"
#ifdef TM1637_CLOCK
#include "tm.h"


static bool showPoints = false;
static bool timeSetted = false;



#if TM_SHOW_BANNER
String splittedIp[5] = {};
// String welcome_banner = "FIRE_START"; // Список букв для вывода A Bb Cc Dd Ee F G Hh Ii J K Ll m Nn Oo P q r S t U v w x Y Z
/* Указывать можно в любом регистре, разделять лучше нижним подчеркиванием "_", если поставить пробел, то слова разделятся и будут отображаться по очереди, например сначала заскроллится "FIRE",
дойдет до конца, потухнет и только тогда появится "START"*/
#endif



void tm_setup() {
    tm1637.init();
    tm1637.begin();
    tm1637.setBrightness(TM_BRIGHTNESS);
    LOG(printf_P, PSTR("TM1637 was initialized \n"));
}


void tm_loop() {

#if TM_SHOW_BANNER
static uint8_t l = 0;           // Переменная для баннера
if (l < 255) l++;   // Добавляем счетчик и ограничиваем, чтобы не гонял по кругу
if (embui.sysData.wifi_sta && l <= 20 && l > 4) {
String ip = (String) "IP." + (String) WiFi.localIP().toString();
splitIp(ip, ".", splittedIp);
tm1637.display(formatIp(splittedIp, ""))->scrollLeft(500, 4); // Запуск баннера (хоть и задержка указана 500, по факту она 1 сек), индекс 4 (выводит 4 цифры за раз)
}
else if (!embui.sysData.wifi_sta && l <= 20 && l > 4) tm1637.display("__AP_192_168___4___1")->scrollLeft(500, 4);  // Если нет подключения, то крутим айпи точки доступа
else {
#endif

  if(!embui.sysData.wifi_sta && !timeSetted) {      // Светим --:--, если не подтянулось время с инета или не было настроено вручную
    auto d =  (showPoints) ? DisplayDigit().setG().setDot() : DisplayDigit().setG();
    const uint8_t rawBuffer[4] = {d, d, d, d};
    tm1637.displayRawBytes(rawBuffer, 4);
  }

else {
  const tm* t = localtime(embui.timeProcessor.now());  // Определяем для вывода времени 
  char dispTime[5];            // Массив для сбора времени

  #ifdef TM_24
  #ifdef TM_ZERO                                              // Заполнение 0 справа
    if (showPoints)
      sprintf(dispTime,"%02d.%02d", t->tm_hour, t->tm_min);  // Заполняем массив данными (обязательно с ., чтобы выводить с ее помощью : на дисплее)
    else
      sprintf(dispTime,"%02d%02d", t->tm_hour, t->tm_min);  // Тоже самое, только уже без . (чтобы было мигание : )

    String timeDisp(dispTime);                              // Из массива делаем строку
    tm1637.display(timeDisp);                               // Выводим на дисплей
  #endif

  #ifndef TM_ZERO
    if (showPoints)
      sprintf (dispTime,"%01d.%02d", t->tm_hour, t->tm_min);
    else
      sprintf (dispTime,"%01d%02d", t->tm_hour, t->tm_min);

    String timeDisp(dispTime);
    if(t->tm_hour < 10)
      tm1637.display(timeDisp, true, false, 1);                // Выводим время с отступом справа
    else
      tm1637.display(timeDisp);
  #endif
  #endif

  #ifndef TM_24
    #ifdef TM_ZERO
    if (showPoints)
      sprintf (dispTime,"%02d.%02d", (t->tm_hour > 12) ? t->tm_hour - 12 : t->tm_hour, t->tm_min);      // Выводим время и отнимаем 12 от часов, если значение выше 12
    else
      sprintf (dispTime,"%02d%02d", (t->tm_hour > 12) ? t->tm_hour - 12 : t->tm_hour, t->tm_min);
      String timeDisp (dispTime);
      tm1637.display(timeDisp);
    #endif


    #ifndef TM_ZERO
    if (showPoints)
      sprintf(dispTime,"%01d.%02d", (t->tm_hour > 12) ? t->tm_hour - 12 : t->tm_hour, t->tm_min);
    else
      sprintf(dispTime,"%01d%02d", (t->tm_hour > 12) ? t->tm_hour - 12 : t->tm_hour, t->tm_min);

    String timeDisp (dispTime);
    if(                                                                                                // Условия для правильного отображения времени
      t->tm_hour > 12
      &&
      t->tm_hour < 22
      ||
      t->tm_hour < 10
    )
      tm1637.display(timeDisp, true, false, 1);
    else
      tm1637.display(timeDisp);
    #endif
  #endif
  }
  #if TM_SHOW_BANNER  
  }
  #endif
  showPoints=!showPoints;


}

void tm_setted() {        // Проверяем, было ли установлено время
  timeSetted++;
}


// | FUNC - Split IP
// |----------
void splitIp(String str, String dlm, String dest[])
{
  int nextPos      = str.indexOf(dlm);

  for(short i=0; i<5; i++)
  {
    dest[i] = str.substring(0, nextPos);

    str     = str.substring(nextPos+1);
    nextPos = str.indexOf(dlm);
  }
}


// | FUNC - Format IP
// |----------
String formatIp(String inArr[], String dlm)
{
  String tmp    = "____";
  String output = "";

  for(short i=0; i<5; i++){
    String crnt = inArr[i];

    for(short j=0; j<crnt.length(); j++){
      tmp.setCharAt(tmp.length()-j-1, crnt.charAt(crnt.length()-j-1));
    }

    output += tmp;
    if(i<3){
      output += dlm;
    }

    tmp    = "____";
  }

  return output;
}

#endif
