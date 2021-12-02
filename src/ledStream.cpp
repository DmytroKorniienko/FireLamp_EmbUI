#include "ledStream.h"
#ifdef USE_STREAMING

Led_Stream *ledStream = nullptr;
// Task *clearTask = nullptr;           //  таска очищала буферы, если терялся коннект, переделал на handle(), который закинул в loop()

uint16_t Led_Stream::getPixelNum(uint16_t x, uint16_t y) {
  return (HEIGHT-1-y) * WIDTH + x;
}


Led_Stream::Led_Stream(const STREAM_TYPE type){
    LOG(printf_P, PSTR("Stream ON, type %d \n"), (uint8_t)type);
    streamType = type;
    if (type == E131) {
        e131Universe = embui.param(FPSTR(TCONST_0077)).toInt();
        e131 = new ESPAsyncE131([](e131_packet_t* p, const IPAddress &clientIP, bool isArtnet){
// void Led_Stream::handleE131Packet(e131_packet_t* p, const IPAddress &clientIP, bool isArtnet) {
            //E1.31 protocol support
            if (ledStream->getBuff() == nullptr || !myLamp.isLampOn())
            return;

            uint16_t uni = 0;
            uint8_t* e131_data = nullptr;
            uint8_t seq = 0;
            
            getUnsafeLedsArray();

            if (isArtnet) {
                uni = p->art_universe;
                e131_data = p->art_data;
                seq = p->art_sequence_number;
            } else {
                uni = htons(p->universe);
                e131_data = p->property_values;
                seq = p->sequence_number;
            }

            // only listen for universes we're handling & allocated memory
            if (uni >= (ledStream->getUni() + ledStream->getUniCt())) {
                return;
            }

            uint16_t previousUniverses = uni - ledStream->getUni();
            uint16_t possibleLEDsInCurrentUniverse  = WIDTH * ledStream->getLineQt();

            // if (ledStream->getLastSeqNum())
            //     if (seq < ledStream->getLastSeqNum()[uni - ledStream->getUni()]
            //             && seq > 20
            //             && ledStream->getLastSeqNum()[uni - ledStream->getUni()] < 250){
            //         LOG(printf_P, PSTR("skipping E1.31 frame (last seq=%d, current seq=%d, universe=%d)\n"), 
            //             ledStream->getLastSeqNum()[uni-ledStream->getUni()], seq, uni);
            //         return;
            //     }
            ledStream->getLastSeqNum()[uni-ledStream->getUni()] = seq;

            if (previousUniverses == 0) {
                // first universe of this fixture
                uint16_t count = min(possibleLEDsInCurrentUniverse, NUM_LEDS);
                memcpy(ledStream->getBuff(),
                    &e131_data[ledStream->getDMXAddress()],
                    count * 3);
            } 
            else if (previousUniverses > 0 && uni < (ledStream->getUni() + ledStream->getUniCt())) {
                // additional universe(s) of this fixture
                uint16_t numberOfLEDsInPreviousUniverses = possibleLEDsInCurrentUniverse;     // first universe
                if (previousUniverses > 1) {
                    numberOfLEDsInPreviousUniverses += possibleLEDsInCurrentUniverse * (previousUniverses - 1);  // extended universe(s) before current
                }
                uint16_t remainingLeds = NUM_LEDS - numberOfLEDsInPreviousUniverses;
                uint16_t count = min(possibleLEDsInCurrentUniverse, remainingLeds);
                memcpy(&ledStream->getBuff()[numberOfLEDsInPreviousUniverses],
                    &e131_data[ledStream->getDMXAddress()],
                    count * 3);
                if (uni == ledStream->getUniCt())
                    return ledStream->fillBuff();
            }
            });

    bufLeds = new CRGB[NUM_LEDS]{CRGB::Black};
    e131LastSequenceNumber = new uint8_t[universeCount];
    e131->begin(e131Multicast, e131Port, e131Universe, universeCount);
    connectError = millis();
    }
    if (myLamp.isDirect()){
        myLamp.effectsTimer(T_DISABLE);
        FastLED.clear();
        FastLED.show();
    }
#ifdef EXT_STREAM_BUFFER
    else
        myLamp.setStreamBuff(true);
#else
    else {
        if(myLamp.isDrawOn()) myLamp.setDraw(false);   // TODO: переделать
        myLamp.setDrawBuff(true);
    }
#endif
}


void Led_Stream::handle() {
    if (isConnected && millis() - lastFrameTimer > TASK_SECOND) {
        clearBuff(); isConnected = false; 
        if (myLamp.isDirect()) 
            myLamp.effectsTimer(T_ENABLE);
    }
}

