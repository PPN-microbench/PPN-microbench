#ifndef PPN_MICROBENCH_MEMORY
#define PPN_MICROBENCH_MEMOERY

// Include necessary headers
#include <PPN-microbench/microbench.hpp>
#include <PPN-microbench/constants.hpp>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <numeric>

// Memory class inherits from Microbench
class Memory : public Microbench {
  private:
    // Vectors to store memory sizes and times
    std::vector<u64> mem_sizes = std::vector<u64>(300);
    std::vector<double> mem_times;

    

  public:
    // Constructor
    Memory();

    // Destructor
    ~Memory();

    // Function to run the benchmark, overrides the base class method
    void run() override;

    // Function to get the results in JSON format, overrides the base class
    // method
    json getJson() override;
};

#endif