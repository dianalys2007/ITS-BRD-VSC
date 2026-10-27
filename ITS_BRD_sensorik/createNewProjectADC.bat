@echo off
setlocal

REM Überprüfen, ob ein Argument übergeben wurde (Projektname)
if "%1"=="" (
    REM Falls kein Argument übergeben wurde, den Benutzer zur Eingabe auffordern
    set /p project_name="Bitte geben Sie einen ADC Projektnamen ein: "
) else (
    REM Projektname aus dem Argument übernehmen
    set "project_name=%1"
)

echo Erstelle ADC-Projekt mit dem Namen: %project_name%
REM Aufruf des PowerShell-Skripts
powershell -ExecutionPolicy Bypass -File "%~dp0createGenericProject.ps1" --adc %project_name%

REM Beende das Skript
exit /b 0
