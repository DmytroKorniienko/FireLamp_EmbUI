color 0A
setlocal
set workdir=%~dp0
PATH=%PATH%;%workdir%;%USERPROFILE%\.platformio\penv\Scripts;
echo off
set PYTHONHOME=%USERPROFILE%\Python
set PYTHONPATH=%USERPROFILE%\Python
@chcp 1251>nul
mode con: cols=88 lines=40
cls

:m1
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *                [96mCOMMANDS[92m                  *  (Russian)      [96mКОМАНДЫ[92m                 *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *              [93mGIT commands[92m                *              [93mРабота с GIT[92m               *
Echo  *  Switch to MASTER branch          - ([97m1[92m)  *  Переключиться на ветку MASTER          *
Echo  *  Switch to DEV branch             - ([97m2[92m)  *  Переключиться на ветку DEV             *
Echo  #------------------------------------------#-----------------------------------------# 
echo  *  Reset changes in local repo!     - ([97m3[92m)  *  Сбросить изменения в локальном репо!   *
Echo  *  [91mWARNING![92m This will revert all changes!  *  [91mВНИМАНИЕ![92m Это сбросит все изменения!   *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *             [93mBuild and flash[92m              *            [93mСборка и прошивка[92m            *
Echo  *  Build - Esp8266 160MHz           - ([97m4[92m)  *  Собрать для Esp8266 на 160МГц          *
Echo  *  Build - Esp8266 80MHz            - ([97m5[92m)  *  Собрать для Esp8266 на 80МГц           *
Echo  *  Build - Esp32                    - ([97m6[92m)  *  Собрать для Esp32                      *
Echo  *                                          *                                         *
Echo  *  Build and upload - Esp8266@160   - ([97m7[92m)  *  Собрать и прошить - Esp8266 на 160МГц  *
Echo  *  Build and upload - Esp8266@80    - ([97m8[92m)  *  Собрать и прошить - Esp8266 на 80МГц   *
Echo  *  Build and upload - Esp32         - ([97m9[92m)  *  Собрать и прошить - Esp32              *
Echo  *                                          *                                         *
Echo  *            [93mFile System[92m                   *           [93mФайловая Система[92m              *
Echo  *  Update FS data from framework    - ([97mu[92m)  *  Обновить файлы ФС из фреймворка        *
Echo  *  Build File System image          - ([97mb[92m)  *  Собрать образ Файловой Системы         *
Echo  *  Build and upload File System     - ([97mf[92m)  *  Собрать и прошить Файловую Систему     *
Echo  *                                          *                                         *
Echo  *  Erase Flash                      - ([97me[92m)  *  Стереть флеш контроллера               *
Echo  *                                          *                                         *
Echo  *  Clean up temp files .pio         - ([97mc[92m)  *  Очистить временные файлы .pio          *
Echo  *  Update libs and PIO Core         - ([97mg[92m)  *  Обновить библиотеки и среду PIO Core   *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  CMD window                       - ([97mm[92m)  *  Открыть окно коммандной строки CMD     *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  [91mRemove[92m Platformio installation   - [91m([97mR[91m)[92m  *  Полностью [91mудалить[92m Platformio с ПК      *
Echo  #------------------------------------------#-----------------------------------------#
Echo.
Set /p choice="[96mYour choice (Ваш выбор): [92m"


if "%choice%"=="1" call update-DEV-from-Git.cmd 1
if "%choice%"=="2" call update-DEV-from-Git.cmd 2
if "%choice%"=="3" call update-DEV-from-Git.cmd 3
if "%choice%"=="4" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp8266@160)
if "%choice%"=="5" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp8266)
if "%choice%"=="6" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp32)
if "%choice%"=="7" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266@160)
if "%choice%"=="8" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266)
if "%choice%"=="9" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp32)
if "%choice%"=="u" (
	cd %workdir%\resources\
	start respack.cmd
	cd %workdir%
)
if "%choice%"=="b" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target buildfs --environment esp8266@160)
if "%choice%"=="f" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target uploadfs --environment esp8266@160)
if "%choice%"=="e" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target erase --environment esp8266@160)
if "%choice%"=="c" (
	pio system prune -f
	rmdir /S /Q %workdir%\.pio
)
if "%choice%"=="g" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" upgrade
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" update
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