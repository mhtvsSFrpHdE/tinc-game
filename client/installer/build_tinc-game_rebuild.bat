@REM Configure again and build parallel

cd /d %~dp0
call "build_tinc-game_configure.bat"
call "build_tinc-game_parallel.bat"

pause
