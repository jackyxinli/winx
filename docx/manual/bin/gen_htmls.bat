@echo off
set AppPath=%~dp0
php "%AppPath%\gen_cpps" %1 %2 %3 %4
set AppPath=