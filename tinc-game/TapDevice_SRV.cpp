#include "TapDevice_SRV.h"
#include "Settings_SRV.h"

void TapDevice_SRV::Init()
{
    namespace sk = SettingKeys;

    defaultVirtualNetworkAdapter = Settings_SRV::config->Read(sk::defaultVirtualNetworkAdapter).ToStdWstring();
    emptyPlaceholder = Settings_SRV::emptyPlaceholder1;

    ReloadAdapterList();
}

void TapDevice_SRV::ReloadAdapterList()
{
    auto newAdapterList = std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>();
    WindowsAPI_SRV::GetAdaptersAddresses(&newAdapterList);

    adapterList.clear();
    for (auto& adapter : newAdapterList) {
        if (adapter.isTap()) {
            adapterList.push_back(adapter);
        }
    }
}

ReturnValue<TapDevice_SRV::GetDefaultTapResult> TapDevice_SRV::GetDefaultTap()
{
    auto result = ReturnValue<GetDefaultTapResult>();
    bool haveAnyInstalledAdapter = adapterList.size() > 0;

    if (defaultVirtualNetworkAdapter == emptyPlaceholder) {
        if (haveAnyInstalledAdapter) {
            result.returnBody.adapter = adapterList[0];
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
            for (auto& adapter : adapterList) {
                if (adapter.friendlyName == defaultVirtualNetworkAdapter) {
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

    Settings_SRV::config->Write(sk::defaultVirtualNetworkAdapter, wxString(adapter.friendlyName));
    Settings_SRV::config->Flush();
}

std::wstring TapDevice_SRV::defaultVirtualNetworkAdapter;
std::wstring TapDevice_SRV::emptyPlaceholder;

std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult> TapDevice_SRV::adapterList;
