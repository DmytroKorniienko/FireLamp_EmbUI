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
Echo  *                COMMANDS                  *  (Russian)      КОМАНДЫ                 *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *              GIT commands                *              Работа с GIT               *
Echo  *  Switch to MASTER branch          - (1)  *  Переключиться на ветку MASTER          *
Echo  *  Switch to DEV branch             - (2)  *  Переключиться на ветку DEV             *
Echo  #------------------------------------------#-----------------------------------------# 
echo  *  Reset changes in local repo!     - (3)  *  Сбросить изменения в локальном репо!   *
Echo  *  WARNING! This will revert all changes!  *  ВНИМАНИЕ! Это сбросит все изменения!   *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *             Build and flash              *            Сборка и прошивка            *
Echo  *  Build - Esp8266 160MHz           - (4)  *  Собрать для Esp8266 на 160МГц          *
Echo  *  Build - Esp8266 80MHz            - (5)  *  Собрать для Esp8266 на 80МГц           *
Echo  *  Build - Esp32                    - (6)  *  Собрать для Esp32                      *
Echo  *                                          *                                         *
Echo  *  Build and upload - Esp8266@160   - (7)  *  Собрать и прошить - Esp8266 на 160МГц  *
Echo  *  Build and upload - Esp8266@80    - (8)  *  Собрать и прошить - Esp8266 на 80МГц   *
Echo  *  Build and upload - Esp32         - (9)  *  Собрать и прошить - Esp32              *
Echo  *                                          *                                         *
Echo  *            File System                   *           Файловая Система              *
Echo  *  Update FS data from framework    - (u)  *  Обновить файлы ФС из фреймворка        *
Echo  *  Build File System image          - (b)  *  Собрать образ Файловой Системы         *
Echo  *  Build and upload File System     - (f)  *  Собрать и прошить Файловую Систему     *
Echo  *                                          *                                         *
Echo  *  Erase Flash                      - (e)  *  Стереть флеш контроллера               *
Echo  *                                          *                                         *
Echo  *  Clean up temp files .pio         - (c)  *  Очистить временные файлы .pio          *
Echo  *  Update libs and PIO Core         - (g)  *  Обновить библиотеки и среду PIO Core   *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  CMD window                       - (m)  *  Открыть окно коммандной строки CMD     *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  Remove Platformio installation   - (R)  *  Полностью удалить Platformio с ПК      *
Echo  #------------------------------------------#-----------------------------------------#
Echo.
Set /p choice="Your choice (Ваш выбор): "


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