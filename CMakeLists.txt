cmake_minimum_required(VERSION 3.5)

project(IntervalMap LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)

add_executable(IntervalMap
  test/main.cpp
)
target_link_libraries(IntervalMap)
