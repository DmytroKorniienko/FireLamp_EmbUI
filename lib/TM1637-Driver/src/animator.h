#ifndef TM1637_ANIMATOR_H
#define TM1637_ANIMATOR_H

#include <Arduino.h>
#include "type_traits.h"
#include "mI2C.h"


class Animator
{

    struct Tasker
    {
        using duration_type = uint32_t;

        void reset(duration_type delay, duration_type start)
        {
            delay_ = delay;
            start_ = start;
        }

        bool check()
        {
            auto current = millis();
            if (current - start_ > delay_) {
                start_ = current;
                return true;
            }
            return false;
        }

    private:
        duration_type delay_;
        duration_type start_;
    };

    friend class TM1637;

    enum class DataCommand_e : uint8_t
    {
        FIXED_ADDRESS                   = 0x44,
        AUTOMATIC_ADDRESS_ADDING        = 0x40,
        READ_KEY_SCAN_DATA              = 0x42,
        WRITE_DATA_TO_DISPLAY_REGISTER  = 0x40,
        NORMAL_MODE                     = 0x40,
        TEST_MODE                       = 0x48
    };

    enum class AddressCommand_e : uint8_t
    {
        C0H                             = 0xC0,
        C1H,
        C2H,
        C3H,
        C4H,
        C5H,
        C6H                             // Ignore scan
    };

    enum class DisplayControl_e : uint8_t
    {
        PULSE_WIDTH_1_16                = 0x88,
        PULSE_WIDTH_2_16,
        PULSE_WIDTH_4_16,
        PULSE_WIDTH_10_16,
        PULSE_WIDTH_11_16,
        PULSE_WIDTH_12_16,
        PULSE_WIDTH_13_16,
        PULSE_WIDTH_14_16,
        DISPLAY_OFF                     = 0x80,
        DISPLAY_ON                      = PULSE_WIDTH_1_16
    };

    enum class Animation
    {
        NONE,
        BLINK,
        FADE_OUT,
        FADE_IN,
        SCROLL_LEFT
    };

public:

    Animator(uint8_t clkPin, uint8_t dataPin, uint8_t totalDigits): mi2C_(clkPin, dataPin), totalDigits_{totalDigits}
    {}

    void blink(Tasker::duration_type delay)
    {
        static DisplayControl_e savedBrightness;
        if (currentAnimation == Animation::BLINK) {
            if (tasker.check()) {
                brightness_ = brightness_ != DisplayControl_e::DISPLAY_OFF ? DisplayControl_e::DISPLAY_OFF : savedBrightness;
                refresh();
            }
            return;
        }
        savedBrightness = brightness_;
        tasker.reset(delay, millis());
        currentAnimation = Animation::BLINK;
    }

    void fadeOut(Tasker::duration_type delay)
    {
        if (currentAnimation == Animation::FADE_OUT) {
            if (tasker.check()) {
                auto current = control2Int(brightness_);
                if (current > 0) --current;
                brightness_ = fetchControl(current);
                refresh();
            }
            return;
        }
        tasker.reset(delay, millis());
        currentAnimation = Animation::FADE_OUT;
    }

    void fadeIn(Tasker::duration_type delay)
    {
        if (currentAnimation == Animation::FADE_IN) {
            if (tasker.check()) {
                if (brightness_ == DisplayControl_e::PULSE_WIDTH_14_16)
                    return;
                auto current = control2Int(brightness_);
                current++;
                brightness_ = fetchControl(current);
                refresh();
            }
            return;
        }
        tasker.reset(delay, millis());
        currentAnimation = Animation::FADE_IN;
    }

    void scrollLeft(Tasker::duration_type delay, byte k=1)  // Добавил возможность выставлять свой индекс (отображение нескольких цифр сразу)
    {
        if (currentAnimation == Animation::SCROLL_LEFT) {
            if (tasker.check()) {
                static unsigned int index = 0;
                if (index > buffer_.length()) {
                    currentAnimation = Animation::NONE;
                    index = 0;
                    return;
                }
                String s(buffer_.substring(index, buffer_.length()));
                index = index + k;
                for (size_t counter = s.length(); counter < totalDigits_; ++counter)
                    s.concat(static_cast<char>(0x00));
                sendToDisplay<DataCommand_e::AUTOMATIC_ADDRESS_ADDING, AddressCommand_e::C0H>(
                        brightness_,
                        reinterpret_cast<const uint8_t*>(s.c_str()),
                        static_cast<size_t>(min((unsigned int)totalDigits_, s.length())));
            }
            return;
        }
        tasker.reset(delay, millis());
        currentAnimation = Animation::SCROLL_LEFT;
    }

    void off() const
    {
        sendToDisplay<DataCommand_e::FIXED_ADDRESS, AddressCommand_e::C6H>(DisplayControl_e::DISPLAY_OFF);
    }

    void on(DisplayControl_e brightness) const
    {
        sendToDisplay<DataCommand_e::FIXED_ADDRESS, AddressCommand_e::C6H>(brightness);
    }

