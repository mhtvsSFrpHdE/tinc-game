# tinc-game

## How to build
Dependencies
- Visual Studio 2019
- wxwidgets `3.1.5`
  - Solution file at `C:\wxWidgets-3.1.5\build\msw\wx_vc16.sln`
  - Build "Debug, Release, Win32, x64", not DLL build profile
  - Environment variable `WXWIN` as `C:\wxWidgets-3.1.5`
  - If want fully static, change each wxwidgets project to Runtime Library `Multi-threaded (/MT)`
- boost `1.75.0`
  - Unzip to folder like `C:\boost\boost_1_75_0`, there should be `C:\boost\boost_1_75_0\b2.exe`
  - Run `b2.exe link=static`, if want fully static, run `b2.exe link=static runtime-link=static` instead
  - Environment variable `BOOST` as `C:\wxWidgets-3.1.5`
 
  Open `tinc-game.sln` with Visual Studio and hit build.
  
