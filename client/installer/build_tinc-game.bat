@REM 32 bit Release build for publish
@REM build: Run "build_tinc-game_rebuild.bat" by first to compile dependencis
@REM Then use this to build directly on only project source code were changed

cd /d %~dp0
call "build_tinc-game_env.bat"
cd ..

cmake --build out_publish %1

pause
