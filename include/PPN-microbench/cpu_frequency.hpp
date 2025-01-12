#ifndef PPN_MICROBENCH_CPU_FREQUENCY
#define PPN_MICROBENCH_CPU_FREQUENCY

#include <PPN-microbench/microbench.hpp>
#include <PPN-microbench/context.hpp>

#include <chrono>
#include <thread>
#include <sched.h>

class CPUFrequency : public Microbench {
    private:
        int nbTestingCores = 0;
        int nbMeasures = 0;
        int nbCores;
        std::unique_ptr<double[]> measures;
        void executeBench();
        
    public:
        CPUFrequency(int nbMeasures);
        ~CPUFrequency();

        void run() override;
        json getJson() override;
};

#endif