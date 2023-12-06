@echo off

xcopy "%~dp0QTSetupPlugin\Release\QTSetupPlugin.dll" "%NSIS_DIR%\Plugins\x86-unicode\" /y

call make_bin.bat

set binDir=%~dp0\Bin

python.exe DeleteUselessDependFile.py "%~dp0Package\Release" "Release"

python.exe NsisScriptGenerate.py "%~dp0nsis_setup_template.nsi" "%binDir%"

"%NSIS_DIR%\makensisw.exe" "%~dp0nsis_setup.nsi"

::PAUSE