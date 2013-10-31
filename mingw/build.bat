@echo off
echo ----------------------------------------------------------------------------------------------
echo   Clean binaries
echo ----------------------------------------------------------------------------------------------
if exist test.exe (
    del test.exe
)
echo Done
echo ----------------------------------------------------------------------------------------------
echo   Make
echo ----------------------------------------------------------------------------------------------
make -f Makefile.win

echo ----------------------------------------------------------------------------------------------
echo   Clean up test stuff
echo ----------------------------------------------------------------------------------------------
if exist testdb.db3 (
    del testdb.db3
)
if exist testout (
    del testout
)
echo Done
pause