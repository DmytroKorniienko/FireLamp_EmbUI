setlocal
set workdir=%~dp0
PATH=%PATH%;%workdir%;%ProgramFiles%\Git
cls
echo off

git checkout dev
git pull
pause
exit

