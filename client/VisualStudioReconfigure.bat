@REM Delete out but keep downloaded _deps

@REM Backup downloaded library
move "out\build\x64-Debug\_deps" "out_deps_x64_debug_bak"
move "out\build\x64-Release\_deps" "out_deps_x64_release_bak"
move "out\build\x86-Debug\_deps" "out_deps_x86_debug_bak"
echo "Backup success? If not close window and check background process."
pause
rd /s /q "out\build"
md "out\build\x64-Debug"
move "out_deps_x64_debug_bak" "out\build\x64-Debug\_deps"
md "out\build\x64-Release"
move "out_deps_x64_release_bak" "out\build\x64-Release\_deps"
md "out\build\x86-Debug"
move "out_deps_x86_debug_bak" "out\build\x86-Debug\_deps"

pause
