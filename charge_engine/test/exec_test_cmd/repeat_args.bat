
@ECHO repeat_args
@SET /a counter=1
:loop
@IF [%1] == [] GOTO end
@ECHO %counter%: %1
@SET /a counter=%counter%+1
@SHIFT
@GOTO loop

:end
