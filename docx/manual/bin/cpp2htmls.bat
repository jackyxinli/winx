@echo off
if "%1" == "" (
	echo "Usage: cpp2htmls <cpp_file>"
	goto exit
	)
set AppPath=%~dp0
"%AppPath%\cpp2json.exe" %1 | php "%AppPath%\gen_htmls"
set AppPath=
:exit
