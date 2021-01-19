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

  (Этот файл — часть FireLamp_JeeUI.

   FireLamp_JeeUI - свободная программа: вы можете перераспространять ее и/или
   изменять ее на условиях Стандартной общественной лицензии GNU в том виде,
   в каком она была опубликована Фондом свободного программного обеспечения;
   либо версии 3 лицензии, либо (по вашему выбору) любой более поздней
   версии.

   FireLamp_JeeUI распространяется в надежде, что она будет полезной,
   но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА
   или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной
   общественной лицензии GNU.

   Вы должны были получить копию Стандартной общественной лицензии GNU
   вместе с этой программой. Если это не так, см.
   <https://www.gnu.org/licenses/>.)
*/

#include "misc.h"

#ifdef ESP8266
 #include <ESP8266WiFi.h>
#endif  // def ESP8266
#include "EmbUI.h"
//#include <LittleFS.h>
#define FTP_DEBUG
#include <ftpSrv.h>

FTPServer ftpSrv(LittleFS); // construct with LittleFS

void ftp_setup(void){
 
  /////FTP Setup, ensure LittleFS is started before ftp;  /////////
  if (LittleFS.begin()) {
      LOG(println, F("LittleFS opened!"));
#ifdef ESP8266
      ftpSrv.begin(F("esp8266"),F("esp8266"));    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
#else
      ftpSrv.begin(F("esp32"),F("esp32"));    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
#endif
  }    
}

void ftp_loop(void){
  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!  
}
