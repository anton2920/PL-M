@echo off

set INST=C:\Intel
set PATH=%PATH%;%INST%;%INST%\Sample

REM PLM86 main.plm TYPE DEBUG LIST CODE XREF OPTIMIZE(1)
PLM86 main.plm DEBUG NOLIST OPTIMIZE(3)
IF errorlevel = 1 GOTO STOP

LINK86 main.obj, %INST%\PLM86.LIB, %INST%\8087.LIB, %INST%\Sample\doslibs.obj TO main.86 BIND
IF errorlevel = 1 GOTO STOP

UDI2DOS main.86
IF errorlevel = 1 GOTO STOP

main

:STOP
