cmake_minimum_required(VERSION 3.10)

project(Manjusaka)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Manjusaka Manjusaka.cpp)



target_link_libraries(Manjusaka PRIVATE pthread)
