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
VerticalBoxSizer - HorizontalBoxSizer - VerticalBoxSizer - ControlA
VerticalBoxSizer - HorizontalBoxSizer - HorizontalBoxSizer - ControlB
ControlB will position at x axis -5 compared to ControlA for unknown reason
Add 5 offset wxLEFT to fix that

Const int 5 already intergrated to Layout_SRV
