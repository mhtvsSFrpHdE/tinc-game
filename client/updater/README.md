# Updater

## How to test
- Copy debug compiled `updater.exe` to uninstall test directory
- Right click on project name, Properties, Configuration Properties, Debugging
- Set `Command` to full path of copied `updater.exe`
- Set `Command Arguments`, append test target, like `-p -u` for full uninstall
- Set `Working Directory` to full path of directory of where copied `updater.exe` placed in
- OK to save, and run debug
- Do not commit these settings to git, so later it can be handy to restore default settings
