# Known bug

## wxWidgets `3.1.5`, `Windows 11`
## Parent frame not show after close frame
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
## Multi level nested BoxSizer layout error
Const BoxSizerVhhOffset already intergrated to Layout_SRV

VerticalBoxSizer - HorizontalBoxSizer - VerticalBoxSizer - ControlA  
VerticalBoxSizer - HorizontalBoxSizer - HorizontalBoxSizer - ControlB  
ControlB will position at x axis `-<BoxSizerVhhOffset>` compared to ControlA for unknown reason  
Add BoxSizerVhhOffset offset wxLEFT or fixed spacer to fix that

If that still doesn't work
- Check if your content total GetMinSize exceeds sizer limit
- Explicitly SetMinSize for each control especially TextCtrl (like wxTE_READONLY)

## wxWidgets `3.1.5`, `Windows 11`
## Redirect log to file doesn't work
Redirect log to file doesn't work if log file opened by Windows `notepad.exe`  
and `wxLog::SetActiveTarget` is called on background thread

Workaround is run `wxLog::SetActiveTarget` by using `CallAfter` and `Lock_SRV` for thread sync  
Bug report at https://github.com/wxWidgets/wxWidgets/issues/25644

## wxWidgets `3.1.5`, `Windows 11`
## Default translate path not match document
[Document](https://docs.wxwidgets.org/3.2/overview_i18n.html) says on Windows translate load from this dir:
```
C:\Program Files\MyApp\myapp.exe
C:\Program Files\MyApp\de\myapp.mo
C:\Program Files\MyApp\fr\myapp.mo
```
But actually in wxWidgets `3.1.5` default is
```
C:\Program Files\MyApp\myapp.exe
C:\Program Files\de\myapp.mo
C:\Program Files\fr\myapp.mo
```
To fix it add program exe dir to `wxLocale` `AddCatalogLookupPathPrefix`

## Visual Studio `2019`, `Windows 11`
## CMake project show non existent error in Visual Studio
When using `CMakeLists.txt` as project (solution / build system)  
After delete `out` folder, Press Ctrl + S in `CMakeLists.txt`,  
a info says "C++ IntelliSense information may be out of date, generate the CMake cache to refresh. Generate Settings"  
and at same time CMake error appears.

This doesn't mean there is error need to be fix in `CMakeLists.txt`  
Press Ctrl + S again or click on `Generate`, error will disappear
