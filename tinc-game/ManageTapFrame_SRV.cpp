#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "ManageTapFrame.h"
#include "String_SRV.h"
#include <wx/filename.h>
#include <sstream>

ReturnValue<InstallTapResult> ManageTapFrame::API_SRV_InstallTap()
{
    auto result = ReturnValue<InstallTapResult>();

    {
        wxFileName driverFile;
        driverFile.AppendDir("bin");
        driverFile.AppendDir("tinc");
        driverFile.AppendDir("tap-win64");

        const std::wstring installerFileName = L"tapinstall.exe";
        driverFile.SetName(installerFileName);
        bool installerFileNotExist = driverFile.Exists() == false;
        if (installerFileNotExist) {
            result.returnBody.messageEnum = InstallTapResult::Enum::DriverNotExist;
            result.returnBody.messageString = driverFile.GetName();
            return result;
        }

        const std::wstring infFileName = L"OemWin2k.inf";
        driverFile.SetName(infFileName);
        bool infFileNotExist = driverFile.Exists() == false;
        if (infFileNotExist) {
            result.returnBody.messageEnum = InstallTapResult::Enum::DriverNotExist;
            result.returnBody.messageString = driverFile.GetName();
            return result;
        }

        const std::wstring catFileName = L"tap0901.cat";
        driverFile.SetName(catFileName);
        bool catFileNotExist = driverFile.Exists() == false;
        if (catFileNotExist) {
            result.returnBody.messageEnum = InstallTapResult::Enum::DriverNotExist;
            result.returnBody.messageString = driverFile.GetName();
            return result;
        }

        const std::wstring sysFileName = L"tap0901.sys";
        driverFile.SetName(sysFileName);
        bool sysFileNotExist = driverFile.Exists() == false;
        if (sysFileNotExist) {
            result.returnBody.messageEnum = InstallTapResult::Enum::DriverNotExist;
            result.returnBody.messageString = driverFile.GetName();
            return result;
        }
    }

    namespace bp = boost::process;
    namespace ss = String_SRV;

    std::ostringstream stdoutSstream;
    std::vector<std::string> debug;
    try
    {
        bp::ipstream is;
        bp::child c(L"installTap.bat", bp::std_out > is, bp::windows::hide);
        std::string line;

        bool deviceNodeCreated = false;
        bool userCancelled = false;
        bool installerFailed = false;
        while (std::getline(is, line)) {
            stdoutSstream << line;
            // Drivers installed successfully
            if (line.find(std::string("success")) != std::string::npos) {
                result.success = true;
                return result;
            }
            // Device node created. Install is complete when drivers are installed...
            if (line.find(std::string("created")) != std::string::npos) {
                deviceNodeCreated = true;
            }
            // UpdateDriverForPlugAndPlayDevices failed, GetLastError=-536870333, tapinstall.exe failed.
            if (line.find(std::string("-536870333")) != std::string::npos) {
                userCancelled = true;
            }
            // tapinstall.exe failed.
            if (line.find(std::string("failed")) != std::string::npos) {
                installerFailed = true;
            }
        }
        c.wait();

        if (userCancelled) {
            result.returnBody.messageEnum = InstallTapResult::Enum::UserCanceled;
            return result;
        }
        if (deviceNodeCreated == false && installerFailed) {
            result.returnBody.messageEnum = InstallTapResult::Enum::NoPermission;
            return result;
        }
    }
    catch (...)
    {
        result.returnBody.messageEnum = InstallTapResult::Enum::InstallerNotExist;
        return result;
    }

    result.returnBody.messageEnum = InstallTapResult::Enum::Other;
    result.returnBody.messageString = ss::ForceToWstring(stdoutSstream.str());
    return result;
}

ReturnValue<std::wstring> ManageTapFrame::API_SRV_UninstallTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter)
{
    auto result = ReturnValue<std::wstring>();
    result.returnBody = adapter.friendlyName + " | " + adapter.modelName + String_SRV::newLine + adapter.windows_LUID;
    result.success = true;
    return result;
}
