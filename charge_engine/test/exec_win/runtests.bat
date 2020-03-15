
@ECHO.
@ECHO # Windows platform process-launching tests

@SET cscript_exe_full_path=%WINDIR%\System32\cscript.exe
@IF EXIST %cscript_exe_full_path% GOTO cscript_ok
@ECHO Failed to find cscript.exe.
@EXIT /B 2
:cscript_ok

@SET test_pgm="%BUILD_ROOT%\bin\run_exec.exe"
@IF EXIST %test_pgm% GOTO build_root_ok
@ECHO BUILD_ROOT is not correctly configured.
@EXIT /B 2
:build_root_ok


@CD exec
@CALL runtests.bat
@CD ..

@CD shell_process
@CALL runtests.bat
@CD ..
