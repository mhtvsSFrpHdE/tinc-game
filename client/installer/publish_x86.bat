cd /d %~dp0

set ENV_VERSION="3.0"

@REM x86
set ENV_UNINSTALL_NAME="tinc game x86"
set ENV_UNINSTALL_ID="tinc-game-HbazvDPShami"
set ENV_TINC_INSTALL_DIR="C:\Program Files (x86)\tinc\"
set ENV_OUT_FILE="tinc-game-3.0-x86-setup.exe"
set ENV_X64=0
makensis tinc-game.nsi

pause
