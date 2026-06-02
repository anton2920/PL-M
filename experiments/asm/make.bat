@ECHO OFF

REM ASM86 main.asm LIST
MASM main.asm
IF ERRORLEVEL 1 GOTO STOP

REM LINK86 main.obj TO main.exe EXE
LINK86 main.obj TO main.86 BIND
LINK86 main.obj TO main.lnk INITCODE
REM IF ERRORLEVEL 1 GOTO STOP

LOC86 main.lnk TO main.bin
REM IF ERRORLEVEL 1 GOTO STOP

LINK /TINY main.lnk,main.com,,,,
IF ERRORLEVEL 1 GOTO STOP

:STOP
