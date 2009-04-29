@echo off
if "%CFG%"=="" set CFG=ALL
msdev mfc-port/mfc-port.dsw /MAKE "ALL - %CFG%"
msdev wtlext/wtlext.dsw /MAKE "ALL - %CFG%"
msdev wtl-port/wtl-port.dsw /MAKE "ALL - %CFG%"
pause
