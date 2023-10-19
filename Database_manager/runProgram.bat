
@REM SET mypath=%~dp0
@REM cd %mypath%

@echo off
if "%1"=="debug" (
    echo Running Debug mode
    @REM %mypath%\bin\debug\arkanoid_d.exe
    bin\debug\DatabaseManager_d.exe
) else if "%1"=="release" (
    echo Running Release mode
    @REM %mypath%\bin\release\arkanoid.exe
    bin\release\DatabaseManager.exe
) else (
    echo Parameter not permitted. Use "debug" or "release".
)