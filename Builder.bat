color 0A
setlocal
set workdir=%~dp0
echo off
if exist "%USERPROFILE%\Python\python.exe" (
	set PYTHONHOME=%USERPROFILE%\Python
	set PYTHONPATH=%USERPROFILE%\Python
)
PATH=%PATH%;%workdir%;%USERPROFILE%\.platformio\penv\Scripts;%PYTHONPATH%;;%ProgramFiles%\Git
@chcp 1251>nul
mode con: cols=88 lines=45
cls 

if not exist "%workdir%\out_bin" (mkdir %workdir%\out_bin)


:m1

Echo  *------------------------------------------*-----------------------------------------* 
Echo  *  Switch/update - MASTER branch         1 *  Ïåðåêëþ÷èòè\îíîâèòè- ã³ëêà MASTER      *
Echo  *  Switch/update - DEV branch            2 *  Ïåðåêëþ÷èòè\îíîâèòè- ã³ëêà DEV         *
Echo  #------------------------------------------#-----------------------------------------# 
echo  *  Reset changes in local repo!          3 *  Â³äì³íèòè çì³íè â ðåïîçèòîð³¿ íà ÏÊ!   *
Echo  *  WARNING! This will revert all changes!  *  ÓÂÀÃÀ! Öå â³äì³íèòü âñ³ çì³íè          *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *                Build only                *              Ò³ëüêè çá³ðêà              *
Echo  *  Build - Esp8266 160MHz                4 *  Ç³áðàòè äëÿ Esp8266 íà 160ÌÃö          *
Echo  *  Build - Esp8266 80MHz                 5 *  Ç³áðàòè äëÿ Esp8266 íà 80ÌÃö           *
Echo  *  Build - Esp32                         6 *  Ç³áðàòè äëÿ Esp32                      *
Echo  *  HTTP Update - Firmware               uf *  Îáíîâèòè ïðîøèâêó ÷åðåç HTTP           *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *             Build and flash              *            Çá³ðêà ³ ïðîøèâàííÿ          *
Echo  *  Build and flash - Esp8266_160         7 *  Ç³áðàòè ³ ïðîøèòè - Esp8266 íà 160ÌÃö  *
Echo  *  Build and flash - Esp8266@80          8 *  Ç³áðàòè ³ ïðîøèòè - Esp8266 íà 80ÌÃö   *
Echo  *  Build and flash - Esp32               9 *  Ç³áðàòè ³ ïðîøèòè - Esp32              *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *         Build and flash (DEBUG)          *      Çá³ðêà ³ ïðîøèâàííÿ  (ÄÅÁÀÃ)       *
Echo  *  Build and flash - Esp8266_160        7D *  Ç³áðàòè ³ ïðîøèòè - Esp8266 íà 160ÌÃö  *
Echo  *  Build and flash - Esp8266@80         8D *  Ç³áðàòè ³ ïðîøèòè - Esp8266 íà 80ÌÃö   *
Echo  *  Serial port monitor (USB)             D *  Ìîí³òîð â³äëàäêè (USB)                 *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Update FS data from framework         u *  Îíîâèòè ôàéëè ÔÑ ç ôðåéìâîðêà          *
Echo  *          FS for ESP8266                  *              äëÿ ESP8266                *
Echo  *  Build File System image               b *  Ç³áðàòè ôàéëîâó ñèñòåìó                *
Echo  *  Build and flash File System           f *  Ç³áðàòè ³ ïðîøèòè ôàéëîâó ñèñòåìó      *
Echo  *            FS for ESP32                  *              äëÿ ESP32                  *
Echo  *  Build File System image              b1 *  Ç³áðàòè ôàéëîâó ñèñòåìó                *
Echo  *  Build and flash File System          f1 *  Ç³áðàòè ³ ïðîøèòè ôàéëîâó ñèñòåìó      *
Echo  *  HTTP Update - FileSystem             us *  Îáíîâèòè ôàéëîâó ñèñòåìó ÷åðåç HTTP    *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Erase Flash    ESP8266                e *  Ñòåðòè Ôëåø    ESP8266                 *
Echo  *  Erase Flash    ESP32                 e1 *  Ñòåðòè ôëåø    ESP32                   *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Update libs and PIO Core              g *  Îíîâèòè á³áë³îòåêè\ñåðèäîâèùå PIO Core *
Echo  *  Clean up temp files .pio              c *  Ââèäàëèòè òèì÷àñîâ³ ôàéëè .pio         *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  CMD window                            m *  Â³äêðèòè êîìàíäíèé ðÿäîê CMD           *
Echo  *------------------------------------------#-----------------------------------------*
Echo.
Echo.
Set /p choice="Your choice (Âàø âèá³ð): "


if "%choice%"=="1" (
	call %workdir%\resources\update-DEV-from-Git.cmd 1
)
if "%choice%"=="2" (
	call %workdir%\resources\update-DEV-from-Git.cmd 2
)
if "%choice%"=="3" call %workdir%\resources\update-DEV-from-Git.cmd 3
if "%choice%"=="4" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp8266_160
	copy %workdir%\.pio\build\esp8266_160\firmware.bin %workdir%\out_bin
)
if "%choice%"=="5" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp8266
	copy %workdir%\.pio\build\esp8266\firmware.bin %workdir%\out_bin
)
if "%choice%"=="6" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp32
	copy %workdir%\.pio\build\esp32\firmware.bin %workdir%\out_bin
)
if "%choice%"=="uf" (
	start %workdir%\Update_Firmware.bat
)
if "%choice%"=="us" (
	start %workdir%\Update_FileSystem.bat
)
if "%choice%"=="7" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266_160)
if "%choice%"=="8" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266)
if "%choice%"=="9" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp32)
if "%choice%"=="7D" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266_160dev)
if "%choice%"=="8D" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266dev)
if "%choice%"=="D" (start %workdir%\resources\SerialMonitor.cmd)
if "%choice%"=="u" (
	cd %workdir%\resources\
	start respack.cmd
	cd %workdir%
)
if "%choice%"=="b" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target buildfs --environment esp8266_160
	copy %workdir%\.pio\build\esp8266_160\littlefs.bin %workdir%\out_bin
)
if "%choice%"=="f" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target uploadfs --environment esp8266_160)
if "%choice%"=="e" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target erase --environment esp8266_160)
if "%choice%"=="b1" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target buildfs --environment esp32
	copy %workdir%\.pio\build\esp32\littlefs.bin %workdir%\out_bin
)
if "%choice%"=="f1" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target uploadfs --environment esp32)
if "%choice%"=="e1" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target erase --environment esp32)
if "%choice%"=="c" (
	pio system prune -f
	rmdir /S /Q %workdir%\.pio
)
if "%choice%"=="g" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" upgrade
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" update
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" pkg update
)
if "%choice%"=="m" (start cmd)

Echo.
Echo.
Echo.
pause
del %workdir%\resources\.wget-hsts
cls
goto m1


exit
