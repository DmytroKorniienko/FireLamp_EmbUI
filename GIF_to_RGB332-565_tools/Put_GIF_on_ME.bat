setlocal
set param=%1
echo off
set workdir=%~dp0
mode con: cols=40 lines=10
cls
Echo .
Set /p choice="For RGB565 input 1, else RGB332: "
Echo Wait, please. Processing...
if "%choice%"=="1" (
	%workdir%\"gif2rgb332_565.exe" %1% --r
) else (
	%workdir%\"gif2rgb332_565.exe" %1%
)
Echo .
pause