cmake_minimum_required(VERSION 3.12)

project(Arduino VERSION 1.0.0)

if (NOT TARGET Arduino)
	add_subdirectory(dep/Serial)

	set(Headers
        inc/Henry/Arduino.hpp
    )
    set(Sources
        src/core/Arduino.cpp
    )

    if (WIN32)
        set(Sources ${Sources}
            src/plat/WindowsArduino.cpp
        )
    elseif (APPLE)
        set(Sources ${Sources})
    elseif (UNIX)
        set(Sources ${Sources}
            src/plat/LinuxArduino.cpp
        )
    endif()

    add_library(Arduino ${Sources} ${Headers})

    target_link_libraries(Arduino PUBLIC Serial)
    target_compile_features(Arduino PUBLIC cxx_std_17)
    target_include_directories(Arduino PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)
endif()
