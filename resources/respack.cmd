setlocal
set workdir=%~dp0
PATH=%PATH%;%workdir%;%ProgramFiles%\Git
cls
echo off

"%ProgramFiles%\Git\"git-bash.exe %workdir%\respack.sh
exit

