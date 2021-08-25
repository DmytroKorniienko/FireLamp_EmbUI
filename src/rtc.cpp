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