cmake_minimum_required(VERSION 3.5)

project(graphite-sheets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD C++17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(graphite-sheets
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  spreadsheet.cpp
  spreadsheet.h
  exprparser.cpp
  exprparser.h
)

target_link_libraries(graphite-sheets PRIVATE Qt5::Widgets)
