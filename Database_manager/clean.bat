
@echo off
if "%1"=="debug" (
    del /q .\build\debug\*
    del /q .\bin\debug\*
) else if "%1"=="release" (
    del /q .\build\release\*
    del /q .\bin\release\*
) else if "%1"=="all" (
    del /q .\build\debug\*
    del /q .\bin\debug\*
    del /q .\build\release\*
    del /q .\bin\release\*
) else (
    echo Parameter not permitted. Use "debug" or "release" or "all".
)