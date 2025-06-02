#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "TapDevice_SRV.h"
#include "Settings_SRV.h"

void TapDevice_SRV::Init()
{
    namespace sk = SettingKeys_Networks;

    defaultTapFriendlyName = Settings_SRV::networksConfig->Read(sk::default_tap).ToStdWstring();
    defaultTapNotSet = Settings_SRV::emptyPlaceholder1;
}

void TapDevice_SRV::ReloadAdapterList()
{
    auto newAdapterList = std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>();
    WindowsAPI_SRV::GetAdaptersAddresses(&newAdapterList);

    tapList.clear();
    for (auto& adapter : newAdapterList) {
        if (adapter.IsTap()) {
            tapList.push_back(adapter);
        }
    }
}

ReturnValue<TapDevice_SRV::GetDefaultTapResult> TapDevice_SRV::GetDefaultTap()
{
    auto result = ReturnValue<GetDefaultTapResult>();
    bool hasAnyInstalledAdapter = HasAnyInstalledTap();

    if (defaultTapFriendlyName == defaultTapNotSet) {
        if (hasAnyInstalledAdapter) {
            result.returnBody.adapter = tapList[0];
            result.success = true;
            return result;
        }
        else {
            result.returnBody.message = GetDefaultTapResult::Enum::NoTapFound;
            return result;
        }
    }
    else {
        if (hasAnyInstalledAdapter) {
            for (auto& adapter : tapList) {
                if (adapter.friendlyName == defaultTapFriendlyName) {
                    result.returnBody.adapter = adapter;
                    result.success = true;
                    return result;
                }
            }
        }
        else {
            result.returnBody.message = GetDefaultTapResult::Enum::ConfiguredTapNotExist;
            return result;
        }
    }

    return result;
}

ReturnValue<TapDevice_SRV::GetDefaultTapResult> TapDevice_SRV::GetDefaultTap(bool refresh)
{
    if (refresh) {
        ReloadAdapterList();
    }
    return TapDevice_SRV::GetDefaultTap();
}

void TapDevice_SRV::SetDefaultTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter)
{
    namespace sk = SettingKeys_Networks;

    defaultTapFriendlyName = adapter.friendlyName;

    Settings_SRV::networksConfig->Write(sk::default_tap, wxString(defaultTapFriendlyName));
    Settings_SRV::networksConfig->Flush();
}

void TapDevice_SRV::UnsetDefaultTap()
{
    namespace sk = SettingKeys_Networks;

    defaultTapFriendlyName = defaultTapNotSet;

    Settings_SRV::networksConfig->Write(sk::default_tap, wxString(defaultTapFriendlyName));
    Settings_SRV::networksConfig->Flush();
}

ReturnValue<std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>> TapDevice_SRV::API_SRV_GetNetworkAdapterList()
{
    auto result = ReturnValue<std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>>();

    WindowsAPI_SRV::GetAdaptersAddresses(&result.returnBody);

    if (result.returnBody.size() > 0) {
        result.success = true;
    }

    return result;
}

bool TapDevice_SRV::API_SRV_OpenNetworkControlPanel()
{
    namespace bp = boost::process;
    try {
        bp::child c("control.exe ncpa.cpl");
        c.wait();
        return true;
    }
    catch (...) {
        return false;
    }
}

bool TapDevice_SRV::API_SRV_OpenDeviceManager()
{
    namespace bp = boost::process;
    try {
        bp::child c(bp::shell(), bp::args({ L"/c", L"start devmgmt.msc" }), bp::windows::hide);
        c.wait();
        return true;
    }
    catch (...) {
        return false;
    }
}

bool TapDevice_SRV::HasAnyInstalledTap()
{
    auto result = tapList.size() > 0;
    return result;
}

std::wstring TapDevice_SRV::defaultTapFriendlyName;
std::wstring TapDevice_SRV::defaultTapNotSet;

std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult> TapDevice_SRV::tapList;
