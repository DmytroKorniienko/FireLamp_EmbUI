/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_JeeUI.

    FireLamp_JeeUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_JeeUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_JeeUI.  If not, see <https://www.gnu.org/licenses/>.

(Цей файл є частиною FireLamp_JeeUI.

   FireLamp_JeeUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_JeeUI поширюється в надії, що вона буде корисною,
   але БЕЗ ВСЯКИХ ГАРАНТІЙ; навіть без неявної гарантії ТОВАРНОГО ВИГЛЯДУ
   або ПРИДАТНОСТІ ДЛЯ ВИЗНАЧЕНИХ ЦІЛЕЙ. Докладніше див. у Стандартній
   громадська ліцензія GNU.

   Ви повинні були отримати копію Стандартної громадської ліцензії GNU
   разом із цією програмою. Якщо це не так, див.
   <https://www.gnu.org/licenses/>.)
*/
#ifndef __OTA_H
#define __OTA_H
/*
 * 11.07.2019
 * Клас, який відстежує дії користувача за запитом оновлення прошивки повітрям і виконує цю прошивку.
 * Запит на оновлення - це виклик методу RequestOtaUpdate(), його потрібно помістити, наприклад, в обробник натискання кнопки, прийому пакета UDP і т.д.
 * Для оновлення користувач повинен ДВАЖДІ запросити оновлення протягом заданого проміжку часу (CONFIRMATION_TIMEOUT), щоб уникнути випадкового переходу в режим оновлення.
 * Режим оновлення - це прослуховування спеціального порту (ESP_OTA_PORT) в очікуванні команди оновлення прошивки повітрям (по мережі).
 * Режим оновлення працює паралельно з основним режимом функціонування, лише за WiFi клієнт, т.к. вимагає доступу до ESP через локальну мережу.
 * Режим оновлення активний протягом заданого проміжку часу (ESP_CONF_TIMEOUT). Потім ESP автоматично перезавантажується.
 * Оновлення проводиться з Arduino IDE: меню Інструменти - Порт - <Вибрати виявлений мережевий COM порт зі списку> (якщо він не виявлений, значить щось налаштовано неправильно), потім звичайна команда "Завантаження" для прошивки.
 * Для включення опції оновлення повітря в основному файлі повинен бути визначений ідентифікатор OTA "#define OTA" і режим "#define ESP_MODE (1U)" (а також в даному проекті повинна бути підключена кнопка).
*/

#include "config.h"
#include "EmbUI.h"
#ifdef OTA
#include <ArduinoOTA.h>

#define ESP_OTA_PORT          (3232U)                       // номер порта, который будет "прослушиваться" в ожидании команды прошивки по воздуху 8266U/3232U
#define CONFIRMATION_TIMEOUT  (30U)                         // время в сеундах, в течение которого нужно дважды подтвердить старт обновлениЯ по воздуху (иначе сброс в None)
#define ESP_CONF_TIMEOUT      (120U)                        // время ожидания ОТА
#define OTA_PASS "12345"

enum OtaPhase                                               // определение стадий процесса обновления по воздуху: нет, получено первое подтверждение, получено второе подтверждение, получено второе подтверждение - в процессе, обновление окончено
{
  None = 0,
  GotFirstConfirm,
  GotSecondConfirm,
  InProgress,
  Done
};

class OtaManager
{
  private:
    OtaPhase OtaFlag = OtaPhase::None;
  public:

    OtaManager()
    {
    }

    bool RequestOtaUpdate()                                 // пользователь однократно запросил обновление по воздуху; возвращает true, когда переходит в режим обновления - startOtaUpdate()
    {
      if (OtaFlag == OtaPhase::None)
      {
        OtaFlag = OtaPhase::GotFirstConfirm;
        momentOfFirstConfirmation = millis();

        LOG(print,F("Получено первое подтверждение обновления по воздуху\nОжидание второго подтверждения\n"));

        return false;
      }

      if (OtaFlag == OtaPhase::GotFirstConfirm)
      {
        OtaFlag = OtaPhase::GotSecondConfirm;
        momentOfOtaStart = millis();

        LOG(print,F("Получено второе подтверждение обновления по воздуху\nСтарт режима обновления\n"));
        return true;
      }

      return false;
    }

    void HandleOtaUpdate()
    {
      if (OtaFlag == OtaPhase::GotFirstConfirm &&
          millis() - momentOfFirstConfirmation >= CONFIRMATION_TIMEOUT * 1000)
      {
        OtaFlag = OtaPhase::None;
        momentOfFirstConfirmation = 0;

        LOG(print,F("Таймаут ожидания второго подтверждения превышен\nСброс флага в исходное состояние\n"));

        return;
      }

      if ((OtaFlag == OtaPhase::GotSecondConfirm || OtaFlag == OtaPhase::InProgress) &&
          millis() - momentOfOtaStart >= ESP_CONF_TIMEOUT * 1000)
      {
        OtaFlag = OtaPhase::None;
        momentOfOtaStart = 0;

        LOG(print,F("Таймаут ожидания прошивки по воздуху превышен\nСброс флага в исходное состояние\nПерезагрузка\n"));
        delay(500);

        ESP.restart();
        return;
      }

      if (OtaFlag == OtaPhase::InProgress)
      {
        ArduinoOTA.handle();
      }
    }

    void startOtaUpdate() {
      char espHostName[65];
      String id = WiFi.softAPmacAddress();
      id.replace(F(":"), F(""));
      sprintf_P(espHostName, PSTR("%s%s"),(char*)TOSTRING(EMBUI_IDPREFIX), id.c_str());

      ArduinoOTA.setPort(ESP_OTA_PORT);
      ArduinoOTA.setHostname(espHostName);
      ArduinoOTA.setPassword(OTA_PASS);

      ArduinoOTA.onStart([this]()
      {
        OtaFlag = OtaPhase::InProgress;
        char type[16];
        if (ArduinoOTA.getCommand() == U_FLASH)
        {
          strcpy_P(type, PSTR("sketch"));
        }
        else // U_LittleFS
        {
          strcpy_P(type, PSTR("filesystem"));
        }

        // NOTE: if updating LittleFS this would be the place to unmount LittleFS using LittleFS.end()

        LOG(printf_P,PSTR("Start updating %s\n"), type);
      });

      ArduinoOTA.onEnd([this]()
      {
        OtaFlag = OtaPhase::Done;
        momentOfFirstConfirmation = 0;
        momentOfOtaStart = 0;

        LOG(print,F("Обновление по воздуху выполнено\nПерезапуск"));
        delay(500);
      });

      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
      {
        LOG(printf_P,PSTR("Ход выполнения: %u%%\r"), (progress / (total / 100)));
      });

      ArduinoOTA.onError([this](ota_error_t error)
      {
        OtaFlag = OtaPhase::None;
        momentOfFirstConfirmation = 0;
        momentOfOtaStart = 0;

        LOG(printf_P,PSTR("Обновление по воздуху завершилось ошибкой [%u]: "), error);

        if (error == OTA_AUTH_ERROR)
        {
          LOG(println,F("Auth Failed"));
        }
        else if (error == OTA_BEGIN_ERROR)
        {
          LOG(println,F("Begin Failed"));
        }
        else if (error == OTA_CONNECT_ERROR)
        {
          LOG(println,F("Connect Failed"));
        }
        else if (error == OTA_RECEIVE_ERROR)
        {
          LOG(println,F("Receive Failed"));
        }
        else if (error == OTA_END_ERROR)
        {
          LOG(println,F("End Failed"));
        }

        LOG(print,F("Сброс флага в исходное состояние\nПереход в режим ожидания запроса прошивки по воздуху\n"));
      });

      ArduinoOTA.setRebootOnSuccess(true);
      ArduinoOTA.begin();
      OtaFlag = OtaPhase::InProgress;
      momentOfOtaStart = millis();

      LOG(printf_P,PSTR("Для обновления в Arduino IDE выберите пункт меню Инструменты - Порт - '%s at "), espHostName);
      LOG(print,WiFi.localIP());
      LOG(println,F("'"));
      LOG(printf_P,PSTR("Затем нажмите кнопку 'Загрузка' в течение %u секунд и по запросу введите пароль '%s'\n"), ESP_CONF_TIMEOUT, OTA_PASS);
      LOG(println,F("Устройство с Arduino IDE должно быть в одной локальной сети с модулем ESP!"));
    }

    private:
    uint64_t momentOfFirstConfirmation = 0;                 // момент времени, когда получено первое подтверждение и с которого начинается отсчёт ожидания второго подтверждения
    uint64_t momentOfOtaStart = 0;                          // момент времени, когда развёрнута WiFi точка доступа для обновления по воздуху
};

#endif
#endif