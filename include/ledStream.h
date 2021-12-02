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
static uint16_t getPixelNum(uint16_t x, uint16_t y);
const uint16_t getUni(){return e131Universe;}
void setUni(uint8_t uni){e131Universe = uni;}
const uint8_t getUniCt(){return universeCount;}
const uint8_t getLineQt(){return lineQt;}
uint8_t *getLastSeqNum(){return e131LastSequenceNumber;}
const uint16_t getPort(){return e131Port;}
const uint16_t getDMXAddress(){return DMXAddress;}
const STREAM_TYPE getStreamType(){return streamType;}
void fillBuff();
void fillBuff(const String &streamLeds);
CRGB *getBuff(){
    return bufLeds;
}
    STREAM_TYPE streamType = E131;
private:
    ESPAsyncE131 *e131 = nullptr;
    uint8_t* e131LastSequenceNumber = nullptr;       // to detect packet loss (9)
    CRGB *bufLeds = nullptr;
    // settings
    uint16_t e131Universe = 1;                       // settings for E1.31 (sACN) protocol (only DMX_MODE_MULTIPLE_* can span over consequtive universes)
    uint16_t e131Port = 5568;                        // DMX in port. E1.31 default is 5568, Art-Net is 6454
    uint16_t DMXAddress = 1;                         // DMX start address of fixture, a.k.a. first Channel [for E1.31 (sACN) protocol]
    bool e131Multicast = true;                    // multicast or unicast
    bool e131SkipOutOfSequence = true;            // freeze instead of flickering
    //uint8_t lastSeq;
    // bool ready = false;
    uint32_t connectError = false;
    // void handleE131Packet(e131_packet_t* p, const IPAddress &clientIP, bool isArtnet);
    const uint8_t lineQt = (512U / (WIDTH * 3));
    const uint8_t universeCount = ceil((float)HEIGHT / lineQt);
    uint32_t lastFrameTimer = 0;
    bool isConnected = false; 
};

extern Led_Stream *ledStream;
#endif
#endif