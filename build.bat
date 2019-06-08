@ECHO off
set PROGRAM_NAME=kxm.exe

set SRC_FILES=^
src\common\main.c ^
src\windows\gui.c ^
src\windows\input_hook\mousehook.c ^
src\windows\input_hook\keyboardhook.c ^
src\common\config\config.c ^
src\common\crypto\tweetnacl.c ^
src\common\crypto\encryption.c ^
src\common\keydefs\keydefs.c ^
src\common\inputhandler\inputhandler.c

set INC=^
-Isrc\windows ^
-Isrc\windows\input_hook ^
-Isrc\common\config ^
-Isrc\common\crypto ^
-Isrc\common\keydefs ^
-Isrc\common\inputhandler

set LINKER_FLAGS=^
-luser32 ^
-ladvapi32

set CC_FLAGS=-O2 -Wall -Werror -pedantic


rem Copy configuration files and compile program using clang
@ECHO on
mkdir build
copy /Y res\common\config.txt build\config.txt
copy /Y res\common\config-client.txt build\config-client.txt
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
