set(sources core/add.cpp)

include_directories(${template_include_dir})

if (WIN32)
    set(sources ${sources} plat/windows/windows_arduino.cpp)
elseif (APPLE)
    set(sources ${sources} plat/macos/macos_arduino.cpp)
elseif (UNIX)
    set(sources ${sources} plat/linux/linux_arduino.cpp)
endif()

add_library(${PROJECT_NAME} ${sources})
