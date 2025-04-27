#pragma once
#include <wx/wx.h>
#include "ReturnValue.h"
#include <vector>
#include <string>

struct ApplyMtuResult {
	enum Enum {
		ApplyMtu_Other,
		ApplyMtu_InvalidAdapterName,
		ApplyMtu_Failed_IPv4,
		ApplyMtu_Failed_IPv6,
		ApplyMtu_Failed_All
	};
	Enum messageEnum;
	std::wstring messageString;
};

class ApplyMtuFrame : public wxFrame
{
public:
	ApplyMtuFrame();

	// UI to SRV
	static ReturnValue<std::vector<std::wstring>> API_SRV_GetNetworkAdapterList();
	static ReturnValue<ApplyMtuResult> API_SRV_ApplyMtu(int mtu_IPv4, int mtu_IPv6, std::wstring adapterName);
	static bool API_SRV_OpenNetworkControlPanel();
};
