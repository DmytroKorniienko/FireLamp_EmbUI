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

#include "misc.h"

// Deprecated
// #if defined(LAMP_DEBUG) && DEBUG_TELNET_OUTPUT
// WiFiServer telnetServer(TELNET_PORT);                       // telnet сервер
// WiFiClient telnet;                                          // обработчик событий telnet клиента
// bool telnetGreetingShown = false;                           // признак "показано приветствие в telnet"

// void handleTelnetClient()
// {
//   if (telnetServer.hasClient())
//   {
//     if (!telnet || !telnet.connected())
//     {
//       if (telnet)
//       {
//         telnet.stop();                                      // клиент отключился
//         telnetGreetingShown = false;
//       }
//       telnet = telnetServer.available();                    // готов к подключению нового клиента
//     }
//     else
//     {
//       telnetServer.available().stop();                      // один клиент уже подключен, блокируем подключение нового
//       telnetGreetingShown = false;
//     }
//   }

//   if (telnet && telnet.connected() && telnet.available())
//   {
//     if (!telnetGreetingShown)
//     {
//       telnet.println(F("Подключение к устройтву по протоколу telnet установлено\n-------"));
//       telnetGreetingShown = true;
//     }
//   }
// }

// #endif