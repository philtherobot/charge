@ECHO.
@ECHO ## Test cases for "find script path"


@ECHO.
@ECHO ### Script path environment variable set in script
@SET CHARGE_SCRIPT=
@SET script_output=
@FOR /f %%i IN ('charge find_script_path_script.cpp') DO @SET script_output=%%i
@SET expected_result=%~dp0find_script_path_script.cpp
@IF "%script_output%" NEQ "%expected_result%" GOTO error
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
