@echo off
if %PROCESSOR_ARCHITECTURE%==x86 (
	set MSBuild="%SystemRoot%\Microsoft.NET\Framework\v4.0.30319\msbuild.exe"
) else (
	set MSBUILD=%WINDIR%\Microsoft.NET\Framework64\v4.0.30319\MSBuild.exe
)

%MSBuild% ctodo_lib_110.vcxproj /p:Configuration=Release
%MSBuild% cctodo_110.vcxproj /p:Configuration=Release
echo ---------------------------
echo   Make bin
echo ---------------------------
mkdir bin
COPY Release\cctodo.exe bin\cctodo.exe
rmdir /s /q Release
echo ---------------------------
echo   Clean up
echo ---------------------------
del todo.db3
del testout
echo ---------------------------
echo   Running CToDo Tests
echo ---------------------------
set todo=Release\cctodo.exe
%todo% --help
echo ---------------------------
echo        Initdb
echo ---------------------------
%todo% initdb
echo Done
echo ---------------------------
echo        Write
echo ---------------------------
%todo% I am very long string and there is no mistakes parsing me. My maximum is 255 so Im not 255
%todo% I contain weird symbols alike -+=!@#$%^&*() and there is still no errors
%todo% --first I am Chuck Norris
%todo% --motivate motivation test
%todo%
echo ---------------------------
echo        Swap
echo ---------------------------
%todo% swap 2 3
%todo%
echo ---------------------------
echo        Remove
echo ---------------------------
%todo% rm 3
%todo%
echo ---------------------------
echo        Edit
echo ---------------------------
%todo% edit 2 Im new string
%todo%
echo ---------------------------
echo        Sync
echo ---------------------------
%todo% set git 0
%todo% set syncfile testout
%todo% sync
echo ---------------------------
type testout
echo ---------------------------
echo        tests passed
echo ---------------------------
del todo.db3
del testout
pause