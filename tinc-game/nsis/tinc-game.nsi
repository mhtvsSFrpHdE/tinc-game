# define const variables
!define PRODUCT_NAME "tinc game"
!define PRODUCT_VERSION "1.0"

!define TINCGAME_DIR "tinc-game"

# define name of installer
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "tinc-game-setup.exe"

# define installer pages
page directory
Page instfiles

# define installation directory
InstallDir "$PROGRAMFILES\${TINCGAME_DIR}"

# install section
Section
    # set the installation directory
    SetOutPath $INSTDIR

    # create the uninstaller
    WriteUninstaller "$INSTDIR\uninstall.exe"

    # create a shortcut in the start menu programs directory
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
    CreateShortcut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\uninstall.exe"
SectionEnd

# uninstall section
Section "uninstall"
    # Remove the link from the start menu
	# Try to remove the Start Menu folder - this will only happen if it is empty
    Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
	RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

    # Delete the uninstaller
    Delete "$INSTDIR\uninstall.exe"

    # Delete install dir
    RMDir "$INSTDIR"
SectionEnd
