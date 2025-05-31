#include "tincTextCtrl.h"

tincTextCtrl::tincTextCtrl(
    wxWindow* parent,
    wxWindowID id,
    const wxString& value,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxValidator& validator,
    const wxString& name
) : wxTextCtrl(parent, id, value, pos, size, style, validator, name) { }

void tincTextCtrl::tincSetMaxLines(int lines)
{
    maxLines = lines;
    tincCb = std::unique_ptr<IntCb>(new IntCb(lines));
}

void tincTextCtrl::tincAppendText(const wxString& text)
{
    if (tincCb == nullptr) {
        AppendText(text);
        return;
    }

    if (maxLines == tincCb->size()) {
        auto firstLineLength = (*tincCb.get())[0];
        Remove(0, firstLineLength + 1);
        tincCb->push_back(text.length());
        AppendText(text);
        return;
    }

    auto lineLength = text.length();
    tincCb->push_back(lineLength);
    AppendText(text);
    return;
}
