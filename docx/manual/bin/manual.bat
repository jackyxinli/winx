@echo off
if "%1" == "" (
	echo "Usage: sdlc <server_description_language_file.sdl>"
	goto exit
	)
set AppPath=%~dp0
"%AppPath%\win32\sdl2json.exe" %1 | php "%AppPath%\gen_cpps"
set AppPath=
:exit
