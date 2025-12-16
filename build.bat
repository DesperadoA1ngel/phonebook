@echo off
echo PhoneBook Build Script
echo =====================
echo.

set QT_PATH=C:\Qt5\5.15.2\mingw81_32
if not exist "%QT_PATH%" (
    echo ERROR: Qt not found at %QT_PATH%
    pause
    exit /b 1
)

echo Using Qt from: %QT_PATH%
echo.

if not exist build mkdir build
cd build

echo Step 1: Configuring with CMake...
echo =================================
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT_PATH%/lib/cmake"

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo Step 2: Building the project (without windeployqt)...
echo ====================================================
cmake --build . --target PhoneBook

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build successful! 
echo.
echo Created: %CD%\PhoneBook.exe
echo.
echo To deploy Qt DLLs, run from this folder:
echo   deploy_qt.bat
echo ========================================
echo.

rem Создаем скрипт для развертывания
echo @echo off > deploy_qt.bat
echo echo Deploying Qt libraries... >> deploy_qt.bat
echo set QT_PATH=%QT_PATH% >> deploy_qt.bat
echo. >> deploy_qt.bat
echo copy "%%QT_PATH%%\bin\Qt5Core.dll" . >> deploy_qt.bat
echo copy "%%QT_PATH%%\bin\Qt5Widgets.dll" . >> deploy_qt.bat
echo copy "%%QT_PATH%%\bin\Qt5Gui.dll" . >> deploy_qt.bat
echo. >> deploy_qt.bat
echo rem MinGW runtime DLLs >> deploy_qt.bat
echo if exist "%%QT_PATH%%\bin\libgcc_s_dw2-1.dll" copy "%%QT_PATH%%\bin\libgcc_s_dw2-1.dll" . >> deploy_qt.bat
echo if exist "%%QT_PATH%%\bin\libstdc++-6.dll" copy "%%QT_PATH%%\bin\libstdc++-6.dll" . >> deploy_qt.bat
echo if exist "%%QT_PATH%%\bin\libwinpthread-1.dll" copy "%%QT_PATH%%\bin\libwinpthread-1.dll" . >> deploy_qt.bat
echo. >> deploy_qt.bat
echo rem Create plugins directory >> deploy_qt.bat
echo if not exist platforms mkdir platforms >> deploy_qt.bat
echo copy "%%QT_PATH%%\plugins\platforms\qwindows.dll" platforms\ >> deploy_qt.bat
echo. >> deploy_qt.bat
echo rem Create imageformats directory >> deploy_qt.bat
echo if not exist imageformats mkdir imageformats >> deploy_qt.bat
echo copy "%%QT_PATH%%\plugins\imageformats\qjpeg.dll" imageformats\ >> deploy_qt.bat
echo. >> deploy_qt.bat
echo echo. >> deploy_qt.bat
echo echo Qt deployment complete! >> deploy_qt.bat
echo echo Run: PhoneBook.exe >> deploy_qt.bat
echo pause >> deploy_qt.bat

echo Created deploy_qt.bat
echo.

if exist "PhoneBook.exe" (
    echo PhoneBook.exe found! Size: %%~zPhoneBook.exe bytes
    echo.
    echo Quick test (will fail without Qt DLLs):
    echo PhoneBook.exe
    echo.
    PhoneBook.exe 2>&1 | findstr /C:"missing" /C:"error" /C:"not found" && (
        echo.
        echo As expected - need Qt DLLs. Run deploy_qt.bat first.
    ) || (
        echo.
        echo Program started successfully!
    )
)

echo.
pause