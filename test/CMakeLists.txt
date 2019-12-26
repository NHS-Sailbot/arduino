add_executable(arduino_master_test master_test.cpp)
add_test(arduino_master_test arduino_master_test)

target_link_libraries(arduino_master_test PRIVATE ${lib_name})
target_include_directories(arduino_master_test PRIVATE ${lib_include_dir})

target_compile_options(arduino_master_test PRIVATE -Werror)
target_compile_features(arduino_master_test PRIVATE cxx_std_17)
