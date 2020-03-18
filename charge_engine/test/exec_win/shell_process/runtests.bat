@ECHO.
@ECHO ## Test cases for ShellProcess

@SET test_pgm="%BIN_UNDER_TEST_PATH%\run_process.exe"
@IF EXIST %test_pgm% GOTO bin_found
@ECHO Cannot find %%BIN_UNDER_TEST_PATH%%\run_process.exe
@EXIT /B 2
:bin_found

@SET capture_fn="%TEMP%\run_process_capture"


@ECHO.
@ECHO ### Exit zero code
@%test_pgm% "%cscript_exe_full_path%" /nologo three_lines.vbs 0 > %capture_fn%
@IF %ERRORLEVEL% NEQ 0 GOTO error
@comp %capture_fn% expected_result_three_lines_exit_0.txt /M >NUL
@IF ERRORLEVEL 1 GOTO error
@ECHO Test passed


@ECHO.
@ECHO ### Exit non-zero code
@%test_pgm% "%cscript_exe_full_path%" /nologo three_lines.vbs 22 > %capture_fn%
@IF %ERRORLEVEL% NEQ 22 GOTO error
@comp %capture_fn% expected_result_three_lines_exit_22.txt /M >NUL
@IF ERRORLEVEL 1 GOTO error
@ECHO Test passed


@ECHO.
@ECHO Ran all tests successfully.
@EXIT /B 0

:error
@ECHO.
@ECHO.
@ECHO -------------
@ECHO Tests failed!
@ECHO -------------
@EXIT /B 1
