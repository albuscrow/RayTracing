cmake_minimum_required(VERSION 3.1)
project(ray_tracing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SRC_FILES *.cpp)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

set(SOURCE_FILES ${SRC_FILES})
add_executable(ray_tracing ${SOURCE_FILES} MainWidget.h)
target_link_libraries(ray_tracing Qt5::Widgets)
