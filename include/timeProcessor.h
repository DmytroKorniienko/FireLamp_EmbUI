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

#ifndef __TIMEPROCESSOR_H
#define __TIMEPROCESSOR_H

#include "config.h"
#include "misc.h"
#include "WiFiClient.h"
#include "ArduinoJson.h"

#ifdef ESP32
//#include <WiFi.h>
#include "HTTPClient.h"
#include "TimeLib.h"
#elif defined(ESP8266)
//#include <ESP8266WiFi.h>
#include "ESP8266HTTPClient.h"
#include "TimeLib.h"
//#include <TimeLib.h>
#endif

#include <WiFiUdp.h>
//#include <NTP.h>

#include <NTPClient.h>
//#include <Timezone.h>

// http://worldtimeapi.org/api/ip
// http://worldtimeapi.org/api/timezone

typedef enum _SYNC_TYPE {NOT_SYNC,JSON_SYNC,NTP_SYNC} SYNC_TYPE;

class TimeProcessor
{
private:
    SYNC_TYPE sync = SYNC_TYPE::NOT_SYNC;

    HTTPClient http;
    WiFiClient client;
    char timezone[32]="";
    String getHttpData(const char *url);

    unsigned int long query_last_timer;
    bool isSynced = false;
    bool isSyncOnline = true;

    byte week_number = 0;
    byte day_of_week = 0;
    int day_of_year = 0;
    unsigned long int unixtime = 0;
    int raw_offset = 0;
    int dst_offset = 0;

    unsigned int long query_last_dirtytime_timer;
    int dirtytime = 0; // приведено к разрешению в millis()

    bool getTimeJson(unsigned long timer);
    bool getTimeNTP(unsigned long timer);
public:
    void setTimezone(const char *var) { strncpy(timezone,var,sizeof(timezone)); }
    void setTime(const char *var);
    bool getIsSyncOnline() {return isSyncOnline;}
    void setIsSyncOnline(bool val) {isSyncOnline=val;}
    long getUnixTime() {return (isSynced?unixtime+raw_offset+dst_offset+(millis()-query_last_timer)/1000:dirtytime+raw_offset+dst_offset+(millis()-query_last_dirtytime_timer)/1000);}
    long getUTCUnixTime() {return (isSynced?unixtime+(millis()-query_last_timer)/1000:dirtytime+(millis()-query_last_dirtytime_timer)/1000);}
    int getOffset() {return raw_offset+dst_offset;}
    String getFormattedShortTime();
    void handleTime(bool force=false);
    void SetOffset(int val) { if(sync==NTP_SYNC) raw_offset=val;}
    bool isDirtyTime() {return sync==SYNC_TYPE::NOT_SYNC;}
    int getYear() { return year(getUnixTime()); }
};
#endif
