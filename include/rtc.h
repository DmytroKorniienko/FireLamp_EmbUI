#ifndef _RTC_H
#define _RTC_H

#include "config.h"
#include "iarduino_RTC.h"
#include "main.h"
#ifdef TM1637_CLOCK
#include "tm.h"
#endif

class Rtc : public iarduino_RTC {

public:

#if RTC_MODULE > (1U)
Rtc(uint8_t module = RTC_MODULE) : iarduino_RTC(module) {}
#endif

#if RTC_MODULE == (1U)
Rtc(uint8_t module = RTC_MODULE, uint8_t rst = pin_RST, uint8_t dat = pin_DAT, uint8_t clk = pin_CLK) : iarduino_RTC(module, rst, dat, clk) {}
#endif

void init();

void updateRtcTime();

private:

};

extern Rtc rtc;

#endif // RTC_H
