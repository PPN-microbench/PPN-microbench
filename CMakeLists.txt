cmake_minimum_required(VERSION 3.25)
project(PPN-microbench)

include(FetchContent)

FetchContent_Declare(
    json 
    URL 
    https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(json)

add_subdirectory(src/PPN-microbench)
add_subdirectory(main)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "")
    target_compile_options(PPN-microbench PRIVATE -Wall -Wextra -g3 -O3 -march=native)
else()
    target_compile_options(PPN-microbench PRIVATE -march=native)
endif()

target_compile_features(PPN-microbench PRIVATE cxx_std_20)