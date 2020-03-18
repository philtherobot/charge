@ECHO.
@ECHO ## Test cases for charge::exec

@SET test_pgm="%BIN_UNDER_TEST_PATH%\run_exec.exe"
@IF EXIST %test_pgm% GOTO bin_found
@ECHO Cannot find %%BIN_UNDER_TEST_PATH%%\run_exec.exe
@EXIT /B 2
:bin_found

@SET capture_fn="%TEMP%\run_exec_capture"


@ECHO.
@ECHO ### exec repeats passed arguments to program
@%test_pgm% "%COMSPEC%" /c repeat_args.bat a b c > %capture_fn%
@comp %capture_fn% expected_result_repeats_arguments.txt /M >NUL
@IF ERRORLEVEL 1 GOTO error
@ECHO Test passed


@ECHO.
@ECHO ### exec repeats quoted arguments
@%test_pgm% "%COMSPEC%" /c repeat_args.bat a "b c" > %capture_fn%
@comp %capture_fn% expected_result_repeats_quoted_arguments.txt /M >NUL
@IF ERRORLEVEL 1 GOTO error
@ECHO Test passed

@ECHO.
@ECHO ### Zero exit code is transmitted
@%test_pgm% "%cscript_exe_full_path%" exit_with_code.vbs 0 >NUL
@IF %ERRORLEVEL% NEQ 0 GOTO error
@ECHO Test passed


@ECHO.
@ECHO ### Non-zero exit code is transmitted
@%test_pgm% "%cscript_exe_full_path%" exit_with_code.vbs 22 >NUL
@IF %ERRORLEVEL% NEQ 22 GOTO error
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
