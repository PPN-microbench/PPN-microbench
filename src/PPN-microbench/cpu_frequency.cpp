#include <PPN-microbench/cpu_frequency.hpp>

#include <sched.h>

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;

CPUFrequency::CPUFrequency(int nbMeasures) : Microbench("CPU Frequency", 999999){
    this->nbMeasures = nbMeasures;
    Context context = Context::getInstance();
    nbCores = context.getCpus();
    measures = std::make_unique<double[]>(nbMeasures * ((nbCores * (nbCores + 1)) / 2));
}

CPUFrequency::~CPUFrequency() {}

json CPUFrequency::getJson() {
    json cpuSpeedJson = json::object();
    cpuSpeedJson["name"] = getName();
    for (int id = 1; id <= nbTestingCores; id++) {
        for (int i = 0; i < nbMeasures * id; i++) {
            cpuSpeedJson["results"]["Cores" + std::to_string(id)][i/nbMeasures] += measures[id * nbCores + i];
        }
    }
    return cpuSpeedJson;
}

void CPUFrequency::run() {
    Context context = Context::getInstance();
    std::vector<size_t> threadMapping = context.getThreadMapping();

    cpu_set_t cpusets[nbCores];

    for (int i = 0; i < nbCores; i++) {
        CPU_ZERO(&cpusets[i]);
        CPU_SET(threadMapping[i], &cpusets[i]);
    }

    std::thread threads[nbCores];

    // To stop earlier if it's needed (but protection if maxCores is bigger than the cores count)
    nbTestingCores = threadMapping.size();

    for (int coresToExecute = 1; coresToExecute <= nbTestingCores; coresToExecute++) { // Main for, equivalent to a graph
        for (int coresExecuted = 1; coresExecuted <= coresToExecute; coresExecuted++) { // For every core count, equivalent to a point in a graph
            for (int sample = -10; sample < nbMeasures; sample++) { // 5 Warmup runs and samples to average tests (in python, later)
                // Execute on 1 Core, then 2 Cores, 3 Cores, etc...
                auto start = steady_clock::now();

                for (int id = 0; id < coresExecuted; id++) {
                    // To call the threads (only 1;  1 and 2;  1, 2 and 3;  etc...)
                    threads[id] = std::thread([this] {
                        int cpt = 0;
                        for (int i = 0; i < getNbIterations(); i++) {
                            // 16 adds
                            cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++;
                            cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++; cpt++;
                        }
                    });
                    pthread_setaffinity_np(threads[id].native_handle(),
                                        sizeof(cpu_set_t), &cpusets[id]);
                }
            
                for (int id = 0; id < coresExecuted; id++) {
                    // Waiting for the threads
                    threads[id].join();
                }

                u64 duration = duration_cast<nanoseconds>(steady_clock::now() - start).count();
                if (sample >= 0) {
                    measures[(coresToExecute * (coresToExecute - 1) / 2) * nbMeasures + (coresExecuted - 1) * nbMeasures + sample] = ((16.f * getNbIterations()) / duration);
                }
            }
            std::cout << "\r# " << name << ": run " << ((coresToExecute * (coresToExecute - 1)) / 2) + coresExecuted << "/" << ((nbCores * (nbCores + 1)) / 2) << std::flush;
        }
    }
    std::cout << std::endl;
}
