#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "JoinNetworkFrame.h"
#include "String_SRV.h"
#include "Resource_SRV.h"
#include "Settings_SRV.h"
#include <sstream>

void JoinTimeout(boost::process::child* c, int timeoutSec) {
    namespace bp = boost::process;
    namespace rsb = Resource_SRV::Bat;

    c->wait_for(std::chrono::seconds(timeoutSec));
    if (c->running()) {
        // Invalid key another situation, only this line can be viewed in cmd console, no future output, and tinc.exe hangs
        // ```
        // Connected to <IP> port <Port>...
        // ```
        // At the same time, nothing could be get from stderr, `is.get()` or `std::getline()`, even cmd `tinc.exe <argument> 2>1.txt` doesn't work
        // Once tinc.exe terminated, printed stderr will gone, unless type `Ctrl+C` in console window, then stderr can be get
        // Maybe because tinc.exe didn't flush after printf

        // Don't use `c->terminate();` instead call taskkill.exe with pid to emulate `Ctrl+C`
        bp::system(bp::shell(), bp::args({ rsb::cmdRumCommand, L"taskkill.exe", L"/PID", std::to_wstring(c->id()) }), bp::windows::hide);
        c->wait();
    }
}

/// <summary>
/// "%tincPath%\%gameFileName%" --config="%tincPath%\%network%" join %inviteCode%
/// </summary>
/// <param name="tincBin"></param>
/// <param name="networkName"></param>
/// <param name="inviteCode"></param>
/// <returns></returns>
ReturnValue<JoinNetworkResult> JoinByInviteCode(std::wstring& tincBin, std::wstring& saveAs, std::wstring& inviteCode) {
    namespace rsn = Resource_SRV::Networks;
    namespace rsb = Resource_SRV::Bat;
    namespace bp = boost::process;
    namespace sr = String_SRV;

    auto result = ReturnValue<JoinNetworkResult>();

    std::wostringstream lines;
    bool connected = false;
    try {
        bp::ipstream is;
        bp::child c(tincBin, bp::args({ std::wstring(L"--config=") + saveAs, L"join", inviteCode }), bp::std_err > is, bp::windows::hide);

        std::thread t1(&JoinTimeout, &c, 10);

        std::string line;
        while (std::getline(is, line)) {
            auto wline = sr::ForceToWstring(line);
            lines << wline;

            // Invalid invitation URL.
            if (line.find(std::string("Invalid i")) != std::wstring::npos) {
                result.returnBody.messageEnum = JoinNetworkResult::Enum::InvalidInviteCode;
                break;
            }

            // Connected to <IP> port <Port>...
            if (line.find(std::string("Connected")) != std::wstring::npos) {
                connected = true;
            }

            // Peer has an invalid key!
            if (line.find(std::string("invalid key")) != std::wstring::npos) {
                result.returnBody.messageEnum = JoinNetworkResult::Enum::AuthenticateFailed;
                break;
            }

            // Could not connect to <IP> port <Port>: No such file or directory"
            if (line.find(std::string("Could not connect")) != std::wstring::npos) {
                result.returnBody.messageEnum = JoinNetworkResult::Enum::ConnectFailed;
                break;
            }

            // Connected to <IP> port <Port>...
            // Both netname and configuration directory given, using the latter...
            // ...............+++++ p
            // ..+++++ q
            // Configuration stored in: <config dir>
            // Invitation successfully accepted.
            // Error reading data from <IP> port <Port>: File exists
            if (line.find(std::string("Invitation successfully")) != std::wstring::npos) {
                result.success = true;
                break;
            }
        }
        t1.join();
    }
    catch (...) {
        result.returnBody.messageEnum = JoinNetworkResult::Enum::TincBinFailed;
    }

    if (result.success == false) {
        result.returnBody.messageString = lines.str();

        if (connected) {
            result.returnBody.messageEnum = JoinNetworkResult::Enum::AuthenticateFailed;
        }
        if (result.returnBody.messageEnum == JoinNetworkResult::Enum::Other) {
            if (connected == false) {
                result.returnBody.messageString = L"Could not connect" + String_SRV::newLine + result.returnBody.messageString;
            }
        }
        wxRmdir(saveAs);
    }

    return result;
}

/// <summary>
/// "%tincPath%\%gameFileName%" --config="%tincPath%\%network%" set Mode switch
/// </summary>
/// <param name="tincBin"></param>
/// <param name="saveAs"></param>
/// <returns></returns>
ReturnValue<JoinNetworkResult> SetSwitchMode(std::wstring& tincBin, std::wstring& saveAs) {
    namespace bp = boost::process;
    namespace rsb = Resource_SRV::Bat;
    namespace sr = String_SRV;

    auto result = ReturnValue<JoinNetworkResult>();

    std::wostringstream lines;
    bp::ipstream is;
    bp::child c(bp::shell(), bp::args({ rsb::cmdRumCommand, tincBin, std::wstring(L"--config=") + saveAs, L"set", L"Mode", L"switch" }), bp::std_err > is, bp::windows::hide);

    std::string line;
    while (std::getline(is, line)) {
        auto wline = sr::ForceToWstring(line);
        lines << wline;

        // On success program prints nothing

        // Could not open configuration file <config dir>\tinc.conf: No such file or directory
    }
    c.wait();

    result.returnBody.messageString = lines.str();
    if (result.returnBody.messageString.length() == 0) {
        result.success = true;
    }

    return result;
}

void JoinNetworkFrame::API_SRV_JoinNetworkByInviteCode(std::wstring networkName, std::wstring inviteCode)
{
    namespace bp = boost::process;
    namespace sr = String_SRV;
    namespace rst = Resource_SRV::TincBin;
    namespace rsb = Resource_SRV::Bat;
    namespace rsn = Resource_SRV::Networks;
    namespace ss = Settings_SRV;

    auto& programGameModeSettingsKey = SettingKeys_Program::settings_gameMode;
    auto gameModeValue = ss::programConfig->Read(programGameModeSettingsKey);
    auto tincGameModeBin = rst::GetTincdGameModeBin(gameModeValue);
    auto tincGameModeBinPath = tincGameModeBin.GetFullPath();

    bool gameModeFileExists = wxFileExists(tincGameModeBinPath);
    if (gameModeFileExists) {
        wxRemoveFile(tincGameModeBinPath);
    }
    auto srcTincPath = rst::GetTincBinAsWxStr();
    auto dstTincPath = tincGameModeBin.GetFullPath();
    wxCopyFile(srcTincPath, dstTincPath);

    auto networksDir = rsn::GetNetworksDir();
    networksDir.AppendDir(networkName);
    auto saveAs = networksDir.GetFullPath();
    saveAs.RemoveLast();

    auto wTincGameModeBinPath = tincGameModeBinPath.ToStdWstring();
    auto wSaveAs = saveAs.ToStdWstring();
    auto joinOrSetResult = JoinByInviteCode(wTincGameModeBinPath, wSaveAs, inviteCode);
    if (joinOrSetResult.success) {
        auto setResult = SetSwitchMode(wTincGameModeBinPath, wSaveAs);
        if (setResult.success == false) {
            joinOrSetResult = setResult;
        }
    }

    wxRemoveFile(tincGameModeBinPath);

    CallAfter(&JoinNetworkFrame::API_UI_ReportErrorMessage, joinOrSetResult);
    CallAfter(&JoinNetworkFrame::API_UI_EndJoinNetworkByInviteCode, joinOrSetResult);
}
