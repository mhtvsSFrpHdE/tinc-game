cd /d "%CPP_DEVEL_BOOST%"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
@REM The "&& exit" trick prevents bootstrap.bat blocks automation
@REM bootstrap.bat end with an interactive cmd shell and cause parent script pause
@REM
@REM boost 1.75 bootstrap.bat line 94, 95
@REM     REM Set an error code to allow `bootstrap && b2`
@REM So likely "bootstrap && b2" won't block and indeed
bootstrap.bat && exit
