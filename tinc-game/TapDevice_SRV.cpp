#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "TapDevice_SRV.h"
#include "Resource_SRV.h"

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

ReturnValue<WindowsAPI_SRV::GetAdaptersAddressesResult> TapDevice_SRV::GetFirstTap()
{
    auto result = ReturnValue<WindowsAPI_SRV::GetAdaptersAddressesResult>();

    bool hasAnyInstalledAdapter = HasAnyInstalledTap();
    if (hasAnyInstalledAdapter) {
        result.returnBody = tapList[0];
        result.success = true;
    }

    return result;
}

ReturnValue<WindowsAPI_SRV::GetAdaptersAddressesResult> TapDevice_SRV::GetFirstTap(bool refresh)
{
    if (refresh) {
        ReloadAdapterList();
    }
    return TapDevice_SRV::GetFirstTap();
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
        bp::child c(Resource_SRV::Bat::controlPanel);
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
    namespace rsb = Resource_SRV::Bat;
    try {
        bp::child c(bp::shell(), bp::args({ rsb::cmdRumCommand, rsb::deviceManager }), bp::windows::hide);
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

std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult> TapDevice_SRV::tapList;
