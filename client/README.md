# tinc-game

## How to build

### Disk space warning
These giant library takes about 12.7 GB or more after build `Win32`, `x64`, `Debug`, `Release`.  
I would create parent folder and enable NTFS Compression for parent folder before anything happen,  
then unpack them directly into parent folder to reduce disk usage.  
After compile, defrag / optimize your disk to improve access speed.  

Build only `CMake Win32 Debug Release`, `b2 address-model=32 variant=debug,release` also takes less.

### If you changed your mind after cmake configure command
Delete and unpack entire cmake source folder again,  
simply delete build folders doesn't fix things.  
Make sure each config steps is executed in fresh environment,  
because here is no one understand how cmake work

### About CMake parallel and Visual Studio 2019
Visual Studio compiler ignores `--parallel` and build with single thread,  
hope spam build task can be faster, by start configure and build for all profile together at once  
If you are weak in CPU, run command one by one manually

Also, add root build dir to Windows Security Virus & threat protection Exclusions

### Static link and single exe
Build step command may have static link (single exe) version and dynamic link (exe with dll) version,  
pick one of your prefer instead of run both  
Static link flags stand for tell cmake to generate project with Code Generation, Runtime Library `/MT`, `MTd`

### Environment variable
Create System variables instead of User variables. Sometimes program need to run as admin

### Dependencies and build steps
- Visual Studio 2019
- wxwidgets `3.1.5`
  - Unpack to folder like `C:\cpp-devel\wxWidgets-3.1.5`, there should be `C:\cpp-devel\wxWidgets-3.1.5\CMakeLists.txt`
  - Open cmd
  - Load Visual Studio 2019 compiler environment variable
    - `"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64` (1)
  - `cd /d "C:\cpp-devel\wxWidgets-3.1.5"`
  - Delete exist `build` folder
  - configure
    - `start "configure Win32" cmd /c cmake -B build -A Win32 ^& cmd && start "configure x64" cmd /c cmake -B build_x64 -A x64 ^& cmd`
      - This start the following command together at once
        - `cmake -B build -A Win32`
        - `cmake -B build_x64 -A x64`
    - Static link
      - `set CPP_DEVEL_STATIC_FLAGS=-DBUILD_SHARED_LIBS=OFF -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>"`
      - `start "configure Win32" cmd /c cmake -B build -A Win32 %CPP_DEVEL_STATIC_FLAGS% ^& cmd && start "configure x64" cmd /c cmake -B build_x64 -A x64 %CPP_DEVEL_STATIC_FLAGS% ^& cmd`
  - build
    - `start "build Win32 Debug" cmd /c cmake --build build --config Debug --parallel ^& cmd && start "build Win32 Release" cmd /c cmake --build build --config Release --parallel ^& cmd && start "build x64 Debug" cmd /c cmake --build build_x64 --config Debug --parallel ^& cmd && start "build x64 Release" cmd /c cmake --build build_x64 --config Release --parallel ^& cmd`
      - This start the following command together at once
        - `cmake --build build --config Debug --parallel`
        - `cmake --build build --config Release --parallel`
        - `cmake --build build_x64 --config Debug --parallel`
        - `cmake --build build_x64 --config Release --parallel`
  - **Copy include to build**
    - `robocopy "include" "build\include" /E && robocopy "include" "build_x64\include" /E`
  - Environment variable `CPP_DEVEL_WXWIN` as `C:\cpp-devel\wxWidgets-3.1.5`
- boost `1.75.0`
  - Unpack to folder like `C:\cpp-devel\boost\boost_1_75_0`, there should be `C:\cpp-devel\boost\boost_1_75_0\bootstrap.bat`
  - `cd /d "C:\cpp-devel\boost\boost_1_75_0"`
  - Reuse cmd that build wxwidgets
    - Actually, `bootstrap.bat` seems automatically detect Visual Studio compiler location even without `vcvarsall.bat`,  
    if not and you build in new cmd window, load Visual Studio 2019 compiler environment variable
      - `"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64`
  - build
    - `bootstrap.bat`, this generate `b2.exe` for next step
    - `b2.exe address-model=32,64 variant=debug,release` (dynamic link, untested, may be different)
    - Static link: `b2.exe link=static runtime-link=static address-model=32,64 variant=debug,release`
  - Environment variable `CPP_DEVEL_BOOST` as `C:\cpp-devel\boost\boost_1_75_0`
