#include <boost/process.hpp>
#include "ApplyMtuFrame.h"

ReturnValue<std::vector<std::wstring>> ApplyMtuFrame::API_SRV_GetNetworkAdapterList()
{
    // TODO
    // Return false if network adapter list has 0 available network adapter
    // Return true if any available network adapter founded
    return ReturnValue<std::vector<std::wstring>>();
}

ReturnValue<ApplyMtuResult> ApplyMtuFrame::API_SRV_ApplyMtu(int mtu_IPv4, int mtu_IPv6, std::wstring adapterName)
{
    // TODO
    // Return false if "Apply MTU failed on adapter's IPv4", report error message from netsh
    // Return true if "All success", "IPv4 success but IPv6 failed"
    auto what = ReturnValue<ApplyMtuResult>();
    what.success = false;
    what.returnBody.messageEnum = ApplyMtuResult::ApplyMtu_Failed_IPv4;
    what.returnBody.messageString = L"";
    return ReturnValue<ApplyMtuResult>();
}

bool ApplyMtuFrame::API_SRV_OpenNetworkControlPanel()
{
    using namespace boost::process;
    try {
        child c("control.exe ncpa.cpl");
        c.wait();
        return true;
    }
    catch (...) {
        return false;
    }
}
