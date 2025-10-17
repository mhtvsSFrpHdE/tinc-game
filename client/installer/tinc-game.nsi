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

# run updater result
#     == 0: success
#     < 0: failed, reason defined by installer
#         -1: updater.exe not exist
#         -2: after unpack files, updater.exe exit code failed
#     > 0: failed, reason defined by updater.exe exit code
Var updaterResult

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
    # has exist install, try to update
    ${If} $uninstallString != ""
        # save the current $OUTDIR
        Push $OUTDIR
        # set the new working directory
        SetOutPath "$InstDir"

        # check updater exist
        #     if exist, continue to run updater
        #     if not exist, skip run and make dummy result
        IfFileExists $InstDir\updater.exe 0 updater_not_exist
        # run prepare update command and store exit code
        ExecWait '"$InstDir\updater.exe" -p' $updaterResult
        Goto has_result
        # dummy result if updater not exist
        updater_not_exist:
        StrCpy $updaterResult -1

        has_result:
        # restore the original $OUTDIR
        Pop $OUTDIR

        # if updater failed, terminate installer
        ${If} $updaterResult < 0
            MessageBox MB_OK "Update failed: $updaterResult"
            Abort
        ${EndIf}
        ${If} $updaterResult > 0
            Abort
        ${EndIf}
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
    File /r /x "Uninstall.exe" "C:\Program Files\tinc\"
    SetOutPath $InstDir

    # update config files with updater after unpack files

    # save the current $OUTDIR
    Push $OUTDIR
    # set the new working directory
    SetOutPath "$InstDir"
    # run updater and store exit code
    ExecWait '"$InstDir\updater.exe"' $updaterResult
    # restore the original $OUTDIR
    Pop $OUTDIR

    ${If} $updaterResult != 0
        MessageBox MB_OK "Install failed: $updaterResult"
        Abort
    ${EndIf}

    # create the uninstaller
    WriteUninstaller "$InstDir\uninstall.exe"

    # create a shortcut in the start menu programs directory
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
    CreateShortcut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$InstDir\tinc-game.exe"
    CreateShortcut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall ${PRODUCT_NAME}.lnk" "$InstDir\uninstall.exe"

    # add windows control panel uninstall entry
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}" "DisplayName" "${PRODUCT_NAME}"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}" "UninstallString" '"$InstDir\uninstall.exe"'
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}" "QuietUninstallString" '"$InstDir\uninstall.exe" /S'
SectionEnd

# uninstall section
Section "uninstall"
        # save the current $OUTDIR
        Push $OUTDIR
        # set the new working directory
        SetOutPath "$InstDir"

        # check updater exist
        #     if exist, continue to run updater
        #     if not exist, skip run and make dummy result
        IfFileExists $InstDir\updater.exe 0 updater_not_exist
        # run prepare update command and store exit code
        ExecWait '"$InstDir\updater.exe" -p -u' $updaterResult
        Goto has_result
        # dummy result if updater not exist
        updater_not_exist:
        StrCpy $updaterResult -1

        has_result:
        # restore the original $OUTDIR
        Pop $OUTDIR

        # if updater failed, terminate installer
        ${If} $updaterResult < 0
            MessageBox MB_OK "Uninstall failed: $updaterResult"
            Abort
        ${EndIf}
        ${If} $updaterResult > 0
            Abort
        ${EndIf}

    # remove the link from the start menu
	# try to remove the Start Menu folder - this will only happen if it is empty
    Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
    Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall ${PRODUCT_NAME}.lnk"
	RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

    # delete the uninstaller
    Delete "$InstDir\updater.exe"
    Delete "$InstDir\uninstall.exe"

    # delete install dir
    RMDir "$InstDir"

    # remove windows control panel uninstall entry
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}"
SectionEnd
