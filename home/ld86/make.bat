@echo off

set INST=C:\Intel
set TPATH=%PATH%
set PATH=%PATH%;%INST%;%INST%\Sample

REM PLM86 main.plm TYPE DEBUG LIST CODE XREF OPTIMIZE(0)
PLM86 main.plm NOTYPE NOLIST NOCODE OPTIMIZE(3)
IF ERRORLEVEL 1 GOTO STOP

LINK86 main.obj, %INST%\PLM86.LIB, %INST%\8087.LIB, %INST%\Sample\doslibs.obj TO main.86 BIND
IF ERRORLEVEL 1 GOTO STOP

UDI2DOS main.86
IF ERRORLEVEL 1 GOTO STOP

main C:\Dist\PLM\EXPERI~1\asm\main.lnk
IF ERRORLEVEL 1 GOTO STOP

main C:\Dist\PLM\EXPERI~1\asm\main.obj >result.obj
main C:\Dist\PLM\EXPERI~1\asm\main.lnk >result.lnk
main C:\Dist\PLM\EXPERI~1\asm\main.86  >result.86
main C:\Dist\PLM\EXPERI~1\asm\main.bin >result.bin

:STOP
set PATH=%TPATH%
set TPATH=
set INST=
