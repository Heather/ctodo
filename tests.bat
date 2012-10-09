@echo off
set todo=Release\ctodo.exe
%todo% --version
%todo% --help
echo ---------------------------
echo        Initdb
echo ---------------------------
%todo% initdb
%todo% clean
echo ---------------------------
echo        Write
echo ---------------------------
%todo% write test1
%todo% write test2
%todo% write test3
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
%todo% edit 2 test4
%todo% read
echo ---------------------------
echo        tests passed
echo ---------------------------
pause