cmake_minimum_required(VERSION 3.10)

project(Manjusaka)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Manjusaka main.cpp)


set_target_properties(Manjusaka PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/summary)

target_link_libraries(Manjusaka PRIVATE pthread)
