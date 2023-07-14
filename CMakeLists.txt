cmake_minimum_required(VERSION 3.18.0)

project(Polyver VERSION 0.1.0 LANGUAGES C CXX)
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
configure_file(version.hpp.in version.hpp)

target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} fmt::fmt)

add_subdirectory(src)
