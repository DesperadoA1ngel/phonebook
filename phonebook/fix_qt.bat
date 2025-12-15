@echo off
echo Установка Qt файлов для PhoneBook...
echo.

cd C:\Projects\phonebook\build

echo 1. Копирование основных DLL...
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5Core.dll" .
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5Widgets.dll" .
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5Gui.dll" .
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5ANGLE.dll" .

echo 2. Создание папки platforms...
if not exist platforms mkdir platforms

echo 3. Копирование плагина windows...
copy "C:\Qt5\5.15.2\mingw81_32\plugins\platforms\qwindows.dll" platforms\

echo 4. Копирование дополнительных плагинов...
if not exist imageformats mkdir imageformats
copy "C:\Qt5\5.15.2\mingw81_32\plugins\imageformats\qjpeg.dll" imageformats\

echo.
echo Готово! Теперь можно запускать программу.
echo Запуск: start PhoneBook.exe
pause