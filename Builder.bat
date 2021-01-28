setlocal
set workdir=%~dp0
PATH=%PATH%;%workdir%;%USERPROFILE%\.platformio\penv\Scripts
echo off
cls

:m1
Echo  #*******************************#
Echo  *            Commands           *
Echo  *-------------------------------*
Echo  * Build and upload firmware - 1 *
Echo  * Build and upload File Sys - 2 *
Echo  * CMD window                - 3 *
Echo  #*******************************#
Echo.
Set /p choice="Your choice: "
if not defined choice goto m1
if "%choice%"=="1" (%USERPROFILE%\.platformio\penv\Scripts\pio.exe run --target upload --environment esp8266@160)
if "%choice%"=="2" (%USERPROFILE%\.platformio\penv\Scripts\pio.exe run --target uploadfs --environment esp8266@160)
if "%choice%"=="3" (start cmd)
Echo.
Echo Не правильно сделан выбор задания
Echo.
Echo.
pause
cls
goto m1

rem %USERPROFILE%\.platformio\penv\Scripts\pio.exe run --environment esp8266@160
rem %USERPROFILE%\.platformio\penv\Scripts\pio.exe run --target upload --environment esp8266@160
rem %USERPROFILE%\.platformio\penv\Scripts\pio.exe run --target uploadfs --environment esp8266@160

rem cmd

exit