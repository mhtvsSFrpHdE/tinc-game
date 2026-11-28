@REM Quick compile and publish 32 bit Release build
@REM build: Run "build_tinc-game_rebuild.bat" by first to compile dependencis
@REM Then use this to build directly on only project source code were changed and generate installer

cd /d %~dp0
call "build_tinc-game_env.bat"
cd ..

cmake --build out_x86 %1

cd /d %~dp0
publish_x86.bat

pause
