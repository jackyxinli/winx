@echo off
set AppPath=%~dp0
php "%AppPath%\jspt" "%AppPath%\inc\print.php" %1 %2 %3 %4
set AppPath=
