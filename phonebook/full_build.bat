@echo off
echo ====================================
echo Полная сборка PhoneBook
echo ====================================

echo 1. Очистка предыдущей сборки...
if exist build rmdir /s /q build
mkdir build
cd build

echo 2. Конфигурация CMake...
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt5/5.15.2/mingw81_32/lib/cmake" ..

if errorlevel 1 (
    echo ОШИБКА: CMake конфигурация не удалась!
    pause
    exit /b 1
)

echo 3. Сборка проекта...
mingw32-make

if errorlevel 1 (
    echo ОШИБКА: Сборка не удалась!
    pause
    exit /b 1
)

echo 4. Копирование Qt файлов...
echo   - Основные DLL...
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5Core.dll" .
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5Widgets.dll" .
copy "C:\Qt5\5.15.2\mingw81_32\bin\Qt5Gui.dll" .

echo   - Плагин platforms...
if not exist platforms mkdir platforms
copy "C:\Qt5\5.15.2\mingw81_32\plugins\platforms\qwindows.dll" platforms\

echo   - Дополнительные плагины...
if not exist imageformats mkdir imageformats
copy "C:\Qt5\5.15.2\mingw81_32\plugins\imageformats\qjpeg.dll" imageformats\

echo.
echo ====================================
echo СБОРКА ЗАВЕРШЕНА УСПЕШНО!
echo ====================================
echo.
echo Для запуска выполните:
echo   start PhoneBook.exe
echo.
echo Или запустите launch.bat
echo.

cd ..
pause