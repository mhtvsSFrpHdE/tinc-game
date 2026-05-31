@cd /d %~dp0
@cd bin\tinc
@set /p networkName=Type network name: 
@set t=tinc --config="%~dp0networks\%networkName%"
@echo.
@echo Use %%t%% as
@echo set t=tinc --config="%~dp0networks\%networkName%"
@echo.
@cmd
