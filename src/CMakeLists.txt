set(Headers
    Arduino.hpp
)
set(Sources
    Arduino.cpp
)

if (WIN32)
    set(Sources ${Sources})
elseif (APPLE)
    set(Sources ${Sources})
elseif (UNIX)
    set(Sources ${Sources}
        LinuxArduino.cpp
    )
endif()

add_library(Arduino ${Sources} ${Headers})
target_link_libraries(Arduino PUBLIC Serial)
target_compile_features(Arduino PUBLIC cxx_std_17)
target_include_directories(Arduino PUBLIC ${CMAKE_CURRENT_LIST_DIR})
