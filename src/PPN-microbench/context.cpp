#include <PPN-microbench/context.hpp>

using std::cout, std::endl;

Context::Context() {
    cpuInfo();
    memoryInfo();
}

Context::~Context() {}

Context &Context::getInstance() {
    static Context instance;
    return instance;
}

// finds and returns the first int in a string, returns 0 if none.
i64 Context::getFirstInt(std::string input) {
    std::regex pattern("(\\d+)");
    std::smatch match;
    std::regex_search(input, match, pattern);

    for (auto m : match) {
        return stoi(m.str());
    }

    return 0;
}

void Context::cpuInfo() {

//////////////////////
// CPU Architecture //
//////////////////////

// ARM
#if defined(__arm__) || defined(__aarch64__)
    cpuArchi = "ARM";
#endif

// x86
#if defined(__i386__) || defined(__x86_64__)
    cpuArchi = "x86";
#endif

    ///////////////
    // Word Size //
    ///////////////

    // I don't know how cursed that is
    wordSize = sizeof(size_t) * 8;

//////////
// SIMD // x86 only for now SORRY
//////////

// bro..
#if defined(__i386__) || defined(__x86_64__)
    if (__builtin_cpu_supports("sse")) {
        simd.emplace("SSE");
    }
    if (__builtin_cpu_supports("sse2")) {
        simd.emplace("SSE2");
    }
    if (__builtin_cpu_supports("sse3")) {
        simd.emplace("SSE3");
    }
    if (__builtin_cpu_supports("sse4.2")) {
        simd.emplace("SSE4.2");
    }
    if (__builtin_cpu_supports("avx")) {
        simd.emplace("AVX");
    }
    if (__builtin_cpu_supports("avx2")) {
        simd.emplace("AVX2");
    }
    if (__builtin_cpu_supports("avx512f")) {
        simd.emplace("AVX512");
    }
#endif

    //////////
    // CPUs //
    //////////

    std::ifstream f("/proc/cpuinfo");
    std::string line;

    size_t currProc = 0;
    std::set<size_t> mappedCores;

    while (std::getline(f, line)) {
        if (line.find("processor") != std::string::npos) {
            currProc = getFirstInt(line);
            threads += 1;
        }

        if (line.find("core id") != std::string::npos) {
            size_t proc = getFirstInt(line);

            // only add the current core to the mapping if it hasn't appeared
            // before.
            if (mappedCores.find(proc) == mappedCores.end()) {
                threadMapping.push_back(currProc);
                mappedCores.emplace(proc);
            }
        }

        if (line.find("physical id") != std::string::npos) {
            size_t sockCout = getFirstInt(line) + 1;
            if (sockets != sockCout) {
                // new socket, we need to clear the mapping set
                mappedCores.clear();
                sockets = sockCout;
            }
        }
    }

    // ARM /proc/cpuinfo has way less information about cpu cores
    // we have to manuall set the mapping
    if (cpuArchi == "ARM") {
        cpus = threads;
        for (size_t i = 0; i < cpus; i++) {
            threadMapping.push_back(i);
        }
    }

    cpus = threadMapping.size();
    f.close();
}

void Context::memoryInfo() {

    /////////
    // RAM //
    /////////

    std::ifstream f("/proc/meminfo");
    std::string line;

    while (std::getline(f, line)) {
        // memory in /proc/meminfo is in kB
        if (line.find("MemTotal:") != std::string::npos)
            memory = getFirstInt(line) * 1000;
    }

    f.close();

    ///////////
    // CACHE //
    ///////////

    l1d = sysconf(_SC_LEVEL1_DCACHE_SIZE);
    l1i = sysconf(_SC_LEVEL1_ICACHE_SIZE);
    l2 = sysconf(_SC_LEVEL2_CACHE_SIZE);
    l3 = sysconf(_SC_LEVEL3_CACHE_SIZE);
}

json Context::getJson() {
    json obj;

    json cpu_info;
    cpu_info["architechture"] = cpuArchi;
    cpu_info["word_size"] = wordSize;
    cpu_info["sockets"] = sockets;
    cpu_info["cpus"] = cpus;
    cpu_info["threads"] = threads;
    cpu_info["mapping"] = threadMapping;
    cpu_info["simd"] = json(simd);

    json mem_info;
    mem_info["total_mem"] = memory;
    mem_info["l1d"] = l1d;
    mem_info["l1i"] = l1i;
    mem_info["l2"] = l2;
    mem_info["l3"] = l3;

    obj["cpu_info"] = cpu_info;
    obj["mem_info"] = mem_info;

    return obj;
}

size_t Context::getCpus() { 
    return this->cpus; 
}

std::vector<size_t> Context::getThreadMapping() {
    return this->threadMapping;
}