
@ECHO.
@ECHO # Windows platform process-launching tests

@ECHO.
@ECHO ## exec

@CD exec_test_cmd
@CALL exec_test_driver.bat
@CD ..

@ECHO.
@ECHO ## process

@CD process_test_cmd
@CALL process_test_driver.bat
@CD ..
