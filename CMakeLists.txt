cmake_minimum_required(VERSION 3.0.0)
project(arduino VERSION 0.1.0)

set(arduino_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(serial_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/dep/serial/include")
set(debug_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/dep/serial/dep/debug/include")

add_subdirectory(dep/serial)

add_subdirectory(src)
add_subdirectory(tests)
