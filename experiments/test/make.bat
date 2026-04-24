@echo off

set INST=C:\Dist\PLM\Intel
set PATH=%PATH%;%INST%;%INST%\Sample

PLM86 main.plm NOTYPE NOLIST CODE OPTIMIZE(3)
IF errorlevel = 1 GOTO STOP

LINK86 main.obj, %INST%\Sample\doslibs.obj TO main.86 BIND
IF errorlevel = 1 GOTO STOP

UDI2DOS main.86
IF errorlevel = 1 GOTO STOP

main

:STOP
