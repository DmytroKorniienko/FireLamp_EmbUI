color 0A
SETLOCAL EnableDelayedExpansion
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
Echo  *                  [96mCommands[92m                *  (Russian)      [96mКоманды[92m                 * 
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *             Install tools                *     Установка инструментов и среды      * 
Echo  *  Install Python          ([93mstep 1[92m) - ([97m0[92m)  *  Установить Python             ([93mШаг 1[92m)  * 
Echo  *  Install Platformio Core ([93mstep 2[92m) - ([97m1[92m)  *  Установить PIO Core           ([93mШаг 2[92m)  * 
Echo  *  Install Git for Windows ([93mstep 3[92m) - ([97m2[92m)  *  Установить Git                ([93mШаг 3[92m)  * 
Echo  *  Get firmware repository ([93mstep 4[92m) - ([97m3[92m)  *  Получить репозиторий прошивки ([93mШаг 4[92m)  *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *  Remove Platformio installation   - [91m([97mR[91m)[92m  *  Полностью удалить Platformio с ПК      * 
Echo  #------------------------------------------#-----------------------------------------#
Echo.
Set /p choice="[96mYour choice (Ваш выбор): [92m"

if "%choice%"=="0" (
	if not exist "%systemdrive%\Program Files (x86)" (
		%workdir%\resources\wget https://www.python.org/ftp/python/3.8.7/python-3.8.7.exe -O "%TMP%\python.exe"
	) else (
		%workdir%\resources\wget https://www.python.org/ftp/python/3.8.7/python-3.8.7-amd64.exe -O "%TMP%\python.exe"
	)
	
	%TMP%\python.exe /passive InstallAllUsers=0 PrependPath=1 Include_pip=1 Include_launcher=1 AssociateFiles=1 TargetDir=%USERPROFILE%\Python
	if exist "%USERPROFILE%\AppData\Local\Microsoft\WindowsApps\python.exe" (
		del %USERPROFILE%\AppData\Local\Microsoft\WindowsApps\python.exe
		del %USERPROFILE%\AppData\Local\Microsoft\WindowsApps\python3.exe
		del %USERPROFILE%\AppData\Local\Microsoft\WindowsApps\pip.exe
		mklink %USERPROFILE%\AppData\Local\Microsoft\WindowsApps\python.exe %USERPROFILE%\Python\python.exe
		mklink %USERPROFILE%\AppData\Local\Microsoft\WindowsApps\python3.exe %USERPROFILE%\Python\python.exe
		mklink %USERPROFILE%\AppData\Local\Microsoft\WindowsApps\pip.exe %USERPROFILE%\Python\Scripts\pip.exe
		mklink %USERPROFILE%\AppData\Local\Microsoft\WindowsApps\pip3.exe %USERPROFILE%\Python\Scripts\pip.exe
	)
)

if "%choice%"=="1" (
	%workdir%\resources\wget https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -O %workdir%\get-platformio.py
	python %workdir%\get-platformio.py
	del %workdir%\get-platformio.py
)

if "%choice%"=="2" (
	if not exist "%systemdrive%\Program Files (x86)" (
		%workdir%\resources\wget https://github.com/git-for-windows/git/releases/download/v2.30.0.windows.2/Git-2.30.0.2-32-bit.exe -O %TMP%\git.exe
	) else (
		%workdir%\resources\wget https://github.com/git-for-windows/git/releases/download/v2.30.0.windows.2/Git-2.30.0.2-64-bit.exe -O %TMP%\git.exe
	)
	
	%TMP%\git.exe /SILENT
	del %TMP%\git.exe
)

if "%choice%"=="3" (
	Set /p diskInstal="Enter a drive letter C,D etc. (Введите букву диска C,D и т.п): "
	rem echo  test !%diskInstal!
	if not exist "!diskInstal!:\" (
		echo Disk letter is wrong!
		pause
		cls
		goto m1

	) else (
		echo "Firmware repo wil be instaled on disk !diskInstal!:"
		!diskInstal!:
		if exist "!diskInstal!:\FireLamp_JeeUI" (rmdir /S /Q !diskInstal!:\FireLamp_JeeUI)
		"%ProgramFiles%\Git\bin\git" clone https://github.com/DmytroKorniienko/FireLamp_JeeUI.git
		start .\FireLamp_JeeUI
	)
)

if "%choice%"=="R" (rmdir /S %USERPROFILE%\.platformio)

Echo.
pause
cls
goto m1