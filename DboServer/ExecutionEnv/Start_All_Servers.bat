@echo off

start MasterServer.exe
timeout /t 1 /nobreak > nul

start QueryServer.exe
timeout /t 1 /nobreak > nul

start AuthServer.exe
timeout /t 1 /nobreak > nul

start CharServer.exe .\\config\\CharServer.ini
timeout /t 1 /nobreak > nul

start GameServer.exe .\\config\\GameServer.ini
timeout /t 1 /nobreak > nul

start ChatServer.exe
timeout /t 1 /nobreak > nul

exit

