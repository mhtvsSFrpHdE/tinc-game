# define const variables
!define PRODUCT_NAME $%ENV_UNINSTALL_NAME%
!define PRODUCT_VERSION $%ENV_VERSION%

!define TINCGAME_INSTALL_DIR "tinc-game"
!define TINCGAME_UNINSTALL_ID $%ENV_UNINSTALL_ID%

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
OutFile $%ENV_OUT_FILE%

# define installer pages
Page Directory
Page InstFiles

# define installation directory
!define VAR_PROGRAMFILES "$PROGRAMFILES32"
!if "$%ENV_X64%" != "${U+24}%ENV_X64%"
    !if "$%ENV_X64%" != "0"
        !undef VAR_PROGRAMFILES
        !define VAR_PROGRAMFILES "$PROGRAMFILES64"
    !endif
!endif

InstallDir "${VAR_PROGRAMFILES}\${TINCGAME_INSTALL_DIR}"
# reuse user choice install dir on update
InstallDirRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}" "InstallDir"

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
    File "$%ENV_BUILD_DIR%\tinc-game.exe"
    File "$%ENV_BUILD_DIR%\updater.exe"
    File "..\tinc-game\src\ShellScript\Windows\getTapHwid.bat"
    File "..\tinc-game\src\ShellScript\Windows\installTap.bat"
    File "..\tinc-game\src\ShellScript\Windows\netsh437.bat"
    File "..\tinc-game\src\ShellScript\Windows\netsh437_start.bat"
    File "..\tinc-game\src\ShellScript\Windows\ping437.bat"
    File "..\tinc-game\src\ShellScript\Windows\ping437_start.bat"
    File "..\tinc-game\src\ShellScript\Windows\tincShell.bat"
    File "..\tinc-game\src\ShellScript\Windows\uninstallTap.bat"
    # install translate file, exclude po and pot files
    File /r /x *.po /x *.pot "..\tinc-game\po"
    # install tinc bin, exclude "Uninstall.exe"
    SetOutPath "$InstDir\bin\tinc"
    File /r /x "Uninstall.exe" $%ENV_TINC_INSTALL_DIR%
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

    # save user choice install dir
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${TINCGAME_UNINSTALL_ID}" "InstallDir" "$InstDir"

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

    # also remove 2.x install id on uninstall
    # 2.x only support x64 but still install to "Program Files (x86)" on x64 system
    # when accidentally install x86 on an x64 system
    # 2.x uninstaller will be overwrite and no longer be able to
    # remove uninstaller from control panel
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\tinc-game-T0MBcLcnSX8S"
SectionEnd