- cpr `1.9.9`
  - Download [this git repository at commit 4844b9d](https://github.com/libcpr/example-cmake-fetch-content/tree/4844b9da480d81dd4cbf1783906eea574e51168b) instead of cpr release source pack
  - Unzip to folder like `C:\cpp-devel\example-cmake-fetch-content-main`, there should be `C:\cpp-devel\example-cmake-fetch-content-main\CMakeLists.txt`
  - Rename `example-cmake-fetch-content-main` to `cpr-1.9.9`
  - Edit `CMakeLists.txt`
    - `set(CMAKE_CXX_STANDARD`: change C++ standard version to `11`
    - `GIT_TAG`: change version to `1.9.9`
  - Reuse cmd that build wxwidgets
    - If you build in new cmd window, load Visual Studio 2019 compiler environment variable
      - `"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64`
  - `cd /d C:\cpp-devel\cpr-1.9.9`
  - Need network to download source code.
    - If behind proxy, `set HTTP_PROXY=http://<proxy address>:<port>`, after commit, `set HTTPS_PROXY=%HTTP_PROXY%`
  - configure
    - `start "configure Win32" cmd /c cmake -B build -A Win32 -DCPR_USE_SYSTEM_CURL=OFF ^& cmd && start "configure x64" cmd /c cmake -B build_x64 -A x64 -DCPR_USE_SYSTEM_CURL=OFF ^& cmd`
      - This start the following command together at once
        - `cmake -B build -A Win32 -DCPR_USE_SYSTEM_CURL=OFF` (2)
        - `cmake -B build_x64 -A x64 -DCPR_USE_SYSTEM_CURL=OFF`
    - Static link
      - `set CPP_DEVEL_STATIC_FLAGS=-DBUILD_SHARED_LIBS=OFF -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>"`
      - `start "configure Win32" cmd /c cmake -B build -A Win32 -DCPR_USE_SYSTEM_CURL=OFF %CPP_DEVEL_STATIC_FLAGS% ^& cmd && start "configure x64" cmd /c cmake -B build_x64 -A x64 -DCPR_USE_SYSTEM_CURL=OFF %CPP_DEVEL_STATIC_FLAGS% ^& cmd`
  - build
    - `start "build Win32 Debug" cmd /c cmake --build build --config Debug --parallel ^& cmd && start "build Win32 Release" cmd /c cmake --build build --config Release --parallel ^& cmd && start "build x64 Debug" cmd /c cmake --build build_x64 --config Debug --parallel ^& cmd && start "build x64 Release" cmd /c cmake --build build_x64 --config Release --parallel ^& cmd`
      - This start the following command together at once
      - `cmake --build build --config Debug --parallel`
      - `cmake --build build --config Release --parallel`
      - `cmake --build build_x64 --config Debug --parallel`
      - `cmake --build build_x64 --config Release --parallel`
  - Environment variable `CPP_DEVEL_CPR` as `C:\cpp-devel\cpr-1.9.9`

Open `tinc-game.sln` with Visual Studio and hit build.

(1): Known issue, [clang_rt.asan_dbg_dynamic-x86_64.dll was not found](https://github.com/libcpr/cpr/issues/582#issuecomment-4562229005)  
(2): `CPR_USE_SYSTEM_CURL=OFF`: Build on Windows instead of Linux, there is no such thing `libcurl-devel`, [reference link](https://github.com/libcpr/cpr/issues/1043).

## Code
In Solution Explorer, find "Show All Files" icon toggle to folder view on editing code  
the default filter view doesn't actually reflects how source file is located on disk

## Common link error
There is library need to add `.lib` files manually in this format  
`<Additional Dependencies (lib file name)>`: `<Additional Library Directories>`  
Some of them not just located in different path, but also in different file name.
- `cpr.lib`: `%CPP_DEVEL_CPR%\<build/build_x64>\_deps\cpr-build\cpr\<Debug/Release>\cpr.lib`
- `<libcurl-d.lib/libcurl.lib>`: `%CPP_DEVEL_CPR%\<build/build_x64>\_deps\curl-build\lib\<Debug/Release>\<libcurl-d.lib/libcurl.lib>`
- `crypt32.lib`: Additional Dependencies only, this one bundled with Visual Studio 2019

```
error LNK2019: unresolved external symbol "public: bool __thiscall cpr::CaseInsensitiveCompare::operator..."
```
Missing `cpr.lib` to Additional Dependencies  
No lib name provided, look namespace `cpr::` for clue  
So `cpr.lib` need to add manually instead of automatically add from Additional Library Directories

```
cpr.lib(session.obj) : error LNK2019: unresolved external symbol _curl_mime_init referenced in function "public: void __thiscall cpr::Session::SetMultipart...
```
Missing `libcurl.lib` or `libcurl-d.lib` to Additional Dependencies depending on you're on Debug or Release profile  
Says from `cpr.lib` and namespace `cpr::...`, then external symbol `_curl...`  
We just added `cpr.lib` to fix `LNK2019`, so this is a dependency of `cpr.lib`, sounds like `curl`.  
Go to cpr build directory and locate all lib files,  
there should be curl lib come together on a success build of cpr.

```
libcurl-d.lib(schannel.obj) : error LNK2019: unresolved external symbol __imp__CertOpenStore@20 referenced in function _schannel_acquire_credential_handle...
```
Missing `crypt32.lib` to Additional Dependencies  
We know this is a missing dependency of `libcurl-d.lib` or `libcurl.lib` in Release  
But we know nothing about external symbol `__imp__CertOpenStore`  
Start a web search and see how other people say, they mentioned `crypt32.lib`

```
LINK : warning LNK4098: defaultlib 'LIBCMT' conflicts with use of other libs; use /NODEFAULTLIB:library
```  
Added `libcurl.lib` to wrong profile, like use Release version instead of Debug version in Debug profile  
This error message is completely tells nothing about real issue is. Love from C++.  
Before you do something add (like add `/NODEFAULTLIB:library`), best do web search and see how other people say

## Old compiler
Untested yet. Currently project need to be developed as fast as possible,  
so Visual Studio 2019 build in compiler is used.  
In future will swap compiler from MSVC to MinGW.
```
REM Build boost Win32 Release
set path=C:\cpp-devel\mingw\i686-4.8.2-release-posix-dwarf-rt_v3-rev3\mingw32\bin;%path%
cd /d "C:\cpp-devel\boost\boost_1_75_0"
b2.exe link=static runtime-link=static address-model=32 variant=release toolset=gcc
```
