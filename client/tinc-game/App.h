#pragma once
#include <wx/wx.h>

class App : public wxApp
{
public:
    bool OnInit() override;

private:
    std::shared_ptr<wxLocale> locale;
};
