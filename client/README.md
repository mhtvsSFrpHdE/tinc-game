# tinc-game

## How to build
- MinGW `i686-4.8.2-release-posix-dwarf-rt_v3-rev3`
- wxWidgets `3.0.5`
- boost `1.75.0`
- cpr `1.5.2`
- tinc `1.1pre18`
These are combination proved to compile and run on Windows XP 2001  
and each of them is newest version as possible.  
Do not upgrade any of them without testing on XP 2001.

### Disk space warning
These giant library takes about 12.7 GB or more after build `Win32`, `x64`, `Debug`, `Release`.  
I would create parent folder and enable NTFS Compression for parent folder before anything happen,  
then unpack them directly into parent folder to reduce disk usage.  
After compile, defrag / optimize your disk to improve access speed.  

Build only `CMake Win32 Debug Release`, `b2 address-model=32 variant=debug,release` also takes less.

### If you changed your mind after CMake configure command
Delete and unpack entire CMake source folder again,  
simply delete build folders doesn't fix things (1).  
Make sure each config steps is executed in fresh environment,  
because here is no one understand how CMake work

(1): In some CMake project like wxwidgets,  
`build` folder is part of source code instead of something generated during compile step  
In these project run `cmake -B build_win32 -A Win32` instead of `cmake -B build -A Win32`  
to avoid putting files into build folder  
If using `build` folder for `cmake -B`, once configure is failed, can't easily clean build result  
and for sure require delete and unpack again on `build` folder gets deleted

### Static link and single exe
Build step command have only static link (single exe) version and doesn't maintain dynamic link (exe with dll) version

Static link flags stand for tell CMake to generate project with Code Generation, Runtime Library `/MT`, `MTd`

### Environment variable
Create System variables instead of User variables. Sometimes Visual Studio need to run as admin to test certain feature

### Dependencies and build steps
- CMake `3.15` or above, suggest `3.31.7` is known to be success
  - Environment variable `CPP_DEVEL_CMAKE` to CMake install location that contain `bin` directory
    - Example: `%CPP_DEVEL_CMAKE%\bin\cmake.exe`
- MSVC / boost `Visual Studio 2022`
- MinGW `i686-4.8.2-release-posix-dwarf-rt_v3-rev3`
  - Environment variable `CPP_DEVEL_MINGW` to MinGW install location that contain `bin` directory
    - Example: `%CPP_DEVEL_MINGW%\bin\g++.exe`
- wxwidgets `3.0.5`
  - Unpack to folder like `C:\cpp-devel\wxWidgets-3.0.5`, there should be `C:\cpp-devel\wxWidgets-3.0.5\build\msw`
  - Environment variable `CPP_DEVEL_WXWIN` as `C:\cpp-devel\wxWidgets-3.0.5`
  - MSVC (x86 and x64) build script `tinc-game\client\build_dependencies_wxWidgets.bat`
    - Open and edit `vcvarsall` to your Visual Studio install location
  - MinGW (x86) build script, read `tinc-game\client\installer\README.md`
- boost `1.75.0`
  - Unpack to folder like `C:\cpp-devel\boost\boost_1_75_0`, there should be `C:\cpp-devel\boost\boost_1_75_0\bootstrap.bat`
  - Environment variable `CPP_DEVEL_BOOST` as `C:\cpp-devel\boost\boost_1_75_0`
  - MSVC (x86 and x64) build
    - `cd /d "C:\cpp-devel\boost\boost_1_75_0"`
    - Actually, `bootstrap.bat` seems automatically detect Visual Studio compiler location even without `vcvarsall.bat`,  
      if not or you build in new cmd window, load Visual Studio 2022 compiler environment variable
        - `"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64`
    - build
      - `bootstrap.bat`, this generate `b2.exe` for next step
      - Static link
        - `b2.exe link=static runtime-link=static address-model=32,64 variant=debug,release`
  - MinGW (x86) build script, read `tinc-game\client\installer\README.md`
    - Before compile, edit `build_dependencies_boost_b2.bat` Visual Studio 2022 `vcvarsall` path
- cpr (and libcurl) `1.5.2`
  - `1.5.2` is the newest cpr that MinGW in this project can build, `1.6.0` and later do not pass MinGW
  - `CMakeLists.txt` download it automatically by `FetchContent`, no manual operation
  - Need network on configure.
    - If behind proxy, before run CMake configure, `set HTTP_PROXY=http://<proxy address>:<port>`, `set HTTPS_PROXY=%HTTP_PROXY%`
    - If build with Visual Studio, set environment variable system wide by "View advanced system settings, Environment variables...", or use `tinc-game\client\StartVisualStudio.bat` to load environment variables
  - If you need to delete build dir, use `tinc-game\client\VisualStudioReconfigure.bat` to backup `out\build\...\_deps` folder to prevent download again

## Common link error
There is library need to add `.lib` files manually in this format  
`<Additional Dependencies (lib file name)>`: `<Additional Library Directories>`  
Some of them not just located in different path, but also in different file name.
- `cpr.lib`: `%CPP_DEVEL_CPR%\<build/build_x64>\_deps\cpr-build\cpr\<Debug/Release>\cpr.lib`
- `<libcurl-d.lib/libcurl.lib>`: `%CPP_DEVEL_CPR%\<build/build_x64>\_deps\curl-build\lib\<Debug/Release>\<libcurl-d.lib/libcurl.lib>`
- `crypt32.lib`: Additional Dependencies only, this one bundled with Visual Studio 2022

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
this one bundled with Visual Studio 2019, add lib file name directly to Additional Dependencies

```
LINK : warning LNK4098: defaultlib 'LIBCMT' conflicts with use of other libs; use /NODEFAULTLIB:library
```  
Added `libcurl.lib` to wrong profile, like use Release version instead of Debug version in Debug profile  
This error message is completely tells nothing about real issue is. Love from C++.  
Before you do something add (like add `/NODEFAULTLIB:library`), best do web search and see how other people say
