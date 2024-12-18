#include <PPN-microbench/memory.hpp>

// Function to generate a random index within a given threshold
inline u64 random_index(u64 threshold) { return rand() % threshold; }

// Function to measure cache/memory latency using pointer chasing
double measure_latency(u64 size, u64 nbIterations) {
    void **memblock = new void *[size];

    // Initialize the memory block with pointers to itself
    for (u64 i = 0; i < size; ++i) {
        memblock[i] = &memblock[i];
    }

    // Shuffle the pointers to create a random access pattern
    for (u64 i = size - 1; i > 0; --i) {
        u64 j = random_index(i + 1);
        std::swap(memblock[i], memblock[j]);
    }

    double total_latency = 0.0;

    // Perform the latency measurement 10 times for better accuracy
    for (int run = 0; run < 10; ++run) {
        void *p = memblock[0];
        auto start = std::chrono::high_resolution_clock::now();

        // Pointer chasing loop
        for (u64 i = 0; i < nbIterations / 16; ++i) {
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
        total_latency += elapsed.count() / nbIterations;
    }

    // free the allocated memory
    delete[] memblock;

    // Calculate the average latency over 10 runs
    return total_latency / 10.0;
}

// Constructor
Memory::Memory() : Microbench("Memory", 1000000) { srand(time(nullptr)); }

// Destructor
Memory::~Memory() {}

void Memory::executeBench() {} // haha

// Execute the benchmark
void Memory::run() {
    // Define the sizes to test (in B)
    // for (size_t size = 512; size < (1 << 30) / sizeof(void *);
    size_t size = 512;
    for (size_t i = 0; i < mem_sizes.size();
         i++) { //////calcul du vect finall à faire
                ///(ln2(10^8) - ln2(512)=taille vect ??)
        mem_sizes[i] = size *= 1.04;
    }

    mem_times.resize(mem_sizes.size());

    std::cout << mem_sizes.size() << std::endl;
    std::cout << mem_times.size() << std::endl;

    // void **memblock = new void *[size];
    // // Warmup runs for the cpu. Reduces performance errors.
    // for (int run = 0; run < 3; ++run) {
    //     void *p = memblock[0];

    //     // Pointer chasing loop
    //     for (u64 i = 0; i < nbIterations; ++i) {
    //         p = *(void **)p;
    //     }
    // }

    for (size_t i = 0; i < mem_sizes.size() - 1; ++i) {
        u64 size_B = mem_sizes[i];
        // u64 size = size_B / sizeof(void *);
        std::cout << size_B << std::endl;
        double latency = measure_latency(size_B, getNbIterations());
        // double latency = 0;
        mem_times[i] = latency;
    }
}

// Get the results in JSON format
json Memory::getJson() {
    json result1, result2, result;
    for (size_t i = 0; i < mem_sizes.size(); ++i) {
        result1["Memory_Size"].push_back(mem_sizes[i]);
        result2["Latency"].push_back(mem_times[i]);
        result["Results"] = {result1, result2};
    }
    return result;
}