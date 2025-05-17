#include "TapDevice_SRV.h"
#include "Settings_SRV.h"

void TapDevice_SRV::Init()
{
    namespace sk = SettingKeys;

    defaultTap = Settings_SRV::config->Read(sk::defaultVirtualNetworkAdapter).ToStdWstring();
    emptyPlaceholder = Settings_SRV::emptyPlaceholder1;
}

void TapDevice_SRV::ReloadAdapterList()
{
    auto newAdapterList = std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>();
    WindowsAPI_SRV::GetAdaptersAddresses(&newAdapterList);

    tapList.clear();
    for (auto& adapter : newAdapterList) {
        if (adapter.isTap()) {
            tapList.push_back(adapter);
        }
    }
}

ReturnValue<TapDevice_SRV::GetDefaultTapResult> TapDevice_SRV::GetDefaultTap()
{
    auto result = ReturnValue<GetDefaultTapResult>();
    bool haveAnyInstalledAdapter = tapList.size() > 0;

    if (defaultTap == emptyPlaceholder) {
        if (haveAnyInstalledAdapter) {
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
        if (haveAnyInstalledAdapter) {
            for (auto& adapter : tapList) {
                if (adapter.friendlyName == defaultTap) {
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

void TapDevice_SRV::SetDefaultTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter)
{
    namespace sk = SettingKeys;

    defaultTap = adapter.friendlyName;

    Settings_SRV::config->Write(sk::defaultVirtualNetworkAdapter, wxString(adapter.friendlyName));
    Settings_SRV::config->Flush();
}

std::wstring TapDevice_SRV::defaultTap;
std::wstring TapDevice_SRV::emptyPlaceholder;

std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult> TapDevice_SRV::tapList;
