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

#include "timeProcessor.h"

String TimeProcessor::getHttpData(const char *url)
{
  String payload(256);
  LOG(println, F("Time updating via HTTP..."));
  http.begin(client, url);
  
  int httpCode = http.GET();
  if (httpCode > 0){
    if (httpCode == HTTP_CODE_OK){
      payload = http.getString(); 
    } else {
        LOG(printf_P, PSTR("HTTPCode=%d\n"), httpCode);
        LOG(println, F("Forcing getString"));
		delay(1000);
		payload = http.getString(); 
	}
  }
  http.end();
  return payload;
}

void TimeProcessor::handleTime(bool force)
{
    static unsigned long timer;
    if (timer + TIME_SYNC_INTERVAL > millis() && !force)
        return;
    
    timer = millis();
    if(!getTimeJson(timer)){
        timer = millis();
        getTimeNTP(timer);
    }
}

String TimeProcessor::getFormattedShortTime()
{
    char buffer[10];
    time_t ut=getUnixTime();
    // unsigned long val, val_tm;

    // if(!isSynced){
    //     val = dirtytime*1000; val_tm = query_last_dirtytime_timer;
    // } else {
    //     val = (unixtime%86400UL+raw_offset)*1000; val_tm = query_last_timer;
    // }

    // //LOG.println((millis()-val_tm));
    // sprintf_P(buffer,PSTR("%02u:%02u"),(unsigned)((val+(millis()-val_tm))/(3600000UL)%24) // 3600*1000
    // ,(unsigned)((val+(millis()-val_tm))%(3600000UL))/(60000UL)); // 60*1000
    sprintf_P(buffer,PSTR("%02u:%02u"),hour(ut),minute(ut));
    return String(buffer);
}

void TimeProcessor::setTime(const char *var)
{
    char st[3];
    int val1, val2;

    st[0]=var[0]; st[1]=var[1]; st[2]='\0';
    val1 = atoi(st);
    st[0]=var[3]; st[1]=var[4]; st[2]='\0';
    val2 = atoi(st);

    dirtytime=(val1*3600+val2*60);
    query_last_dirtytime_timer=millis();
}

bool TimeProcessor::getTimeJson(unsigned long timer)
{
    String result;
    if(!strlen(timezone)){
        timer = millis();
        result = getHttpData(PSTR("http://worldtimeapi.org/api/ip"));
    }
    else {
        timer = millis();
        String tmpStr(PSTR("http://worldtimeapi.org/api/timezone/"));
        tmpStr+=timezone;
        result = getHttpData(tmpStr.c_str());
        if(result.length()<50){ // {"error":"unknown location"}
            timer = millis();
            result = getHttpData(PSTR("http://worldtimeapi.org/api/ip"));
        }
    }
    query_last_timer = millis()-((millis()-timer)/2); // значение в millis() на момент получения времени, со смещением на половину времени ушедшего на получение времени.
    DynamicJsonDocument doc(768);
    DeserializationError error = deserializeJson(doc, result);
    LOG(println, result);

    if (error) {
        LOG(print, F("deserializeJson error: "));
        LOG(println, error.code());
        return false;
    }

    if(!doc[F("error")].size()){
        week_number=doc[F("week_number")];
        day_of_week=doc[F("day_of_week")];
        day_of_year=doc[F("day_of_year")];
        unixtime=doc[F("unixtime")];
        raw_offset=doc[F("raw_offset")];
        dst_offset=doc[F("dst_offset")]; // смещение на летнее время, если есть
        if(unixtime) isSynced = true;
        sync = SYNC_TYPE::JSON_SYNC;
    } else {
        if(!unixtime) isSynced = false;
    }

    return isSynced;
}

bool TimeProcessor::getTimeNTP(unsigned long timer)
{
    WiFiUDP wifiUdp;
    char ntpNameBuffer[64]; // NTPClient почему-то падает в Exception (3) при попытке передать PSTR, поэтому ход конем - копируем во временный буффер
    strncpy_P(ntpNameBuffer, NTP_ADDRESS, sizeof(ntpNameBuffer)-1);
    NTPClient timeClient(wifiUdp, ntpNameBuffer, 0); // utcOffsetInSeconds
    //NTPClient timeClient(wifiUdp, String(FPSTR(NTP_ADDRESS)).c_str(), 0); // utcOffsetInSeconds
    Serial.println(F("Time updating via NTP..."));

    timeClient.begin();
    //delay(300);
    timer = millis();
    timeClient.update();
    if(timeClient.getEpochTime()<9999999) {  // что-то пошло не так, попытаемся еще раз c задержкой
        timer = millis();
        delay(300);
        timeClient.update();
    }

    LOG(println, timeClient.getEpochTime());

    if(timeClient.getEpochTime()>9999999){
        unixtime = timeClient.getEpochTime();
        if(unixtime) isSynced = true;
        query_last_timer = millis()-((millis()-timer)/2); // значение в millis() на момент получения времени, со смещением на половину времени ушедшего на получение времени.
        sync = SYNC_TYPE::NTP_SYNC;
        return true;
    } else {
        return false;
    }
}