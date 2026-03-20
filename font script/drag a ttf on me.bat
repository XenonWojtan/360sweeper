@echo off
if "%~1"=="" (
    echo Drag a .ttf file onto this script to use it.
    pause
    exit /b
)
python font.py "%~1"
pause