    void reset(const String& value) {
        buffer_ = "";
        for (decltype(value.length()) counter{}; counter < value.length(); ++counter) {
            auto d = toDisplayDigit(value[counter]);
            if (d == 0x80u && buffer_.length() > 0) {
                buffer_[buffer_.length() - 1] |= static_cast<char>(0x80u);
            } else
                buffer_.concat(static_cast<char>(d));
        }
        resetAnimation();
    }

    void clear()
    {
        buffer_ = "";
        for (size_t counter{}; counter < totalDigits_; ++counter)
            buffer_.concat(static_cast<char>(0x00u));
        refresh();
    }

    void refresh()
    {
        sendToDisplay<DataCommand_e::AUTOMATIC_ADDRESS_ADDING, AddressCommand_e::C0H>(
                brightness_,
                reinterpret_cast<const uint8_t*>(buffer_.c_str()),
                static_cast<size_t>(min((unsigned int)totalDigits_, buffer_.length())));
    }

private:
    inline uint8_t toDisplayDigit(signed char c) noexcept
    {
        return c < 0 ? 0x00 : ascii[static_cast<unsigned>(c)];
    }

    static inline uint8_t control2Int(DisplayControl_e e) noexcept
    {
        auto temp = static_cast<int>(e) - 0x87;
        return temp > 0 ? static_cast<uint8_t>(temp) : 0x00;
    }

    inline void resetAnimation() noexcept
    {
        currentAnimation = Animation::NONE;
    }

    static inline DisplayControl_e fetchControl(uint8_t value) noexcept
    {
#define CASER(val0, val1) \
        case val0:        \
            return DisplayControl_e::val1;
        switch (value) {
            CASER(0, DISPLAY_OFF)
            CASER(1, PULSE_WIDTH_1_16)
            CASER(2, PULSE_WIDTH_2_16)
            CASER(3, PULSE_WIDTH_4_16)
            CASER(4, PULSE_WIDTH_10_16)
            CASER(5, PULSE_WIDTH_11_16)
            CASER(6, PULSE_WIDTH_12_16)
            CASER(7, PULSE_WIDTH_13_16)
            default:
                return DisplayControl_e::PULSE_WIDTH_14_16;
        }
#undef CASER
    }

    template <DataCommand_e DATA_COMMAND, AddressCommand_e ADDRESS_COMMAND>
    void sendToDisplay(DisplayControl_e displayControl) const noexcept
    {
        mi2C_.beginTransmission();
        mi2C_.send(static_cast<uint8_t>(DATA_COMMAND));
        mi2C_.endTransmission();
        mi2C_.beginTransmission();
        mi2C_.send(static_cast<uint8_t>(ADDRESS_COMMAND));
        mi2C_.endTransmission();
        mi2C_.beginTransmission();
        mi2C_.send(static_cast<uint8_t>(displayControl));
        mi2C_.endTransmission();
    }

    template <DataCommand_e DATA_COMMAND, AddressCommand_e ADDRESS_COMMAND>
    void sendToDisplay(DisplayControl_e displayControl, const uint8_t* values, size_t size) const noexcept
    {
        mi2C_.beginTransmission();
        mi2C_.send(static_cast<uint8_t>(DATA_COMMAND));
        mi2C_.endTransmission();
        mi2C_.beginTransmission();
        mi2C_.send(static_cast<uint8_t>(ADDRESS_COMMAND));
        for (decltype(size) counter{}; counter < size; ++counter)
            mi2C_.send((colon_ || (dp_ & (1u << counter))) ? values[counter] | 0x80u : values[counter]);
        mi2C_.endTransmission();
        mi2C_.beginTransmission();
        mi2C_.send(static_cast<uint8_t>(displayControl));
        mi2C_.endTransmission();
    }

    String              buffer_ = "";
    MI2C                mi2C_;
    DisplayControl_e    brightness_ = DisplayControl_e::DISPLAY_ON;
    uint8_t             dp_;
    uint8_t             colon_;
    //const unsigned int  totalDigits_;
	const uint8_t		totalDigits_;
    Tasker              tasker;
    Animation           currentAnimation = Animation::NONE;
    const uint8_t ascii[128] =				// Массив изменен, добавлены все буквы латиницы
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x6D, 0x63, 0x00, 0x20,
        0x39, 0x0F, 0x61, 0x00, 0x10, 0x40, 0x80, 0x52, 0x3F, 0x06,
        0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00,
        0x00, 0x48, 0x00, 0x53, 0x00, 0x77, 0x7f, 0x39, 0x3f, 0x79,
        0x71, 0x3D, 0x76, 0x30, 0x1E, 0x72, 0x38, 0x55, 0x37, 0x3F,
        0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x6A, 0x1D, 0x6E,
        0x5B, 0x39, 0x64, 0x00, 0x0F, 0x08, 0x20, 0x5f, 0x7C, 0x58,
        0x5E, 0x7b, 0x71, 0x3D, 0x74, 0x04, 0x0e, 0x72, 0x38, 0x55,
        0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x1C, 0x6A,
        0x1D, 0x6E, 0x5B, 0x39, 0x30, 0x0F, 0x40, 0x00,
    };
};

#endif //TM1637_ANIMATOR_H
