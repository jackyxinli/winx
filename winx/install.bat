@cd bin
@if not exist "%MSDEVDIR%\Template" md "%MSDEVDIR%\Template" 
copy winxwiz60.awx "%MSDEVDIR%\Template" 
@pause
