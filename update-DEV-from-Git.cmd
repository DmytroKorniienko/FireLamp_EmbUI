setlocal
echo off
set workdir=%~dp0
PATH=%PATH%;%workdir%;%ProgramFiles%\Git;%ProgramFiles%\Git\bin
cls
Echo.

"%ProgramFiles%\Git\bin\git" checkout dev

Echo.
Echo.
Echo.
Echo.

"%ProgramFiles%\Git\bin\git" pull

pause
exit


