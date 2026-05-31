call pythonEnvironment.bat
call .venv\Scripts\activate.bat
cd Downloads
python -m uploadserver 8001 --allow-replace
