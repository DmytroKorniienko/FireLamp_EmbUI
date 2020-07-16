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

#include <TZ.h>
#include <coredecls.h>                  // settimeofday_cb()
#include <sntp.h>

#ifdef ESP8266
 #include "ESP8266HTTPClient.h"
#endif

#ifdef ESP32
 #include "HTTPClient.h"
#endif

#ifndef TZONE
    #include "ArduinoJson.h"
#endif

TimeProcessor::TimeProcessor()
{
    eGotIPHandler = WiFi.onStationModeGotIP(std::bind(&TimeProcessor::onSTAGotIP, this, std::placeholders::_1));
    eDisconnectHandler = WiFi.onStationModeDisconnected(std::bind(&TimeProcessor::onSTADisconnected, this, std::placeholders::_1));

    settimeofday_cb(std::bind(&TimeProcessor::timeavailable, this));

    #ifdef TZONE
      //setTZ(TZONE);
      configTime(TZONE, NTP1ADDRESS, NTP2ADDRESS);
      LOG(print, F("Time Zone: "));      LOG(print, TZONE);
    #else
      configTime(TZ_Etc_GMT, NTP1ADDRESS, NTP2ADDRESS);
    #endif
    sntp_stop();    // отключаем ntp пока нет подключения к AP
}

String TimeProcessor::getFormattedShortTime()
{
    char buffer[6];
    sprintf_P(buffer,PSTR("%02u:%02u"), localtime(now())->tm_hour, localtime(now())->tm_min);
    return String(buffer);
}

/**
 * установка строки с текущей временной зоной в текстовом виде
 * влияет, на запрос через http-api за временем в конкретной зоне,
 * вместо автоопределения по ip
 */
void TimeProcessor::setTimezone(const char *var){
  if (!var)
    return;
  tzone = var;
}

/**
 * по идее это функция установки времени из гуя.
 * Но похоже что выставляет она только часы и минуты, и то не очень понятно куда?
 * надо переделать под выставление даты/веремени из браузера (например) когда нормально заработает гуй
 */
void TimeProcessor::setTime(const char *timestr){
    String _str(timestr);
    setTime(_str);
}

void TimeProcessor::setTime(const String &timestr){
    //"YYYY-MM-DDThh:mm:ss"    [19]
    if (timestr.length()<DATETIME_STRLEN)
        return;

    struct tm t;
    tm *tm=&t;

    tm->tm_year = timestr.substring(0,4).toInt() - TM_BASE_YEAR;
    tm->tm_mon = timestr.substring(5,7).toInt()-1;
    tm->tm_mday = timestr.substring(8,10).toInt();
    tm->tm_hour= timestr.substring(11,13).toInt();
    tm->tm_min = timestr.substring(14,16).toInt();
    tm->tm_sec = timestr.substring(17,19).toInt();

    timeval tv = { mktime(tm), 0 };
    settimeofday(&tv, NULL);
}


/**
 * установки системной временной зоны/правил сезонного времени.
 * по сути дублирует системную функцию setTZ, но работает сразу
 * со строкой из памяти, а не из PROGMEM
 * Может использоваться для задания настроек зоны/правил налету из
 * браузера/апи вместо статического задания Зоны на этапе компиляции
 * @param tz - указатель на строку в формате TZSET(3)
 * набор отформатированных строк зон под прогмем лежит тут
 * https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
 */
void TimeProcessor::tzsetup(const char* tz){
    // https://stackoverflow.com/questions/56412864/esp8266-timezone-issues
    if (!tz || !*tz)
             return;

    setenv("TZ", tz, 1/*overwrite*/);
    tzset();
    tzone = ""; // сбрасываем костыльную зону
    usehttpzone = false;  // запрещаем использование http
    LOG(printf_P, PSTR("TZSET rules changed to: %s\n"), tz);
}

#ifndef TZONE
/**
 * берем урл и записываем ответ в переданную строку
 * в случае если в коде ответа ошибка, обнуляем строку
 */ 
unsigned int TimeProcessor::getHttpData(String &payload, const String &url)
{
  WiFiClient client;
  HTTPClient http;
  LOG(println, F("TimeZone updating via HTTP..."));
  http.begin(client, url);

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK){
    payload = http.getString(); 
  } else {
    LOG(printf_P, PSTR("Time HTTPCode=%d\n"), httpCode);
  }
  http.end();
  return payload.length();
}

