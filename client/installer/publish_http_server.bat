@REM Start HTTP server
@REM Allow transfer installer to XP 2001 and upload logs back
@cd /d %~dp0

@REM python -m venv .venv
@call .venv\Scripts\activate

@REM pip install uploadserver
@python -m uploadserver
