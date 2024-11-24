add_executable(main)
target_sources(main PRIVATE main.cpp)
target_link_libraries(main PPN-microbench)

add_executable(conductor_main)
target_sources(conductor_main PRIVATE conductor_main.cpp)
target_link_libraries(conductor_main PPN-microbench)
