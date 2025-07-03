#pragma once
#include <wx/wx.h>
namespace Layout_SRV
{
    const int SpaceToFrameBorder = 20;
    const int SpaceBetweenControl = 10;
    /// <summary>
    /// Stretch proportion that take as much space as possible
    /// </summary>
    const int TakeAllSpace = 100;
    /// <summary>
    /// VerticalBoxSizer - HorizontalBoxSizer - VerticalBoxSizer - ControlA
    /// VerticalBoxSizer - HorizontalBoxSizer - HorizontalBoxSizer - ControlB
    /// ControlB will position at x axis -5 compared to ControlA for unknown reason
    /// Add 5 offset wxLEFT to fix that
    /// </summary>
    const int BoxSizerVhhOffset = 5;

    void AddFixedSpacer(wxDirection direction, int value, wxSizer* sizer);
};
