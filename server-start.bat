@ECHO off
rem nc64.exe can be found: here https://eternallybored.org/misc/netcat/

@ECHO on
.\build\kxm.exe build\config.txt  | nc64.exe -Lvp 63445
@ECHO off

echo.
echo ========================
echo [-] Server Exited early.
echo ========================
echo.

pause
