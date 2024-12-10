#include <PPN-microbench/ops.hpp>

using std::chrono::high_resolution_clock;
using std::chrono::time_point;

Ops::Ops(int reps) : Microbench("OPS", reps) {
    cpus = context.getJson()["cpu_info"]["cpus"];
    size_t rnd = high_resolution_clock::to_time_t(high_resolution_clock::now());
    n_ops = 1024 * 1024 * 8 + (rnd % 123123);
}

Ops::~Ops() {}

// #pragma GCC push_options
// #pragma GCC optimize("O0")
template <class T> void Ops::benchhaha(T val) {
    volatile T acc = val;
// T *acc = new T[16];
#pragma omp for simd linear(&acc : 0)
    for (size_t i = 0; i < n_ops; i++) {
        // acc[0] += val;
        // acc[1] += val;
        // acc[2] += val;
        // acc[3] += val;
        // acc[4] += val;
        // acc[5] += val;
        // acc[6] += val;
        // acc[7] += val;
        // acc[8] += val;
        // acc[9] += val;
        // acc[10] += val;
        // acc[11] += val;
        // acc[12] += val;
        // acc[13] += val;
        // acc[14] += val;
        // acc[15] += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
    }
    // delete acc;
    // return &acc;
    // std::cout << acc << std::endl;
}
// #pragma GCC pop_options

template <class T> void Ops::benchSIMD(T val) {
    // T *acc = new T[16];
    volatile T acc = val;
    for (size_t i = 0; i < n_ops; i++) {
        // #pragma omp for simd linear(acc : 1)
        // for (int j = 0; j < 16; j++) {
        //     acc[j] += val;
        // }
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
        acc += val;
    }
    // delete acc;
}

void Ops::run() {
    time_point<high_resolution_clock> t1, t2;
    std::vector<size_t> mapping = context.getJson()["cpu_info"]["mapping"];
    cpu_set_t cpusets[cpus];

    for (size_t i = 0; i < cpus; i++) {
        CPU_ZERO(&cpusets[i]);
        CPU_SET(mapping[i], &cpusets[i]);
    }

    // Warmup runs
    {
        std::jthread threads[cpus];
        for (int j = 0; j < WARMUP_RUNS; j++) {
            for (size_t k = 0; k < cpus; k++) {
                threads[k] = std::jthread([this, t1] {
                    this->benchhaha((i32)t1.time_since_epoch().count());
                });
                pthread_setaffinity_np(threads[k].native_handle(),
                                       sizeof(cpu_set_t), &cpusets[k]);
            }
        }
    }

    // Actual runs
    for (int j = 0; j < RUNS; j++) {

        // i32
        t1 = high_resolution_clock::now();
        {
            std::jthread threads[cpus];
            for (size_t k = 0; k < cpus; k++) {
                threads[k] = std::jthread([this, t1] {
                    this->benchhaha((i32)t1.time_since_epoch().count());
                });
                pthread_setaffinity_np(threads[k].native_handle(),
                                       sizeof(cpu_set_t), &cpusets[k]);
            }
        }
        t2 = high_resolution_clock::now();
        results[0][j] = (t2 - t1).count();

        // i64
        t1 = high_resolution_clock::now();
        {
            std::jthread threads[cpus];
            for (size_t k = 0; k < cpus; k++) {
                threads[k] = std::jthread([this, t1] {
                    this->benchhaha((i64)t1.time_since_epoch().count());
                });
                pthread_setaffinity_np(threads[k].native_handle(),
                                       sizeof(cpu_set_t), &cpusets[k]);
            }
        }
        t2 = high_resolution_clock::now();
        results[1][j] = (t2 - t1).count();

        // f32
        t1 = high_resolution_clock::now();
        {
            std::jthread threads[cpus];
            for (size_t k = 0; k < cpus; k++) {
                threads[k] = std::jthread([this, t1] {
                    this->benchhaha((float)t1.time_since_epoch().count());
                });
                pthread_setaffinity_np(threads[k].native_handle(),
                                       sizeof(cpu_set_t), &cpusets[k]);
            }
        }
        t2 = high_resolution_clock::now();
        results[2][j] = (t2 - t1).count();

        // f64
        t1 = high_resolution_clock::now();
        {
            std::jthread threads[cpus];
            for (size_t k = 0; k < cpus; k++) {
                threads[k] = std::jthread([this, t1] {
                    this->benchhaha((double)t1.time_since_epoch().count());
                });
                pthread_setaffinity_np(threads[k].native_handle(),
                                       sizeof(cpu_set_t), &cpusets[k]);
            }
        }
        t2 = high_resolution_clock::now();
        results[3][j] = (t2 - t1).count();

        // i64 SIMD
        t1 = high_resolution_clock::now();
        {
            std::jthread threads[cpus];
            for (size_t k = 0; k < cpus; k++) {
                threads[k] = std::jthread([this, t1] {
                    this->benchSIMD((i64)t1.time_since_epoch().count());
                });
                pthread_setaffinity_np(threads[k].native_handle(),
                                       sizeof(cpu_set_t), &cpusets[k]);
            }
        }
        t2 = high_resolution_clock::now();
        results[4][j] = (t2 - t1).count();

        // f64 SIMD
        t1 = high_resolution_clock::now();
        {
            std::jthread threads[cpus];
            for (size_t k = 0; k < cpus; k++) {
                threads[k] = std::jthread([this, t1] {
                    this->benchSIMD((double)t1.time_since_epoch().count());
                });
                pthread_setaffinity_np(threads[k].native_handle(),
                                       sizeof(cpu_set_t), &cpusets[k]);
            }
        }
        t2 = high_resolution_clock::now();
        results[5][j] = (t2 - t1).count();
    }
}

json Ops::getJson() {
    json obj;

    obj["name"] = name;
    obj["ops_count"] = n_ops * 16;
    obj["results"] = json::array();

    for (int i = 0; i < 6; i++) {
        obj["results"][i] = json::array();
        for (int j = 0; j < RUNS; j++) {
            obj["results"][i].push_back(results[i][j]);
        }
    }

    return obj;
}