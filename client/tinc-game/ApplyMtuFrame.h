#pragma once
#include <wx/wx.h>
#include "..\resource\ReturnValue.h"
#include <vector>
#include <string>
#include "OptimizeMtuFrame.h"
#include <unordered_map>
#include "WindowsAPI_SRV.h"

struct ApplyMtuResult {
    enum class Enum {
        Other,
        InvalidAdapterName,
        Failed_IPv4,
        Failed_IPv6
    };
    Enum messageEnum = Enum::Other;
    std::wstring messageString;
};

class ApplyMtuFrame : public wxFrame
{
public:
    ApplyMtuFrame(OptimizeMtuFrame* parentFrame, int mtuValue_IPv4, int mtuValue_IPv6);

    static ReturnValue<ApplyMtuResult> API_SRV_ApplyMtu(int mtu_IPv4, int mtu_IPv6, std::wstring adapterName);
    static void API_SRV_OpenCommandPrompt();
    static std::wstring API_SRV_GetNetshCommand(std::wstring adapterName, int mtu_IPv4, int mtu_IPv6);

private:
    wxFrame* _parentFrame;

    int _mtuValue_IPv4 = 0;
    int _mtuValue_IPv6 = 0;

    wxWindowDisabler makeModal;

    wxPanel* rootPanel;

    wxStaticText* chooseAdapter_StaticText;
    wxComboBox* chooseAdapter_ComboBox;
    std::unordered_map<int, WindowsAPI_SRV::GetAdaptersAddressesResult> chooseAdapter_ComboBox_RawData;
    wxButton* chooseAdapter_HelpMeDecideButton;
    wxStaticText* displayMtu_IPv4;
    wxStaticText* displayMtu_IPv6;
    wxStaticText* yourCommand_StaticText;
    wxTextCtrl* yourCommand_TextCtrl;
    wxButton* yourCommand_CopyButton;
    wxButton* navigate_ApplyButton;
    wxButton* navigate_CancelButton;

    void Init_CreateControls();
    void Init_Layout();

    void OnHelpMeDecideButton(wxCommandEvent& evt);
    void OnChooseTargetInterfaceChange(wxCommandEvent& evt);
    void OnCopyButton(wxCommandEvent& evt);
    void OnConfirmButton(wxCommandEvent& evt);
    void OnCancelButton(wxCommandEvent& evt);

    void OnClose(wxCloseEvent& event);
};
