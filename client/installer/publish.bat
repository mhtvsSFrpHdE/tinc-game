cd /d %~dp0

set ENV_VERSION="3.0"

@REM x64
set ENV_UNINSTALL_ID="tinc-game-T0MBcLcnSX8S"
set ENV_TINC_INSTALL_DIR="C:\Program Files\tinc\"
set ENV_OUT_FILE="tinc-game-3.0-x64-setup.exe"
makensis tinc-game.nsi

@REM x86
set ENV_UNINSTALL_ID=HbazvDPShami
set ENV_TINC_INSTALL_DIR="C:\Program Files (x86)\tinc\"
set ENV_OUT_FILE="tinc-game-3.0-x86-setup.exe"
makensis tinc-game.nsi

pause