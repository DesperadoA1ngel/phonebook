@echo off
echo Clean build of PhoneBook...
echo.

set QT_PATH=C:\Qt5\5.15.2\mingw81_32

echo 1. Removing old build...
if exist build rmdir /s /q build

echo 2. Creating new build directory...
mkdir build
cd build

echo 3. Configuring CMake...
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT_PATH%/lib/cmake"

if %ERRORLEVEL% NEQ 0 (
    echo CMake failed!
    pause
    exit /b 1
)

echo 4. Building...
mingw32-make

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo SUCCESS! PhoneBook.exe created.
echo.
echo To run the program:
echo   1. Copy Qt DLLs: deploy_qt.bat
echo   2. Run: PhoneBook.exe
echo ========================================
echo.

rem Создаем deploy скрипт
echo @echo off > deploy_qt.bat
echo echo Copying Qt DLLs... >> deploy_qt.bat
echo set QT_PATH=%QT_PATH% >> deploy_qt.bat
echo. >> deploy_qt.bat
echo copy "%%QT_PATH%%\bin\Qt5Core.dll" . >> deploy_qt.bat
echo copy "%%QT_PATH%%\bin\Qt5Widgets.dll" . >> deploy_qt.bat
echo copy "%%QT_PATH%%\bin\Qt5Gui.dll" . >> deploy_qt.bat
echo. >> deploy_qt.bat
echo if exist "%%QT_PATH%%\bin\libgcc_s_dw2-1.dll" copy "%%QT_PATH%%\bin\libgcc_s_dw2-1.dll" . >> deploy_qt.bat
echo if exist "%%QT_PATH%%\bin\libstdc++-6.dll" copy "%%QT_PATH%%\bin\libstdc++-6.dll" . >> deploy_qt.bat
echo if exist "%%QT_PATH%%\bin\libwinpthread-1.dll" copy "%%QT_PATH%%\bin\libwinpthread-1.dll" . >> deploy_qt.bat
echo. >> deploy_qt.bat
echo if not exist platforms mkdir platforms >> deploy_qt.bat
echo copy "%%QT_PATH%%\plugins\platforms\qwindows.dll" platforms\ >> deploy_qt.bat
echo. >> deploy_qt.bat
echo echo Qt DLLs copied successfully! >> deploy_qt.bat
echo echo Run: PhoneBook.exe >> deploy_qt.bat
echo pause >> deploy_qt.bat

echo Created deploy_qt.bat
echo.
dir PhoneBook.exe
echo.
pause