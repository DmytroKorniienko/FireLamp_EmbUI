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
@echo on
curl -v http://%LAMP_IP: =%/update -F "name=@out_bin\littlefs.bin"
pause
exit
