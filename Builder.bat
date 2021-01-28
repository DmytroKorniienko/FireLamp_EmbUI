setlocal
set workdir=%~dp0
PATH=%PATH%;%workdir%;%USERPROFILE%\.platformio\penv\Scripts
echo off
cls

:m1
Echo  #**************************************#
Echo  *               Commands               *
Echo  *--------------------------------------*
Echo  *            Install tools             * 
Echo  * Install Platformio CLI           - 0 *
Echo  * Install Git for Windows          - 1 *
Echo  *--------------------------------------*
Echo  *       Build and upload firmware      * 
Echo  * Update DEV branch from GitHub    - 2 *
Echo  * Esp8266@160                      - 3 *
Echo  * Esp8266@80                       - 4 *
Echo  * Esp32                            - 5 *
Echo  * Update FS data from framework    - 6 *
Echo  * Build and upload File System (FS)- 7 *
Echo  * Clean                            - 8 *
Echo  *--------------------------------------*
Echo  * CMD window                       - 9 *
Echo  *--------------------------------------*
Echo  * Clean Platformio installation    - R *
Echo  #**************************************#
Echo.
Set /p choice="Your choice: "

if not defined choice goto m1

if "%choice%"=="0" (start python Get-Platformio.py)
if "%choice%"=="1" (
if not exist "%systemdrive%\Program Files (x86)" (
    %workdir%\resources\wget https://github.com/git-for-windows/git/releases/download/v2.30.0.windows.2/Git-2.30.0.2-32-bit.exe -O %TMP%\git.exe
) else (
    %workdir%\resources\wget https://github.com/git-for-windows/git/releases/download/v2.30.0.windows.2/Git-2.30.0.2-64-bit.exe -O %TMP%\git.exe
)
%TMP%\git.exe
del %TMP%\git.exe
)

if "%choice%"=="2" (start update-DEV-from-Git.cmd)
if "%choice%"=="3" (%USERPROFILE%\.platformio\penv\Scripts\pio.exe run --target upload --environment esp8266@160)
if "%choice%"=="4" (%USERPROFILE%\.platformio\penv\Scripts\pio.exe run --target upload --environment esp8266)
if "%choice%"=="6" (cd %workdir%\resources\
respack.cmd
cd %workdir*
)

if "%choice%"=="7" (%USERPROFILE%\.platformio\penv\Scripts\pio.exe run --target uploadfs --environment esp8266@160)
if "%choice%"=="8" (rmdir /S %workdir%\.pio)
if "%choice%"=="9" (start cmd)
if "%choice%"=="R" (rmdir /S %USERPROFILE%\.platformio)
Echo.
Echo.
Echo.
pause
cls
goto m1


exit