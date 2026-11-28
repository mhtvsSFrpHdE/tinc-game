set vcvarsall="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
call %vcvarsall% x64

cd /d "%CPP_DEVEL_WXWIN%\build\msw"
start "build x64 Debug" cmd /c nmake /F makefile.vc TARGET_CPU=X64 SHARED=0 UNICODE=1 BUILD=debug RUNTIME_LIBS=static ^& cmd
start "build x64 Release" cmd /c nmake /F makefile.vc TARGET_CPU=X64 SHARED=0 UNICODE=1 BUILD=release RUNTIME_LIBS=static ^& cmd

call %vcvarsall% x86
start "build x86 Debug" cmd /c nmake /F makefile.vc TARGET_CPU=X86 SHARED=0 UNICODE=1 BUILD=debug RUNTIME_LIBS=static ^& cmd

pause
