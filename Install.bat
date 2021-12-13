color 0A
SETLOCAL EnableDelayedExpansion
set workdir=%~dp0
PATH=%PATH%;%workdir%;%USERPROFILE%\.platformio\penv\Scripts;
echo off
@chcp 1251>nul
mode con: cols=88 lines=40
cls

:m1
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *                  Commands                *  (Russian)      Êîìàíäû                 * 
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *             Install tools                *     Óñòàíîâêà èíñòðóìåíòîâ è ñðåäû      * 
Echo  *  Install Python          (step 1) - (0)  *  Óñòàíîâèòü Python             (Øàã 1)  * 
Echo  *  Install Platformio Core (step 2) - (1)  *  Óñòàíîâèòü PIO Core           (Øàã 2)  * 
Echo  *  Install Git for Windows (step 3) - (2)  *  Óñòàíîâèòü Git                (Øàã 3)  * 
Echo  *  Get firmware repository (step 4) - (3)  *  Ïîëó÷èòü ðåïîçèòîðèé ïðîøèâêè (Øàã 4)  *
Echo  #------------------------------------------#-----------------------------------------# 
Echo  *  Remove Platformio installation   - (R)  *  Ïîëíîñòüþ óäàëèòü Platformio ñ ÏÊ      * 
Echo  #------------------------------------------#-----------------------------------------#
Echo.
Set /p choice="Your choice (Âàø âûáîð): "

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
	if exist "%USERPROFILE%\Python\python.exe" (
		mklink %USERPROFILE%\Python\python3.exe %USERPROFILE%\Python\python.exe
		mklink %USERPROFILE%\Python\Scripts\pip3.exe %USERPROFILE%\Python\Scripts\pip.exe
		set PYTHONHOME=%USERPROFILE%\Python
		set PYTHONPATH=%USERPROFILE%\Python
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
	Set /p diskInstal="Enter a drive letter C,D etc. (Ââåäèòå áóêâó äèñêà C,D è ò.ï): "
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
		"%ProgramFiles%\Git\bin\git" clone https://github.com/andy040670/FireLamp_JeeUI.git
		start .\FireLamp_JeeUI
	)
)

if "%choice%"=="R" (rmdir /S %USERPROFILE%\.platformio)

Echo.
pause
cls
goto m1
