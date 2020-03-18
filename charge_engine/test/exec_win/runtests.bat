
@ECHO.
@ECHO # Windows platform process-launching tests

@SET cscript_exe_full_path=%WINDIR%\System32\cscript.exe
@IF EXIST %cscript_exe_full_path% GOTO cscript_ok
@ECHO Failed to find cscript.exe.
@EXIT /B 2
:cscript_ok


@CD exec
@CALL runtests.bat
@CD ..

@CD shell_process
@CALL runtests.bat
@CD ..
