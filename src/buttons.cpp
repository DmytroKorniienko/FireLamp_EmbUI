#include "buttons.h"
#ifdef ESP_USE_BUTTON
#include "main.h"

const char *btn_get_desc(BA action){
	switch (action) {
		case BA_BRIGHT: return PSTR("BRIGHT");
		case BA_SPEED: return PSTR("SPEED");
		case BA_SCALE: return PSTR("SCALE");
		case BA_ON: return PSTR("ON");
		case BA_OFF: return PSTR("OFF");
		case BA_DEMO: return PSTR("DEMO");
		case BA_AUX_TOGLE: return PSTR("AUX");
		case BA_OTA: return PSTR("OTA");
		case BA_EFF_NEXT: return PSTR("NEXT");
		case BA_EFF_PREV: return PSTR("PREV");
		case BA_SEND_TIME: return PSTR("TIME");
		case BA_SEND_IP: return PSTR("IP");
		case BA_WHITE_HI: return PSTR("WHITE HI");
		case BA_WHITE_LO: return PSTR("WHITE LO");
		case BA_WIFI_REC: return PSTR("WIFI REC");
		default:;
	}
	return PSTR("");
}

bool Button::activate(bool reverse){
		uint8_t newval;
		RA ract = RA_UNKNOWN;
		bool ret = false;
		if (reverse) direction = !direction;
		switch (action) {
			case BA_BRIGHT:
				newval = constrain(myLamp.getLampBrightness() + (myLamp.getLampBrightness() / 25 + 1) * (direction * 2 - 1), 1 , 255);
				if (newval == 1 || newval == 255) direction = !direction;
#ifdef VERTGAUGE
				myLamp.GaugeShow(newval, 255, 10);
#endif
				remote_action(RA::RA_BRIGHT_NF, String(newval).c_str(), NULL);
				return true;
			case BA_SPEED:
				newval = constrain(myLamp.effects.getSpeed() + (myLamp.effects.getSpeed() / 25 + 1) * (direction * 2 - 1), 1 , 255);
				if (newval == 1 || newval == 255) direction = !direction;
#ifdef VERTGAUGE
				myLamp.GaugeShow(newval, 255, 100);
#endif
				remote_action(RA::RA_SPEED, String(newval).c_str(), NULL);
				return true;
			case BA_SCALE:
				newval = constrain(myLamp.effects.getScale() + (myLamp.effects.getScale() / 25 + 1) * (direction * 2 - 1), 1 , 255);
				if (newval == 1 || newval == 255) direction = !direction;
#ifdef VERTGAUGE
				myLamp.GaugeShow(newval, 255, 150);
#endif
				remote_action(RA::RA_SCALE, String(newval).c_str(), NULL);
				return true;
			case BA_ON: ract = RA_ON; break;
			case BA_OFF: ract = RA_OFF; break;
			case BA_DEMO: ract = RA_DEMO; break;
#ifdef AUX_PIN
			case BA_AUX_TOGLE: ract = RA_AUX_TOGLE; break;
#endif
#ifdef OTA
			case BA_OTA: ract = RA_OTA; break;
#endif
			case BA_EFF_NEXT: ract = RA_EFF_NEXT; break;
			case BA_EFF_PREV: ract = RA_EFF_PREV; break;
			case BA_SEND_TIME: ract = RA_SEND_TIME; break;
			case BA_SEND_IP: ract = RA_SEND_IP; break;
			case BA_WHITE_HI: ract = RA_WHITE_HI; break;
			case BA_WHITE_LO: ract = RA_WHITE_LO; break;
			case BA_WIFI_REC: ract = RA_WIFI_REC; break;
			default:;
		}
		remote_action(ract, NULL);
		return ret;
}

String Button::getName(){
		String buffer;
		buffer.concat(flags.on? F("ON: ") : F("OFF: "));
		if (flags.hold) {
			if (flags.click) {
				buffer.concat(String(flags.click));
				buffer.concat(F(" Click + "));
			}
			buffer.concat(F("HOLD - "));
		} else
		if (flags.click) {
			buffer.concat(String(flags.click));
			buffer.concat(F(" Click - "));
		}

		buffer.concat(String(btn_get_desc(action)));

		return buffer;
};

Buttons::Buttons(): buttons(), holdtm(NUMHOLD_TIME), touch(BTN_PIN, PULL_MODE, NORM_OPEN){
	holding = false;
	buttonEnabled = true; // кнопка обрабатывается если true, пока что обрабатывается всегда :)
	pinTransition = true;

	clicks = 0;

#if (PULL_MODE == LOW_PULL)
		pinMode(BTN_PIN, INPUT);
#else
		pinMode(BTN_PIN, INPUT_PULLUP);
#endif
	touch.setTickMode(MANUAL);    // мы сами говорим когда опрашивать пин
	touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
	touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
	touch.setTimeout(BUTTON_TIMEOUT);
	touch.setDebounce(BUTTON_DEBOUNCE);   // т.к. работаем с прерываниями, может пригодиться для железной кнопки
	_buttonTicker.attach_scheduled(1, std::bind(&Buttons::buttonTick, this));   // "ленивый" опрос 1 раз в сек
}

