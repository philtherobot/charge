WScript.StdOut.Write("program with" + vbCrLf)
WScript.StdErr.Write("three lines" + vbCrLf)
WScript.StdOut.Write("of output." + vbCrLf)
Set args = WScript.Arguments
WScript.Quit(args(0))
