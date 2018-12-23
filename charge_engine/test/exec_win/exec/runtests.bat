
@SET cscript_exe_full_path=%WINDIR%\System32\cscript.exe
@IF EXIST %cscript_exe_full_path% GOTO cscript_ok
@ECHO Failed to find cscript.exe.
@EXIT /B 2
:cscript_ok

@SET test_pgm="%BUILD_ROOT%\bin\exec_test.exe"
@IF EXIST %test_pgm% GOTO build_root_ok
@ECHO BUILD_ROOT is not correctly configured.
@EXIT /B 2
:build_root_ok

@SET capture_fn="%TEMP%\exec_test_capture"


@ECHO Running tests:


@ECHO - Repeats passed arguments to program
@%test_pgm% "%COMSPEC%" /c repeat_args.bat a b c > %capture_fn%
@comp %capture_fn% expected_result_01 /M >NUL
@IF ERRORLEVEL 1 GOTO error


@ECHO - Repeats quoted arguments
@%test_pgm% "%COMSPEC%" /c repeat_args.bat a "b c" > %capture_fn%
@comp %capture_fn% expected_result_02 /M >NUL
@IF ERRORLEVEL 1 GOTO error


@ECHO - Exit zero code
@%test_pgm% "%cscript_exe_full_path%" exit_with_code.vbs 0 >NUL
@IF %ERRORLEVEL% NEQ 0 GOTO error


@ECHO - Exit non-zero code
@%test_pgm% "%cscript_exe_full_path%" exit_with_code.vbs 22 >NUL
@IF %ERRORLEVEL% NEQ 22 GOTO error


@ECHO Ran all tests successfully.
@EXIT /B 0

:error
@ECHO ###################
@ECHO    Tests failed!
@ECHO ###################
@EXIT /B 1
