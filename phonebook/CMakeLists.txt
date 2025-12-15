cmake_minimum_required(VERSION 3.16)
project(PhoneBook VERSION 1.0.0)

# Для Windows с MinGW
if(WIN32)
    # Устанавливаем путь к Qt MinGW
    set(CMAKE_PREFIX_PATH "C:/Qt5/5.15.2/mingw81_32/lib/cmake")
    
    # Указываем использовать MinGW Makefiles
    if(NOT CMAKE_GENERATOR)
        set(CMAKE_GENERATOR "MinGW Makefiles" CACHE STRING "Generator" FORCE)
    endif()
    
    # Добавляем путь к MinGW в переменные окружения (если нужно)
    if(NOT DEFINED ENV{MINGW_PATH})
        set(ENV{PATH} "$ENV{PATH};C:/Qt5/Tools/mingw810_32/bin")
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Находим Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

# Сообщаем о найденной версии Qt
message(STATUS "Found Qt5 version: ${Qt5Widgets_VERSION}")
message(STATUS "Qt5 libraries: ${Qt5Widgets_LIBRARIES}")
message(STATUS "Qt5 include dirs: ${Qt5Widgets_INCLUDE_DIRS}")

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/phonebook.cpp
    src/contact.cpp
    src/searchdialog.cpp
)

# Заголовочные файлы
set(HEADERS
    src/mainwindow.h
    src/phonebook.h
    src/contact.h
    src/searchdialog.h
)

# Добавляем исполняемый файл
add_executable(PhoneBook ${SOURCES} ${HEADERS})

# Подключаем Qt библиотеки
target_link_libraries(PhoneBook 
    Qt5::Widgets 
    Qt5::Core
)

# Для Windows добавляем флаги компоновщика
if(WIN32)
    # Добавляем флаг для отображения консоли (для отладки)
    target_link_options(PhoneBook PRIVATE -mconsole)
    
    # Копируем необходимые DLL файлы после сборки
    add_custom_command(TARGET PhoneBook POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:Qt5::Widgets>"
            "$<TARGET_FILE_DIR:PhoneBook>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:Qt5::Core>"
            "$<TARGET_FILE_DIR:PhoneBook>"
        COMMENT "Copying Qt DLLs to output directory"
    )
endif()

# Установка
install(TARGETS PhoneBook RUNTIME DESTINATION bin)

# Включаем каталоги с заголовками
target_include_directories(PhoneBook PRIVATE src)

# Дополнительные настройки для отладки/релиза
set_target_properties(PhoneBook PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE FALSE
)

# Опционально: добавляем ресурсы если они есть
# qt5_add_resources(RESOURCES resources.qrc)
# target_sources(PhoneBook PRIVATE ${RESOURCES})