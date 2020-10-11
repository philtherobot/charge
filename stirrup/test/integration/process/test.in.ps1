Write-Host "Test [Stirrup | SystemProcess]" -ForegroundColor Magenta

function Invoke-ChildProcessActor
{
    ${CMAKE_BINARY_DIR}\bin\stirrup_process_invoke output_capture_off ${CMAKE_BINARY_DIR}\bin\stirrup_child_process_actor.exe $args | Out-String
}

function Invoke-CaptureChildProcessActor
{
    ${CMAKE_BINARY_DIR}\bin\stirrup_process_invoke output_capture_on ${CMAKE_BINARY_DIR}\bin\stirrup_child_process_actor.exe $args | Out-String
}

function Test-Equality($value, $expected)
{
    if($value -eq $expected)
    {
        Write-Host Passed -ForegroundColor Green
    }
    else
    {
        Write-Host "** FAILED" -ForegroundColor DarkRed
        Write-Host Got `"$value`"
        Write-Host Expected `"$expected`"
    }
}

Write-Host "Case: no argument and zero exit code"

$output = Invoke-ChildProcessActor

$expected = "stirrup_process_invoke: start`r`nstirrup_child_process_actor`r`nstirrup_process_invoke: exit code 0`r`n"

Test-Equality $output $expected


Write-Host "Case: two arguments and non-zero exit code"

$output = Invoke-ChildProcessActor arg1 exit_with_error

$expected = "stirrup_process_invoke: start`r`nstirrup_child_process_actor`r`n1: arg1`r`n2: exit_with_error`r`nstirrup_process_invoke: exit code 22`r`n"

Test-Equality $output $expected


Write-Host "Case: capture child process the output"

$output = Invoke-CaptureChildProcessActor

$expected = "stirrup_process_invoke: start`r`nstirrup_process_invoke: exit code 0`r`nstirrup_process_invoke: captured`r`n"

Test-Equality $output $expected
