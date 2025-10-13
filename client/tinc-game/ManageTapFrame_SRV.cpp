#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include "ManageTapFrame.h"
#include "..\resource\String_SRV.h"
#include "..\resource\Resource_SRV_TincBin.h"
#include "..\resource\Resource_SRV_Bat.h"
#include <sstream>
#include <unordered_map>

ReturnValue<InstallTapResult> ManageTapFrame::API_SRV_InstallTap()
{
    auto result = ReturnValue<InstallTapResult>();

    {
        namespace rst = Resource_SRV::TincBin;

        auto driverFile = rst::GetTapInstallerDir();

        driverFile.SetName(rst::tapInstallerBin);
        bool installerFileNotExist = driverFile.Exists() == false;
        if (installerFileNotExist) {
            result.returnBody.messageEnum = InstallTapResult::Enum::DriverNotExist;
            result.returnBody.messageString = driverFile.GetName();
            return result;
        }

        driverFile.SetName(rst::tapInstallerInf);
        bool infFileNotExist = driverFile.Exists() == false;
        if (infFileNotExist) {
            result.returnBody.messageEnum = InstallTapResult::Enum::DriverNotExist;
            result.returnBody.messageString = driverFile.GetName();
            return result;
        }

        driverFile.SetName(rst::tapInstallerCat);
        bool catFileNotExist = driverFile.Exists() == false;
        if (catFileNotExist) {
            result.returnBody.messageEnum = InstallTapResult::Enum::DriverNotExist;
            result.returnBody.messageString = driverFile.GetName();
            return result;
        }

        driverFile.SetName(rst::tapInstallerSys);
        bool sysFileNotExist = driverFile.Exists() == false;
        if (sysFileNotExist) {
            result.returnBody.messageEnum = InstallTapResult::Enum::DriverNotExist;
            result.returnBody.messageString = driverFile.GetName();
            return result;
        }
    }

    namespace bp = boost::process;
    namespace ss = String_SRV;
    namespace rsb = Resource_SRV::Bat;

    std::ostringstream stdoutSstream;
    try
    {
        bp::ipstream is;
        bp::child c(rsb::installTap, bp::std_out > is, bp::windows::hide);
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
            result.returnBody.messageString = ss::ForceToWstring(stdoutSstream.str());
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

typedef std::unordered_map<std::wstring, std::string> GetTapHwidResult;

ReturnValue<GetTapHwidResult> GetTapHwid() {
    namespace bp = boost::process;
    namespace ss = String_SRV;
    namespace rsb = Resource_SRV::Bat;

    auto result = ReturnValue<GetTapHwidResult>();
    try
    {
        bp::ipstream is;
        bp::child c(rsb::getTapHwid, bp::std_out > is, bp::windows::hide);
        std::string line;

        while (std::getline(is, line)) {
            // ROOT\NET\0002                                               : TAP-Win32 Adapter V9
            if (line.find(std::string(": ")) != std::string::npos) {
                auto firstSpaceIndex = line.find(" ");
                auto hwid = line.substr(0, firstSpaceIndex);

                auto splitterIndex = line.find(": ");
                auto lineEndIndex = line.find("\r");
                auto modelName = line.substr(splitterIndex + 2, lineEndIndex - splitterIndex - 2);
                auto wModelName = ss::ForceToWstring(modelName);

                result.returnBody.insert({ wModelName, hwid });
            }
            // No matching devices found.
            if (line.length() == 27 && line[0] == 'N' && line[3] == 'm') {
                return result;
            }
        }
        c.wait();
    }
    catch (...)
    {
        return result;
    }

    if (result.returnBody.size() > 0) {
        result.success = true;
    }
    return result;
}

ReturnValue<UninstallTapResult> ManageTapFrame::API_SRV_UninstallTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter)
{
    auto result = ReturnValue<UninstallTapResult>();

    {
        namespace rst = Resource_SRV::TincBin;

        auto driverFile = rst::GetTapInstallerDir();
        driverFile.SetName(rst::tapInstallerBin);
        bool uninstallerFileNotExist = driverFile.Exists() == false;
        if (uninstallerFileNotExist) {
            result.returnBody.messageEnum = UninstallTapResult::Enum::UninstallerNotExist;
            result.returnBody.messageString = driverFile.GetName();
            return result;
        }
    }

    auto getTapHwid = GetTapHwid();
    if (getTapHwid.success == false) {
        result.returnBody.messageEnum = UninstallTapResult::Enum::NoAnyAdapter;
        return result;
    }

    auto hwid = getTapHwid.returnBody[adapter.modelName];
    if (hwid.empty()) {
        result.returnBody.messageEnum = UninstallTapResult::Enum::NotFound;
        return result;
    }

    namespace bp = boost::process;
    namespace ss = String_SRV;
    namespace rsb = Resource_SRV::Bat;

    std::ostringstream stdoutSstream;
    try
    {
        bp::ipstream is;
        std::wostringstream command;
        command << rsb::uninstallTap << ss::space
            << ss::doubleQuotes << L"@" << hwid << ss::doubleQuotes;
        bp::child c(command.str(), bp::std_out > is, bp::windows::hide);
        std::string line;

        bool uninstallFailed = false;
        bool noDeviceRemoved = false;
        while (std::getline(is, line)) {
            stdoutSstream << line;
            // ROOT\NET\0002                                               : Removed
            // 1 device(s) were removed.
            if (line.find(std::string("(s) we")) != std::string::npos) {
                result.success = true;
                return result;
            }
            // ROOT\NET\0002                                               : Remove failed
            if (line.find(std::string("ve failed")) != std::string::npos) {
                uninstallFailed = true;
            }
            // No devices were removed.
            if (line.find(std::string("No devices")) != std::string::npos) {
                noDeviceRemoved = true;
            }
        }
        c.wait();

        if (uninstallFailed) {
            result.returnBody.messageEnum = UninstallTapResult::Enum::NoPermission;
            return result;
        }
        if (noDeviceRemoved) {
            result.returnBody.messageEnum = UninstallTapResult::Enum::NotFound;
            return result;
        }
    }
    catch (...)
    {
        result.returnBody.messageEnum = UninstallTapResult::Enum::UninstallerNotExist;
        return result;
    }

    result.returnBody.messageEnum = UninstallTapResult::Enum::Other;
    result.returnBody.messageString = ss::ForceToWstring(stdoutSstream.str());
    return result;
}
