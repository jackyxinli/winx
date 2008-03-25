@echo off

if "%MSDEVDIR%"=="" goto error
if not exist "%MSDEVDIR%\Template" md "%MSDEVDIR%\Template" 
echo on
copy zh-CN\winxwiz60.awx "%MSDEVDIR%\Template"
@pause
exit


:error
echo ERROR: Environ var 'MSDEVDIR' is undefined. MAYBE you can set 'MSDEVDIR' to be 'C:\Program Files\Microsoft Visual Studio\Common\MSDev98'?
pause
