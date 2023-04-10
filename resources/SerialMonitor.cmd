color 0A
setlocal
set workdir=%~dp0
echo off
if exist "%USERPROFILE%\Python\python.exe" (
	set PYTHONHOME=%USERPROFILE%\Python
	set PYTHONPATH=%USERPROFILE%\Python
)
PATH=%PATH%;%workdir%;%USERPROFILE%\.platformio\penv\Scripts;%PYTHONPATH%;
@chcp UTF8>nul
cls 
platformio.exe device monitor -f esp8266_exception_decoder -f log2file