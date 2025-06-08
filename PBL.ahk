#SingleInstance Force
Gui, Add, Text,, Click the button to run your app:
Gui, Add, Button, gRunProgram, Run Program
Gui, Show, , My App
return

RunProgram:
Run, %A_ScriptDir%\PHASE2_001.exe
return