link_libraries(serial)
include_directories(${arduino_include_dir})
include_directories(${serial_include_dir})
include_directories(${debug_include_dir})

set(sources ${sources} core/arduino.cpp)

if (WIN32)
    set(sources ${sources} plat/windows/windows_arduino.cpp)
elseif (APPLE)
    set(sources ${sources} plat/macos/macos_arduino.cpp)
elseif (UNIX)
    set(sources ${sources} plat/linux/linux_arduino.cpp)
endif()

add_library(${PROJECT_NAME} ${sources})
