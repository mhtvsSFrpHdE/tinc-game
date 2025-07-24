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

    // Allow frame close
    event.Skip();
}
```

It is irrelevant to not calling wxApp::Init base function or not calling SetTopWindow

## wxWidgets `3.1.5`, `Windows 11`
Const BoxSizerVhhOffset already intergrated to Layout_SRV

VerticalBoxSizer - HorizontalBoxSizer - VerticalBoxSizer - ControlA  
VerticalBoxSizer - HorizontalBoxSizer - HorizontalBoxSizer - ControlB  
ControlB will position at x axis `-<BoxSizerVhhOffset>` compared to ControlA for unknown reason  
Add BoxSizerVhhOffset offset wxLEFT or fixed spacer to fix that

If that still doesn't work
- Check if your content total GetMinSize exceeds sizer limit
- Explicitly SetMinSize for each control especially TextCtrl (like wxTE_READONLY)

## wxWidgets `3.1.5`, `Windows 11`
Redirect log to file doesn't work if log file opened by Windows `notepad.exe`  
and `wxLog::SetActiveTarget` is called on background thread

Workaround is run `wxLog::SetActiveTarget` by using `CallAfter` and `Lock_SRV` for thread sync  
Bug report at https://github.com/wxWidgets/wxWidgets/issues/25644
