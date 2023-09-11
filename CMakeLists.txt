cmake_minimum_required(VERSION 3.18.0)

project(Polyver VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
configure_file(version.hpp.in version.hpp)


find_package(fmt REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} fmt::fmt)

find_package(spdlog REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} spdlog::spdlog)

add_subdirectory(src)
