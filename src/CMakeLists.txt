if (WIN32)
set(sources ${sources}
    windows_arduino.cpp
)
elseif (APPLE)
set(sources ${sources} 
    macos_arduino.cpp
)
elseif (UNIX)
set(sources ${sources} 
    linux_arduino.cpp
)
endif()

add_library(${lib_name} ${sources})

target_link_libraries(${lib_name} PUBLIC serial1.0.0 debug1.0.0)
target_include_directories(${lib_name} PRIVATE ${lib_include_dir})

target_compile_options(${lib_name} PRIVATE -Werror)
target_compile_features(${lib_name} PRIVATE cxx_std_17)

install(TARGETS ${lib_name} DESTINATION lib)
install(DIRECTORY ${lib_include_dir}/HENRY DESTINATION include)
