@REM CMake download proxy
@REM http_proxy=http://ip:port
set http_proxy=
set https_proxy=%http_proxy%

cd /d "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\"
start devenv.exe
