color 0A
setlocal
echo off
@chcp UTF8>nul
del %workdir%\platformio-device-monitor-*.log
cls 
platformio.exe device monitor -f esp8266_exception_decoder -f log2file