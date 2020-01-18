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

add_library(${LibName} ${Sources} ${Headers})
target_compile_features(${LibName} PRIVATE cxx_std_17)
target_link_libraries(${LibName} PRIVATE Serial Debug)

install(TARGETS ${LibName} DESTINATION lib)
install(FILES ${Headers} DESTINATION include/Henry)
