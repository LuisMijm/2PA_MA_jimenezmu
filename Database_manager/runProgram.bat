
@REM SET mypath=%~dp0
@REM cd %mypath%

@echo off
if "%1"=="debug" (
    echo Running Debug mode
    @REM %mypath%\bin\debug\arkanoid_d.exe
    cd bin\debug\
    .\DatabaseManager_d.exe
    cd ..\..
) else if "%1"=="release" (
    echo Running Release mode
    @REM %mypath%\bin\release\arkanoid.exe
    cd bin\release
    .\DatabaseManager.exe
    cd ..\..
) else (
    echo Parameter not permitted. Use "debug" or "release".
)