@echo off
@chcp 1251>nul
set workdir=%~dp0

if not exist %workdir%\LAMP_user.cfg (
	if exist %workdir%\LAMP_user.cfg.default (
		copy %workdir%\LAMP_user.cfg.default %workdir%\LAMP_user.cfg
		"C:\Windows\System32\notepad.exe" %workdir%\LAMP_user.cfg
	)
)

rem Read values from cfg file
For /F "eol=# tokens=1* delims==" %%i In (%workdir%\LAMP_user.cfg) Do Set %%i=%%j

rem Check filename extention
If "%~1"=="" Goto :M1
For %%a In (565 332) Do If ".%%a"=="%~x1" Goto :M2
echo Wrong file
pause>Nul
Exit


:M2
if exist "%workdir%\ftpscript.txt" (del %workdir%\ftpscript.txt)

echo open %LAMP_IP% >> %workdir%\ftpscript.txt
echo %FTP_USER% >> %workdir%\ftpscript.txt
echo %FTP_PASSWORD% >> %workdir%\ftpscript.txt
echo binary >> %workdir%\ftpscript.txt
echo cd animations >> %workdir%\ftpscript.txt
echo put %1 >> %workdir%\ftpscript.txt
echo quit >> %workdir%\ftpscript.txt

ftp -s:%workdir%\ftpscript.txt

del %workdir%\ftpscript.txt
pause
exit


:M0
Echo  #------------------------------------------#-----------------------------------------#
Echo  *  Get list of animations files          1 *  Отримати список файлів анімації        *
Echo  *  Delete file (by filename)             2 *  Видалити файл (за ім'ям)               *
Echo  *  Exit                                  3 *  Вийти                                  *
Echo  #------------------------------------------#-----------------------------------------#
Echo.
Echo.
Set /p choice="Your choice (Ваш вибір): "
if "%choice%"=="1" goto :M1
if "%choice%"=="2" (
	Set /p filename="Filename to delete (Їм'я файла для видалення): "
	goto :M1
)
if "%choice%"=="3" exit

:M1
if exist "%workdir%\ftpscript.txt" (del %workdir%\ftpscript.txt)
cls
echo open %LAMP_IP% >> %workdir%\ftpscript.txt
echo %FTP_USER% >> %workdir%\ftpscript.txt
echo %FTP_PASSWORD% >> %workdir%\ftpscript.txt
rem echo binary >> %workdir%\ftpscript.txt
echo cd animations >> %workdir%\ftpscript.txt
if "%choice%"=="2" (
	echo del %filename%  >> %workdir%\ftpscript.txt
)
echo dir >> %workdir%\ftpscript.txt
echo quit >> %workdir%\ftpscript.txt

ftp -s:%workdir%\ftpscript.txt

del %workdir%\ftpscript.txt
goto :M0


pause
Exit