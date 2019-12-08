include(CTest)

link_libraries(${PROJECT_NAME})
include_directories(${template_include_dir})

add_executable(arduino_master_test master_test.cpp)
add_test(arduino_master_test arduino_master_test)
