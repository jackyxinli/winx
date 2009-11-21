@echo off
set AppPath=%~dp0
php "%AppPath%\gen_htmls" %1 %2 %3 %4
set AppPath=