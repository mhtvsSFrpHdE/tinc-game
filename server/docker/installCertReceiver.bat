call pythonEnvironment.bat
python -m venv .venv
call .venv\Scripts\activate.bat
pip install uploadserver==6.0.1
