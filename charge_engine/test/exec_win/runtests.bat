
@ECHO.
@ECHO # Windows platform process-launching tests

@SET cscript_exe_full_path=%WINDIR%\System32\cscript.exe
@IF NOT EXIST %cscript_exe_full_path% (
    @ECHO Failed to find cscript.exe.
    @EXIT /B 2
)

@CD exec
@CALL runtests.bat
@IF %ERRORLEVEL% NEQ 0 (CD .. & EXIT /B 1)
@CD ..

@CD shell_process
@CALL runtests.bat
@IF %ERRORLEVEL% NEQ 0 (CD .. & EXIT /B 1)
@CD ..
