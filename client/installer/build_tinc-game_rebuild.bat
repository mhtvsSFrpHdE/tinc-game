@REM 32 bit Release build for publish

cd /d %~dp0
call "build_tinc-game_env.bat"
cd ..

@REM Backup downloaded library
move "out_publish\_deps" "out_publish_deps_bak"
echo "Backup success? If not close window and check background process."
pause
rd /s /q "out_publish"
md "out_publish"
move "out_publish_deps_bak" "out_publish\_deps"

@REM Boost_ARCHITECTURE is required to locate correct lib files when using legacy compiler
cmake -B out_publish -G "MinGW Makefiles" ^
-DCMAKE_BUILD_TYPE=Release ^
-DBoost_ARCHITECTURE=-x32 ^
-DCMAKE_USE_OPENSSL=OFF

@REM Skip googletest and only build tinc-game exe
@REM cmake --build out_publish
cmake --build out_publish --target tinc-game updater --parallel

pause
