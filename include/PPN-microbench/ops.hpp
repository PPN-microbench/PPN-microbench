#ifndef _OPS_HPP
#define _OPS_HPP

#include <PPN-microbench/microbench.hpp>

#include <chrono>
#include <thread>
// needed to set thread affinty
#include <threads.h>
// clock
#include <time.h>

class Ops : public Microbench {
  private:
    size_t const N_OPS = 1024 * 1024 * 16;
    size_t cpus;

    size_t **results;

    template <class T> void bench(T);
    template <class T> void benchSIMD(T);
    void executeBench() {};

  public:
    Ops(int reps);
    ~Ops();

    void run();
    json getJson();
};

#endif