@ECHO off
set PROGRAM_NAME=kxm.exe

set SRC_FILES=^
src\main.c ^
src\gui.c ^
src\config.c ^
src\inputhandler.c ^
src\input_hook\mousehook.c ^
src\input_hook\keyboardhook.c ^
src\input_hook\keydefs.c ^
src\crypto\tweetnacl.c ^
src\crypto\encryption.c

set INC=^
-Isrc ^
-Isrc\crypto ^
-Isrc\input_hook

set LINKER_FLAGS=^
-luser32 ^
-ladvapi32

set CC_FLAGS=-O2 -Wall -Werror

@ECHO on
mkdir build
copy /A /Y res\config.txt build\config.txt
clang -o build\%PROGRAM_NAME% %CC_FLAGS% %LINKER_FLAGS% %INC% %SRC_FILES%
@ECHO off

if %errorlevel% == 0 (
    echo.
    echo ============================
    echo Successfully built %PROGRAM_NAME%
    echo ============================
    echo.
)

pause

