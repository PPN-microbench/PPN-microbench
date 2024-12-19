#ifndef CPU_FREQUENCY
#define CPU_FREQUENCY

#include <PPN-microbench/microbench.hpp>
#include <PPN-microbench/rdtsc.hpp>
#include <PPN-microbench/context.hpp>
#include <chrono>
#include <thread>
#include <threads.h>

class CPUFrequency : public Microbench {
    private:
        int nbTestingCores = 0;
        int nbMeasures = 0;
        int nbCores;
        std::unique_ptr<double[]> measures;
        
    public:
        CPUFrequency(int nbMeasures);
        ~CPUFrequency();

        void run() override;
        json getJson() override;
};

#endif