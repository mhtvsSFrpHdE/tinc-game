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
