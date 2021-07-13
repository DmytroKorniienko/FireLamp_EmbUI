#ifndef TM1637_MI2C_H
#define TM1637_MI2C_H

#include <Arduino.h>

// Put a longer delay in case your TM1637 is having issues with timing due to onboard capacitors
#define TM1637_DELAY 0

class MI2C
{
public:
    MI2C(uint8_t clkPin, uint8_t dataPin) noexcept : clkPin_{clkPin}, dataPin_{dataPin}
    {}

    ~MI2C() = default;

    MI2C(const MI2C&) = delete;

    MI2C& operator=(const MI2C&) = delete;

    void begin() const noexcept
    {
        pinMode(clkPin_, OUTPUT);
        pinMode(dataPin_, OUTPUT);
    }

    void beginTransmission() const noexcept
    {
        digitalWrite(clkPin_, HIGH);
        digitalWrite(dataPin_, HIGH);
        delayMicroseconds(TM1637_DELAY);
        digitalWrite(dataPin_, LOW);
    }

    void endTransmission() const noexcept
    {
        digitalWrite(clkPin_, LOW);
        digitalWrite(dataPin_, LOW);
        digitalWrite(clkPin_, HIGH);
        delayMicroseconds(TM1637_DELAY);
        digitalWrite(dataPin_, HIGH);
    }

    uint8_t send(uint8_t value) const noexcept
    {
        uint8_t ACK;
        for (uint8_t i = 0; i < 8; ++i)
        {
            digitalWrite(clkPin_, LOW);
            digitalWrite(dataPin_, value & 0x01);
            delayMicroseconds(TM1637_DELAY);
            digitalWrite(clkPin_, HIGH);
            delayMicroseconds(TM1637_DELAY);
            value >>= 1u;
        }
        digitalWrite(clkPin_, LOW);
        digitalWrite(dataPin_, HIGH);
        digitalWrite(clkPin_, HIGH);
        pinMode(dataPin_, INPUT);
        if ((ACK = digitalRead(dataPin_)) == 0) {
            pinMode(dataPin_, INPUT);
            digitalWrite(dataPin_, LOW);
        }
        pinMode(dataPin_, OUTPUT);
        return ACK;
    }

private:
    const uint8_t clkPin_, dataPin_;
};

#endif //TM1637_MI2C_H
