cd /d "%~dp0"
xgettext -d tinc-game -s --keyword=_ -p ./po -o tinc-game-App.pot App.cpp
xgettext -d tinc-game -s --keyword=_ -p ./po -o tinc-game-MainFrame.pot MainFrame.cpp
xgettext -d tinc-game -s --keyword=_ -p ./po -o tinc-game-OptimizeMtuFrame.pot OptimizeMtuFrame.cpp
xgettext -d tinc-game -s --keyword=_ -p ./po -o tinc-game-SettingsFrame.pot SettingsFrame.cpp
