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
Echo  *                COMMANDS                  *               ����������               *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *  Switch/update - MASTER branch         1 *  �����������\�������- ����� MASTER      *
Echo  *  Switch/update - DEV branch            2 *  �����������\�������- ����� DEV         *
Echo  #------------------------------------------#-----------------------------------------# 
echo  *  Reset changes in local repo!          3 *  ������� ���� � �������� ����������! *
Echo  *  WARNING! This will revert all changes!  *  �����! �� ����� �� ����              *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *                Build only                *              ҳ���� �����              *
Echo  *  Build - Esp8266 160MHz                4 *  ǳ����� ��� Esp8266 �� 160���          *
Echo  *  Build - Esp8266 80MHz                 5 *  ǳ����� ��� Esp8266 �� 80���           *
Echo  *  Build - Esp32                         6 *  ǳ����� ��� Esp32                      *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *             Build and flash              *            ����� � ����������          *
Echo  *  Build and upload - Esp8266@160        7 *  ǳ����� � ������� - Esp8266 �� 160���  *
Echo  *  Build and upload - Esp8266@80         8 *  ǳ����� � ������� - Esp8266 �� 80���   *
Echo  *  Build and upload - Esp32              9 *  ǳ����� � ������� - Esp32              *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *         Build and flash (DEBUG)          *      ����� � ����������  (�����)       *
Echo  *  Build and upload - Esp8266@160       7D *  ǳ����� � ������� - Esp8266 �� 160���  *
Echo  *  Build and upload - Esp8266@80        8D *  ǳ����� � ������� - Esp8266 �� 80���   *
Echo  *  Serial port monitor                   D *  ���� � ������� �������              *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Update FS data from framework         u *  ������� ����� �� � ����������          *
Echo  *          FS for ESP8266                  *              ��� ESP8266                *
Echo  *  Build File System image               b *  ǳ����� ������� �������                *
Echo  *  Build and upload File System          f *  ǳ����� � ������� ������� �������      *
Echo  *            FS for ESP32                  *              ��� ESP32                  *
Echo  *  Build File System image              b1 *  ǳ����� ������� �������                *
Echo  *  Build and upload File System         f1 *  ǳ����� � ������� ������� �������      *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Erase Flash    ESP8266                e *  ������ ����    ESP8266                 *
Echo  *  Erase Flash    ESP32                 e1 *  ������ ����    ESP32                   *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Update libs and PIO Core              g *  ������� �������� � ���������� PIOCore*
Echo  *  Clean up temp files .pio              c *  ³�������� �������� ����� .pio        *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  CMD window                            m *  ³������ ��������� �����               *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  Remove Platformio installation        R *  ������� �������� Platformio           *
Echo  #------------------------------------------#-----------------------------------------#
Echo.
Set /p choice="Your choice (��� ����): "


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