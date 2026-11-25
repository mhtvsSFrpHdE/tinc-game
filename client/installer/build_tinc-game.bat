@REM 32 bit Release build for publish

cd /d %~dp0
cd ..

set path=%CPP_DEVEL_CMAKE%\bin;%CPP_DEVEL_MINGW%\bin;%path%

@REM Backup downloaded library
@REM move "out_publish\_deps" "out_publish_deps_bak"
@REM rd /s /q "out_publish"
@REM md "out_publish"
@REM move "out_publish_deps_bak" "out_publish\_deps"

@REM Boost_ARCHITECTURE is required to locate correct lib files when using legacy compiler
cmake -B out_publish -G "MinGW Makefiles" ^
-DCMAKE_BUILD_TYPE=Release ^
-DBoost_ARCHITECTURE=-x32

@REM cmake --build out_publish --parallel
cmake --build out_publish

pause