@echo off
if "%1" == "" (
	echo "Usage: manual <cpp_file>"
	goto exit
	)
set AppPath=%~dp0
"%AppPath%\win32\cpp2json.exe" %1 | php "%AppPath%\gen_htmls"
set AppPath=
:exit
