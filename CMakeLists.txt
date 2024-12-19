cmake_minimum_required(VERSION 3.25)
project(PPN-microbench)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
    json 
    URL 
    https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(json)

add_subdirectory(src/PPN-microbench)
add_subdirectory(main)