#ifndef PPN_MICROBENCH_OPS_HPP
#define PPN_MICROBENCH_OPS_HPP

#include <PPN-microbench/asm_functions.hpp>
#include <PPN-microbench/microbench.hpp>

#include <chrono>
#include <thread>

class Ops : public Microbench {
  private:
    int static const WARMUP_RUNS = 2;
    int static const RUNS = 10;

    int64_t n_ops;
    size_t cpus;
    cpu_set_t *cpusets;

    size_t results[6][RUNS];

    size_t wrap(void (*f)(i64));
    void executeBench() {};

  public:
    Ops(int reps);
    ~Ops();

    void run();
    json getJson();
};

#endif