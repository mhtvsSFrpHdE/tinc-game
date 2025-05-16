#include "Layout_SRV.h"

void Layout_SRV::AddFixedSpacer(wxDirection direction, int value, wxSizer* sizer)
{
	sizer->Add(0, 0, 0, direction, value);
}
