cmake_minimum_required(VERSION 3.25)
project(AVLTree)

set(CMAKE_CXX_STANDARD 14)

add_library(AVLTree INTERFACE)
target_include_directories(AVLTree INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_executable(Example example/Example.cpp)
target_link_libraries(Example PRIVATE AVLTree)
