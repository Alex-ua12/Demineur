cmake_minimum_required(VERSION 3.18)

project(demineur)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
    main.cc
    mainwindow.hh
    mainwindow.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)