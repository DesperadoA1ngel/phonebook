@echo off
echo Installing Qt files for PhoneBook...
echo.

cd C:\Projects\phonebook\build

echo 1. Copying core DLLs...
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5Core.dll" .
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5Widgets.dll" .
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5Gui.dll" .
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5ANGLE.dll" .

echo 2. Creating platforms folder...
if not exist platforms mkdir platforms

echo 3. Copying windows plugin...
copy "C:\Qt5\5.15.2\mingw81_32\plugins\platforms\qwindows.dll" platforms\

echo 4. Copying additional plugins...
if not exist imageformats mkdir imageformats
copy "C:\Qt5\5.15.2\mingw81_32\plugins\imageformats\qjpeg.dll" imageformats\

echo.
echo Done! Now you can run the program.
echo To run: start PhoneBook.exe
pause