#pragma once
#include "ReturnValue.h"
#include "wx/wx.h"

/// <summary>
/// If something is reused many times, make it Resource_SRV to
/// - Avoid copy paste / type error
/// - Cache calculate result for next use
/// 
/// One time string may leave as is
/// 
/// Put important resource here allows hash them together to ensure more integrity in future
/// </summary>
namespace Resource_SRV
{
    std::wstring IntegrityCheck();

    /// <summary>
    /// If false, returnBody will be first invalid char
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    ReturnValue<wxString> IsValidFileOrDir(wxString& name);
};
