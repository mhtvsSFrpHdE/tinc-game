set lang=zh
@REM https://mlocati.github.io/articles/gettext-iconv-windows.html

cd /d "%~dp0"
md po\%lang%
set xgt=xgettext -d tinc-game -s --keyword=_ -p ./po/%lang%
%xgt% -o tinc-game-ApplyMtuFrame.pot src/Frame/ApplyMtu/ApplyMtuFrame.h src/Frame/ApplyMtu/ApplyMtuFrame.cpp
%xgt% -o tinc-game-EditNetworkFrame.pot src/Frame/EditNetwork/EditNetworkFrame.h src/Frame/EditNetwork/EditNetworkFrame.cpp
%xgt% -o tinc-game-JoinNetworkFrame.pot src/Frame/JoinNetwork/JoinNetworkFrame.h src/Frame/JoinNetwork/JoinNetworkFrame.cpp
%xgt% -o tinc-game-JoinNetworkFrame_SRV.pot src/Frame/JoinNetwork/JoinNetworkFrame_SRV.cpp
%xgt% -o tinc-game-MainFrame.pot src/Frame/Main/MainFrame.h src/Frame/Main/MainFrame.cpp
%xgt% -o tinc-game-MainFrame_SRV.pot src/Frame/Main/MainFrame_SRV.cpp
%xgt% -o tinc-game-MainFrameInit.pot src/Frame/Main/MainFrameInit.cpp
%xgt% -o tinc-game-MainFrameMenu.pot src/Frame/Main/MainFrameMenu.cpp
%xgt% -o tinc-game-ManageTapFrame.pot src/Frame/ManageTap/ManageTapFrame.cpp
%xgt% -o tinc-game-OptimizeMtuFrame.pot src/Frame/OptimizeMtu/OptimizeMtuFrame.h src/Frame/OptimizeMtu/OptimizeMtuFrame.cpp
%xgt% -o tinc-game-RenameNetworkFrame.pot src/Frame/RenameNetwork/RenameNetworkFrame.h src/Frame/RenameNetwork/RenameNetworkFrame.cpp
%xgt% -o tinc-game-SettingsFrame.pot src/Frame/Settings/SettingsFrame.cpp
