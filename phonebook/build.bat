@echo off
echo Building PhoneBook...

REM Создаем папку build если ее нет
if not exist "build" mkdir build

REM Переходим в build
cd build

REM Конфигурируем CMake
echo Configuring CMake...
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt5/5.15.2/mingw81_32/lib/cmake" ..

REM Собираем проект
echo Building...
mingw32-make

REM Возвращаемся назад
cd ..

echo Build complete!
echo Run: build\PhoneBook.exe
pause