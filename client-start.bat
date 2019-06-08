@ECHO off
set REMOTE_HOST=192.168.11.126
set REMOTE_PORT=63445

set NC_EXE=nc64.exe
set NC_FLAGS=-v
set PROG_NAME=.\build\kxm.exe

@ECHO on
%NC_EXE% %NC_FLAGS% %REMOTE_HOST% %REMOTE_PORT% | %PROG_NAME% build\config-client.txt
@ECHO off

echo.
echo ========================
echo [-] Client exited early.
echo ========================
echo.

pause
