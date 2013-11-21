@echo off
if %PROCESSOR_ARCHITECTURE%==x86 (
         set MSBuild="%SystemRoot%\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
) else ( set MSBuild="%SystemRoot%\Microsoft.NET\Framework64\v4.0.30319\MSBuild.exe"
)
echo ---------------------------
echo   Clean up
echo ---------------------------
del todo.db3
del testout
rmdir /s /q bin
rmdir /s /q lib
echo ---------------------------
%MSBuild% ctodo_dll_100.vcxproj /p:Configuration=Release

pause