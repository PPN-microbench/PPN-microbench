cmake_minimum_required(VERSION 3.25)
project(PPN-microbench)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    json 
    URL 
    https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

# FetchContent_Declare(
#     kokkos
#     URL
#     https://github.com/kokkos/kokkos/releases/download/4.5.00/kokkos-4.5.00.tar.gz
# )

FetchContent_MakeAvailable(json)
# FetchContent_MakeAvailable(kokkos)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -g -O3 -march=native -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -fopenmp")

add_subdirectory(src/PPN-microbench)
add_subdirectory(main)