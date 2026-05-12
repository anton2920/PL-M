@echo off

set INST=C:\Intel
set TPATH=%PATH%
set PATH=%PATH%;%INST%;%INST%\Sample;%INST%\IC86

REM PLM86 main.plm TYPE DEBUG LIST CODE XREF OPTIMIZE(0)
PLM86 main.plm NOTYPE NOLIST CODE OPTIMIZE(3)
IF errorlevel = 1 GOTO STOP

IC86 test.c CODE
IF errorlevel = 1 GOTO STOP

ASM86 dos.asm NOLIST
IF errorlevel = 1 GOTO STOP

LINK86 main.obj, test.obj, dos.obj TO main.86 BIND
IF errorlevel = 1 GOTO STOP

UDI2DOS main.86
IF errorlevel = 1 GOTO STOP

main

:STOP
set PATH=%TPATH%
set TPATH=
set INST=
