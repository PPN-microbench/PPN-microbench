#include <PPN-microbench/memory.hpp>

// Function to generate a random index within a given threshold
inline u64 random_index(u64 threshold) { return rand() % threshold; }

// Function to measure cache/memory latency using pointer chasing
double measure_latency(u64 size, double nbIterations) {
    std::vector<void**> memblock(size);

    // Initialize the memory block with pointers to itself
    for (u64 i = 0; i < size; ++i) {
        if (i + 1 < size) {
            memblock[i] = (void**)&memblock[i + 1];
        } else {
            memblock[i] = (void**)&memblock[0];
        }
    }

    // Shuffle the pointers to create a random access pattern
    for (u64 i = size - 1; i > 0; --i) {
        u64 j = random_index(i + 1);
        std::swap(memblock[i], memblock[j]);
    }

    // Warmup run
    void *p = memblock[0];
    for (u64 i = 0; i < nbIterations; i+=16) {
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
        p = *(void **)p;
    }

    double total_latency = 0.0;

    // Perform the latency measurement 11 times for better accuracy
    for (int run = 0; run < 11; ++run) {
        p = memblock[0];

        auto start = std::chrono::high_resolution_clock::now();
        // Pointer chasing loop
        for (u64 i = 0; i < nbIterations; i+=16) {
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
            p = *(void **)p;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;

        // Accumulate the latency
        total_latency += (double)elapsed.count() / (double)nbIterations;
    }

    // Calculate the average latency over 11 runs
    return (double)total_latency / 11.0;
}

// Constructor
Memory::Memory() : Microbench("Memory", 1000000) { srand(time(nullptr)); }

// Destructor
Memory::~Memory() {}



// Execute the benchmark
void Memory::run() {
    // Define the sizes to test (in B)
    size_t size = 512;
    for (size_t i = 0; i < mem_sizes.size(); i++) { 
        mem_sizes[i] = size *= 1.04;
    }

    mem_times.resize(mem_sizes.size());
    std::cout << "Memory benchmark start \n";
    for (size_t i = 0; i < mem_sizes.size(); ++i) {
        u64 size_B = mem_sizes[i] ;

        double latency = measure_latency(size_B, getNbIterations());

        if (latency == 0.0) {
            std::cerr << "Warning: Measured latency is 0.0 for size " << size_B << std::endl;
        }
        
        mem_times[i] = latency;
    }
    std::cout << "Memory benchmark end \n";
}
// Get the results in JSON format
json Memory::getJson() {
    json result1, result2, result;
    for (size_t i = 0; i < mem_sizes.size(); ++i) {
        result1["Memory_Size"].push_back(mem_sizes[i]*sizeof(void *));
        result2["Latency"].push_back((double)mem_times[i]);
        result["Results"] = {result1, result2};
    }
    return result;
}

