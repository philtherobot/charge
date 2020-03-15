
@ECHO.
@ECHO ## Test cases for ShellProcess

@SET capture_fn="%TEMP%\run_process_capture"


@ECHO.
@ECHO ### Exit zero code
@%test_pgm% "%cscript_exe_full_path%" /nologo three_lines.vbs 0 > %capture_fn%
@IF %ERRORLEVEL% NEQ 0 GOTO error
@comp %capture_fn% expected_result_01 /M >NUL
@IF ERRORLEVEL 1 GOTO error


@ECHO.
@ECHO ### Exit non-zero code
@%test_pgm% "%cscript_exe_full_path%" /nologo three_lines.vbs 22 > %capture_fn%
@IF %ERRORLEVEL% NEQ 22 GOTO error
@comp %capture_fn% expected_result_02 /M >NUL
@IF ERRORLEVEL 1 GOTO error

@ECHO.
@ECHO Ran all tests successfully.
@EXIT /B 0

:error
@ECHO ###################
@ECHO    Tests failed!
@ECHO ###################
@EXIT /B 1
