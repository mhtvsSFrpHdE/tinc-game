#pragma once
#include <wx/wx.h>
namespace Layout_SRV
{
    const int SpaceToFrameBorder = 20;
    const int SpaceBetweenControl = 10;
    // Stretch proportion that take as much space as possible
    const int TakeAllSpace = 100;

    void AddFixedSpacer(wxDirection direction, int value, wxSizer* sizer);
};
