set PROGRAM_NAME=kxm.exe

set CC_FLAGS=-O2

set LINKER_FLAGS=-luser32

set SRC_FILES=^
src\main.c ^
src\mousehook.c ^
src\keyboardhook.c ^
src\inputhandler.c ^
src\gui.c


rem make all
mkdir build
clang -o build\%PROGRAM_NAME% %CC_FLAGS% %LINKER_FLAGS% %SRC_FILES%

pause

