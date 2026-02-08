#include <PPN-microbench/core_to_core_latency.hpp>

using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::steady_clock;

constexpr int preheat = 50;

CoreToCoreLatency::CoreToCoreLatency(int nbMeasures) : Microbench("Core To Core Latency", 150) {
    this->nbMeasures = nbMeasures;
    nbCores = context.getCpus();
    resultsMin.reserve(nbCores * nbCores);
    resultsMean.reserve(nbCores * nbCores);
}

CoreToCoreLatency::~CoreToCoreLatency() {}

void CoreToCoreLatency::pinThread(int core) {
    // To call a thread
    Context context = Context::getInstance();
    std::vector<size_t> threadMapping = context.getThreadMapping();
    cpu_set_t cpusets;
    CPU_ZERO(&cpusets);
    CPU_SET(threadMapping[core], &cpusets);
    if (sched_setaffinity(0, sizeof(cpusets), &cpusets) == -1) {
        perror("sched_setaffinity");
        exit(1);
    }
}

void CoreToCoreLatency::run() {
    // Set all threads to 0
    cpu_set_t cpusets;
    CPU_ZERO(&cpusets);
    if (sched_getaffinity(0, sizeof(cpusets), &cpusets) == -1) {
        perror("sched_getaffinity");
        exit(1);
    }

    // Enumerate available cores
    std::vector<int> cpus;
    cpus.reserve(nbCores);
    for (int i = 0; i < nbCores; ++i) {
        if (CPU_ISSET(i, &cpusets)) {
            cpus.push_back(i);
        }
    }

    for (int id_1 = 0; id_1 < nbCores; ++id_1) {
        for (int id_2 = 0; id_2 < nbCores; ++id_2) {
            if (id_1 == id_2) {
                resultsMin.push_back(0);
                resultsMean.push_back(0);
            } else {
                sumDuration = 0;
                minDuration = INT64_MAX;
                alignas(64) std::atomic<int> core1 = {-1};
                alignas(64) std::atomic<int> core2 = {-1};

                std::thread t = std::thread([&] {
                    pinThread(cpus[id_1]);

                    for (int sample = -preheat; sample < nbMeasures; sample++) {
                        for (int n = 0; n < (int)getNbIterations(); ++n) {
                            while (core1.load(std::memory_order_acquire) != n) {;}
                            core2.store(n, std::memory_order_release);
                        }
                    }
                });

                pinThread(cpus[id_2]);

                for (int sample = -preheat; sample < nbMeasures; sample++) {
                    core1, core2 = -1;
                    auto start = steady_clock::now();

                    for (int n = 0; n < (int)getNbIterations(); ++n) {
                        core1.store(n, std::memory_order_release);
                        while (core2.load(std::memory_order_acquire) != n) {;}
                    }

                    auto stop = steady_clock::now();

                    uint64_t duration = duration_cast<nanoseconds>(stop - start).count();
                    
                    // To preheat before true bench
                    if (sample >= 0) {
                        sumDuration += duration;
                        minDuration = std::min(minDuration, duration);
                    }
                }
                t.join();
                resultsMin.push_back(minDuration / getNbIterations() / 2);
                resultsMean.push_back(sumDuration / nbMeasures / getNbIterations() / 2);
            }
            spdlog::debug("{}: run {} / {}", name, id_1 * nbCores + (id_2 + 1), nbCores * nbCores);
        }
    }
}

json CoreToCoreLatency::getJson() { 
    json coreToCoreJson = json::object();
    coreToCoreJson["name"] = getName();
    for (int line = 0; line < nbCores; line++) {
        for (int col = 0; col < nbCores; col++) {
            coreToCoreJson["results"]["min"][line] += resultsMin[line * nbCores + col];
            coreToCoreJson["results"]["mean"][line] += resultsMean[line * nbCores + col];
        }
    }
    return coreToCoreJson;
}
