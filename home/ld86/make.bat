@echo off

REM PLM86 main.plm TYPE DEBUG LIST CODE XREF OPTIMIZE(0)
PLM86 main.plm NOTYPE NOLIST NOCODE OPTIMIZE(3)
IF ERRORLEVEL 1 GOTO STOP

LINK86 main.obj, C:\Intel\PLM86.LIB, C:\Intel\8087.LIB, C:\Intel\doslibs.obj TO main.86 BIND
IF ERRORLEVEL 1 GOTO STOP

UDI2DOS main.86
IF ERRORLEVEL 1 GOTO STOP

main
IF ERRORLEVEL 1 GOTO STOP

main %BASE%\EXPERI~1\asm\main.obj >result.obj
main %BASE%\EXPERI~1\asm\main.lnk >result.lnk
main %BASE%\EXPERI~1\asm\main.86  >result.86
main %BASE%\EXPERI~1\asm\main.bin >result.bin

:STOP
