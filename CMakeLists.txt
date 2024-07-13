cmake_minimum_required(VERSION 3.28)
project(ForestFireCPP)

set(CMAKE_CXX_STANDARD 17)

# Add the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "C:/Users/0piet/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_executable(ForestFireCPP main.cpp)

target_link_libraries(ForestFireCPP PRIVATE sfml-system sfml-window sfml-graphics)