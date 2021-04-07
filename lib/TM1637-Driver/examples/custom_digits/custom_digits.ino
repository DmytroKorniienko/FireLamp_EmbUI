#include <TM1637.h>

/**
 *
 * Visit https://github.com/AKJ7/TM1637/ for more info
 *
 * API
 * class TM1637 {
    public:
        static constexpr uint8_t TOTAL_DIGITS = 4;
        TM1637(uint8_t clkPin, uint8_t dataPin) noexcept;
        TM1637(const TM1637&) = delete;
        TM1637& operator=(const TM1637&) = delete;
        ~TM1637() = default;
        void begin();
        inline void init();
        inline Animator* refresh();
        template <typename T>
        typename type_traits::enable_if<
            type_traits::is_string<T>::value ||
            type_traits::is_floating_point<T>::value ||
            type_traits::is_integral<T>::value,
            Animator*>::type
        display(const T value, bool overflow = true, bool pad = false, uint8_t offset = 0);
        Animator* displayRawBytes(const uint8_t* buffer, size_t size);
        void offMode() const noexcept;
        void onMode() const noexcept;
        inline void colonOff() noexcept;
        inline void colonOn() noexcept;
        inline Animator* switchColon() noexcept;
        void clearScreen() noexcept;
        inline void setDp(uint8_t value) noexcept;
        inline uint8_t getBrightness() const noexcept;
        void changeBrightness(uint8_t value) noexcept;
        void setBrightness(uint8_t value) noexcept;
    };

    class Animator
    {
        Animator(uint8_t clkPin, uint8_t dataPin, uint8_t totalDigits);
        void blink(Tasker::duration_type delay);
        void fadeOut(Tasker::duration_type delay);
        void fadeIn(Tasker::duration_type delay);
        void scrollLeft(Tasker::duration_type delay);
        void off() const;
        void on(DisplayControl_e brightness) const;
        void reset(const String& value);
        void clear();
        void refresh();
    }

    struct DisplayDigit
    {
        DisplayDigit& setA();
        DisplayDigit& setB();
        DisplayDigit& setC();
        DisplayDigit& setD();
        DisplayDigit& setE();
        DisplayDigit& setF();
        DisplayDigit& setG();
        DisplayDigit& setDot();
        operator uint8_t();
    }
 */


TM1637 tm(2, 3);

void setup()
{
    tm.begin();
    tm.setBrightness(4);
}

void loop()
{
    auto d = DisplayDigit().setA().setE().setF().setD();
    const uint8_t rawBuffer[4] = {d, 0x09, DisplayDigit().setA().setD(), 15};
    tm.displayRawBytes(rawBuffer, 4)->blink(1000);
    delay(2000);
}