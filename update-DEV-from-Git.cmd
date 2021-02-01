setlocal
set param=%1
echo off
set workdir=%~dp0
PATH=%PATH%;%workdir%;%ProgramFiles%\Git;%ProgramFiles%\Git\bin
cls
Echo.

if "%param%"=="1" ( rem переключаем на мастер
	"%ProgramFiles%\Git\bin\git" checkout master
	"%ProgramFiles%\Git\bin\git" pull
)
if "%param%"=="2" ( rem переключаем на дев
	"%ProgramFiles%\Git\bin\git" checkout dev
	"%ProgramFiles%\Git\bin\git" pull
)
if "%param%"=="3" ( rem сброс состояния локального репозитория
	"%ProgramFiles%\Git\bin\git" reset
	"%ProgramFiles%\Git\bin\git" checkout .
	"%ProgramFiles%\Git\bin\git" pull
)
Echo.




