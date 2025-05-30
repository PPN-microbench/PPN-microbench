#ifndef PPN_MICROBENCH_MICROBENCH
#define PPN_MICROBENCH_MICROBENCH

#include <PPN-microbench/context.hpp>
#include <PPN-microbench/asm_functions.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <threads.h>

#include <vector>

using json = nlohmann::ordered_json;

class Microbench {
  protected:
    std::string name;
    uint64_t nbIterations;

    // easier OpenCL managment, those are gathered from context.
    cl::Device mainDevice;
    cl::Context mainDeviceContext;
    cl::CommandQueue mainDeviceQueue;

  public:
    Context context = Context::getInstance();

    Microbench(std::string name, uint64_t nbIterations);
    virtual ~Microbench();

    std::string getName();
    uint64_t getNbIterations();

    virtual void run() = 0;
    virtual json getJson() = 0;
};

#endif