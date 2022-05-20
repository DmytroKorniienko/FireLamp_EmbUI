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
#include "main.h"
#ifndef _LED_STREAM_H_
#define _LED_STREAM_H_
#ifdef USE_STREAMING
#include "ESPAsyncE131.h"

enum STREAM_TYPE {
    E131,
    SOUL_MATE
};

class Led_Stream{
public:
    Led_Stream(const STREAM_TYPE type);
    ~Led_Stream();
    void handle();
    static void clearStreamObj();
    static void clearBuff();
    static void newStreamObj(STREAM_TYPE type);
    static uint16_t getPixelNumE131(uint16_t x, uint16_t y);
    const uint16_t getUni(){return firstUni;}
    void setUni(uint8_t uni){firstUni = uni;}
    const uint8_t getUniCt(){return uniQt;}
    const uint8_t getLineQt(){return lineQt;}
    void sendConfig(uint32_t id);
    uint8_t *getLastSeqNum(){return lastSeqNum;}
    const uint16_t getPort(){return e131Port;}
    const uint16_t getDMXAddress(){return DMXAddress;}
    const STREAM_TYPE getStreamType(){return streamType;}
    void fillLeds(CRGB *ledsPtr, uint16_t ledsQt, uint16_t lastLed = 0);
    void fillBuff();
    void fillBuff(const uint8_t *streamLeds);
    void clearWSbuff();
    static void handleE131Packet(e131_packet_t* p, const IPAddress &clientIP, bool isArtnet);
    static void handleWSPacket(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    // void onStreamEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    // CRGB *getBuff(){return bufLeds;}
    STREAM_TYPE streamType = E131;
private:
    ESPAsyncE131 *e131 = nullptr;
    // AsyncWebSocket wsStream;
    uint8_t* lastSeqNum = nullptr;       // to detect packet loss (9)
    CRGB *bufLeds = nullptr;
    uint8_t *wsBuff = nullptr;
    uint16_t buffSize;
    uint32_t id = 0;
    // settings
    uint16_t firstUni = 1;                       // settings for E1.31 (sACN) protocol (only DMX_MODE_MULTIPLE_* can span over consequtive universes)
    uint16_t e131Port = 5568;                        // DMX in port. E1.31 default is 5568, Art-Net is 6454
    uint16_t DMXAddress = 1;                         // DMX start address of fixture, a.k.a. first Channel [for E1.31 (sACN) protocol]
    bool multicast = true;                    // multicast or unicast
    // bool e131SkipOutOfSequence = true;            // freeze instead of flickering
    //uint8_t lastSeq;
    const uint8_t lineQt = (512U / (WIDTH * 3));
    const uint8_t uniQt = ceil((float)HEIGHT / lineQt);
    uint32_t lastFrameTimer = 0;
    bool isConnected = false; 
};

extern Led_Stream *ledStream;
#endif
#endif