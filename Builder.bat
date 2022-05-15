color 0A
setlocal
set workdir=%~dp0
echo off
if exist "%USERPROFILE%\Python\python.exe" (
	set PYTHONHOME=%USERPROFILE%\Python
	set PYTHONPATH=%USERPROFILE%\Python
)
PATH=%PATH%;%workdir%;%USERPROFILE%\.platformio\penv\Scripts;%PYTHONPATH%;
@chcp 1251>nul
mode con: cols=88 lines=45
cls 

if not exist "%workdir%\out_bin" (mkdir %workdir%\out_bin)

:m1
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *                COMMANDS                  *               Українською               *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *  Switch/update - MASTER branch         1 *  Переключити\оновити- ветка MASTER      *
Echo  *  Switch/update - DEV branch            2 *  Переключити\оновити- ветка DEV         *
Echo  #------------------------------------------#-----------------------------------------# 
echo  *  Reset changes in local repo!          3 *  Скинути зміни в місцевому репозиторію! *
Echo  *  WARNING! This will revert all changes!  *  УВАГА! Це скине всі зміни              *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *                Build only                *              Тільки збірка              *
Echo  *  Build - Esp8266 160MHz                4 *  Зібрати для Esp8266 на 160МГц          *
Echo  *  Build - Esp8266 80MHz                 5 *  Зібрати для Esp8266 на 80МГц           *
Echo  *  Build - Esp32                         6 *  Зібрати для Esp32                      *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *             Build and flash              *            Збірка і прошивання          *
Echo  *  Build and upload - Esp8266@160        7 *  Зібрати і прошити - Esp8266 на 160МГц  *
Echo  *  Build and upload - Esp8266@80         8 *  Зібрати і прошити - Esp8266 на 80МГц   *
Echo  *  Build and upload - Esp32              9 *  Зібрати і прошити - Esp32              *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *         Build and flash (DEBUG)          *      Збірка і прошивання  (ДЕБАГ)       *
Echo  *  Build and upload - Esp8266@160       7D *  Зібрати і прошити - Esp8266 на 160МГц  *
Echo  *  Build and upload - Esp8266@80        8D *  Зібрати і прошити - Esp8266 на 80МГц   *
Echo  *  Serial port monitor                   D *  Вивід з монітору відлатки              *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Update FS data from framework         u *  Оновити файли ФС з фреймворка          *
Echo  *          FS for ESP8266                  *              для ESP8266                *
Echo  *  Build File System image               b *  Зібрати файлову систему                *
Echo  *  Build and upload File System          f *  Зібрати і прошити файлову систему      *
Echo  *            FS for ESP32                  *              для ESP32                  *
Echo  *  Build File System image              b1 *  Зібрати файлову систему                *
Echo  *  Build and upload File System         f1 *  Зібрати і прошити файлову систему      *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Erase Flash    ESP8266                e *  Стерти Флеш    ESP8266                 *
Echo  *  Erase Flash    ESP32                 e1 *  Стерти флеш    ESP32                   *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Update libs and PIO Core              g *  Оновити бібліотеку і серидовище PIOCore*
Echo  *  Clean up temp files .pio              c *  Відчистити тимчасові файли .pio        *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  CMD window                            m *  Відкрити командний рядок               *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  Remove Platformio installation        R *  Повністю видалити Platformio           *
Echo  #------------------------------------------#-----------------------------------------#
Echo.
Set /p choice="Your choice (Ваш вибір): "


if "%choice%"=="1" (
	call update-DEV-from-Git.cmd 1
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" lib update
)
if "%choice%"=="2" (
	call update-DEV-from-Git.cmd 2
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" lib update
)
if "%choice%"=="3" call update-DEV-from-Git.cmd 3
if "%choice%"=="4" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp8266@160
	copy %workdir%\.pio\build\esp8266@160\firmware.bin %workdir%\out_bin
)
if "%choice%"=="5" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp8266
	copy %workdir%\.pio\build\esp8266\firmware.bin %workdir%\out_bin
)
if "%choice%"=="6" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp32
	copy %workdir%\.pio\build\esp32\firmware.bin %workdir%\out_bin
)
if "%choice%"=="7" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266@160)
if "%choice%"=="8" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266)
if "%choice%"=="9" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp32)
if "%choice%"=="7D" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266@160dev)
if "%choice%"=="8D" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266dev)
if "%choice%"=="D" (start %workdir%\SerialMonitor.cmd)
if "%choice%"=="u" (
	cd %workdir%\resources\
	start respack.cmd
	cd %workdir%
)
if "%choice%"=="b" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target buildfs --environment esp8266@160
	copy %workdir%\.pio\build\esp8266@160\littlefs.bin %workdir%\out_bin
)
if "%choice%"=="f" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target uploadfs --environment esp8266@160)
if "%choice%"=="e" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target erase --environment esp8266@160)
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
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" lib update
)
if "%choice%"=="m" (start cmd)
if "%choice%"=="R" (rmdir /S "%USERPROFILE%\.platformio")

Echo.
Echo.
Echo.
pause
del %workdir%\resources\.wget-hsts
cls
goto m1


exit