# Known bug

## wxWidgets `3.1.5`, `Windows 11`
When closing frame, if there is it's parent frame, will not present as top frame  
instead the whole programe act like minimized and other unrelated program show as top window  
Solution is mount a OnClose event, save it's parent frame pointer  
later OnClose call Raise on pointer
```
void HelpFrame::OnClose(wxCloseEvent& event)
{
    _parentFrame->Raise();

    event.Skip();
}
```

## wxWidgets `3.1.5`, `Windows 11` 
Const BoxSizerVhhOffset already intergrated to Layout_SRV

VerticalBoxSizer - HorizontalBoxSizer - VerticalBoxSizer - ControlA  
VerticalBoxSizer - HorizontalBoxSizer - HorizontalBoxSizer - ControlB  
ControlB will position at x axis -<BoxSizerVhhOffset> compared to ControlA for unknown reason  
Add BoxSizerVhhOffset offset wxLEFT or fixed spacer to fix that

If that still doesn't work
- Check if your content total GetMinSize exceeds sizer limit
- Explicitly SetMinSize for each control especially TextCtrl (like wxTE_READONLY)
