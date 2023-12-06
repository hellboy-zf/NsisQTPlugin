set binSrc=%~dp0App
set binDst=%~dp0Bin


echo #删除Bin中旧数据
rd "%binDst%" /s /q

echo #Client\bin文件夹到Bin中
xcopy "%binSrc%" "%binDst%\" /e /i /y

echo #复制windows系统dll

set sysWOWPath=%windir%\SysWOW64

xcopy "%sysWOWPath%\concrt140.dll" "%binDst%\" /y
xcopy "%sysWOWPath%\msvcp140.dll" "%binDst%\" /y
xcopy "%sysWOWPath%\msvcp140_1.dll" "%binDst%\" /y
xcopy "%sysWOWPath%\msvcp140_2.dll" "%binDst%\" /y
xcopy "%sysWOWPath%\ucrtbase.dll" "%binDst%\" /y
xcopy "%sysWOWPath%\vcruntime140.dll" "%binDst%\" /y
xcopy "%sysWOWPath%\msvcrt.dll" "%binDst%\" /y
xcopy "%sysWOWPath%\msvcp140_atomic_wait.dll" "%binDst%\" /y

echo #删除Bin中临时文件

del /a /f /s /q  "%~dp0Bin*.log" "*.bak" "*.iobj" "*.pdb" "*.ilk" "*.exp" 

::pause