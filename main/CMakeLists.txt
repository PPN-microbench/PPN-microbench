add_executable(main)
target_sources(main PRIVATE main.cpp)
target_link_libraries(main PPN-microbench nlohmann_json::nlohmann_json spdlog::spdlog CLI11::CLI11 Eigen3::Eigen OpenMP::OpenMP_CXX OpenCL::OpenCL OpenCL::HeadersCpp)