void TimeProcessor::getTimeHTTP()
{
    if (!usehttpzone)
        return;     // выходим если не выставлено разрешение на использование http

    String result((char *)0);
    result.reserve(TIMEAPI_BUFSIZE);
    if(tzone.length()){
        String url(FPSTR(PG_timeapi_tz_url));
        url+=tzone;
        getHttpData(result, url);
    }

    if(!result.length()){
        String url(FPSTR(PG_timeapi_ip_url));
        if(!getHttpData(result, url))
            return;
    }

    LOG(println, result);
    DynamicJsonDocument doc(TIMEAPI_BUFSIZE);
    DeserializationError error = deserializeJson(doc, result);
    result="";

    if (error) {
        LOG(print, F("Time deserializeJson error: "));
        LOG(println, error.code());
        return;
    }

    int raw_offset, dst_offset = 0;

    raw_offset=doc[F("raw_offset")];    // по сути ничего кроме текущего смещения от UTC от сервиса не нужно
                                        // правила перехода сезонного времени в формате, воспринимаемом системной
                                        // либой он не выдает, нужно писать внешний парсер. Мнемонические определения
                                        // слишком объемные для контроллера чтобы держать и обрабатывать их на лету.
                                        // Вероятно проще будет их запихать в js веб-интерфейса
    dst_offset=doc[F("dst_offset")];
    setOffset(raw_offset+dst_offset);

    if (!tzone.length()) {
        const char *tz = doc[F("timezone")];
        tzone+=tz;
    }
    LOG(printf_P, PSTR("HTTP TimeZone: %s, offset: %d, dst offset: %d\n"), tzone.c_str(), raw_offset, dst_offset);

    if (doc[F("dst_from")]!=nullptr){
        Serial.println("Zone has DST, rescheduling refresh");
        httprefreshtimer();
    }
}

void TimeProcessor::httprefreshtimer(const uint32_t delay){
    if (!usehttpzone){
        _wrk.detach();
        return;     // выходим если не выставлено разрешение на использование http
    }

    time_t timer;

    if (delay){
        timer = delay;
    } else {
        struct tm t;
        tm *tm=&t;
        localtime_r(now(), tm);

        tm->tm_mday++;                  // выставляем "завтра"
        tm->tm_hour= HTTP_REFRESH_HRS;
        tm->tm_min = HTTP_REFRESH_MIN;

        timer = (mktime(tm) - getUnixTime())% DAYSECONDS;

        LOG(printf_P, PSTR("Schedule TZ refresh in %d\n"), timer);
    }

    _wrk.once_scheduled(timer, std::bind(&TimeProcessor::getTimeHTTP, this));
}
#endif

/**
 * обратный вызов при подключении к WiFi точке доступа
 * запускает синхронизацию времени
 */
void TimeProcessor::onSTAGotIP(const WiFiEventStationModeGotIP ipInfo)
{
    sntp_init();
    #ifndef TZONE
        // отложенный запрос смещения зоны через http-сервис
        httprefreshtimer(HTTPSYNC_DELAY);
    #endif
}

void TimeProcessor::onSTADisconnected(const WiFiEventStationModeDisconnected event_info)
{
  sntp_stop();
  #ifndef TZONE
    _wrk.detach();
  #endif
}

void TimeProcessor::timeavailable(){
    LOG(println, F("Полученно точное время"));
    isSynced = true;
    if(_timecallback)
        _timecallback();
}

/**
 * функция допечатывает в переданную строку текущие дату/время в формате "YYYY-MM-DDThh:mm:ss"
 * 
 */
void TimeProcessor::getDateTimeString(String &buf){
  char tmpBuf[17];
  const tm* tm = localtime(now());
  sprintf_P(tmpBuf,PSTR("%04u-%02u-%02uT%02u:%02u"), tm->tm_year + TM_BASE_YEAR, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min);
  buf.concat(tmpBuf);
}

/**
 * установка текущего смещения от UTC в секундах
 */
void TimeProcessor::setOffset(const int val){
    LOG(printf_P, PSTR("Set time zone offset to: %d\n"), val);
    sntp_set_timezone_in_seconds(val);
    //sntp_set_timezone_in_seconds(-1*val);   // в правилах TZSET смещение имеет обратный знак (TZ-OffSet=UTC)
                                              // возможно это нужно будет учесть если задавать смещение для tz из правил (на будущее)
}

/**
 * Возвращает текущее смещение локального системного времени от UTC в секундах
 * с учетом часовой зоны и правил смены сезонного времени (если эти параметры были
 * корректно установленно ранее каким-либо методом)
 */
long int TimeProcessor::getOffset(){
    const tm* tm = localtime(now());
    auto tz = __gettzinfo();
    return *(tm->tm_isdst == 1 ? &tz->__tzrule[1].offset : &tz->__tzrule[0].offset);
}

void TimeProcessor::setcustomntp(const char* ntp){
    if (!ntp || !*ntp)
             return;

    sntp_setservername(CUSTOM_NTP_INDEX, (char*)ntp);
    LOG(printf_P, PSTR("Set custom NTP to: %s\n"), ntp);
}

void TimeProcessor::attach_callback(callback_function_t callback){
    _timecallback = std::move(callback);
}
