# prefetch

## How to build
Dependencies
- Visual Studio 2019
- wxwidgets `3.1.5`
  - Solution file at `C:\wxWidgets-3.1.5\build\msw\wx_vc16.sln`
  - Build "Debug, Release, Win32, x64", not DLL build profile
  - Environment variable `WXWIN` as `C:\wxWidgets-3.1.5`
- boost `1.75.0`
  - Unzip to folder like `C:\boost\boost_1_75_0`, there should be `C:\boost\boost_1_75_0\b2.exe`
  - Run `b2.exe link=static`, if want fully static, run `b2.exe link=static runtime-link=static` instead
  - Environment variable `BOOST` as `C:\wxWidgets-3.1.5`
- clip `1.9`
  - Unzip to folder like `C:\clip-1.9`, there should be `C:\clip-1.9\CMakeLists.txt`
  - Install cmake-gui Windows `3.31.7`
  - Open CMake, source code folder browse to `C:\clip-1.9`
  - Configure and generate win32 and x64 build profile to these build binaries folder
    - `C:/clip-1.9/build/win32`
    - `C:/clip-1.9/build/x64`
  - Go to these folder, open `clip.sln`, build for `Debug, Release`
  - Environment variable `CLIP` to C:\clip-1.9
 
  Open `tinc-game.sln` with Visual Studio and hit build.
  
