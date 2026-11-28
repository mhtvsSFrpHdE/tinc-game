@REM CPP_DEVEL_MINGW: path to g++.exe located
@REM CPP_DEVEL_CMAKE: pato to cmake.exe located
set path=%CPP_DEVEL_CMAKE%\bin;%CPP_DEVEL_MINGW%\bin;%path%

@REM wxWidgets
cd /d "%CPP_DEVEL_WXWIN%\build\msw"
@REM https://wiki.wxwidgets.org/Compiling_wxWidgets_with_MinGW
@REM Under some circumstances, while compiling on multiple cores, you may receive compilation error at the very beginning about missing setup.h file. To overcome this issue, generate this file first with setup_h tag at the end of build parameters
@REM     Didn't work, but add "SHELL=CMD.exe" fixed -j
@REM If you cannot build wxWidgets static library, try the following commands (specify SHELL=CMD.exe, otherwise mingw32-make (GNU make) will use sh.exe as SHELL by default)
@REM Build the static library (on Windows)
mingw32-make SHELL=CMD.exe -j6 -f makefile.gcc SHARED=0 UNICODE=1 BUILD=release

@REM boost
@REM This need to be done in another shell to prevent introduce vcvarsall environment variables
cd /d %~dp0
start "boost b2 bootstrap" /wait "build_dependencies_boost_b2.bat"
cd /d "%CPP_DEVEL_BOOST%"
b2.exe link=static runtime-link=static address-model=32 variant=release toolset=gcc

pause
