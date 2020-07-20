#pragma once
#include "config.h" // подключаем эффекты, там же их настройки
#ifdef ESP_USE_BUTTON
#include "misc.h"
#include <Ticker.h>
#include "../../include/LList.h"

typedef enum _button_action {
	BA_NONE,
	BA_BRIGHT,
	BA_SPEED,
	BA_SCALE,
	BA_ON,
	BA_OFF,
	BA_DEMO,
	BA_AUX_TOGLE,
	BA_OTA,
	BA_EFF_NEXT,
	BA_EFF_PREV,
	BA_SEND_TIME,
	BA_SEND_IP,
	BA_WHITE_HI,
	BA_WHITE_LO,
	BA_WIFI_REC,
	BA_END			// признак конца enum
} BA;

const char *btn_get_desc(BA action);

class Button{
	typedef union _bflags {
		uint8_t mask;
		struct {
			uint8_t on:1;
			uint8_t hold:1;
			uint8_t click:3;
		};
	} btnflags;

	bool direction; // направление изменения


	friend bool operator== (const Button &f1, const Button &f2) { return (f1.flags.mask == f2.flags.mask); }
	friend bool operator!= (const Button &f1, const Button &f2) { return (f1.flags.mask != f2.flags.mask); }

	public:
		Button(uint8_t on, uint8_t hold, uint8_t click, BA act = BA_NONE) { direction = false; flags.mask = 0; flags.on = on; flags.hold = hold; flags.click = click; action = act; }
		Button(uint8_t mask, BA act = BA_NONE) { direction = false; flags.mask = mask; action = act; }

		bool activate(bool reverse);
		String getName();

		BA action;
		btnflags flags;
};

class Buttons {
	private:
#pragma pack(push,1)
 struct {
	bool buttonEnabled:1; // кнопка обрабатывается если true
	bool holding:1; // кнопка удерживается
	bool pinTransition:1;  // ловим "нажатие" кнопки
 };
 #pragma pack(pop)

	byte clicks = 0;
	Ticker _buttonTicker; // планировщик кнопки
	LList<Button*> buttons;
	timerMinim holdtm; // таймаут удержания кнопки в мс

	public:
	bool getpinTransition() { return pinTransition; }
	void setpinTransition(bool val) { pinTransition = val; }

	void setButtonOn(bool flag) { buttonEnabled = flag; }
	bool isButtonOn() { return buttonEnabled; }

	inline Button* operator[](int i) { return buttons[i]; }

	int size(){ return buttons.size(); }
	void add(Button *btn) { buttons.add(btn); }
	void remove(int i) { buttons.remove(i); }
	void clear();

	/*
	 * крючёк для обработки нажатия кнопки по прерываниям
	 * вызов метода, готоврит о том что состояние пина изменилось, нужно его перечитать
	 * @param bool state - true, кнопку "нажали", false - "отпустили"
	 */
	void buttonPress(bool state);

	Buttons();

	int loadConfig(const char *cfg = nullptr);
	void saveConfig(const char *cfg = nullptr);

	GButton touch;
	void buttonTick(); // обработчик кнопки
	timerMinim tmChangeDirectionTimer;     // таймаут смены направления увеличение-уменьшение при удержании кнопки
};
#endif
