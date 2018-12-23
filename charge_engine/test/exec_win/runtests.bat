
@ECHO.
@ECHO # Windows platform process-launching tests

@ECHO.
@ECHO ## exec

@CD exec
@CALL runtests.bat
@CD ..

@ECHO.
@ECHO ## shell process

@CD shell_process
@CALL runtests.bat
@CD ..
