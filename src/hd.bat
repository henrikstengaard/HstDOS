@echo off
:menu
hstdos.exe /d e:\
if ERRORLEVEL 1 goto end
call %TEMP%_run.bat
goto menu
:end