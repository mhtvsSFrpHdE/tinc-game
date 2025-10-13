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

struct PrepareUpdateResult {
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
    static bool command_prepareUpdate;
    static bool command_uninstall;

private:
    std::mutex uiMutex;
    std::condition_variable uiCb;

    wxPanel* rootPanel;
    wxStaticText* text;

    void API_UI_ReportUpdateResult(ReturnValue<UpdateResult> result);
    void API_UI_ReportPrepareUpdateResult(ReturnValue<PrepareUpdateResult> result);

    void API_SRV_Update_EndLogToFile();
    void API_SRV_Update_OnCopyFail(ReturnValue<UpdateResult>& result, wxFileName& srcfile, wxFileName& destfile);
    void API_SRV_Update_OnRenameFail(ReturnValue<UpdateResult>& result, wxFileName& srcfile, wxFileName& destfile);
    void API_SRV_Update_OnRemoveFail(ReturnValue<UpdateResult>& result, wxFileName& file);
    void API_SRV_Update();

    void API_SRV_PrepareUpdate_EndLogToFile();
    void API_SRV_PrepareUpdate_OnCopyFail(ReturnValue<PrepareUpdateResult>& result, wxFileName& srcfile, wxFileName& destfile);
    void API_SRV_PrepareUpdate_OnRenameFail(ReturnValue<PrepareUpdateResult>& result, wxFileName& srcfile, wxFileName& destfile);
    void API_SRV_PrepareUpdate_OnRemoveFail(ReturnValue<PrepareUpdateResult>& result, wxFileName& file);
    bool API_SRV_PrepareUpdate();
    void API_SRV_ProcessAdditionalArgument();

    void Init_CreateControls();
    void Init_Layout();
    void Init_PostLayout();
};
