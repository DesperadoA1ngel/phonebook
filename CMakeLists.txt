cmake_minimum_required(VERSION 3.16)
project(PhoneBook VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/phonebook.cpp
    src/contact.cpp
    src/searchdialog.cpp
)

set(HEADERS
    src/mainwindow.h
    src/phonebook.h
    src/contact.h
    src/searchdialog.h
)

add_executable(PhoneBook ${SOURCES} ${HEADERS})

target_link_libraries(PhoneBook Qt5::Widgets Qt5::Core)

install(TARGETS PhoneBook RUNTIME DESTINATION bin)