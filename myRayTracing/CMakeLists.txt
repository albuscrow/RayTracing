# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)


##### set for qt
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

##### find all source file ####
file(GLOB_RECURSE CPP_FILES *.cpp)
file(GLOB_RECURSE HEAD_FILES *.h)
list(APPEND SRC_FILES ${CPP_FILES} ${HEAD_FILES})

add_executable(ray_tracing ${SRC_FILES})
target_link_libraries(ray_tracing Qt5::Widgets)
