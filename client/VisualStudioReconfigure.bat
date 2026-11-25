@REM Delete out but keep downloaded _deps

@REM Backup downloaded library
move "out\build\x64-Debug\_deps" "out_deps_x64_debug_bak"
echo "Backup success? If not close window and check background process."
pause
rd /s /q "out\build"
md "out\build\x64-Debug"
move "out_deps_x64_debug_bak" "out\build\x64-Debug\_deps"

pause
