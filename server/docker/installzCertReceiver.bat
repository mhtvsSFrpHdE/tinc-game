REM file name z for avoid it being autocomplete in first place
call pythonEnvironment.bat
python -m venv .venv
call .venv\Scripts\activate.bat
pip install uploadserver==6.0.1