/*
 * buttonPress - управление планировщиком опроса кнопки
 * оберка нужна т.к. touch.tick() нельзя положить в ICACHE_RAM
 * по наступлению прерывания "нажато" врубаем опрос событий кнопки не реже чем BUTTON_STEP_TIMEOUT/2 чтобы отловить "удержание"
 *
 * т.к. гайвербаттон не умеет работать чисто по событиям, при "отпускании" продолжаем дергать обработчик раз в секунду,
 * чтобы не он забыл зачем живет :)
 */
void Buttons::buttonPress(bool state){
	if (!buttonEnabled) {
		// события кнопки не обрабатываются, если она заблокирована
		_buttonTicker.detach();
		return;
	}

	LOG(printf_P, PSTR("Button %s: %lu\n"), state ? PSTR("press") : PSTR("release"), millis());

	_buttonTicker.attach_ms_scheduled(state ? BUTTON_STEP_TIMEOUT/2 : 1000, std::bind(&Buttons::buttonTick, this));

	buttonTick();   // обрабатываем текущее нажатие вне очереди
}

void Buttons::buttonTick(){
	if (!buttonEnabled) return;
	touch.tick();
	bool reverse = false;
	if ((holding = touch.isHolded())) {
		if (holdtm.isReady()) {
			clicks = touch.getHoldClicks();
		}
		reverse = true;
	} else
	if ((holding = touch.isStep())) {
		holdtm.reset();
	} else
	if (!touch.hasClicks() || !(clicks = touch.getClicks())) {
		return;
	}

	if (myLamp.isAlarm()) {
		// нажатие во время будильника
		myLamp.stopAlarm();
		return;
	}

	Button btn(myLamp.isLampOn(), holding, clicks);
	for (int i = 0; i < buttons.size(); i++) {
		if (btn == *buttons[i]) {
			if (!buttons[i]->activate(reverse)) {
				// действие не подразумевает повтора
				touch.resetStates();
			}
			// break; // Не выходим после первого найденного совпадения. Можем делать макросы из нажатий
		}
	}
}

void Buttons::clear() {
	while (buttons.size()) {
		Button *btn = buttons.shift();
		delete btn;
	}
}

int Buttons::loadConfig(const char *cfg){
	if (LittleFS.begin()) {
		File configFile;
		if (cfg == nullptr) {
			LOG(println, F("Load default buttons config file"));
			configFile = LittleFS.open(F("/buttons_config.json"), "r"); // PSTR("r") использовать нельзя, будет исключение!
		} else {
			LOG(printf_P, PSTR("Load %s buttons config file\n"), cfg);
			configFile = LittleFS.open(cfg, "r"); // PSTR("r") использовать нельзя, будет исключение!
		}
		String cfg_str = configFile.readString();
		if (cfg_str == F("")){
			LOG(println, F("Failed to open buttons config file"));
			return 0;
		}

		DynamicJsonDocument doc(2048);
		DeserializationError error = deserializeJson(doc, cfg_str);
		if (error) {
			LOG(print, F("deserializeJson error: "));
			LOG(println, error.code());
			LOG(println, cfg_str);
			return 0;
		}
		JsonArray arr = doc.as<JsonArray>();
		for (size_t i = 0; i < arr.size(); i++) {
			JsonObject item = arr[i];
			uint8_t mask = item[F("flg")].as<uint8_t>();
			BA ac = (BA)item[F("ac")].as<int>();
			buttons.add(new Button(mask, ac));
		}
		doc.clear();
	}
	return 1;
}

void Buttons::saveConfig(const char *cfg){
	if (LittleFS.begin()) {
		File configFile;
		if (cfg == nullptr) {
			LOG(println, F("Save default buttons config file"));
			configFile = LittleFS.open(F("/buttons_config.json"), "w"); // PSTR("w") использовать нельзя, будет исключение!
		} else {
			LOG(printf_P, PSTR("Save %s buttons config file\n"), cfg);
			configFile = LittleFS.open(cfg, "w"); // PSTR("w") использовать нельзя, будет исключение!
		}
		configFile.print("[");

		for (int i = 0; i < buttons.size(); i++) {
			Button *btn = buttons[i];
			configFile.printf_P(PSTR("%s{\"flg\":%u,\"ac\":%u}"),
				(char*)(i? F(",") : F("")), btn->flags.mask, btn->action
			);
			LOG(printf_P, PSTR("%s{\"flg\":%u,\"ac\":%u}"),
				(char*)(i? F(",") : F("")), btn->flags.mask, btn->action
			);
		}
		configFile.print("]");
		configFile.flush();
		configFile.close();
	}
}

#endif
