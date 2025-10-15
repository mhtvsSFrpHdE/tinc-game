# define const variables
!define PRODUCT_NAME "tinc game"
!define PRODUCT_VERSION "1.0"

!define TINCGAME_INSTALL_DIR "tinc-game"
!define TINCGAME_UNINSTALL_ID "tinc-game-T0MBcLcnSX8S"

# detect exist install
Var uninstallString
!include LogicLib.nsh

Function .onInit
ReadRegStr $uninstallString HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}" "UninstallString"
FunctionEnd

# define name of installer
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "tinc-game-setup.exe"

# define installer pages
Page Directory
Page InstFiles

# define installation directory
InstallDir "$PROGRAMFILES\${TINCGAME_INSTALL_DIR}"

# install section
Section
    ${If} $uninstallString != ""
        MessageBox MB_OK "Uninstall exist version first"
        Abort
    ${EndIf}

    # install root installation directory
    SetOutPath $InstDir
    # specify program files
    File "..\Release\tinc-game.exe"
    File "..\Release\updater.exe"
    File "..\tinc-game\getTapHwid.bat"
    File "..\tinc-game\installTap.bat"
    File "..\tinc-game\netsh437.bat"
    File "..\tinc-game\netsh437_start.bat"
    File "..\tinc-game\ping437.bat"
    File "..\tinc-game\ping437_start.bat"
    File "..\tinc-game\tincShell.bat"
    File "..\tinc-game\uninstallTap.bat"
    # install tinc bin
    SetOutPath "$InstDir\bin\tinc"
    File /r "C:\Program Files\tinc\"

    # create the uninstaller
    WriteUninstaller "$InstDir\uninstall.exe"

    # create a shortcut in the start menu programs directory
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
    CreateShortcut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$InstDir\uninstall.exe"

    # add windows control panel uninstall entry
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}" "DisplayName" "${PRODUCT_NAME}"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}" "UninstallString" '"$InstDir\uninstall.exe"'
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}" "QuietUninstallString" '"$InstDir\uninstall.exe" /S'
SectionEnd

# uninstall section
Section "uninstall"
    # remove the link from the start menu
	# try to remove the Start Menu folder - this will only happen if it is empty
    Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
	RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

    # delete the uninstaller
    Delete "$InstDir\uninstall.exe"

    # delete install dir
    RMDir "$InstDir"

    # remove windows control panel uninstall entry
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}"
SectionEnd
