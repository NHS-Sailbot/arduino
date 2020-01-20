cmake_minimum_required(VERSION 3.12)

if (not TARGET Arduino)
    project(Arduino VERSION 1.0.0)
    add_subdirectory(dep/Serial)
    add_subdirectory(src)
endif()
