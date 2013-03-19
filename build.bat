@echo off
make -f Makefile.win clean
make -f Makefile.win

todo.exe --help
echo ---------------------------------------------------
todo.exe
echo ---------------------------------------------------

pause