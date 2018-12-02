
@SET cscript_exe_full_path=%WINDIR%\System32\cscript.exe
@IF EXIST %cscript_exe_full_path% GOTO cscript_ok
@ECHO Failed to find cscript.exe.
@EXIT /B 2
:cscript_ok

@IF [%BUILD_ROOT%] NEQ [] GOTO build_root_ok
@ECHO BUILD_ROOT is not configured.
@EXIT /B 2
:build_root_ok

@SET capture_fn="%TEMP%\exec_test_capture"


@ECHO Running test:


@ECHO - Repeats passed arguments to program
@"%BUILD_ROOT%\bin\exec_test" "%COMSPEC%" /c repeat_args.bat a b c > %capture_fn%
@comp %capture_fn% expected_results_01 /M >NUL
@IF ERRORLEVEL 1 GOTO error


@ECHO - Repeats quoted arguments
@"%BUILD_ROOT%\bin\exec_test" "%COMSPEC%" /c repeat_args.bat a "b c" > %capture_fn%
@comp %capture_fn% expected_results_02 /M >NUL
@IF ERRORLEVEL 1 GOTO error


@ECHO - Exit zero code
@"%BUILD_ROOT%\bin\exec_test" "%cscript_exe_full_path%" exit_with_code.vbs 0 >NUL
@IF %ERRORLEVEL% NEQ 0 GOTO error


@ECHO - Exit non-zero code
@"%BUILD_ROOT%\bin\exec_test" "%cscript_exe_full_path%" exit_with_code.vbs 22 >NUL
@IF %ERRORLEVEL% NEQ 22 GOTO error


@ECHO Ran all tests successfully.
@EXIT /B 0

:error
@ECHO ###################
@ECHO    Tests failed!
@ECHO ###################
@EXIT /B 1
