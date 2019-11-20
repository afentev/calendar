cmake_minimum_required(VERSION 3.1.0)
project(untitled)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(HW_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include_directories(${HW_HEADER_DIR})

add_executable(file.cpp
        main.cpp
        inc/ClickableLabel.h
        )

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
#find_package(SFML REQUIRED)

add_executable(untitled main.cpp file.cpp inc/ClickableLabel.h ClickableLabel.cpp)

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
