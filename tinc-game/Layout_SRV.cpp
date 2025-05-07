#include "Layout_SRV.h"

void Layout_SRV::AddTopSpacer(wxSizer* sizer, int value)
{
	sizer->Add(0, 0, 0, wxTOP, value);
}

void Layout_SRV::AddLeftSpacer(wxSizer* sizer, int value)
{
	sizer->Add(0, 0, 0, wxLEFT, value);
}
