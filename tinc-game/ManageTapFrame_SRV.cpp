#include "ManageTapFrame.h"
#include "String_SRV.h"

ReturnValue<std::wstring> ManageTapFrame::API_SRV_InstallTap()
{
    auto result = ReturnValue<std::wstring>();
    result.success = false;
    result.returnBody = L"Unknown";
    return result;
}

ReturnValue<std::wstring> ManageTapFrame::API_SRV_UninstallTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter)
{
    auto result = ReturnValue<std::wstring>();
    result.returnBody = adapter.friendlyName + " | " + adapter.modelName + String_SRV::newLine + adapter.windows_LUID;
    result.success = true;
    return result;
}
