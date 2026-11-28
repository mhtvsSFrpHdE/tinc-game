@REM 32 bit Release build for publish

cd /d %~dp0
call "build_tinc-game_env.bat"
cd ..

@REM Backup downloaded library
move "out_x86\_deps" "out_x86_deps_bak"
echo "Backup success? If not close window and check background process."
pause
rd /s /q "out_x86"
md "out_x86"
move "out_x86_deps_bak" "out_x86\_deps"

@REM Boost_ARCHITECTURE is required to locate correct lib files when using legacy compiler
cmake -B out_x86 -G "MinGW Makefiles" ^
-DCMAKE_BUILD_TYPE=Release

pause
