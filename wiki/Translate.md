## External links
https://docs.wxwidgets.org/3.2/overview_i18n.html  
https://wiki.wxwidgets.org/Internationalization

## Translate tinc game to your language in short
For research purpose in order to know how to translate our program  
we have a zh translation quickly done by Google Translate  
here is what we got
- Install [gettext](https://www.gnu.org/software/gettext/), Windows version [download here](https://mlocati.github.io/articles/gettext-iconv-windows.html)
- Add `gettext\bin` to `PATH`, where `xgettext.exe` located
- Edit `tinc-game\GenerateTranslatePot.bat`, change zh to your language code
- Run `tinc-game\GenerateTranslatePot.bat`, this get text from program code and save `pot` files to `po\<language code>`
- Open these `pot` files with [PoEdit](http://poedit.sourceforge.net), make translate and save to `po` file
- Now folder look like this, for example `po\de`  
  - `MainFrame.pot`
  - `MainFrame.po`
  - `MainFrame.mo`
  - `ApplyMtuFrame.pot`
  - `ApplyMtuFrame.po`
  - `ApplyMtuFrame.mo`
  - ...
- Edit `tinc-game\MergeTranslatePo.py`, change zh to your language code
- Run `cd tinc-game`, `python MergeTranslatePo.py`
- You have `po\de\tinc-game.mo`, copy po folder to where `tinc-game.exe` located
- Edit `App.cpp` add your language to program depending on exist language load code
- Copy updated `tinc-game.exe` and run it to confirm translate work or not
