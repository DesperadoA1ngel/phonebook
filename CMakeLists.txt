cmake_minimum_required(VERSION 3.16)
project(PhoneBook VERSION 1.0.0 LANGUAGES CXX)

# Кроссплатформенные настройки
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Настройки для разных платформ
if(WIN32)
    message(STATUS "Building for Windows")
    
    # Установите правильный путь к Qt (ваш путь)
    set(QT_PATH "C:/Qt5/5.15.2/mingw81_32")
    if(NOT EXISTS "${QT_PATH}")
        # Попробуйте другие возможные пути
        set(QT_PATH "C:/Qt/5.15.2/mingw81_32")
    endif()
    
    if(EXISTS "${QT_PATH}")
        set(CMAKE_PREFIX_PATH "${QT_PATH}/lib/cmake" CACHE PATH "Qt path")
        message(STATUS "Using Qt from: ${QT_PATH}")
    else()
        message(WARNING "Qt not found in default locations")
        message(STATUS "Please set CMAKE_PREFIX_PATH manually to your Qt installation")
    endif()
    
    # Для MinGW добавляем консоль
    add_compile_options(-mconsole)
    
else()
    message(STATUS "Building for Linux/Unix")
endif()

# Ищем Qt5
find_package(Qt5 5.12 REQUIRED COMPONENTS Widgets Core)

# Сообщаем о найденной версии Qt
message(STATUS "Found Qt5 version: ${Qt5Widgets_VERSION}")

# Исходные файлы (все в src директории)
set(SRC_DIR src)
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/mainwindow.cpp
    ${SRC_DIR}/phonebook.cpp
    ${SRC_DIR}/contact.cpp
)

# Проверяем наличие searchdialog
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/searchdialog.cpp")
    list(APPEND SOURCES ${SRC_DIR}/searchdialog.cpp)
    message(STATUS "Found searchdialog.cpp, adding to build")
endif()

# Заголовочные файлы
set(HEADERS
    ${SRC_DIR}/mainwindow.h
    ${SRC_DIR}/phonebook.h
    ${SRC_DIR}/contact.h
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}/searchdialog.h")
    list(APPEND HEADERS ${SRC_DIR}/searchdialog.h)
endif()

# Добавляем исполняемый файл
add_executable(PhoneBook ${SOURCES} ${HEADERS})

# Подключаем Qt библиотеки
target_link_libraries(PhoneBook PRIVATE
    Qt5::Widgets 
    Qt5::Core
)

# Включаем каталог с заголовками
target_include_directories(PhoneBook PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_DIR}
)

# Настройки для Windows
if(WIN32)
    # Добавляем макрос для Windows
    target_compile_definitions(PhoneBook PRIVATE 
        WIN32 
        _WINDOWS
    )
    
    # Создаем пост-билд команду для уведомления о необходимости копирования DLL
    add_custom_command(TARGET PhoneBook POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "========================================="
        COMMAND ${CMAKE_COMMAND} -E echo "Build complete! PhoneBook.exe created."
        COMMAND ${CMAKE_COMMAND} -E echo ""
        COMMAND ${CMAKE_COMMAND} -E echo "To run the application:"
        COMMAND ${CMAKE_COMMAND} -E echo "1. Copy Qt DLLs to this folder:"
        COMMAND ${CMAKE_COMMAND} -E echo "   - Qt5Core.dll"
        COMMAND ${CMAKE_COMMAND} -E echo "   - Qt5Widgets.dll"
        COMMAND ${CMAKE_COMMAND} -E echo "   - Qt5Gui.dll"
        COMMAND ${CMAKE_COMMAND} -E echo "2. Create 'platforms' folder and copy:"
        COMMAND ${CMAKE_COMMAND} -E echo "   - platforms/qwindows.dll"
        COMMAND ${CMAKE_COMMAND} -E echo "3. Run PhoneBook.exe"
        COMMAND ${CMAKE_COMMAND} -E echo "========================================="
    )
endif()

# Создаем простой файл version.h
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/version.h"
    "#ifndef VERSION_H\n"
    "#define VERSION_H\n\n"
    "#define PROJECT_NAME \"PhoneBook\"\n"
    "#define PROJECT_VERSION \"1.0.0\"\n"
    "#define PROJECT_DESCRIPTION \"Phone Book Application\"\n"
    "#define COMPANY_NAME \"MyCompany\"\n\n"
    "#endif // VERSION_H\n"
)

message(STATUS "Build configuration complete")