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
mode con: cols=88 lines=48
cls 

:m1
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *                COMMANDS                  *  (Russian)      �������                 *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *              Switch/update               *          �����������\��������           *
Echo  *  to MASTER branch                 - (1)  *  �� ����� MASTER                        *
Echo  *  to DEV branch                    - (2)  *  �� ����� DEV                           *
Echo  #------------------------------------------#-----------------------------------------# 
echo  *  Reset changes in local repo!     - (3)  *  �������� ��������� � ��������� ����!   *
Echo  *  WARNING! This will revert all changes!  *  ��������! ��� ������� ��� ���������!   *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *              Build only for              *              ������� ���                *
Echo  *  Esp8266 160MHz                   - (4)  *  Esp8266 �� 160���                      *
Echo  *  Esp8266 80MHz                    - (5)  *  Esp8266 �� 80���                       *
Echo  *  Esp32                            - (6)  *  Esp32                                  *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *           Build and flash for            *          ������ � �������� ���          *
Echo  *  Esp8266 160MHz                   - (7)  *  Esp8266 �� 160���                      *
Echo  *  Esp8266 80MHz                    - (8)  *  Esp8266 �� 80���                       *
Echo  *  Esp32                            - (9)  *  Esp32                                  *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *         Build and flash (DEBUG)          *      ������ � ��������  (� �����)       *
Echo  *  Esp8266 160MHz                  - (7d)  *  Esp8266 �� 160���                      *
Echo  *  Esp8266 80MHz                   - (8d)  *  Esp8266 �� 80���                       *
Echo  *  Serial port monitor              - (d)  *  ����� ���������� ���������� (���)      *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *              File System                 *            �������� �������             *
Echo  *  Update FS data from framework    - (u)  *  �������� ����� �� �� ����������        *
Echo  *  ** ESP8266                              *  ** ESP8266                             *
Echo  *  Build File System image          - (b)  *  ������� ����� �������� �������         *
Echo  *  Build and upload File System     - (f)  *  ������� � ������� �������� �������     *
Echo  *  ** ESP32 -----                          * ** ESP32 ------                         *
Echo  *  Build File System image         - (b1)  *  ������� ����� �������� �������         *
Echo  *  Build and upload File System    - (f1)  *  ������� � ������� �������� �������     *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Erase Flash ESP8266              - (e)  *  ������� ���� ����������� ESP8266       *
Echo  *  Erase Flash ESP32               - (e1)  *  ������� ���� ����������� ESP32         *
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Update libs and PIO Core         - (g)  *  �������� ���������� � ����� PIO Core   *
Echo  *  Clean up temp files .pio         - (c)  *  �������� ��������� ����� .pio          *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  CMD window                       - (m)  *  ������� ���� ���������� ������ CMD     *
Echo  *------------------------------------------#-----------------------------------------*
Echo  *  Remove Platformio installation   - (R)  *  ��������� ������� Platformio � ��      *
Echo  #------------------------------------------#-----------------------------------------#
Echo.
Set /p choice="Your choice (��� �����): " 


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
)
if "%choice%"=="5" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp8266)
if "%choice%"=="6" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --environment esp32)
if "%choice%"=="7" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266@160)
if "%choice%"=="8" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266)
if "%choice%"=="9" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp32)
if "%choice%"=="7d" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266@160dev)
if "%choice%"=="8d" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target upload --environment esp8266dev)
if "%choice%"=="d" (start %workdir%\SerialMonitor.cmd)
if "%choice%"=="u" (
	cd %workdir%\resources\
	start respack.cmd
	cd %workdir%
)
if "%choice%"=="b" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target buildfs --environment esp8266@160)
if "%choice%"=="f" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target uploadfs --environment esp8266@160)
if "%choice%"=="b1" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target buildfs --environment esp32)
if "%choice%"=="f1" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target uploadfs --environment esp32)
if "%choice%"=="e" ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target erase --environment esp8266@160)
if "%choice%"=="e1 ("%USERPROFILE%\.platformio\penv\Scripts\pio.exe" run --target erase --environment esp32)
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