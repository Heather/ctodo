set MSBuild="%SystemRoot%\Microsoft.NET\Framework\v4.0.30319\msbuild.exe"

%MSBuild% ctodo.vcxproj /p:Configuration=Release

@echo off
echo ---------------------------
echo   Running CToDo Tests
echo ---------------------------
set todo=Release\ctodo.exe
%todo% --help
echo ---------------------------
echo        Initdb
echo ---------------------------
%todo% initdb
%todo% clean
echo ---------------------------
echo        Write
echo ---------------------------
%todo% write I am very long string and there is no mistakes parsing me. My maximum is 255 so Im not 255
%todo% write I contain weird symbols alike -+=!@#$%^&*() and there is still no errors
%todo% write I am Chuck Norris
%todo% read
echo ---------------------------
echo        Swap
echo ---------------------------
%todo% swap 2 3
%todo% read
echo ---------------------------
echo        Remove
echo ---------------------------
%todo% rm 3
%todo% read
echo ---------------------------
echo        Edit
echo ---------------------------
%todo% edit 2 Im new string and now Im replacing Chuck Norris
%todo% read
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
pause