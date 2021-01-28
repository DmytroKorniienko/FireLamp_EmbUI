setlocal
set workdir=%~dp0
PATH=%PATH%;%workdir%;%USERPROFILE%\.platformio\penv\Scripts
echo off
cls

:m1
Echo  #*******************************#
Echo  *            Commands           *
Echo  *-------------------------------*
Echo  * Install Platformio CLI    - 0 *
Echo  * Update DEV from GitHub    - 1 *
Echo  *-------------------------------*
Echo  *   Build and upload firmware   * 
Echo  * Esp8266@160               - 2 *
Echo  * Esp8266@80                  3 *
Echo  *-------------------------------*
Echo  * Build and upload File Sys - 4 *
Echo  *-------------------------------*
Echo  * CMD window                - 5 *
Echo  #*******************************#
Echo.
Set /p choice="Your choice: "
if not defined choice goto m1
if "%choice%"=="0" (start python Get-Platformio.py)
if "%choice%"=="1" (start update-DEV-from-Git.cmd)
if "%choice%"=="2" (%USERPROFILE%\.platformio\penv\Scripts\pio.exe run --target upload --environment esp8266@160)
if "%choice%"=="3" (%USERPROFILE%\.platformio\penv\Scripts\pio.exe run --target upload --environment esp8266)
if "%choice%"=="4" (%USERPROFILE%\.platformio\penv\Scripts\pio.exe run --target uploadfs --environment esp8266@160)
if "%choice%"=="5" (start cmd)
Echo.
Echo Choice wrong!
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