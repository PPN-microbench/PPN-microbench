add_library(PPN-microbench STATIC)

target_sources(PPN-microbench PRIVATE microbench.cpp context.cpp conductor.cpp ops.cpp)

target_sources(
    PPN-microbench
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
        FILES ${PROJECT_SOURCE_DIR}/include/PPN-microbench/microbench.hpp
        FILES ${PROJECT_SOURCE_DIR}/include/PPN-microbench/context.hpp
        FILES ${PROJECT_SOURCE_DIR}/include/PPN-microbench/conductor.hpp
        FILES ${PROJECT_SOURCE_DIR}/include/PPN-microbench/ops.hpp
)

set_target_properties(PPN-microbench PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)
# target_link_libraries(PPN-microbench PRIVATE nlohmann_json::nlohmann_json Kokkos::kokkos)
target_link_libraries(PPN-microbench PRIVATE nlohmann_json::nlohmann_json)

target_compile_options(PPN-microbench PRIVATE "$<$<CONFIG:DEBUG>:SHELL:${DEBUG_FLAGS}>")
target_compile_options(PPN-microbench PRIVATE "$<$<CONFIG:RELEASE>:SHELL:${RELEASE_FLAGS}>")
