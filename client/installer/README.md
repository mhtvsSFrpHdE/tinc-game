# installer

### Build scripts (Sort by running order)
- `build_tinc-game_env.bat`  
  Edit this file before any other script  
  Set all required environment variable for other scripts to use
  - `CPP_DEVEL_CMAKE`
  - `CPP_DEVEL_MINGW`
  - `CPP_DEVEL_WXWIN`
  - `CPP_DEVEL_BOOST`
  - `http_proxy`
  - `https_proxy`
- `build_dependencies.bat`  
  Build wxWidgets and boost with MinGW  
- `build_tinc-game_configure.bat`
  CMake configure, need network to download cpr library  
  After first download, on next run `_deps` is reserved to prevent download again
- `build_tinc-game.bat`
  CMake build then `publish_x86.bat`, quick get `tinc-game-3.0-x86-setup.exe`
- `build_tinc-game_parallel.bat`
  Same as `build_tinc-game.bat` with `--parallel`  
  Build faster but error logs are messy on build failed
- `build_tinc-game_rebuild.bat`
  `build_tinc-game_configure.bat` then `build_tinc-game_parallel.bat`.
- `publish_x86.bat`  
  Collect MinGW compiled x86 files to NSIS installer
- `publish_x64.bat`  
  Collect MSVC compiled x64 files to NSIS installer
- `build_shell.bat`  
  Open a shell with proxy server, MinGW and CMake for manual build commands.
- `publish_http_server.bat`  
  Run `python -m uploadserver`, venv commands are embedded in file as comment  
  Allow XP 2001 download x86 installer and upload files back

## NSIS
Install NSIS, version `3.11`  
https://nsis.sourceforge.io/Main_Page

[Creating modern installers for your software using NSIS #1 - Installation](https://www.youtube.com/watch?v=5HcLY8g5rSs)

Open `tinc-game.nsi` with VS Code, install extension [idleberg.nsis](https://marketplace.visualstudio.com/items?itemName=idleberg.nsis)