void Led_Stream::fillBuff() {
    // if (clearTask) 
    //     clearTask->cancel();
    // if (!myLamp.isLampOn()) return;
    if (myLamp.isDirect()){
        if (!isConnected)
            myLamp.effectsTimer(T_DISABLE);
        if (myLamp.isMapping()){
            for(uint8_t x = 0; x < WIDTH; x++) {
                for (uint8_t y = 0; y < HEIGHT; y++) {
                    EffectMath::getPixel(x, y) = bufLeds[getPixelNum(x, y)];
                }
            }
        }
        else {
            memcpy(getUnsafeLedsArray(), bufLeds, NUM_LEDS);
        }
        FastLED.show();
    }
    else {
        if (myLamp.isMapping()){
            for(uint8_t x = 0; x < WIDTH; x++) {
                for (uint8_t y = 0; y < HEIGHT; y++) {
#ifdef EXT_STREAM_BUFFER
                    myLamp.writeStreamBuff(bufLeds[getPixelNum(x, y)], x, y);
#else
                    myLamp.writeDrawBuf(bufLeds[getPixelNum(x, y)], x, y);
#endif
                }
            }
        }
        else { 
            for (uint16_t i = 0; i < NUM_LEDS; i++)
#ifdef EXT_STREAM_BUFFER
                myLamp.writeStreamBuff(bufLeds[i], i);
#else
                myLamp.writeDrawBuf(bufLeds[i], i);
#endif
        }
    }
    lastFrameTimer = millis();
    isConnected = true;
    // clearTask = new Task(TASK_SECOND, TASK_ONCE, Led_Stream::clearBuff, &ts, false, nullptr, []{
    //         TASK_RECYCLE;
    //         clearTask = nullptr;
    //     });
    // clearTask->enableDelayed();
}

void Led_Stream::fillBuff(const String &str){
    // if (clearTask) 
    //     clearTask->cancel();
    // if (!myLamp.isLampOn()) return;
    if (myLamp.isDirect()){
        if (!isConnected)
            myLamp.effectsTimer(T_DISABLE);
        if (myLamp.isMapping()) {
            for(uint8_t x = 0; x < WIDTH; x++) {
                for (uint8_t y = 0; y < HEIGHT; y++) {
                    uint16_t i = EffectMath::getPixelNumberBuff((WIDTH-1) - x, y, WIDTH, HEIGHT) * 3;
                    EffectMath::getPixel(x, y) = CRGB((uint8_t)str[i], (uint8_t)str[i+1], (uint8_t)str[i+2]);
                }
            }
        }
        else {
            for (size_t i = 0; i < (str.length() < NUM_LEDS*3 ? str.length() : NUM_LEDS*3); i+=3) {
                leds[i/3] = CRGB((uint8_t)str[i], (uint8_t)str[i+1], (uint8_t)str[i+2]);
            }
        }
        FastLED.show();
    }
    else {
        if (myLamp.isMapping()) {
            for(uint8_t x = 0; x < WIDTH; x++) {
                for (uint8_t y = 0; y < HEIGHT; y++) {
                    uint16_t i = EffectMath::getPixelNumberBuff((WIDTH-1) - x, y, WIDTH, HEIGHT) * 3;
                    CRGB temp = CRGB((uint8_t)str[i], (uint8_t)str[i+1], (uint8_t)str[i+2]);
#ifdef EXT_STREAM_BUFFER
                    myLamp.writeStreamBuff(temp, x, y);
#else
                    myLamp.writeDrawBuf(temp, x, y);
#endif
                }
            }
        }
        for (size_t i = 0; i < (str.length() < NUM_LEDS*3 ? str.length() : NUM_LEDS*3); i+=3) {
            CRGB temp = CRGB((uint8_t)str[i], (uint8_t)str[i+1], (uint8_t)str[i+2]);
#ifdef EXT_STREAM_BUFFER
            myLamp.writeStreamBuff(temp, i/3);
#else
            myLamp.writeDrawBuf(temp, i/3);
#endif
        }
    }
    lastFrameTimer = millis();
    isConnected = true;
//     clearTask = new Task(TASK_SECOND, TASK_ONCE, Led_Stream::clearBuff, &ts, false, nullptr, []{
//             TASK_RECYCLE;
//             clearTask = nullptr;
//         });
//     clearTask->enableDelayed();
}

Led_Stream::~Led_Stream(){
    LOG(printf_P, PSTR("Stream OFF, Type %d \n"), streamType);
    Led_Stream::clearBuff();
    if (streamType == E131) {
        delete[] e131LastSequenceNumber;
        delete e131;
        delete[] bufLeds;
        e131LastSequenceNumber = nullptr;
        e131 = nullptr;
        bufLeds = nullptr;
    }
    if (myLamp.isDirect())
        myLamp.effectsTimer(T_ENABLE);
#ifdef EXT_STREAM_BUFFER
    else 
        myLamp.setStreamBuff(false);
#endif
}


void Led_Stream::clearStreamObj(){
    if (ledStream) {
        delete ledStream;
        ledStream = nullptr;
    }
}

void Led_Stream::clearBuff(){
    if (ledStream){
        if (myLamp.isDirect()){
            FastLED.clear();
            FastLED.show();
        }
        else {
#ifdef EXT_STREAM_BUFFER
            myLamp.clearStreamBuff();
#else
            myLamp.clearDrawBuf();
#endif
        }
    }
}

void Led_Stream::newStreamObj(STREAM_TYPE type){
    clearStreamObj();
    ledStream = new Led_Stream(type);
}

#endif