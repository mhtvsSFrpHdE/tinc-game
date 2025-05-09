#pragma once
#include <wx/wx.h>
#include "ReturnValue.h"
#include <vector>
#include <string>
#include "OptimizeMtuFrame.h"

struct ApplyMtuResult {
	enum Enum {
		ApplyMtu_Other,
		ApplyMtu_InvalidAdapterName,
		ApplyMtu_Failed_IPv4,
		ApplyMtu_Failed_IPv6
	};
	Enum messageEnum;
	std::wstring messageString;
};

class ApplyMtuFrame : public wxFrame
{
public:
	ApplyMtuFrame(OptimizeMtuFrame* parentFrame, int mtuValue_IPv4, int mtuValue_IPv6);

	// UI to SRV
	static ReturnValue<std::vector<std::wstring>> API_SRV_GetNetworkAdapterList();
	static ReturnValue<ApplyMtuResult> API_SRV_ApplyMtu(int mtu_IPv4, int mtu_IPv6, std::wstring adapterName);
	static bool API_SRV_OpenNetworkControlPanel();
	static void API_SRV_OpenCommandPrompt();
	static std::wstring API_SRV_GetNetshCommand(std::wstring adapterName, int mtu_IPv4, int mtu_IPv6);
	static void API_SRV_CopyNetshCommand(std::wstring command);

private:
	int _mtuValue_IPv4 = 0;
	int _mtuValue_IPv6 = 0;

	wxWindowDisabler makeModal;

	wxPanel* rootPanel = nullptr;

	wxStaticText* chooseAdapter_StaticText = nullptr;
	wxComboBox* chooseAdapter_ComboBox = nullptr;
	wxButton* chooseAdapter_HelpMeDecideButton = nullptr;
	wxStaticText* displayMtu_IPv4 = nullptr;
	wxStaticText* displayMtu_IPv6 = nullptr;
	wxStaticText* yourCommand_StaticText = nullptr;
	wxTextCtrl* yourCommand_TextCtrl = nullptr;
	wxButton* yourCommand_CopyButton = nullptr;
	wxButton* navigate_ApplyButton = nullptr;
	wxButton* navigate_CancelButton = nullptr;

	void Init_CreateControls();
	void Init_BindEventHandlers();
	void Init_Layout();

	void OnHelpMeDecideButton(wxCommandEvent& evt);
	void OnChooseTargetInterfaceChange(wxCommandEvent& evt);
	void OnCopyButton(wxCommandEvent& evt);
	void OnConfirmButton(wxCommandEvent& evt);
	void OnCancelButton(wxCommandEvent& evt);
};
