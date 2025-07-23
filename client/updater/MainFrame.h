#pragma once
#include <wx/wx.h>
#include "../resource/Lock_SRV.h"
#include "../resource/ReturnValue.h"
#include <wx/filename.h>

struct UpdateResult {
    enum class Enum {
        NoLogFile,
        Copy,
        Rename,
        Remove,
        Other
    };
    Enum messageEnum = Enum::Other;
    std::wstring messageString;
};

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    std::mutex uiMutex;
    std::condition_variable uiCb;

    wxPanel* rootPanel;
    wxStaticText* text;

    void API_UI_ReportUpdateResult(ReturnValue<UpdateResult> result);

    void API_SRV_Update_EndLogToFile();
    void API_SRV_Update_OnCopyFail(ReturnValue<UpdateResult>& result, wxFileName& srcfile, wxFileName& destfile);
    void API_SRV_Update_OnRenameFail(ReturnValue<UpdateResult>& result, wxFileName& srcfile, wxFileName& destfile);
    void API_SRV_Update_OnRemoveFail(ReturnValue<UpdateResult>& result, wxFileName& file);
    void API_SRV_Update();

    void Init_CreateControls();
    void Init_Layout();
    void Init_PostLayout();
};
