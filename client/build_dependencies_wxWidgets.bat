set vcvarsall="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"

@REM (1): Known issue, [clang_rt.asan_dbg_dynamic-x86_64.dll was not found](https://github.com/libcpr/cpr/issues/582#issuecomment-4562229005)
call %vcvarsall% x64

cd /d "C:\cpp-devel\wxWidgets-3.0.5\build\msw"
start "build x64 Debug" cmd /c nmake /F makefile.vc TARGET_CPU=X64 SHARED=0 UNICODE=1 BUILD=debug RUNTIME_LIBS=static ^& cmd
start "build x64 Release" cmd /c nmake /F makefile.vc TARGET_CPU=X64 SHARED=0 UNICODE=1 BUILD=release RUNTIME_LIBS=static ^& cmd
call %vcvarsall% x86
start "build x86 Debug" cmd /c nmake /F makefile.vc TARGET_CPU=X86 SHARED=0 UNICODE=1 BUILD=debug RUNTIME_LIBS=static ^& cmd
call %vcvarsall% x64
