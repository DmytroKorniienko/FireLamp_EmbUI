@rem Look at this for zip command support: https://stackoverflow.com/questions/38782928/how-to-add-man-and-zip-to-git-bash-installation-on-windows
setlocal
set workdir=%~dp0
PATH=%PATH%;%workdir%;%ProgramFiles%\Git
cls
echo off

"%ProgramFiles%\Git\"git-bash.exe %workdir%\respack.sh
exit

