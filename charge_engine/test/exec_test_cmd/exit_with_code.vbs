' Have to use VBScript here as "cmd /c mybatch.bat"
' always has exit code zero no matter what mybatch.bat sets.
Set args = WScript.Arguments
WScript.Quit(args(0))
