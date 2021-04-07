#ifndef TM1637_TM1637_H
#define TM1637_TM1637_H

#include <Arduino.h>
#include "type_traits.h"
#include "animator.h"

#ifndef TM1637_LEGACY
#define TM1637_LEGACY 1
#endif

#if __GNUC__ >= 3
#define DEPRECATED          __attribute__((deprecated))
#else
#define DEPRECATED
#endif

struct DisplayDigit
{
    DisplayDigit& setA() { value |= 0x01; return *this; }
    DisplayDigit& setB() { value |= 0x02; return *this; }
    DisplayDigit& setC() { value |= 0x04; return *this; }
    DisplayDigit& setD() { value |= 0x08; return *this; }
    DisplayDigit& setE() { value |= 0x10; return *this; }
    DisplayDigit& setF() { value |= 0x20; return *this; }
    DisplayDigit& setG() { value |= 0x40; return *this; }
    DisplayDigit& setDot() { value |= 0x80; return *this; }
    operator uint8_t() { return value; }
    DisplayDigit& operator=(uint8_t rhs) { value = rhs; return *this; }

    uint8_t value{};
};


class TM1637
{
    friend Animator;

public:
    static constexpr uint8_t TOTAL_DIGITS = 4;

    TM1637(uint8_t clkPin, uint8_t dataPin) noexcept: animator_(clkPin, dataPin, TOTAL_DIGITS)
    {};
    TM1637(const TM1637&) = delete;
    TM1637& operator=(const TM1637&) = delete;
    ~TM1637() = default;
    void begin()
    {
        animator_.mi2C_.begin();
        offMode();
    };
    inline void init() { begin(); }
    inline Animator* refresh() { animator_.refresh(); return &animator_; }

    template <typename T>
    typename type_traits::enable_if<
        type_traits::is_string<T>::value ||
        type_traits::is_floating_point<T>::value ||
        type_traits::is_integral<T>::value,
        Animator*>::type
    display(const T value, bool overflow = true, bool pad = false, uint8_t offset = 0)
    {
        String temp = stringer<T>(value);
        if (temp == cache_)
            return &animator_;
        cache_ = temp;
        String cache;
        cache = "";
        for (decltype(offset) counter{}; counter < offset; ++counter)
            cache.concat(static_cast<char>(0x00));
        cache.concat(temp);
        if (pad)
            for (size_t counter = cache.length(); counter < animator_.totalDigits_; ++counter)
                cache.concat(static_cast<char>('O'));
        if (!overflow)
            cache.substring(0, animator_.totalDigits_);
        animator_.reset(cache);
        animator_.refresh();
        return &animator_; // TODO: Use weak_ptr
    }

    Animator* displayRawBytes(const uint8_t* buffer, size_t size)
    {
        cache_ = "";
        animator_.buffer_ = "";
        for (decltype(size) counter{}; counter < size; ++counter)
            animator_.buffer_.concat(static_cast<char>(buffer[counter]));
        animator_.refresh();
        return &animator_; // TODO: Use weak_ptr
    }

    void offMode() const noexcept { animator_.off(); };
    void onMode() const noexcept { animator_.on(animator_.brightness_); };
    inline void colonOff() noexcept { animator_.colon_ = false; };
    inline void colonOn() noexcept { animator_.colon_ = true; };
    inline Animator* switchColon() noexcept { animator_.colon_ = !animator_.colon_; return &animator_; };
    void clearScreen() noexcept { animator_.clear(); animator_.refresh(); };
    inline void setDp(uint8_t value) noexcept { animator_.dp_ = value; }
    inline uint8_t getBrightness() const noexcept { return static_cast<uint8_t>(animator_.brightness_); }
    void changeBrightness(uint8_t value) noexcept { setBrightness(value);};
    void setBrightness(uint8_t value) noexcept { animator_.brightness_ = Animator::fetchControl(value); };

private:
    template <typename T>
    typename type_traits::enable_if<type_traits::is_string<T>::value, String>::type stringer(T value)
    {
        return value;
    }

    template <typename T>
    typename type_traits::enable_if<type_traits::is_integral<T>::value, String>::type stringer(T value)
    {
        return String(value);
    }

    template <typename T>
    typename type_traits::enable_if<type_traits::is_floating_point<T>::value, String>::type stringer(T value)
    {
        return String(value, TOTAL_DIGITS);
    }



    Animator                animator_;
    String                  cache_ = "";
};

#endif //TM1637_TM1637_H
