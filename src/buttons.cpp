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
		case BA_EFFECT: return PSTR("EFFECT");
		default:;
	}
	return PSTR("");
}

bool Button::activate(btnflags& flg, bool reverse){
		uint8_t newval;
		RA ract = RA_UNKNOWN;
		bool ret = false;
		if (reverse) flags.direction = !flags.direction;
		switch (action) {
			case BA_BRIGHT:
				newval = constrain(myLamp.getLampBrightness() + (myLamp.getLampBrightness() / 25 + 1) * (flags.direction * 2 - 1), 1 , 255);
				if ((newval == 1 || newval == 255) && tReverseTimeout==nullptr){
					tReverseTimeout = new Task(2 * TASK_SECOND, TASK_ONCE,
									[this](){ flags.direction = !flags.direction; LOG(println,F("reverse")); TASK_RECYCLE; tReverseTimeout = nullptr; },
									&ts, false
							);
					tReverseTimeout->enableDelayed();
				}
#ifdef VERTGAUGE
				myLamp.GaugeShow(newval, 255, 10);
#endif
				remote_action(RA::RA_BRIGHT_NF, String(newval).c_str(), NULL);
				return true;
			case BA_SPEED: {
				byte speed = (myLamp.effects.getControls()[1]->getVal()).toInt();
				newval = constrain( speed + (speed / 25 + 1) * (flags.direction * 2 - 1), 1 , 255);
				if ((newval == 1 || newval == 255) && tReverseTimeout==nullptr){
					tReverseTimeout = new Task(2 * TASK_SECOND, TASK_ONCE,
									[this](){ flags.direction = !flags.direction; LOG(println,F("reverse")); TASK_RECYCLE; tReverseTimeout = nullptr; },
									&ts, false
							);
					tReverseTimeout->enableDelayed();
				}
#ifdef VERTGAUGE
				myLamp.GaugeShow(newval, 255, 100);
#endif
				remote_action(RA::RA_SPEED, String(newval).c_str(), NULL);
				return true;
			}
			case BA_SCALE: {
				byte scale = (myLamp.effects.getControls()[2]->getVal()).toInt();
				newval = constrain(scale + (scale / 25 + 1) * (flags.direction * 2 - 1), 1 , 255);
				if ((newval == 1 || newval == 255) && tReverseTimeout==nullptr){
					tReverseTimeout = new Task(2 * TASK_SECOND, TASK_ONCE,
									[this](){ flags.direction = !flags.direction; LOG(println,F("reverse")); TASK_RECYCLE; tReverseTimeout = nullptr; },
									&ts, false
							);
					tReverseTimeout->enableDelayed();
				}
#ifdef VERTGAUGE
				myLamp.GaugeShow(newval, 255, 150);
#endif
				remote_action(RA::RA_SCALE, String(newval).c_str(), NULL);
				return true;
			}
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
			case BA_EFFECT: ract = RA_EFFECT; break;
			default:;
		}
		if(!(flg.onetime&2)){ // только если не установлен бит сработавшего однократного события
			LOG(printf_P,PSTR("Button send action: %d\n"), ract);
			if(param.isEmpty())
				remote_action(ract, NULL);
			else
				remote_action(ract, param.c_str(), NULL);
		}
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

Buttons::Buttons(uint8_t _pin, uint8_t _pullmode, uint8_t _state): buttons(), touch(_pin, _pullmode, _state){
	pin = _pin;
	pullmode = _pullmode;
	state = _state;
	holding = false;
	holded = false;
	buttonEnabled = true; // кнопка обрабатывается если true, пока что обрабатывается всегда :)
	pinTransition = true;

	clicks = 0;

	if (pullmode == LOW_PULL)
		pinMode(pin, INPUT);
	else
		pinMode(pin, INPUT_PULLUP);

	touch.setType(pullmode);
	touch.setTickMode(MANUAL);    // мы сами говорим когда опрашивать пин
	touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
	touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
	touch.setTimeout(BUTTON_TIMEOUT);
	touch.setDebounce(BUTTON_DEBOUNCE);   // т.к. работаем с прерываниями, может пригодиться для железной кнопки
	touch.resetStates();

	tButton.set(TASK_SECOND, TASK_FOREVER, std::bind(&Buttons::buttonTick, this));	// "ленивый" опрос 1 раз в сек
	ts.addTask(tButton);
	tButton.enableDelayed();

	attachInterrupt(pin, std::bind(&Buttons::isrPress,this), pullmode!=LOW_PULL ? RISING : FALLING );

	tClicksClear.set(NUMHOLD_TIME, TASK_ONCE, [this](){ holded = true; clicks=0; });
	ts.addTask(tClicksClear);

	isrEnable();
}

void Buttons::buttonTick(){
	if (!buttonEnabled) return;

	static bool startLampState = myLamp.isLampOn();
	
	touch.tick();
	bool reverse = false;

	if ((holding = touch.isHolded())) {
		// начало удержания кнопки
		byte tstclicks = touch.getHoldClicks();
		if(!tClicksClear.isEnabled() || (tstclicks && tstclicks!=clicks)) // нажатия после удержания не сбрасываем!!! они сбросятся по tClicksClear или по смене кол-ва нажатий до удержания
			clicks=tstclicks;
		tClicksClear.enableIfNot();
		startLampState = myLamp.isLampOn(); // получить начальный статус
		reverse = true;
		LOG(printf_P, PSTR("start hold - buttonEnabled=%d, startLampState=%d, holding=%d, holded=%d, clicks=%d, reverse=%d\n"), buttonEnabled, startLampState, holding, holded, clicks, reverse);
	} else if ((holding = touch.isStep())) {
		// кнопка удерживается
		tClicksClear.restartDelayed(); // отсрочиваем сброс нажатий
	} else if (!touch.hasClicks() || !(clicks = touch.getClicks())) {
		if( (!touch.isHold() && holded) )	{ // кнопку уже не трогают
			LOG(println,F("Сброс состояния кнопки после окончания удержания"));
			resetStates();
			startLampState = myLamp.isLampOn();
			LOG(printf_P, PSTR("reset - buttonEnabled=%d, startLampState=%d, holding=%d, holded=%d, clicks=%d, reverse=%d\n"), buttonEnabled, startLampState, holding, holded, clicks, reverse);
			for (int i = 0; i < buttons.size(); i++) {
				buttons[i]->flags.onetime&=1;
			}
			isrEnable(); // переключение на ленивый опрос
		}
		// здесь баг, этот выход часто перехватывает "одиночные" нажатия и превращает их в "клик"
		return;
	}
	
	if (myLamp.isAlarm()) {
		// нажатие во время будильника
		myLamp.stopAlarm();
		return;
	}

	if(!holding){
		startLampState=myLamp.isLampOn();
		LOG(printf_P, PSTR("onetime click - buttonEnabled=%d, startLampState=%d, holding=%d, holded=%d, clicks=%d, reverse=%d\n"), buttonEnabled, startLampState, holding, holded, clicks, reverse);
	}
	
	Button btn(startLampState, holding, clicks, true); // myLamp.isLampOn() - анализироваться будет состояние на начало нажимания кнопки
	for (int i = 0; i < buttons.size(); i++) {
		if (btn == *buttons[i]) {
			//if(buttons[i]->action==1) continue; // отладка, отключить действие увеличения яркости
			if (!buttons[i]->activate(buttons[i]->flags, reverse)) {
				//LOG(println,buttons[i]->action); // отладка
				// действие не подразумевает повтора
				if(buttons[i]->flags.onetime && touch.isHold()){ // в процессе удержания
					buttons[i]->flags.onetime|=3; // установить старший бит сработавшего действия
				}
			}
			// break; // Не выходим после первого найденного совпадения. Можем делать макросы из нажатий
		}
	}

	// Здесь уже все отработало, и кнопка точно не удерживается
	if(!holding){
		LOG(println,F("Сброс состояния кнопки"));
		resetStates();
		//touch.resetStates();
		startLampState = myLamp.isLampOn();
		isrEnable();
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
			if(item.containsKey(F("p"))){
				String param = item[F("p")].as<String>();
				buttons.add(new Button(mask, ac, param));
			} else {
				buttons.add(new Button(mask, ac));
			}
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
			configFile.printf_P(PSTR("%s{\"flg\":%u,\"ac\":%u,\"p\":\"%s\"}"),
				(char*)(i? F(",") : F("")), btn->flags.mask, btn->action, btn->getParam().c_str()
			);
			LOG(printf_P, PSTR("%s{\"flg\":%u,\"ac\":%u,\"p\":\"%s\"}"),
				(char*)(i? F(",") : F("")), btn->flags.mask, btn->action, btn->getParam().c_str()
			);
		}
		configFile.print("]");
		configFile.flush();
		configFile.close();
	}
}

void IRAM_ATTR Buttons::isrPress() {
  detachInterrupt(pin);
	tButton.setInterval(20); // переключение в режим удержания кнопки
}

void Buttons::isrEnable(){
	tButton.setInterval(TASK_SECOND);
	tButton.enableIfNot();
	LOG(println,F("Button switch to isr+Lazy"));
	attachInterrupt(pin, std::bind(&Buttons::isrPress,this), pullmode==LOW_PULL ? RISING : FALLING );
}

void Buttons::setButtonOn(bool flag) {
	buttonEnabled = flag;
	resetStates();
	if (flag){
		LOG(println,F("Button watch enabled"));
		isrEnable();
	} else {
	    detachInterrupt(pin);
		tButton.disable();
		LOG(println,F("Button watch disabled"));
	}
}

#endif
