@echo off 
echo Copying Qt DLLs... 
set QT_PATH=C:\Qt5\5.15.2\mingw81_32 
 
copy "%QT_PATH%\bin\Qt5Core.dll" . 
copy "%QT_PATH%\bin\Qt5Widgets.dll" . 
copy "%QT_PATH%\bin\Qt5Gui.dll" . 
 
if exist "%QT_PATH%\bin\libgcc_s_dw2-1.dll" copy "%QT_PATH%\bin\libgcc_s_dw2-1.dll" . 
if exist "%QT_PATH%\bin\libstdc++-6.dll" copy "%QT_PATH%\bin\libstdc++-6.dll" . 
if exist "%QT_PATH%\bin\libwinpthread-1.dll" copy "%QT_PATH%\bin\libwinpthread-1.dll" . 
 
if not exist platforms mkdir platforms 
copy "%QT_PATH%\plugins\platforms\qwindows.dll" platforms\ 
 
echo Qt DLLs copied successfully! 
echo Run: PhoneBook.exe 
pause 
