#pragma once
#include <wx/wx.h>
#include <boost/circular_buffer.hpp>

/// <summary>
/// wxTextCtrl with max lines limit
/// After set max lines, will automatically remove first line before append any text on max line exceed
/// All function name starts with "tinc...();"
/// </summary>
class tincTextCtrl : public wxTextCtrl
{
public:
    tincTextCtrl(wxWindow* parent, wxWindowID id,
        const wxString& value = wxEmptyString,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxASCII_STR(wxTextCtrlNameStr)
    );
    /// <summary>
    /// Call this before any text append
    /// </summary>
    /// <param name="lines"></param>
    void tincSetMaxLines(int lines);
    void tincAppendText(const wxString& text);

private:
    typedef boost::circular_buffer<int> IntCb;
    std::unique_ptr<IntCb> tincCb;
    int maxLines = 0;
};
