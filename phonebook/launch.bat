@echo off
echo Запуск PhoneBook...
echo.

REM Переходим в папку с программой
cd /d "C:\Projects\phonebook\build"

REM Проверяем наличие необходимых файлов
if not exist Qt5Core.dll (
    echo ОШИБКА: Не найден Qt5Core.dll
    echo Выполните сначала fix_qt.bat
    pause
    exit /b 1
)

if not exist platforms\qwindows.dll (
    echo ОШИБКА: Не найден плагин platforms\qwindows.dll
    echo Выполните сначала fix_qt.bat
    pause
    exit /b 1
)

REM Запускаем программу
echo Запуск приложения...
start "" "PhoneBook.exe"

echo Программа запущена.
echo Это окно можно закрыть.
pause