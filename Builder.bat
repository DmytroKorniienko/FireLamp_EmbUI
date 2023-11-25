color 0A
setlocal enabledelayedexpansion
set workdir=%~dp0
echo off
if exist "%USERPROFILE%\Python\python.exe" (
	set PYTHONHOME=%USERPROFILE%\Python
	set PYTHONPATH=%USERPROFILE%\Python
	"%USERPROFILE%\Python\python.exe" builder_translate.py en_EN.lang
)
PATH=%PATH%;%workdir%;%USERPROFILE%\.platformio\penv\Scripts;%PYTHONPATH%;;%ProgramFiles%\Git
@chcp 65001>nul
mode con: cols=88 lines=45
cls 

if not exist "%workdir%\out_bin" (mkdir %workdir%\out_bin)


set "counter=0"
set "selection=-1"

:menu
cls

set "fileCount=0"
for %%f in (*.lang) do (
    set /a "fileCount+=1"
    set "fileName=%%~nf"
    echo !fileCount!. !fileName!
)

if %fileCount% equ 0 (
    echo There are no files with the extension .lang
    pause
    exit
)

choice /c 1234567890 /n /m "Choice your language: "
set "key=%errorlevel%"

if %key% gtr %fileCount% goto menu

set "selectedFile="
set "fileCount=0"
for %%f in (*.lang) do (
    set /a "fileCount+=1"
    if %key% equ !fileCount! set "selectedFile=%%~nf"
)


:m1
cls
type !selectedFile!.lang
Echo .
Echo .
Set /p choice="Your choice (Ваш вибір): "


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
if "%choice%"=="u1" (
	start %workdir%\Update_Firmware.bat
)
if "%choice%"=="u2" (
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
if "%choice%"=="b0" (
	"%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target buildfs --environment esp8266_160
	copy %workdir%\.pio\build\esp8266_160\littlefs.bin %workdir%\out_bin
)
if "%choice%"=="f0" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target uploadfs--environment esp8266_160)
if "%choice%"=="e0" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target erase --environment esp8266_160)
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
if "%choice%"=="w" (start cmd)

Echo.
Echo.
Echo.
pause
del %workdir%\resources\.wget-hsts
cls
goto m1


exit