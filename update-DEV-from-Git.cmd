setlocal
cls
echo off
set workdir=%~dp0
PATH=%PATH%;%workdir%;%ProgramFiles%\Git;%ProgramFiles%\Git\bin

"%ProgramFiles%\Git\bin\git" checkout dev

Echo.

"%ProgramFiles%\Git\bin\git" pull

Echo  #***************************************#
Echo  *     Trying to update your project     *
Echo  *     libraries and Platformio Core?    * 
Echo  *---------------------------------------*
Echo  * Yes                              - 1  *
Echo  * No (exit)          - any other input  *
Echo  #***************************************#
Echo.
Set /p choice="Your choice: "

if not defined choice exit
if "%choice%"=="1" (
%USERPROFILE%\.platformio\penv\Scripts\pio.exe upgrade
%USERPROFILE%\.platformio\penv\Scripts\pio.exe update
)
pause
exit


