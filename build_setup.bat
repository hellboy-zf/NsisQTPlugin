@echo off

python.exe DeleteUselessDependFile.py "%~dp0Package\Release" "Release"

python.exe NsisScriptGenerate.py "%~dp0nsis_setup_template.nsi" "%~dp0App"

"%NSIS_DIR%\makensisw.exe" "%~dp0\nsis_setup.nsi"

::PAUSE