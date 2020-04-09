@echo off
:menu
hstdos.exe /d e:\
if ERRORLEVEL 1 goto end
call c:\_run.bat
goto menu
:end