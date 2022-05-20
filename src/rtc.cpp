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

#include "rtc.h"
#ifdef RTC


Rtc rtc;

void Rtc::init() {
    begin();
    if (gettimeUnix() > 1600000000UL) {
        embui.timeProcessor.setTime(rtc.gettime("Y-m-dTH:i:s"));
        LOG(printf_P, PSTR("RTC READ %s \n"), rtc.gettime("Y-m-dTH:i:s"));
    }
    else {
        LOG(printf_P, PSTR("RTC not setted \n"));
        Task *t = new Task(10*TASK_SECOND, TASK_FOREVER, []{
            if (embui.timeProcessor.isDirtyTime()) return;
            if (rtc.gettimeUnix() > 1600000000UL) TASK_RECYCLE;
            const tm* t = localtime(embui.timeProcessor.now());  // Определяем для вывода времени
            rtc.settime(t->tm_sec, t->tm_min, t->tm_hour, t->tm_mday, t->tm_mon + 1, t->tm_year - 100);
            LOG(printf_P, PSTR("Sec %d min %d hour %d day %d mon %d year %d \n"), t->tm_sec, t->tm_min, t->tm_hour, t->tm_mday, t->tm_mon + 1, t->tm_year - 100);
            LOG(printf_P, PSTR("RTC setted %s \n"), rtc.gettime("Y-m-dTH:i:s"));
#ifdef TM1637_CLOCK
            tm1637.tm_setup();
#endif
            },
            &ts, false);
        t->enableDelayed();
    }
}

void Rtc::updateRtcTime() {
#ifdef EVERY_N_HOURS
    EVERY_N_HOURS(RTC_SYNC_PERIOD)
#else
    static uint32_t rtcUpdTimer;
    if (rtcUpdTimer + RTC_SYNC_PERIOD * 3600U * 1000U > millis())
    return;
    rtcUpdTimer = millis();
#endif
    {
        if (embui.timeProcessor.isDirtyTime())
            return;
        const tm* t = localtime(embui.timeProcessor.now());  // Определяем для вывода времени 
        settime(t->tm_sec, t->tm_min, t->tm_hour, t->tm_mday, t->tm_mon + 1, t->tm_year - 100);
        LOG(printf_P, PSTR("Sec %d min %d hour %d day %d mon %d year %d \n"), t->tm_sec, t->tm_min, t->tm_hour, t->tm_mday, t->tm_mon + 1, t->tm_year - 100);
        LOG(printf_P, PSTR("RTC setted %s \n"), rtc.gettime("Y-m-dTH:i:s"));
#ifdef TM1637_CLOCK
        tm1637.tm_setup();
#endif
    }
}
#endif