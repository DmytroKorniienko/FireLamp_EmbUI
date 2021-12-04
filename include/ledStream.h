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
void fillBuff();
void fillBuff(const uint8_t *streamLeds);
void clearWSbuff();
static void handleE131Packet(e131_packet_t* p, const IPAddress &clientIP, bool isArtnet);
static void handleWSPacket(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
// void onStreamEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
CRGB *getBuff(){
    return bufLeds;
}
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